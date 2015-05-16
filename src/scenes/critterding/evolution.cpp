#ifdef _MINGW
	#include <unistd.h>
#endif


#include "entities/displaylists.h"
#include "entities/dirlayout.h"
#include "gui/helpinfo.h"
#include "gui/statsgraph.h"
#include "gui/infostats.h"
#include "gui/textverbosemessage.h"
#include "gui/mutationpanel.h"
#include "gui/globalsettingspanel.h"
#include "gui/settingsbrainpanel.h"
#include "gui/settingsbodypanel.h"
#include "gui/hud.h"
#include "gui/critterview.h"
#include "gui/brainview.h"
#include "gui/speciesview.h"
#include "gui/populationcontrolpanel.h"
#include "gui/settingslightpanel.h"

#include "gui/exitpanel.h"
#include "gui/enginesettingspanel.h"

#include "utils/be_settings_loader.h"

#include "evolution.h"

#include <math.h>
#include <boost/bind/bind.hpp>

#include "modes/race.h" // FIXME MOVE TO EVOLUTION
#include "modes/roundworld.h"

#include <iomanip>

#include "entities/food.h"
#include "entities/critterb.h"
#include "gui/textprinter.h"
#include "entities/genotypes.h"
#include "entities/critterselection.h"
#include "entities/worldb.h"


using namespace std;

