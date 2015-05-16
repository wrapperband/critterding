#ifndef BE_TEXTURE_H_INCLUDED
#define BE_TEXTURE_H_INCLUDED

#include <GL/glew.h>
#include <string>

#include "be_graphics_kernel.h"
#include "common/be_image.h"

#include "kernel/be_profiler.h"

class BeTexture
{
public:
protected:
	BeTexture(const GLenum target);
	~BeTexture();
public:
	GLuint handle() const { return m_handle; }

	void bind();
	void unbind();

	void parameter(const GLenum name, const GLint param);
	void parameter(const GLenum name, const GLfloat param);
	void data(const GLint level, const GLint internalformat, const GLsizei width, const GLsizei height, const GLint border, const GLenum format, const GLenum type, const GLvoid* const data);
	void generateMipmap();
private:
	BeGraphicsKernel	m_graphicsKernel;
	const GLenum	m_target;
	GLuint			m_handle;
};

class BeTexture2D : public BeTexture
{
public:
	BeTexture2D() : BeTexture(GL_TEXTURE_2D) {}
	BeTexture2D(const GLenum internalFormat, const GLsizei width, const GLsizei height) : BeTexture(GL_TEXTURE_2D) 
	{
		GLenum format=0;
		GLenum type=0;

		switch(internalFormat)
		{
			//Integral RGB
		case 3:
		case GL_RGB:
		case GL_RGB8:
			format = GL_RGB;
			type = GL_UNSIGNED_BYTE;
			break;

			//Integral RGBA
		case 4:
		case GL_RGBA:
		case GL_RGBA8:
			format = GL_RGBA;
			type = GL_UNSIGNED_BYTE;
			break;

			//Floating point RGB
		case GL_RGB16F:
		case GL_RGB32F:
			format = GL_RGB;
			type = GL_FLOAT;
			break;

			//Floating point RGBA
		case GL_RGBA16F:
		case GL_RGBA32F:
			format = GL_RGBA;
			type = GL_FLOAT;
			break;

		}
		data(0, internalFormat, width, height, 0, format, type, 0);
		parameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		parameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
	~BeTexture2D() {}
};

class BeImageTexture2D : public BeTexture2D
{
public:
	BeImageTexture2D(const std::string& path)
	{
		BeImage image(path.c_str());
		data(0, image.getInternalFormat(), image.getWidth(), image.getHeight(), 0, image.getFormat(), GL_UNSIGNED_BYTE, image.getPixels());
		generateMipmap();
	}
};

#endif
