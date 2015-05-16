#include "glwindow.h"
#include "gui/textprinter.h"
#include "common/be_event_system.h"

#include <GL/glew.h>

#if defined(WIN32)
#	include <shlobj.h> // for SHGetFolderPath and CSIDL_PERSONAL
#endif

GLWindow::GLWindow( BeEventSystem* eventsystem ) :
 m_eventsystem(eventsystem),
 w_bpp(0),
 w_width(0),
 w_height(0),
 n_width(0),
 n_height(0),
 fs(0),
 vidInfo(0),
 m_surface(0),
 vidFlags(0),
 hwaccel(0),
 m_window_fullscreen_p(0),
 mousex(0),
 mousey(0),
 m_logDebug("WINDOW"),
 m_resized(false),
 m_resizable(false)
{
	// register an exit event from glwindow
	m_eventsystem->registerEvent( DOWN, "glwindow-quit", boost::shared_ptr<Event>(new Event(BeCommand("quit"), EVENT_NOREPEAT, 0)) );	
} 

void GLWindow::createSafe( const std::string& title )
{
	Settings * const settingsPointer=Settings::Instance();
	if(settingsPointer)
	{
		Settings& settings=*settingsPointer;

		const int width=settings.getCVar("window_width");
		const int height=settings.getCVar("window_height");

		vidInfo = SDL_GetVideoInfo();

		if ( !vidInfo )
		{
			m_logDebug << "::WINDOW error: Cannot get SDL video information '" <<  SDL_GetError() << "'\n";
			exit(1);
		}

		w_bpp = vidInfo->vfmt->BitsPerPixel;

		vidFlags = SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_HWPALETTE;

		hwaccel = false;
		if( vidInfo->hw_available )
		{
			hwaccel = true;
			vidFlags |= SDL_HWSURFACE;
		}
		else
			vidFlags |= SDL_SWSURFACE;

		if( vidInfo->blit_hw != 0 )
			vidFlags |= SDL_HWACCEL;

		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 4);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 4);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 4);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 4);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		setTitle(title);
		// 	SDL_WM_SetIcon(SDL_LoadBMP("other files/critterding icon.png"), 0);
		// 	SDL_WM_SetIcon(SDL_LoadBMP("other files/cd.bmp"), 0);
		// 	string pixmappath = Settings::Instance()->binarypath;
		// 	pixmappath.append( "../share/critterding/pixmaps/cd.bmp" );
		// 	SDL_WM_SetIcon(SDL_LoadBMP(pixmappath.c_str()), 0);
		// 	SDL_WM_SetIcon(SDL_LoadBMP("/projects/lessons/lesson20/data/image2.bmp"), 0);

		w_width = width;
		w_height = height;
		n_width = width;
		n_height = height;

		Settings::Instance()->winWidth = &w_width;
		Settings::Instance()->winHeight = &w_height;

		m_window_fullscreen_p = Settings::Instance()->getCVarPtr("window_fullscreen");
		fs = *m_window_fullscreen_p;

		if ( fs == 1 )
			toggleFs();
	// 	if ( fs == 1 )
	// 	{
	// 		SDL_FreeSurface(m_surface);
	// 		m_surface = SDL_SetVideoMode( w_width, w_height, w_bpp, vidFlags | SDL_FULLSCREEN );
	// 	}
	// 	else
	// 	{
	// 		SDL_FreeSurface(m_surface);
	// 		m_surface = SDL_SetVideoMode( w_width, w_height, w_bpp, vidFlags | SDL_RESIZABLE );
	// 	}
	// 	glewInit();
		m_logDebug << "::WINDOW SDL subsystem initialized\n";
		// 	std::cerr << "Video " << front.width() << "x" << front.height() << "x" << int(front.getSurface()->format->BitsPerPixel) << "\n";
		// 	std::cerr << "Render Mode: " <<  ((hwaccel) ? "Direct Rendering" : "Software Rendering")   << "\n";
		// 	std::cerr << "Hardware Blit Acceleration: " << ((vidInfo->blit_hw) ? "Yes": "No") << "\n";
	}
}


