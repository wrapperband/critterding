#ifndef BE_IMAGE_H_INCLUDED
#define BE_IMAGE_H_INCLUDED

#include <GL/glew.h>
#include <SDL/SDL_image.h>

class BeImage
{
public:
	BeImage(const char* const path);
	~BeImage();
	GLint getInternalFormat() const { return m_internalFormat; }
	GLsizei getWidth() const { return m_width; }
	GLsizei getHeight() const { return m_height; }
	GLenum getFormat() const { return m_format; }
	const GLvoid* getPixels() const { return m_pixels; }

private:
	GLint			m_internalFormat;
	GLsizei			m_width;
	GLsizei			m_height;
	GLenum			m_format;
	Uint8*			m_pixels;
};


#endif