Evolution::Evolution(BeFilesystem& filesystem)
 : m_filesystem(filesystem)
 , settings(Settings::Instance())
 , m_glsl_initialised(0)
 , benchmark_start(0)
 , m_mode_race(settings->getCVarPtr("race"))
 , m_mode_round(settings->getCVarPtr("roundworld"))

 , m_light_ambient_modelR(settings->getCVarPtr("light_ambient_modelR"))
 , m_light_ambient_modelG(settings->getCVarPtr("light_ambient_modelG"))
 , m_light_ambient_modelB(settings->getCVarPtr("light_ambient_modelB"))
 , m_light_ambientR(settings->getCVarPtr("light_ambientR"))
 , m_light_ambientG(settings->getCVarPtr("light_ambientG"))
 , m_light_ambientB(settings->getCVarPtr("light_ambientB"))
 , m_light_diffuseR(settings->getCVarPtr("light_diffuseR"))
 , m_light_diffuseG(settings->getCVarPtr("light_diffuseG"))
 , m_light_diffuseB(settings->getCVarPtr("light_diffuseB"))
 , m_light_specularR(settings->getCVarPtr("light_specularR"))
 , m_light_specularG(settings->getCVarPtr("light_specularG"))
 , m_light_specularB(settings->getCVarPtr("light_specularB"))
 , m_light_attenuation_constant(settings->getCVarPtr("light_attenuation_constant"))
 , m_light_attenuation_linear(settings->getCVarPtr("light_attenuation_linear"))
 , m_light_attenuation_quadratic(settings->getCVarPtr("light_attenuation_quadratic"))
{
	m_graphicsSystem = boost::shared_ptr<BeGraphicsSystem>(new BeGraphicsSystem(filesystem));
	eventsystem = BeEventSystem::Instance();

	m_glsl = settings->getCVarPtr("glsl");
	m_hdr = settings->getCVarPtr("hdr");
	benchmark = settings->getCVarPtr("benchmark");
	benchmark_frames = settings->getCVarPtr("benchmark_frames");
// 	headless = settings->getCVarPtr("headless");
	drawscene = settings->getCVarPtr("drawscene");
	drawdebug = settings->getCVarPtr("drawdebug");
	m_cameraSensitivity_move = settings->getCVarPtr("camerasensitivity_move");
	m_cameraSensitivity_look = settings->getCVarPtr("camerasensitivity_look");

	m_logBuffer = boost::shared_ptr<Logbuffer>(new Logbuffer);

	boost::shared_ptr<Textverbosemessage> textverbosemessage;
// 	if ( settings->getCVar("headless") == 0 )
	{
		m_glwindow= new GLWindow(BeEventSystem::Instance());
		m_glwindow->create("Critterding beta14");

		m_canvas.reset( new Canvas(filesystem) );
		m_glwindow->setCanvas(m_canvas);
		textverbosemessage.reset( new Textverbosemessage() );
		m_canvas->addWidget( "textverbosemessage", textverbosemessage );
	}

	if ( settings->getCVar("race") == 1 )
		world = new WorldRace( m_graphicsSystem, filesystem, textverbosemessage );
	else if ( settings->getCVar("roundworld") == 1 )
		world = new Roundworld( m_graphicsSystem, filesystem, textverbosemessage );
// 	else if ( settings->getCVar("concavefloor") == 1 )
// 		world = new Concavefloor( m_graphicsSystem, filesystem, textverbosemessage );
	else
		world = new WorldB( m_graphicsSystem, filesystem, textverbosemessage );

	world->setLogbuffer(m_logBuffer);
	settings->setLogbuffer(m_logBuffer);

// 	if ( !*world->headless )
	{
		// init gui panels
		m_canvas->addWidget( "enginesettingspanel", boost::shared_ptr<BeWidget>(new Enginesettingspanel()) );
		m_canvas->addWidget( "helpinfo", boost::shared_ptr<BeWidget>(new Helpinfo()) );
		m_canvas->addWidget( "logbuffermessage", boost::shared_ptr<BeWidget>(new Logbuffermessage( m_logBuffer )) );
		m_canvas->addWidget( "statsgraph", boost::shared_ptr<BeWidget>(new Statsgraph()) );
		m_infobar.reset(new Infobar(&m_timer));
		m_canvas->addWidget( "infobar", m_infobar );
// 		m_canvas->addWidget( "infobar", boost::shared_ptr<BeWidget>(new Infobar(&m_timer)) );
// 		m_canvas->addWidget( "infobar",		BeWidgetPtr(new Infobar( m_currentFramesPerSecond, m_averageFramesPerSecond )) );
		
		m_canvas->addWidget( "infostats", boost::shared_ptr<BeWidget>(new Infostats()) );
		m_canvas->addWidget( "panel_exit", boost::shared_ptr<BeWidget>(new Exitpanel()) );
		m_canvas->addWidget( "mutationpanel", boost::shared_ptr<BeWidget>(new Mutationpanel()) );
		m_canvas->addWidget( "globalsettingspanel", boost::shared_ptr<BeWidget>(new Globalsettingspanel()) );
		m_canvas->addWidget( "settingsbrainpanel", boost::shared_ptr<BeWidget>(new Settingsbrainpanel()) );
		m_canvas->addWidget( "settingsbodypanel", boost::shared_ptr<BeWidget>(new Settingsbodypanel()) );
		m_canvas->addWidget( "critterview", boost::shared_ptr<BeWidget>(new Critterview(m_graphicsSystem)) );
		m_canvas->addWidget( "brainview", boost::shared_ptr<BeWidget>(new Brainview()) );
		m_canvas->addWidget( "hud", boost::shared_ptr<BeWidget>(new Hud(m_graphicsSystem)) );
		m_canvas->addWidget( "speciesview", boost::shared_ptr<BeWidget>(new Speciesview()) );
		m_canvas->addWidget( "populationcontrolpanel", boost::shared_ptr<BeWidget>(new Populationcontrolpanel()) );
		m_canvas->addWidget( "settingslightpanel", boost::shared_ptr<BeWidget>(new Settingslightpanel()) );
		m_canvas->setDefaultZAxis();

		static_cast<Hud*>(m_canvas->children["hud"].get())->world = world;
		static_cast<Critterview*>(m_canvas->children["critterview"].get())->world = world;
	}
// 	else
// 	{
// 		// check if raycastvision is enabled, if not die
// 		if ( settings->getCVar("critter_raycastvision") == 0 )
// 		{
// 			cout << "headless mode requires critter_raycastvision to be enabled" << endl;
// 			exit(1);
// 		}
// 	}
	
 	BeCommandSystem::Instance()->registerVoidCommand("quit", boost::bind(&Evolution::quit, this));
	BeCommandSystem::Instance()->registerIntCommand("cs_unregister", boost::bind(&Evolution::unregisterCritterVID, this, _1));
	BeCommandSystem::Instance()->registerVoidCommand("cs_clear", boost::bind(&Evolution::clear, this));

	BeCommandSystem::Instance()->registerVoidCommand("decreaseenergy", boost::bind(&Evolution::decreaseenergy, this));
	BeCommandSystem::Instance()->registerVoidCommand("increaseenergy", boost::bind(&Evolution::increaseenergy, this));
	BeCommandSystem::Instance()->registerVoidCommand("dec_foodmaxenergy", boost::bind(&Evolution::decreasefoodmaxenergy, this));
	BeCommandSystem::Instance()->registerVoidCommand("inc_foodmaxenergy", boost::bind(&Evolution::increasefoodmaxenergy, this));
	BeCommandSystem::Instance()->registerVoidCommand("dec_worldsizex", boost::bind(&Evolution::dec_worldsizex, this));
	BeCommandSystem::Instance()->registerVoidCommand("inc_worldsizex", boost::bind(&Evolution::inc_worldsizex, this));
	BeCommandSystem::Instance()->registerVoidCommand("dec_worldsizey", boost::bind(&Evolution::dec_worldsizey, this));
	BeCommandSystem::Instance()->registerVoidCommand("inc_worldsizey", boost::bind(&Evolution::inc_worldsizey, this));
	BeCommandSystem::Instance()->registerVoidCommand("dec_worldsizez", boost::bind(&Evolution::dec_worldsizez, this));
	BeCommandSystem::Instance()->registerVoidCommand("inc_worldsizez", boost::bind(&Evolution::inc_worldsizez, this));
	BeCommandSystem::Instance()->registerVoidCommand("camera_moveup", boost::bind(&Evolution::camera_moveup, this));
	BeCommandSystem::Instance()->registerVoidCommand("camera_movedown", boost::bind(&Evolution::camera_movedown, this));
	BeCommandSystem::Instance()->registerVoidCommand("camera_moveforward", boost::bind(&Evolution::camera_moveforward, this));
	BeCommandSystem::Instance()->registerVoidCommand("camera_movebackward", boost::bind(&Evolution::camera_movebackward, this));
	BeCommandSystem::Instance()->registerVoidCommand("camera_moveleft", boost::bind(&Evolution::camera_moveleft, this));
	BeCommandSystem::Instance()->registerVoidCommand("camera_moveright", boost::bind(&Evolution::camera_moveright, this));
	BeCommandSystem::Instance()->registerVoidCommand("camera_lookup", boost::bind(&Evolution::camera_lookup, this));
	BeCommandSystem::Instance()->registerVoidCommand("camera_lookdown", boost::bind(&Evolution::camera_lookdown, this));
	BeCommandSystem::Instance()->registerVoidCommand("camera_lookleft", boost::bind(&Evolution::camera_lookleft, this));
	BeCommandSystem::Instance()->registerVoidCommand("camera_lookright", boost::bind(&Evolution::camera_lookright, this));

	BeCommandSystem::Instance()->registerFloatCommand("camera_lookleftright_mouse", boost::bind(&Evolution::camera_lookleftright_mouse, this, _1));
	BeCommandSystem::Instance()->registerFloatCommand("camera_lookupdown_mouse", boost::bind(&Evolution::camera_lookupdown_mouse, this, _1));
		
	
	BeCommandSystem::Instance()->registerVoidCommand("camera_rollleft", boost::bind(&Evolution::camera_rollleft, this));
	BeCommandSystem::Instance()->registerVoidCommand("camera_rollright", boost::bind(&Evolution::camera_rollright, this));
// 	BeCommandSystem::Instance()->registerVoidCommand("camera_lookhorizontal", &Evolution::camera_lookhorizontal);
// 	BeCommandSystem::Instance()->registerVoidCommand("camera_lookvertical", &Evolution::camera_lookvertical);
// 	BeCommandSystem::Instance()->registerVoidCommand("camera_movehorizontal", &Evolution::camera_movehorizontal);
// 	BeCommandSystem::Instance()->registerVoidCommand("camera_movevertical", &Evolution::camera_movevertical);
	BeCommandSystem::Instance()->registerVoidCommand("cs_selectall", boost::bind(&Evolution::selectCritterAll, this));

	BeCommandSystem::Instance()->registerVoidCommand("loadallcritters", boost::bind(&WorldB::loadAllCritters, world));
	BeCommandSystem::Instance()->registerVoidCommand("saveallcritters", boost::bind(&WorldB::saveAllCritters, world));
	BeCommandSystem::Instance()->registerVoidCommand("insertcritter", boost::bind(&WorldB::insertCritter, world));
	BeCommandSystem::Instance()->registerVoidCommand("killhalfofcritters", boost::bind(&WorldB::killHalfOfCritters, world));
	BeCommandSystem::Instance()->registerVoidCommand("camera_resetposition", boost::bind(&WorldB::resetCamera, world));
	BeCommandSystem::Instance()->registerVoidCommand("toggle_pause", boost::bind(&WorldB::togglePause, world));
// 	BeCommandSystem::Instance()->registerVoidCommand("toggle_sleeper", &WorldB::toggleSleeper);
	BeCommandSystem::Instance()->registerVoidCommand("toggle_mouselook", boost::bind(&WorldB::toggleMouselook, world));
// 	BeCommandSystem::Instance()->registerVoidCommand("critter_select", boost::bind(&WorldB::selectBody, world));
	BeCommandSystem::Instance()->registerVoidCommand("critter_select", boost::bind(&Evolution::selectBody, this));
	BeCommandSystem::Instance()->registerVoidCommand("critter_deselect", boost::bind(&WorldB::deselectBody, world));
	BeCommandSystem::Instance()->registerVoidCommand("critter_pick", boost::bind(&WorldB::pickBody, world));
	BeCommandSystem::Instance()->registerVoidCommand("critter_unpick", boost::bind(&WorldB::unpickBody, world));
	BeCommandSystem::Instance()->registerVoidCommand("critter_follow", boost::bind(&WorldB::followCritterRaycast, world));
	BeCommandSystem::Instance()->registerVoidCommand("cs_kill", boost::bind(&WorldB::removeSelectedCritter, world));
	BeCommandSystem::Instance()->registerVoidCommand("cs_killall", boost::bind(&WorldB::removeAllSelectedCritters, world));
	BeCommandSystem::Instance()->registerVoidCommand("cs_duplicate", boost::bind(&WorldB::duplicateSelectedCritter, world));
	BeCommandSystem::Instance()->registerVoidCommand("cs_spawnbrainmutant", boost::bind(&WorldB::spawnBrainMutantSelectedCritter, world));
	BeCommandSystem::Instance()->registerVoidCommand("cs_spawnbodymutant", boost::bind(&WorldB::spawnBodyMutantSelectedCritter, world));
	BeCommandSystem::Instance()->registerVoidCommand("cs_spawnbrainbodymutant", boost::bind(&WorldB::spawnBrainBodyMutantSelectedCritter, world));
	BeCommandSystem::Instance()->registerVoidCommand("cs_duplicateall", boost::bind(&WorldB::duplicateAllSelectedCritters, world));
	BeCommandSystem::Instance()->registerVoidCommand("cs_spawnbrainmutantall", boost::bind(&WorldB::spawnBrainMutantAllSelectedCritters, world));
	BeCommandSystem::Instance()->registerVoidCommand("cs_spawnbodymutantall", boost::bind(&WorldB::spawnBodyMutantAllSelectedCritters, world));
	BeCommandSystem::Instance()->registerVoidCommand("cs_spawnbrainbodymutantall", boost::bind(&WorldB::spawnBrainBodyMutantAllSelectedCritters, world));
	BeCommandSystem::Instance()->registerVoidCommand("cs_feed", boost::bind(&WorldB::feedSelectedCritter, world));
	BeCommandSystem::Instance()->registerVoidCommand("cs_resetage", boost::bind(&WorldB::resetageSelectedCritter, world));
	BeCommandSystem::Instance()->registerVoidCommand("cs_follow", boost::bind(&WorldB::followCritter, world));

	BeCommandSystem::Instance()->registerIntCommand("cs_select", boost::bind(&Evolution::gui_selectCritter, this, _1));
	BeCommandSystem::Instance()->registerIntCommand("cs_select_species", boost::bind(&Evolution::gui_selectSpecies, this, _1));
	BeCommandSystem::Instance()->registerStringCommand("gui_togglepanel", boost::bind(&Evolution::canvas_swapchild, this, _1));
	BeCommandSystem::Instance()->registerVoidCommand("gui_toggle", boost::bind(&Evolution::gui_swap, this));

	BeCommandSystem::Instance()->registerFloatCommand("movepickedbodyX", boost::bind(&Evolution::movePickedBodyX, this, _1));
	BeCommandSystem::Instance()->registerFloatCommand("movepickedbodyY", boost::bind(&Evolution::movePickedBodyY, this, _1));
/*	BeCommandSystem::Instance()->registerVoidCommand("canvas_press", boost::bind(&Evolution::canvas_press, this));
	BeCommandSystem::Instance()->registerVoidCommand("canvas_release", boost::bind(&Evolution::canvas_release, this));
	BeCommandSystem::Instance()->registerVoidCommand("canvas_pressAlt", boost::bind(&Evolution::canvas_pressAlt, this));
	BeCommandSystem::Instance()->registerVoidCommand("canvas_releaseAlt", boost::bind(&Evolution::canvas_releaseAlt, this));*/
	
	//Load control definitions
	BeFile befileControls;
	if ( filesystem.load(befileControls, "settings.xml") )
	{
		BeSettingsLoader settingsLoader;
		settingsLoader.load( BeEventSystem::Instance(), &befileControls );
	}
	
	if ( *benchmark == 1 )
	{
		m_canvas->deactivate();
// 		settings->setCVar("startseed", 11);
		settings->setCVar("fpslimit", 0);
	}

	world->init();
	
	
				glewInit();
				if(*m_glsl)
				{
					initGLSL();
				}
				else
				{
// 					m_graphicsSystem->setNormalMappingIDs(-1, -1);
// 					m_v_inv = -1;
				}
// 				world->m_v_inv = m_v_inv;
	
	m_select_up.setIdentity();
	m_select_up.setOrigin( btVector3(0.0f, 4.0f, 0.0f) );

	
// 	debugDrawer.setDebugMode(btIDebugDraw::DBG_DrawWireframe|btIDebugDraw::DBG_DrawWireframe|btIDebugDraw::DBG_DrawAabb|btIDebugDraw::DBG_DrawContactPoints|btIDebugDraw::DBG_DrawText|btIDebugDraw::DBG_DrawConstraints | btIDebugDraw::DBG_DrawConstraintLimits);
	world->m_dynamicsWorld->setDebugDrawer(&debugDrawer);
}

