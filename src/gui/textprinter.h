#ifndef TEXTPRINTER_H
#define TEXTPRINTER_H

// #include <stdarg.h>
#include <string>

class Vector2i;
class FTTextureFont;
class BeFilesystem;

class Textprinter
{
	public:
		static Textprinter* Instance();
		void printTextprinter();

		// get bounding boxes
		unsigned int getWidth(const std::string& str);
		unsigned int getWidth(const int& number);
		void getDimensions(const std::string& str, Vector2i& v);

		// print left aligned
		void print(int x, int y, const char *fmt, ...);
		void print(int x, int y, const long unsigned int& longuint);
		void print(const Vector2i& pos, const std::string& str);
		void print(const Vector2i& pos, const int* num);
		// print right aligned
		void printR(int x, int y, const char *fmt, ...);

		void setUpFonts(BeFilesystem& filesystem);
		FTTextureFont* m_font;
	protected:
		Textprinter();
		virtual ~Textprinter()
		{
// 			if ( m_font )
// 			{
// 				/* Destroy the font object. */
// // 				FTGL::ftglDestroyFont(fonts[0]);
// 				delete m_font;
// // 				delete *fonts;
// 			}
		};
	private:
// 		FTFont** fonts;
// 		FTTextureFont* m_font;
		va_list ap;

// 		typedef boost::shared_array<char> charPtr;
// 		charPtr getChar( const std::string& str )
// 		{
// 			charPtr writable(new char[str.size() + 1]);
// 			std::copy(str.begin(), str.end(), writable.get());
// 			writable[str.size()] = '\0';
// 			return writable;
// 		}
};

#endif
