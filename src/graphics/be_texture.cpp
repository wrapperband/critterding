#include "be_texture.h"

BeTexture::BeTexture( const GLenum target ) : m_target(target), m_handle(0)
{
	m_graphicsKernel.genTextures(1, &m_handle);
}

BeTexture::~BeTexture()
{
	m_graphicsKernel.deleteTextures(1, &m_handle);
}

void BeTexture::bind()
{
	m_graphicsKernel.bindTexture(m_target, m_handle);
}

void BeTexture::unbind()
{
	m_graphicsKernel.bindTexture(m_target, 0);
}

void BeTexture::parameter( const GLenum name, const GLint param )
{
	bind();
	m_graphicsKernel.texParameteri(m_target, name, param);
	unbind();
}

void BeTexture::parameter( const GLenum name, const GLfloat param )
{
	bind();
	//m_graphicsKernel.texParameterf(m_target, name, param);
	glTexParameterf(m_target, name, param);
	unbind();
}

void BeTexture::data( const GLint level, const GLint internalformat, const GLsizei width, const GLsizei height, const GLint border, const GLenum format, const GLenum type, const GLvoid* const data )
{
	bind();
	m_graphicsKernel.texImage2D(m_target, level, internalformat, width, height, border, format, type, data);
	unbind();
}

void BeTexture::generateMipmap()
{
	bind();
	m_graphicsKernel.generateMipmap(m_target);
	unbind();
}