void Evolution::initGLSL()
{
	if ( !m_glsl_initialised )
	{
		m_graphics.reset(new ScGraphics(m_graphicsSystem, m_filesystem));
// 					m_effect.reset(new BeGraphicsEffect(m_filesystem, "shaders/crittervision_new.vert", "shaders/crittervision_new.frag"));
// // 					m_effect.reset(new BeGraphicsEffect(m_filesystem, "shaders/sandbox.vert", "shaders/sandbox.frag"));
// // 					m_effect.reset(new BeGraphicsEffect(m_filesystem, "shaders/crittervision.vert", "shaders/crittervision.frag"));
// // 					m_effect.reset(new BeGraphicsEffect(m_filesystem, "shaders/test330.vert", "shaders/test330.frag"));
// // 					m_effect.reset(new BeGraphicsEffect(m_filesystem, "shaders/default.vert", "shaders/default.frag"));
// // 					m_crittervision_effect.reset(new BeGraphicsEffect(m_filesystem, "shaders/crittervision.vert", "shaders/crittervision.frag"));
// // 					m_crittervision_effect.reset(new BeGraphicsEffect(m_filesystem, "shaders/crittervision_new.vert", "shaders/crittervision_new.frag"));
// 					
// 					GLint m_tex1Loc = glGetUniformLocation(m_effect->m_program.get()->handle(), "tex1");
// 					GLint m_doNormalMapping = glGetUniformLocation(m_effect->m_program.get()->handle(), "normal_map_do");
// 					m_graphicsSystem->setNormalMappingIDs(m_tex1Loc, m_doNormalMapping);
					
// 					m_v_inv = glGetUniformLocation(m_graphics->m_effect->m_program.get()->handle(), "v_inv");
					world->setGraphics(m_graphics);
		
		m_glsl_initialised=true;
	}
	
}

void Evolution::setLights()
{

// 		GLfloat ambientLight[] = {0.05f, 0.05f, 0.05f, 1.0f};
// 		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
// 
// 		GLfloat lightAmbient[] = {0.025f, 0.025f, 0.025f, 1.0f};
// 		glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
// 	
// 		GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
// 		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
// 		
// 		if(*m_glsl)
// 		{
// 		// LIGHT
// 			GLfloat lightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
// 			glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
// 		}
	
		const double factor( 1.0/255 );

		GLfloat ambientLight[] = {factor*  *m_light_ambient_modelR, factor*  *m_light_ambient_modelG, factor*  *m_light_ambient_modelB, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

		GLfloat lightAmbient[] = {factor*  *m_light_ambientR, factor*  *m_light_ambientG, factor*  *m_light_ambientB, 1.0f};
		glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	
		GLfloat lightDiffuse[] = { factor*  *m_light_diffuseR, factor*  *m_light_diffuseG, factor*  *m_light_diffuseB, 1.0f };
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
		
		if(*m_glsl)
		{
		// LIGHT
			GLfloat lightSpecular[] = {factor*  *m_light_specularR, factor*  *m_light_specularG, factor*  *m_light_specularB, 1.0f};
			glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
		}


			world->setLights();

// 			GLfloat lightDir[] = { 0.0f, -1.0f, 0.0f };
// 			glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightDir);

		// 	GLfloat lightCutoff[] = { 90.0f }; // angle is 0 to 180
			GLfloat lightCutoff[] = { 180.0f }; // angle is 0 to 180
			glLightfv(GL_LIGHT0, GL_SPOT_CUTOFF, lightCutoff);

			GLfloat lightExponent[] = { 128.0f }; // exponent is 0 to 128
			glLightfv(GL_LIGHT0, GL_SPOT_EXPONENT, lightExponent);
			
			
			
			GLfloat lightAttenuation[] = { 0.001f * *m_light_attenuation_constant };
			glLightfv(GL_LIGHT0, GL_CONSTANT_ATTENUATION, lightAttenuation);

			GLfloat lightLAttenuation[] = { 0.000001f * *m_light_attenuation_linear };
			glLightfv(GL_LIGHT0, GL_LINEAR_ATTENUATION, lightLAttenuation);

			GLfloat lightQAttenuation[] = { 0.000000001f * *m_light_attenuation_quadratic };
			glLightfv(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, lightQAttenuation);

			
	// 		GLfloat lightAttenuation[] = { 0.5f };
	// 		glLightfv(GL_LIGHT0, GL_CONSTANT_ATTENUATION, lightAttenuation);
	// 
	// 		GLfloat lightLAttenuation[] = { 0.000005f };
	// 		glLightfv(GL_LIGHT0, GL_LINEAR_ATTENUATION, lightLAttenuation);
	// 
	// 		GLfloat lightQAttenuation[] = { 0.000000f };
	// 		glLightfv(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, lightQAttenuation);

			
			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);
						
			
// 			GLfloat lightCutoff[] = { m_spot_cutoff };
// 			glLightfv(GL_LIGHT0, GL_SPOT_CUTOFF, lightCutoff);
// 
// 			GLfloat lightExponent[] = { m_spot_exponent };
// 			glLightfv(GL_LIGHT0, GL_SPOT_EXPONENT, lightExponent);
// 			
			
// 		glPopMatrix();
}


