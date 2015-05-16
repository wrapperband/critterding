#include "kernel/be_log.h"
#include "be_image.h"
#include <cassert>

BeImage::BeImage( const char* const path ) : m_internalFormat(0), m_width(0), m_height(0), m_format(0), m_pixels(0)
{
	const int flags=IMG_INIT_JPG|IMG_INIT_PNG|IMG_INIT_TIF;
	const int result=IMG_Init(flags);

	if ( flags != result )
		BE_ERROR("IMAGE error IMG_Init failed");
// 	assert();

	SDL_Surface* const surface = IMG_Load(path);
	if(surface)
	{
		bool supported=true;
		m_internalFormat = surface->format->BytesPerPixel;
		if(m_internalFormat==4)
		{
			if(surface->format->Rmask==0x000000ff)
			{
				m_format=GL_RGBA;
				//m_internalFormat=GL_COMPRESSED_RGBA;
			}
			else
			{
				m_format=GL_BGRA;
				//m_internalFormat=GL_COMPRESSED_RGBA;
			}
		}
		else if(m_internalFormat==3)
		{
			if(surface->format->Rmask==0x000000ff)
			{
				m_format=GL_RGB;
				//m_internalFormat=GL_COMPRESSED_RGB;
			}
			else
			{
				m_format=GL_BGR;
				//m_internalFormat=GL_COMPRESSED_RGB;
			}
		}
		else if(m_internalFormat==1)
		{
			m_format=GL_LUMINANCE;
			//m_internalFormat=GL_COMPRESSED_LUMINANCE;
		}
		else
		{
// 			fprintf(stderr, "Unsupported image: %s\nBytes per pixel: %u\nRmask %p\nGmask %p\nBmask %p\nAmask %p\n", path, surface->format->BytesPerPixel, (void*)surface->format->Rmask, (void*)surface->format->Gmask, (void*)surface->format->Bmask, (void*)surface->format->Amask );
			fprintf(stderr, "Unsupported image: %s\n", path );
			supported=false;
		}

		if(supported==true)
		{
			m_width=surface->w;
			m_height=surface->h;

			//Fill pixels flipping image vertically
			if(SDL_LockSurface(surface)==0)
			{
				const size_t	rows=surface->h;
				const size_t	rowSize=surface->pitch;
				const size_t	size=rows*rowSize;
				Uint8* const	pixels=static_cast<Uint8*>(surface->pixels);
				m_pixels=static_cast<Uint8*>(malloc(size));
				if(m_pixels)
				{
					size_t s=rows-1;
					for(size_t r=0; r<rows; ++r)
					{
						memcpy(m_pixels+r*rowSize, pixels+s*rowSize, rowSize);
						--s;
					}
				}
				SDL_UnlockSurface(surface);
			}
			SDL_FreeSurface(surface);
		}

	}
	else
		fprintf( stderr, "WARNING: Image could not be loaded: %s\n", path );
}

BeImage::~BeImage()
{
	if(m_pixels)
	{
		free(m_pixels);
	}
}
