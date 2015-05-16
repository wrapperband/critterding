#ifndef BE_FILEBUFFER_H_INCLUDED
#define BE_FILEBUFFER_H_INCLUDED

// #include <string>
#include <sstream>

class BeFile
{
	public:
		BeFile();
		~BeFile();

		void clear();
		bool setFilename( const std::string& fullfilename );
		bool setFilename( const std::string& directory, const std::string& filename );

		std::string getFilename( ) const;
		std::string getFullFilename( ) const;
		std::string getDirectory( ) const;
		bool loadFile( );
		const std::stringstream& getContent( );
		bool getLine( std::string& line );
		void appendLine( const std::string& append );
		void prependLine( const std::string& prepend );

	protected:
	private:
		std::string m_filename;
		std::string m_directory;
		std::string m_fullfilename;
		std::stringstream m_content;
};

#endif