void Evolution::process()
{
// 	std::cout << "tick" << std::endl;
	m_timer.mark();
	framelimiter.mark( &m_timer );

	const float timeDelta=m_timer.getSeconds();
	m_cameraTranslateSpeed=0.04 * timeDelta * *m_cameraSensitivity_move; //0.5f**m_cameraSensitivity;
	m_cameraRotateSpeed=0.0005 * timeDelta * *m_cameraSensitivity_look; //0.0001f**m_cameraSensitivity;
	
	m_glwindow->process();
	eventsystem->processEvents( m_timer.getMilliSeconds() );

	if ( *benchmark == 1 )
	{
		const auto bench_start_at( 2 );
		const auto bench_total_frames( *benchmark_frames );
		const auto bench_stop_at( bench_start_at + bench_total_frames );

		if ( Statsbuffer::Instance()->frameCounter == bench_start_at )
		{
			benchmark_start = m_timer.getTotalMilliSeconds();
// 			cout << "time skipped" << benchmark_start << endl;
		}

		else if ( Statsbuffer::Instance()->frameCounter == bench_stop_at )
		{
			const auto ms = m_timer.getTotalMilliSeconds() - benchmark_start;

			cout << std::setprecision(8);
			cout << "> benchmark result : " << std::endl;
			cout << ">   frames      :" << bench_total_frames << std::endl;
			cout << ">   time        :" << 0.001f * ms << " seconds" << std::endl;
			cout << ">   seed        :" << world->randgen->Instance()->getSeed() << std::endl;
			cout << ">   checksum    :" << fabs(world->m_freeEnergy) + world->critters.size() + world->food_units.size() << std::endl;
			cout << ">   average fps :" << (1000.0f * Statsbuffer::Instance()->frameCounter) / ms << " fps" << std::endl;




// 			cout << "BENCHMARK RESULT : ";
// 			cout << "(seed:" << world->randgen->Instance()->getSeed() << ",checksum:" << fabs(world->m_freeEnergy) + world->critters.size() + world->entities.size() << ") ";
// 			cout << frameCounter << "f/" << 0.001 * ms << "s " << " = ";
// 			cout << (1000.0 * frameCounter) / ms << " fps";
// 			cout << endl;



			exit(0);
		}
	}

	
	
	
	
	
	
	
	
	
	
	initGLSL();

	
	
			
	SDL_GL_SwapBuffers();
	

	
// 	m_timer.mark();
// 	framelimiter.mark( &m_timer );

	m_infobar->mark();
	++Statsbuffer::Instance()->frameCounter;
	
	m_logBuffer->deleteExpiredMsg( timeDelta );
	

// 	if ( !m_glwindow->resized() )
	{

	// 	if ( !*world->headless )
		{
// 			m_glwindow->process();
		
			// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);
			glEnable(GL_TEXTURE_2D);
			glDepthMask( GL_TRUE );
		}

		if(*m_glsl)
		{
			m_graphicsSystem->useProgram(m_graphics->m_effect->m_program.get());
	// 		m_graphicsSystem->useProgram(m_graphics->m_critter_effect->m_program.get());
		}
		else
		{
			glEnable(GL_NORMALIZE);
		}

// 		const int width=*settings->winWidth;
// 		const int height=*settings->winHeight;
// 		const int wwidth=*settings->winWidth;
// 		const int wheight=*settings->winHeight;

		const int width=m_glwindow->width();
		const int height=m_glwindow->height();
		
		glColor4f( 1.0f, 1.0f, 1.0f, 0.0f );
		
		
		
		setLights();

		world->updateCameraTransform( m_timer.getMilliSeconds() );

		world->process(m_timer);

	// 	if ( !*world->headless )
		{
			m_graphicsSystem->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			glViewport(0,0,width,height);
			
			
			if ( *drawscene == 1 || *drawdebug > 0 )
			{
				if( *m_glsl == 1 )
				{
					
	// 				m_graphicsSystem->useProgram(m_graphics->m_effect->m_program.get());

					if ( *m_hdr == 1 )
					{
						m_graphics->resetBrightTexture(width, height);

						m_graphics->m_sceneFramebuffer->bind();
						m_graphicsSystem->useProgram(m_graphics->m_dofSceneEffect->m_program.get());
					}

					btScalar modelview2[16];
	// 				world->m_sceneNodeCamera.getTransform().getOpenGLMatrix(modelview2);
					world->getCameraTransform().getOpenGLMatrix(modelview2);
					glUniformMatrix4fv(m_graphics->m_v_inv, 1, GL_FALSE, modelview2);
				}

				m_graphicsSystem->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	// 			glViewport(0,0,TONETEXTURE_W,TONETEXTURE_H);
			
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();

				world->m_camera.m_aspect = (float)width / height;
// 				world->m_camera.m_aspect = (float) *settings->winWidth / *settings->winHeight;
				const float frustumHalfHeight = tan( world->m_camera.m_fovy * 180 / M_PI * M_PI / 360.0 ) * world->m_camera.m_zNear;
				const float frustumHalfWidth = frustumHalfHeight * world->m_camera.m_aspect;
				glFrustum(-frustumHalfWidth, frustumHalfWidth, -frustumHalfHeight, frustumHalfHeight, world->m_camera.m_zNear, world->m_camera.m_zFar);

				
				world->getCameraTransform().inverse().getOpenGLMatrix(cam_position);
				glMultMatrixf(cam_position);
			
	// 			if ( world->m_follow_critterP == 0 )
	// 			
	// 			{
	// 				world->m_sceneNodeCamera.getTransform().inverse().getOpenGLMatrix(cam_position);
	// 				glMultMatrixf(cam_position);
	// 			}
	// 			else
	// 			{
	// 				auto& cam(world->m_sceneNodeCamera);
	// 				
	// 				const auto& tr(world->m_follow_critterP->body.mouths[0]->ghostObject->getWorldTransform());
	// 				cam.setOrigin(btVector3(tr.getOrigin().x(),cam.getOrigin().getY()+5.0f,tr.getOrigin().z()));
	// 				
	// // 				cam.setOrigin(world->critterselection->clist[0]->body.mouths[0]->ghostObject->getWorldTransform().getOrigin() + btVector3(0,cam.getOrigin().getY()+5.0f,0));
	// // 				cam.getOrigin().setY(cam.getOrigin().getY()+5.0f);
	// 				cam.pitch( -SIMD_HALF_PI ); // 1.5707f  (float)*energy/10
	// 
	// 				cam.getTransform().inverse().getOpenGLMatrix(cam_position);
	// 				glMultMatrixf(cam_position);
	// 			}
				

				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
		
				if ( *drawscene == 1 )
				{
	// 				if ( world->critterselection->clist.empty() )
						world->drawWithGrid();
	// 				else
	// 					world->drawWithinCritterSight( world->critterselection->clist[0] );
				}
				
				
				
						
				if(*m_glsl)
				{
					if ( *m_hdr )
					{
						m_graphicsSystem->flush();
						m_graphicsSystem->useProgram(0);

	// 						glDepthMask( GL_TRUE );
							glEnable(GL_DEPTH_TEST);
				// 			glDisable (GL_LIGHTING);
				// 			glDisable(GL_CULL_FACE);
				// 			glDisable(GL_TEXTURE_2D);
				// 		
							glDisable(GL_ALPHA_TEST);
							glDisable(GL_BLEND);

// 						glColor4f(1,1,1,0);
							
							
						float exposure = 0.0255f;
					
						float brightness_exposure = 0.0255f;
						float brightness_threshold = 0.2f;

						
						// First Pass
						{
							m_graphicsSystem->reset2D(1, 1);
							m_graphicsSystem->useProgram(m_graphics->m_luminanceEffect2->m_program.get());
							m_graphicsSystem->bindFramebuffer(m_graphics->m_luminanceFramebuffer2.get());
							m_graphicsSystem->bindTexture2D(m_graphics->m_sceneTexture.get());
							//m_graphicsSystem->viewport(0, 0, width/2, height/2);
	// 						m_graphicsSystem->viewport(0, 0, LUM2TEXTURE_W, LUM2TEXTURE_H);
							m_graphicsSystem->viewport(0, 0, width, height);
							m_graphicsSystem->drawQuad();
							//m_graphicsSystem->bindTexture2D(0);
							//m_graphicsSystem->bindFramebuffer(0);
							//m_graphics->m_luminanceTexture2->generateMipmap();
							//m_graphicsSystem->useProgram(0);
						}

							int chain_length = 1;
	// 						int start_size = (LUM3TEXTURE_H < LUM3TEXTURE_W) ? LUM3TEXTURE_H: LUM3TEXTURE_W; // min(width/4, height/4)
							int start_size = (height < width) ? height: width; // min(width/4, height/4)
							int size = 1;
							for (size = 1; size < start_size; size *= 2) {
								chain_length++;
							}
							size /= 2;
							chain_length--;
						
						
						// Second Pass
						{
							m_graphicsSystem->useProgram(m_graphics->m_luminanceEffect3->m_program.get());
							m_graphicsSystem->bindTexture2D(m_graphics->m_luminanceTexture2.get());
	// 						m_graphics->m_luminanceEffect3->m_program->uniform("Width", LUM3TEXTURE_W);
	// 						m_graphics->m_luminanceEffect3->m_program->uniform("Height", LUM3TEXTURE_H);
							m_graphics->m_luminanceEffect3->m_program->uniform("Width", width);
							m_graphics->m_luminanceEffect3->m_program->uniform("Height", height);
							for (int i = 0; i < chain_length; i++) {
								m_graphicsSystem->bindFramebuffer(m_graphics->m_luminanceFramebuffer3[i].get());
								m_graphicsSystem->viewport(0, 0, size, size);
								m_graphicsSystem->drawQuad();
								//m_graphicsSystem->bindTexture2D(0);
								//m_graphicsSystem->bindFramebuffer(0);
								//m_graphics->m_luminanceTexture3[i]->generateMipmap();
								m_graphicsSystem->bindTexture2D(m_graphics->m_luminanceTexture3[i].get());
								m_graphics->m_luminanceEffect3->m_program->uniform("Width", size);
								m_graphics->m_luminanceEffect3->m_program->uniform("Height", size);
								size /= 2;
							}
							//m_graphicsSystem->bindFramebuffer(0);
							m_graphicsSystem->useProgram(0);
						}

						// Third Pass: bright pass filter
						{
							m_graphicsSystem->useProgram(m_graphics->m_brightEffect->m_program.get());
							m_graphics->m_brightEffect->m_program->uniform("Exposure", brightness_exposure);
							m_graphics->m_brightEffect->m_program->uniform("Threshold", brightness_threshold);
							m_graphicsSystem->bindMultiTexture2D(GL_TEXTURE0,m_graphics->m_sceneTexture.get());
							m_graphicsSystem->bindMultiTexture2D(GL_TEXTURE1,m_graphics->m_luminanceTexture3[chain_length-1].get());
							m_graphicsSystem->bindFramebuffer(m_graphics->m_brightFramebuffer.get());
				// 			m_graphicsSystem->viewport(0, 0, width/4, height/4);
	// 						m_graphicsSystem->viewport(0, 0, BRIGHTTEXTURE_W, BRIGHTTEXTURE_H);
							m_graphicsSystem->viewport(0, 0, width, height);
							
							m_graphicsSystem->drawQuad();
							//m_graphicsSystem->bindFramebuffer(0);
							//m_graphicsSystem->bindMultiTexture2D(GL_TEXTURE0,0);
							m_graphicsSystem->bindMultiTexture2D(GL_TEXTURE1,0);
							//m_graphicsSystem->useProgram(0);
							//m_graphics->m_brightTexture->generateMipmap();
						}




						// Fourth Pass: gaussian blur along the X axis
						{
							m_graphicsSystem->useProgram(m_graphics->m_gaussianBlurHorizonalEffect->m_program.get());
							m_graphics->m_gaussianFramebuffer[0]->bind();
							m_graphicsSystem->bindTexture2D(m_graphics->m_brightTexture.get());
					// 		m_graphicsSystem->viewport(0,0,width/4,height/4);
	// 						m_graphicsSystem->viewport(0,0,GAUSSIANTEXTURE_W, GAUSSIANTEXTURE_H);
							m_graphicsSystem->viewport(0,0,width, height);
							m_graphicsSystem->drawQuad();
							//m_graphicsSystem->bindTexture2D(0);
							//m_graphicsSystem->bindFramebuffer(0);
							//m_graphicsSystem->useProgram(0);
							//m_graphics->m_gaussianTexture[0]->generateMipmap();
						}

						// Fifth Pass: gaussian blur along the Y axis
						{
							m_graphicsSystem->useProgram(m_graphics->m_gaussianBlurVerticalEffect->m_program.get());
							m_graphics->m_gaussianFramebuffer[1]->bind();
							m_graphicsSystem->bindTexture2D(m_graphics->m_gaussianTexture[0].get());
							m_graphicsSystem->viewport(0,0, width, height);
							
							m_graphicsSystem->drawQuad();

							//m_graphicsSystem->bindTexture2D(0);
							//m_graphicsSystem->bindFramebuffer(0);
							//m_graphicsSystem->useProgram(0);
							//m_graphics->m_gaussianTexture[1]->generateMipmap();

							//m_graphicsSystem->reset2D(1, 1);
						}

						// Sixth Pass: tone mapping
						{
							m_graphicsSystem->useProgram(m_graphics->m_tonemapEffect->m_program.get());
							m_graphics->m_tonemapEffect->m_program->uniform("Exposure", exposure);
					// 		m_graphics->m_tonemapEffect->m_program->uniform("Threshold", threshold);
							m_graphics->m_tonemapFramebuffer->bind();
							m_graphicsSystem->bindMultiTexture2D(GL_TEXTURE0, m_graphics->m_sceneTexture.get());
							m_graphicsSystem->bindMultiTexture2D(GL_TEXTURE1, m_graphics->m_luminanceTexture3[chain_length-1].get());
							
							m_graphicsSystem->viewport(0, 0, width, height);
							
							m_graphicsSystem->drawQuad();
							m_graphicsSystem->bindMultiTexture2D(GL_TEXTURE0,0);
							m_graphicsSystem->bindMultiTexture2D(GL_TEXTURE1,0);
							//m_graphicsSystem->bindFramebuffer(0);
							m_graphicsSystem->useProgram(0);
							//m_graphics->m_tonemapTexture->generateMipmap();
						}



						//m_graphicsSystem->bindFramebuffer(0);
						m_graphics->m_sceneFramebuffer->bind();
						m_graphicsSystem->viewport(0, 0, width, height);

						// Seventh Pass: glow effect
						{
							m_graphicsSystem->useProgram(m_graphics->m_addRgbEffect->m_program.get());
							m_graphicsSystem->bindMultiTexture2D(GL_TEXTURE0,m_graphics->m_tonemapTexture.get());
							m_graphicsSystem->bindMultiTexture2D(GL_TEXTURE1,m_graphics->m_gaussianTexture[1].get());
							m_graphicsSystem->drawQuad();
							m_graphicsSystem->bindMultiTexture2D(GL_TEXTURE0,0);
							m_graphicsSystem->bindMultiTexture2D(GL_TEXTURE1,0);
							m_graphicsSystem->useProgram(0);

							m_graphicsSystem->reset2D(1, 1);
							m_graphicsSystem->useProgram(m_graphics->m_dofDownsampleEffect->m_program.get());
							m_graphicsSystem->bindFramebuffer(m_graphics->m_dofDownsampleFramebuffer[0].get());
							m_graphicsSystem->bindTexture2D(m_graphics->m_sceneTexture.get());
							m_graphicsSystem->viewport(0, 0, width, height);
							m_graphicsSystem->drawQuad();

							m_graphicsSystem->reset2D(1, 1);
							m_graphicsSystem->useProgram(m_graphics->m_dofFilterXEffect->m_program.get());
							m_graphicsSystem->bindFramebuffer(m_graphics->m_dofDownsampleFramebuffer[1].get());
							m_graphicsSystem->bindTexture2D(m_graphics->m_dofDownsampleTexture[0].get());
							m_graphicsSystem->viewport(0, 0, width, height);
							m_graphicsSystem->drawQuad();

							m_graphicsSystem->reset2D(1, 1);
							m_graphicsSystem->useProgram(m_graphics->m_dofFilterYEffect->m_program.get());
							m_graphicsSystem->bindFramebuffer(m_graphics->m_dofDownsampleFramebuffer[2].get());
							m_graphicsSystem->bindTexture2D(m_graphics->m_dofDownsampleTexture[1].get());
							m_graphicsSystem->viewport(0, 0, width, height);
							m_graphicsSystem->drawQuad();

							m_graphicsSystem->reset2D(1, 1);
							m_graphicsSystem->useProgram(m_graphics->m_dofFinalEffect->m_program.get());
							m_graphicsSystem->bindFramebuffer(m_graphics->m_dofSceneFramebuffer.get());
							m_graphicsSystem->bindMultiTexture2D(GL_TEXTURE0, m_graphics->m_sceneTexture.get());
							m_graphicsSystem->bindMultiTexture2D(GL_TEXTURE1, m_graphics->m_dofDownsampleTexture[2].get());
							m_graphicsSystem->viewport(0, 0, width, height);
							m_graphicsSystem->drawQuad();

							m_graphicsSystem->bindMultiTexture2D(GL_TEXTURE0,0);
							m_graphicsSystem->bindMultiTexture2D(GL_TEXTURE1,0);
							m_graphicsSystem->useProgram(0);
						}


						m_graphicsSystem->bindFramebuffer(0);
						m_graphicsSystem->reset2D(1, 1);
						
	// 					if ( *m_hdr == 0 || *m_glsl == 0 )
	// 					{
	// 						m_graphicsSystem->clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	// 						glViewport(0,0,*settings->winWidth,*settings->winHeight);
	// 					}
						glViewport(0, 0, width,height);
						
						
				// 		glEnable(GL_TEXTURE_2D);
						m_graphicsSystem->bindTexture2D(m_graphics->m_dofSceneTexture.get());
						m_graphicsSystem->drawQuad();
						m_graphicsSystem->bindTexture2D(0);
				// 		glDisable(GL_TEXTURE_2D);

					}
				// 	else
				// 	{
				// 		m_graphicsSystem->bindFramebuffer(0);
				// 		m_graphicsSystem->reset2D(1, 1);
				// 		glViewport(0, 0, winwidth, winheight);
				// 		glEnable(GL_TEXTURE_2D);
				// 		m_graphicsSystem->bindTexture2D(m_graphics->m_sceneTexture.get());
				// 		m_graphicsSystem->drawQuad();
				// 		m_graphicsSystem->bindTexture2D(0);
				// 		glDisable(GL_TEXTURE_2D);
				// 	}					
				}	
		
		}
		
		
			m_graphicsSystem->bindFramebuffer(0);
		
			
			
							
							
							
							
							
							
							
			
			if ( m_canvas->isActive() || *drawdebug > 0 || (*drawscene == 1 && !world->critterselection->clist.empty()) )
			{
				if(*m_glsl)
				{
					m_graphicsSystem->useProgram(0);
				}
				glDisable (GL_LIGHTING);
				glDisable(GL_CULL_FACE);
				glDisable(GL_TEXTURE_2D);
				
				// DEBUG DRAWING
				if ( *drawdebug > 0 )
				{
// 					glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
					
					if ( *drawdebug == 1 )
					{
	// 					debugDrawer.setDebugMode(btIDebugDraw::DBG_DrawWireframe|btIDebugDraw::DBG_DrawAabb|btIDebugDraw::DBG_DrawText|btIDebugDraw::DBG_DrawConstraints | btIDebugDraw::DBG_DrawConstraintLimits);
						debugDrawer.setDebugMode(btIDebugDraw::DBG_MAX_DEBUG_DRAW_MODE);
					}
					else if ( *drawdebug == 2 )
					{
						debugDrawer.setDebugMode(btIDebugDraw::DBG_DrawWireframe);
					}
					else if ( *drawdebug == 3 )
					{
						debugDrawer.setDebugMode(btIDebugDraw::DBG_DrawAabb);
					}
					else if ( *drawdebug == 4 )
					{
						debugDrawer.setDebugMode(btIDebugDraw::DBG_DrawConstraints);
					}
					else if ( *drawdebug == 5 )
					{
						debugDrawer.setDebugMode(btIDebugDraw::DBG_DrawConstraintLimits);
					}
					world->m_dynamicsWorld->debugDrawWorld();
				}

			// SELECTION LIFEBOXES
				bool disabled_depth(false);
				if ( *drawscene == 1 && !world->critterselection->clist.empty() )
				{
					m_graphicsSystem->clear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
					glDisable(GL_DEPTH_TEST);
					disabled_depth=true;
					
					
					// draw selected info
					btTransform trans;
					trans.setIdentity();

					const float width(5.0f);
					const float hwidth( width/2 );
					const float height(0.8f);
					const float hheight( height/2 );
					const float width_ratio_energy( width / settings->getCVar("critter_maxenergy") );
					const float maxlifetime(settings->getCVar("critter_maxlifetime"));
					const float width_ratio_age( width / maxlifetime );

					const float seperation(hheight+0.16f);
					
					for ( unsigned int i=0; i < world->critterselection->clist.size(); i++ )
					{
						const auto& c( world->critterselection->clist[i] );

						trans.setOrigin(c->body.mouths[0]->ghostObject->getWorldTransform().getOrigin());
						trans.getOrigin().setY(trans.getOrigin().getY()+0.5f);
						trans.setBasis(world->getCameraTransform().getBasis());
						trans *= m_select_up;
						trans.getOpenGLMatrix(cam_position);

						glPushMatrix(); 
		// 				glMultMatrixf(position);
		// 				glMultMatrixd(position); // FIXME WATCHIT OK WENT TO DOUBLE HERE 
						Displaylists::Instance()->glMultiMatrix(cam_position);

						
						
						// ENERGY
							glBegin(GL_QUADS);
								const float e_fill( -hwidth + (width_ratio_energy * c->energyLevel) );
	// 									m_graphicsSystem->color(BeColor(0.0f, 1.0f, 0.0f, 1.0f));
								glColor4f(0.0f, 1.0f, 0.0f, 0.0f);
								glVertex2f(-hwidth, hheight-seperation);
								glVertex2f(-hwidth, -hheight-seperation);
								glVertex2f(e_fill, -hheight-seperation);
								glVertex2f(e_fill,  hheight-seperation);

						// AGE
								const float a_fill( -hwidth + (width_ratio_age * c->totalFrames) );
	// 									m_graphicsSystem->color(BeColor(0.0f, 0.0f, 1.0f, 1.0f));
								glColor4f(0.0f, 0.0f, 1.0f, 0.0f);
								glVertex2f(-hwidth, hheight+seperation);
								glVertex2f(-hwidth, -hheight+seperation);
								glVertex2f(a_fill, -hheight+seperation);
								glVertex2f(a_fill,  hheight+seperation);
							glEnd();
							
	// 								m_graphicsSystem->color(BeColor(1.0f, 1.0f, 1.0f, 1.0f));
							glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
							
							glBegin(GL_LINES);
						// ENERGY
									glVertex2f(-hwidth, hheight-seperation);
									glVertex2f(-hwidth,-hheight-seperation);

									glVertex2f(-hwidth,-hheight-seperation);
									glVertex2f(hwidth, -hheight-seperation);

									glVertex2f(hwidth, -hheight-seperation);
									glVertex2f(hwidth,  hheight-seperation);

									glVertex2f(hwidth,  hheight-seperation);
									glVertex2f(-hwidth, hheight-seperation);
									
									
						// AGE
									glVertex2f(-hwidth, hheight+seperation);
									glVertex2f(-hwidth,-hheight+seperation);

									glVertex2f(-hwidth,-hheight+seperation);
									glVertex2f(hwidth, -hheight+seperation);

									glVertex2f(hwidth, -hheight+seperation);
									glVertex2f(hwidth,  hheight+seperation);

									glVertex2f(hwidth,  hheight+seperation);
									glVertex2f(-hwidth, hheight+seperation);
									
							glEnd();

						glPopMatrix();
						
						
					}
				}
				
			// CANVAS
				if ( m_canvas->isActive() )
				{
					if ( !disabled_depth )
					{
						m_graphicsSystem->clear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
						glDisable(GL_DEPTH_TEST);
					}
						world->m_graphicsSystem->matrixLoadIdentity(GL_PROJECTION);
						
						world->m_graphicsSystem->matrixOrtho(GL_PROJECTION, 0, *settings->winWidth, *settings->winHeight, 0,  0, 1);
						world->m_graphicsSystem->matrixLoadIdentity(GL_MODELVIEW);
						world->m_graphicsSystem->matrixTranslate(GL_MODELVIEW, 0.5f, -0.5f, 0.0f); // pixel precision offset
	// 					world->m_graphicsSystem->matrixTranslate(GL_MODELVIEW, 0.0f, 0.0f, 0.0f); // pixel precision offset

						m_canvas->draw();

						if ( *drawscene == 1 || *drawdebug > 0  )
						{
							
							
							
							if (!world->mouselook && !m_canvas->mouseFocus )
							{
								world->castMouseRay();

								// hover test
								if ( world->mouseRayHit )
								{
									const unsigned int margin = 20;
									const unsigned int rmargindistance = 70;
									const unsigned int vspacer = 12;
									glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
									if ( world->mouseRayHitEntity->type == 1 )
									{
										Textprinter::Instance()->print( world->mousex+margin, world->mousey,    "food");
										Textprinter::Instance()->print( world->mousex+margin, world->mousey+vspacer, "energy");
										Textprinter::Instance()->print(world->mousex+rmargindistance, world->mousey+vspacer, "%1.1f", static_cast<const Food*>(world->mouseRayHitEntity)->energyLevel);
									}
									else if ( world->mouseRayHitEntity->type == 0 )
									{
										const CritterB* c = static_cast<const CritterB*>(world->mouseRayHitEntity);
										Textprinter::Instance()->print( world->mousex+margin, world->mousey,    "critter");
										Textprinter::Instance()->print(world->mousex+rmargindistance, world->mousey, "%1i", c->critterID);
										Textprinter::Instance()->print( world->mousex+margin, world->mousey+vspacer, "energy");
										Textprinter::Instance()->print(world->mousex+rmargindistance, world->mousey+vspacer, "%1.1f", c->energyLevel);
									}
								}
							}
						}
	// 				}
	// 				glPopMatrix(); 
				}
				
							
				
				
				
			}
			
		

	// 		#ifdef _DEBUG

				GLenum err (glGetError());
				while(err!=GL_NO_ERROR)
				{
					std::string error;

					switch(err)
					{
						case GL_INVALID_OPERATION:	error="INVALID_OPERATION";	break;
						case GL_INVALID_ENUM:		error="INVALID_ENUM";		break;
						case GL_INVALID_VALUE:		error="INVALID_VALUE";		break;
						case GL_OUT_OF_MEMORY:		error="OUT_OF_MEMORY";		break;
						case GL_INVALID_FRAMEBUFFER_OPERATION:	error="INVALID_FRAMEBUFFER_OPERATION";	break;
					}

					std::cout << "GL_" << error << std::endl;
					err=glGetError();
				}
	// 		#endif


		}

		if ( world->critters.size() == 0 && settings->getCVar("exit_if_empty") )
		{
			cout << "world is empty, exiting..." << endl;
	// 		Cdcommands::Instance()->quit();
			quit();
		}

// 		SDL_GL_SwapBuffers();
	}
}