void GLWindow::create(const std::string& title)
{
	if( SDL_Init(SDL_INIT_VIDEO) < 0 )
	{
		m_logDebug << "::WINDOW error: SDL Video initialization failed with error '" << SDL_GetError() << "'\n";
		exit(1);
	}
// 	std::cout << "GLWindow::create" << std::endl;

	Settings * const settingsPointer=Settings::Instance();
	if(settingsPointer)
	{
		Settings& settings=*settingsPointer;
		if(settings.getCVar("window_safemode")==0)
		{
			const int width=settings.getCVar("window_width");
			const int height=settings.getCVar("window_height");
			const int colorBits=settings.getCVar("window_color_bits");
			const int depthBits=settings.getCVar("window_depth_bits");
			const int stencilBits=settings.getCVar("window_stencil_bits");
			const int multisamples=settings.getCVar("window_multisamples");
			const int doublebuffer=settings.getCVar("window_doublebuffer");
			const int vsync=settings.getCVar("window_vsync");
			m_resizable=settings.getCVar("window_resizable");
// 			const int fullscreen=settings.getCVar("window_fullscreen");
			const int hardwareGl=settings.getCVar("window_hardware_gl");

			//Hack
// 			settings.setCVar("fullscreen", fullscreen);
			m_window_fullscreen_p = settings.getCVarPtr("window_fullscreen");
			fs = *m_window_fullscreen_p;

			if(colorBits==32||colorBits==24)
			{
				SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
				SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
				SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
				SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
			}
			else
			{
				SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 4);
				SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 4);
				SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 4);
				SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 4);
			}

			SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, depthBits);
			SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, stencilBits);

			if(multisamples>1)
			{
				SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
				SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, multisamples);
			}
			else
			{
				SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
				SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 0);
			}

			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, doublebuffer);
			SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, vsync);

			Uint32 videoFlags = SDL_OPENGL;

			if(m_resizable)
			{
				videoFlags |= SDL_RESIZABLE;
			}

			if(fs)
			{
				videoFlags |= SDL_FULLSCREEN;
			}

			SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, hardwareGl);


			vidInfo = SDL_GetVideoInfo();

			if ( !vidInfo )
			{
				m_logDebug << "::WINDOW error: Cannot get SDL video information '" <<  SDL_GetError() << "'\n";
				exit(1);
			}

			w_bpp = vidInfo->vfmt->BitsPerPixel;

			vidFlags = SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_HWPALETTE;

			hwaccel = false;
			if( vidInfo->hw_available )
			{
				hwaccel = true;
				vidFlags |= SDL_HWSURFACE;
			}
			else
				vidFlags |= SDL_SWSURFACE;

			if( vidInfo->blit_hw != 0 )
				vidFlags |= SDL_HWACCEL;


			//Hacky
// 			w_bpp = colorBits; // HACK EY WE GOT THIS DOUBE ?
			w_width = width;
			w_height = height;
			n_width = width;
			n_height = height;

			Settings::Instance()->winWidth = &w_width;
			Settings::Instance()->winHeight = &w_height;

			//Set window title
			setTitle(title);

			//Create window
// 			SDL_FreeSurface(m_surface);
// 			m_surface = SDL_SetVideoMode(w_width, w_height, w_bpp, videoFlags);
			
			toggleFs();
			
			GLenum err = glewInit();
			if (GLEW_OK != err)
			{
			// failed to initialize GLEW!
			}
// 			std::cout << "GLEW Version: " << glewGetString(GLEW_VERSION) << std::endl;
// 			std::cout << "Extensions: " << glGetString(GL_EXTENSIONS) << std::endl;
			

            SDL_EnableUNICODE( SDL_ENABLE );

// 			//Enable GL multisampled rendering if required
// 			int value;
// 			SDL_GL_GetAttribute(SDL_GL_MULTISAMPLEBUFFERS, &value);
// 			if(value)
// 			{
// 				glEnable(GL_MULTISAMPLE);
// 				glHint(GL_LINE_SMOOTH_HINT, GL_NICEST );
// 				glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST );
// 				glEnable(GL_LINE_SMOOTH);
// 				glEnable(GL_POLYGON_SMOOTH);
// 			}

// 			usleep (100000);
			
			m_logDebug << "::WINDOW SDL subsystem initialized\n";
			return;

		}
	}
	createSafe(title);
}

void GLWindow::setTitle(const std::string& title)
{
	SDL_WM_SetCaption(title.c_str(),0);

	//On Windows also set the process console title
#ifdef WIN32
	SetConsoleTitle(title.c_str());
#endif
}

void GLWindow::resize()
{
	m_resized = true;

	if ( w_height == 0 ) w_height = 1;
	if ( w_width == 0 ) w_width = 1;
// 	std::cout << "resize" << std::endl;
	
	if ( m_canvas )
		m_canvas->resize(w_width, w_height);

	SDL_FreeSurface(m_surface);
	m_surface = SDL_SetVideoMode( w_width, w_height, w_bpp, vidFlags | SDL_RESIZABLE );
	
}

void GLWindow::toggleFs()
{
	m_resized = true;

	Uint32 videoFlags=vidFlags;
	if ( fs )
	{
		if ( w_height == 0 ) w_height = 1;
		if ( w_width == 0 ) w_width = 1;
		n_width = w_width;
		n_height = w_height;
		w_width = Settings::Instance()->getCVar("fsX");
		w_height = Settings::Instance()->getCVar("fsY");
		videoFlags |= SDL_FULLSCREEN;
	}
	else
	{
		w_width = n_width;
		w_height = n_height;
		if ( m_resizable )
			videoFlags |= SDL_RESIZABLE;
	}
	SDL_FreeSurface(m_surface);
	m_surface = SDL_SetVideoMode( w_width, w_height, w_bpp, videoFlags );
	glewInit();
	if ( m_canvas )
		m_canvas->resize(w_width, w_height);
}

