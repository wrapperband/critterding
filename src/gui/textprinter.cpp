#include "textprinter.h"
#include <GL/glew.h>
#include <iostream>
#include "math/vector2i.h"
#include "utils/ftgl/FTGL/ftgl.h"
#include "filesystem/be_filesystem.h"


Textprinter* Textprinter::Instance () 
{
	static Textprinter t;
	return &t;
}

Textprinter::Textprinter()
{
// 	fonts = 0;
	m_font = 0;
// 	if ( Settings::Instance()->getCVar("headless") == 0 )
// 		setUpFonts();
}

void Textprinter::print(int x, int y, const char *fmt, ...)
{
	if (fmt == NULL) return;
	if (!m_font) return;

	glPushMatrix();

// 		glTranslatef(x, y, 0);
		glTranslatef(-0.5f+x, +0.5f+y, 0);
		glScalef(1,-1,1);

		va_start(ap, fmt);
		char text[1024];
		vsprintf(text, fmt, ap);

		m_font->Render(text);

	glPopMatrix();
}

void Textprinter::printR(int x, int y, const char *fmt, ...)
{
	if (!m_font) return;

	va_start(ap, fmt);
	char text[1024];
	vsprintf(text, fmt, ap);
	
	glPushMatrix();

// 		glTranslatef(x - (int)m_font->BBox(text).Upper().X(), y, 0);
		glTranslatef(-0.5f +  x - (int)m_font->BBox(text).Upper().X(), +0.5f+y, 0);
		glScalef(1,-1,1);
		m_font->Render(text);

	glPopMatrix();
}

unsigned int Textprinter::getWidth(const std::string& str)
{
	if (!m_font) return 0;
	return static_cast<unsigned int>(m_font->BBox(str.c_str()).Upper().X());
}

unsigned int Textprinter::getWidth(const int& number)
{
	if (!m_font) return 0;
	std::stringstream sstream;
	sstream << number;

	return static_cast<unsigned int>(m_font->BBox(sstream.str().c_str()).Upper().X());
}

void Textprinter::getDimensions(const std::string& str, Vector2i& v)
{
	if (!m_font) return;
	FTBBox test = m_font->BBox(str.c_str());
	v.set(static_cast<int>(test.Upper().X()), static_cast<int>(test.Upper().Y()));
}

void Textprinter::print(int x, int y, const long unsigned int& longuint)
{
	std::stringstream sstream;
	sstream << longuint;
	print(x, y, sstream.str().c_str());
}

void Textprinter::print(const Vector2i& pos, const std::string& str)
{
	if (str.empty()) return;
	if (!m_font) return;

	glPushMatrix();

// 		glTranslatef(pos.m_x, pos.m_y, 0);
		glTranslatef(-0.5f+pos.m_x, +0.5f+pos.m_y, 0);
		glScalef(1,-1,1);
		m_font->Render(str.c_str());

	glPopMatrix();

// 	print(pos.m_x, pos.m_y, str.c_str());
}

void Textprinter::print(const Vector2i& pos, const int* num)
{
	std::stringstream str; str << *num;
	print(pos.m_x, pos.m_y, str.str().c_str());
}

void Textprinter::setUpFonts(BeFilesystem& filesystem)
{
	BeFile befileFont;
	if ( filesystem.load( befileFont, "font.ttf" ) )
	{
		
		// clear if exists
// 		if ( fonts )
// 		{
// 			delete m_font;
// 			delete fonts;
// 		}
		
		// Allocate an array to hold all fonts
// 		fonts = new FTFont *[1];

		
// 		font = new FTBitmapFont(file.c_str());
// 		font = new FTPixmapFont(file.c_str());
// 		font = new FTOutlineFont(file.c_str());
// 		font = new FTPolygonFont(file.c_str());
// 		font = new FTExtrudeFont(file.c_str());
// 		font = new FTTextureFont(file.c_str());
// 		font = new FTBufferFont(file.c_str());

		m_font = new FTTextureFont( befileFont.getFullFilename().c_str() );
		m_font->UseDisplayList(true);

		m_font->Depth(1);
		m_font->CharMap(ft_encoding_unicode);

		if(!m_font->FaceSize(11))
		{
			std::cerr << "Failed to set size" << std::endl;
			exit(1);
		}
		
	}

}