// void Evolution::handleMouseMotionAbs(int x, int y)
// {
// 	std::cout << "abs" << std::endl;
// 	if ( !world->mouselook )
// 	{
// 		world->mousex = x;
// 		world->mousey = y;
// 		
// 		// world mouse dynamics
// 		world->calcMouseDirection();
// 		world->movePickedBodyTo();
// 	}
// }
// 
// void Evolution::handleMouseMotionRel(int x, int y)
// {
// 	std::cout << "rel" << std::endl;
// 	if ( world->mouselook )
// 	{
// 		world->relx = x;
// 		world->rely = y;
// 		eventsystem->activateKeystate(2248+0);
// 		eventsystem->activateKeystate(2248+1);
// 	}
// }

// COMMANDS
	void Evolution::selectCritterAll()
	{
		world->critterselection->clear();
		for ( unsigned int i=0; i < world->critters.size(); i++ )
		{
			world->critterselection->registerCritter(world->critters[i]);
// 			gui_selectCritter(i);
		}
	}

	// FIXME Move this shit to evolution.cpp?
	void Evolution::selectCritter(const unsigned int& c)
	{
	// 	m_canvas->swapChild("critterview");
		world->critterselection->selectCritterVID(c);
	}

// 	void Evolution::selectSpecies(const unsigned int& s)
// 	{
// 	// 	m_canvas->swapChild("critterview");
// // 		world->critterselection->selectCritterVID(c);
// 		std::cout << "select species " << s << std::endl;
// 	}

	void Evolution::decreaseenergy()
	{
		if ( ( (int)settings->getCVar("energy") - 1 ) >= 0 )
		{
			settings->setCVar("energy", settings->getCVar("energy")-1 );
// 			world->m_freeEnergy -= settings->getCVar("food_maxenergy");
			
// 			stringstream buf;
// 			buf << "energy: " << settings->getCVar("energy");
// 			m_logBuffer->add(buf);
		}
	}

	void Evolution::increaseenergy()
	{
		settings->setCVar("energy", settings->getCVar("energy")+1 );
// 		world->m_freeEnergy += settings->getCVar("food_maxenergy");

// 		stringstream buf;
// 		buf << "energy: " << settings->getCVar("energy");
// 		m_logBuffer->add(buf);
	}

	void Evolution::decreasefoodmaxenergy()
	{
		if ( ( (int)settings->getCVar("food_maxenergy") - 1 ) >= 0 )
		{
// 			world->m_freeEnergy -= settings->getCVar("energy");
			settings->setCVar("food_maxenergy", settings->getCVar("food_maxenergy")-1 );
		}
	}

	void Evolution::increasefoodmaxenergy()
	{
// 		world->m_freeEnergy += settings->getCVar("energy");
		settings->setCVar("food_maxenergy", settings->getCVar("food_maxenergy")+1 );
	}

	void Evolution::dec_worldsizex() { settings->decreaseCVar("worldsizeX", 1); world->makeFloor(); }
	void Evolution::inc_worldsizex() { settings->increaseCVar("worldsizeX", 1); world->makeFloor(); }
	void Evolution::dec_worldsizey() { settings->decreaseCVar("worldsizeY", 1); world->makeFloor(); }
	void Evolution::inc_worldsizey() { settings->increaseCVar("worldsizeY", 1); world->makeFloor(); }
	void Evolution::dec_worldsizez() { settings->decreaseCVar("worldsizeZ", 1); world->makeFloor(); }
	void Evolution::inc_worldsizez() { settings->increaseCVar("worldsizeZ", 1); world->makeFloor(); }

	void Evolution::camera_moveup()
	{
		BeSceneNode* const sceneNode=world->m_camera.getSceneNode();
		if(sceneNode)
		{
			sceneNode->translateLocal(btVector3(0,m_cameraTranslateSpeed,0));
		}
		world->movePickedBodyFrom();
	}
	void Evolution::camera_movedown()
	{
		BeSceneNode* const sceneNode=world->m_camera.getSceneNode();
		if(sceneNode)
		{
			sceneNode->translateLocal(btVector3(0,-m_cameraTranslateSpeed,0));
		}
		world->movePickedBodyFrom();
	}
	void Evolution::camera_moveforward()
	{ 
// 		if ( world->m_follow_critterP == 0 )
		{
			BeSceneNode* const sceneNode=world->m_camera.getSceneNode();
			if(sceneNode)
			{
				sceneNode->translateLocal(btVector3(0,0,-m_cameraTranslateSpeed));
			}
		}
// 		else
// 		{
// 			world->m_follow_critter_transform.setOrigin( world->m_follow_critter_transform.getOrigin() + btVector3(0,-m_cameraTranslateSpeed,0));
// 		}
		world->movePickedBodyFrom(); 
	}
	void Evolution::camera_movebackward() 	
	{ 
// 		if ( world->m_follow_critterP == 0 )
		{
			BeSceneNode* const sceneNode=world->m_camera.getSceneNode();
			if(sceneNode)
			{
				sceneNode->translateLocal(btVector3(0,0,m_cameraTranslateSpeed));
			}
		}
// 		else
// 		{
// 			world->m_follow_critter_transform.setOrigin( world->m_follow_critter_transform.getOrigin() + btVector3(0,m_cameraTranslateSpeed,0));
// 		}
		world->movePickedBodyFrom(); 
	}
	void Evolution::camera_moveleft() 		
	{ 
		BeSceneNode* const sceneNode=world->m_camera.getSceneNode();
		if(sceneNode)
		{
			sceneNode->translateLocal(btVector3(-m_cameraTranslateSpeed,0,0));
		}
		world->movePickedBodyFrom(); 
	}
	void Evolution::camera_moveright() 		
	{ 
		BeSceneNode* const sceneNode=world->m_camera.getSceneNode();
		if(sceneNode)
		{
			sceneNode->translateLocal(btVector3(m_cameraTranslateSpeed,0,0));
		}
		world->movePickedBodyFrom(); 
	}
	void Evolution::camera_lookup() 		
	{ 
		BeSceneNode* const sceneNode=world->m_camera.getSceneNode();
		if(sceneNode)
		{
			sceneNode->pitch(m_cameraRotateSpeed);
		}
		world->calcMouseDirection(); 
		world->movePickedBodyTo(); 
	}
	void Evolution::camera_lookdown() 		
	{ 
		BeSceneNode* const sceneNode=world->m_camera.getSceneNode();
		if(sceneNode)
		{
			sceneNode->pitch(-m_cameraRotateSpeed);
		}
		world->calcMouseDirection(); 
		world->movePickedBodyTo(); 
	}
	void Evolution::camera_lookleft()
	{ 
		BeSceneNode* const sceneNode=world->m_camera.getSceneNode();
		if(sceneNode)
		{
			sceneNode->yaw(m_cameraRotateSpeed); 
		}
		world->calcMouseDirection(); 
		world->movePickedBodyTo(); 
	}
	void Evolution::camera_lookright() 		
	{ 
		BeSceneNode* const sceneNode=world->m_camera.getSceneNode();
		if(sceneNode)
		{
			sceneNode->yaw(-m_cameraRotateSpeed);
		}
		world->calcMouseDirection(); 
		world->movePickedBodyTo(); 
	}
	void Evolution::camera_rollleft() 		
	{ 
		BeSceneNode* const sceneNode=world->m_camera.getSceneNode();
		if(sceneNode)
		{
			sceneNode->roll(m_cameraRotateSpeed); 		
		}
		world->calcMouseDirection(); 
		world->movePickedBodyTo(); 
	}
	void Evolution::camera_rollright() 		
	{ 
		BeSceneNode* const sceneNode=world->m_camera.getSceneNode();
		if(sceneNode)
		{
			sceneNode->roll(-m_cameraRotateSpeed); 
		}	
		world->calcMouseDirection(); 
		world->movePickedBodyTo(); 
	}
	
	