void GLWindow::process()
{
	m_resized = false;

	if ( fs != *m_window_fullscreen_p )
	{
		fs = *m_window_fullscreen_p;
		toggleFs();
	}

    SDL_Event event;
	while( SDL_PollEvent(&event) )
	{
		if(event.type == SDL_VIDEORESIZE)
		{
			w_width = event.resize.w;
			w_height = event.resize.h;
			resize();
		}

		else if(event.type == SDL_KEYDOWN)
		{
            const std::string key=SDL_GetKeyName( event.key.keysym.sym );
			if ( !m_canvas || !m_canvas->keyPress( event.key ) )
            {
// 				std::cout << key << std::endl;
				m_eventsystem->activateKeystate( key );
            }
		}
		else if(event.type == SDL_KEYUP)
		{
//             if(event.key.keysym.sym==SDLK_PRINT)
//             {
//                 dumpScreenShot();
//             }
//             else
            {
                const std::string key=SDL_GetKeyName( event.key.keysym.sym );
			    if ( !m_canvas || !m_canvas->keyRelease( event.key ) )
                {
				    m_eventsystem->deactivateKeystate( key );
                }
            }
// 			m_eventsystem->deactivateKeystate( SDL_GetKeyName( event.key.keysym.sym ) );
		}

		else if (event.type == SDL_MOUSEBUTTONDOWN)
		{
// 			std::cout << (int)event.button.button << std::endl;
			if ( !m_canvas || !m_canvas->buttonPress(event.button.button) )
				m_eventsystem->activateKeystate( event.button.button+2048 );
		}
		else if (event.type == SDL_MOUSEBUTTONUP)
		{
			if ( m_canvas ) m_canvas->buttonRelease(event.button.button);
				m_eventsystem->deactivateKeystate( event.button.button+2048 );
		}

		else if (event.type == SDL_JOYBUTTONDOWN)
		{
			m_eventsystem->activateKeystate( event.button.button+4096 );
		}
		else if (event.type == SDL_JOYBUTTONUP)
		{
			m_eventsystem->deactivateKeystate( event.button.button+4096 );
		}

		else if(event.type == SDL_JOYAXISMOTION)
		{
			m_eventsystem->setAxisstate( event.jaxis.axis+6144, event.jaxis.value );
		}

		else if(event.type == SDL_MOUSEMOTION)
		{
			if ( m_canvas )
				m_canvas->moveMouse( event.motion.x, event.motion.y );

			m_eventsystem->setAxisstate( 0+7168, event.motion.x );
			m_eventsystem->setAxisstate( 1+7168, event.motion.y );
			m_eventsystem->setAxisstate( 2+7168, event.motion.xrel );
			m_eventsystem->setAxisstate( 3+7168, event.motion.yrel );
		}

		else if(event.type == SDL_QUIT)
		{
			m_eventsystem->activateKeystate( "glwindow-quit" );
		}
	}
}

GLWindow::~GLWindow()
{
// 	std::cout << "GLWindow::~GLWindow()" << std::endl;
// 	SDL_FreeSurface(m_surface);
	SDL_Quit();
}

void GLWindow::dumpScreenShot()
{
    SDL_Surface *temp;
    unsigned char *pixels;
    int i;

    temp = SDL_CreateRGBSurface(SDL_SWSURFACE, 
        m_surface->w, m_surface->h, 24,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
        0x000000FF, 0x0000FF00, 0x00FF0000, 0
#else
        0x00FF0000, 0x0000FF00, 0x000000FF, 0
#endif
        );
    if (temp == NULL) return;

    pixels = (unsigned char*)malloc(3 * m_surface->w * m_surface->h);
    if (pixels == NULL) {
        SDL_FreeSurface(temp);
    }

    glReadPixels(0, 0, m_surface->w, m_surface->h, 
        GL_RGB, GL_UNSIGNED_BYTE, pixels);

    for (i=0; i<m_surface->h; i++)
        memcpy(((char *) temp->pixels) + temp->pitch * i, 
        pixels + 3*m_surface->w * (m_surface->h-i-1), 
        m_surface->w*3);
    free(pixels);

    for(unsigned int i(0); i<99; ++i)
    {
        char filename[1024];
        sprintf(filename,"screenshot_%2u.bmp", i+1);
        FILE* file=fopen(filename,"rb");
        if(file==0)
        {
            SDL_SaveBMP(temp, filename);
            break;
        }
    }


 
    SDL_FreeSurface(temp);
}