// 	void Evolution::camera_movedown() { world->camera.moveDown(0.01f); world->movePickedBodyFrom(); }
// 	void Evolution::camera_moveforward() { world->camera.moveForward(0.01f); world->movePickedBodyFrom(); }
// 	void Evolution::camera_movebackward() { world->camera.moveBackward(0.01f); world->movePickedBodyFrom(); }
// 	void Evolution::camera_moveleft() { world->camera.moveLeft(0.01f); world->movePickedBodyFrom(); }
// 	void Evolution::camera_moveright() { world->camera.moveRight(0.01f); world->movePickedBodyFrom(); }
// 	void Evolution::camera_lookup() { world->camera.lookUp(0.001f); world->calcMouseDirection(); world->movePickedBodyTo(); }
// 	void Evolution::camera_lookdown() { world->camera.lookDown(0.001f); world->calcMouseDirection(); world->movePickedBodyTo(); }
// 	void Evolution::camera_lookleft() { world->camera.lookLeft(0.001f); world->calcMouseDirection(); world->movePickedBodyTo(); }
// 	void Evolution::camera_lookright() { world->camera.lookRight(0.001f); world->calcMouseDirection(); world->movePickedBodyTo(); }
// 	void Evolution::camera_rollleft() { world->camera.rollLeft(0.001f); world->calcMouseDirection(); world->movePickedBodyTo(); }
// 	void Evolution::camera_rollright() { world->camera.rollRight(0.001f); world->calcMouseDirection(); world->movePickedBodyTo(); }
// /*/*/*	void Evolution::camera_lookhorizontal() { world->camera.lookRight((float)world->relx/3000); world->calcMouseDirection(); world->movePickedBodyTo(); }
// 	void Evolution::camera_lookvertical() { world->camera.lookDown((float)world->rely/3000); world->calcMouseDirection(); world->movePickedBodyTo(); }
// 	void Evolution::camera_movehorizontal() { world->camera.moveRight((float)world->relx/300); world->movePickedBodyFrom(); }
// 	void Evolution::camera_movevertical() { world->camera.moveDown((float)world->rely/300); world->movePickedBodyFrom(); }*/*/*/

	void Evolution::unregisterCritterVID(int vectorID)
	{
		world->critterselection->unregisterCritterVID(vectorID);
	}

	void Evolution::clear()
	{
		world->critterselection->clear();
	}

	void Evolution::selectBody()
	{
		if ( *drawscene == 1 || *drawdebug > 0 )
		{
			const int c = world->selectBody();
			if ( c != -1 )
				gui_selectCritter(c);
		}
	}

	void Evolution::gui_selectCritter(int c)
	{
		const auto cv( m_canvas->children["critterview"] );
		if ( cv )
		{
			cv->activate();
			m_canvas->raisePanel(cv.get());
			world->critterselection->selectCritterVID(c);
		}
	}

	void Evolution::gui_selectSpecies(int s)
	{
		const auto sv( m_canvas->children["speciesview"] );
		if ( sv )
		{
			sv->activate();
			m_canvas->raisePanel(sv.get());
			
			for ( unsigned int i(0); i < world->critters.size(); ++i )
			{
				if ( world->critters[i]->genotype == Genotypes::Instance()->list[s] )
					world->critterselection->registerCritter(world->critters[i]);
			}
			
// 			world->critterselection->selectCritterVID(c);
			
			
			
// 			std::cout << "select species " << s << std::endl;
		}
	}

	void Evolution::gui_swap()
	{
		m_canvas->swap();
	}
	void Evolution::movePickedBodyX( const float value )
	{
		if ( !world->mouselook )
		{
			world->mousex = value;
			world->calcMouseDirection();
			world->movePickedBodyTo();
		}
	}

	void Evolution::movePickedBodyY( const float value )
	{
		if ( !world->mouselook )
		{
			world->mousey = value;
			// world mouse dynamics
			world->calcMouseDirection();
			world->movePickedBodyTo();
		}		
	}

	void Evolution::camera_lookleftright_mouse( const float value )
	{
		if ( world->mouselook )
		{
			world->m_camera.getSceneNode()->yaw( -0.000003 * *m_cameraSensitivity_look * value );
		}
	}

	void Evolution::camera_lookupdown_mouse( const float value )
	{
		if ( world->mouselook )
		{
			world->m_camera.getSceneNode()->pitch( -0.000003 * *m_cameraSensitivity_look * value );
		}
	}

	
// 	void Evolution::canvas_press()
// 	{
// 		m_canvas->buttonPress( 1 );
// 		world->selectBody();
// 	}
// 	void Evolution::canvas_release()
// 	{
// 		m_canvas->buttonRelease( 1 );
// 	}
// 	void Evolution::canvas_pressAlt()
// 	{
// 		m_canvas->buttonPress( 3 );
// 		world->pickBody();
// 	}
// 	void Evolution::canvas_releaseAlt()
// 	{
// 		m_canvas->buttonRelease( 3 );
// 		world->unpickBody();
// 	}
	void Evolution::canvas_swapchild(const string& child)
	{
		m_canvas->swapChild( child );
	}

	

Evolution::~Evolution()
{
	delete m_glwindow;
	delete world;
}
