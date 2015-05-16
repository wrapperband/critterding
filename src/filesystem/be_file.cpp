// #include <cstring>
#include "be_file.h"

#include <iostream>
#include <fstream>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/bzip2.hpp>

BeFile::BeFile()
{
}

bool BeFile::setFilename( const std::string& fullfilename )
{
	size_t pos = fullfilename.find_last_of("/", fullfilename.size());
	if ( pos != std::string::npos )
	{
		m_directory = fullfilename.substr( 0, pos+1 );
		m_filename = fullfilename.substr( pos+1, fullfilename.size() );
		return setFilename( fullfilename.substr( 0, pos+1 ), fullfilename.substr( pos+1, fullfilename.size() ) );
	}
	return false;
}

bool BeFile::setFilename( const std::string& directory, const std::string& filename )
{
	m_directory = directory;
	m_filename = filename;

	m_fullfilename = m_directory + m_filename;
	return loadFile( );
}

std::string BeFile::getFilename( ) const
{
	return m_filename;
}

std::string BeFile::getFullFilename( ) const
{
	return m_fullfilename;
}

std::string BeFile::getDirectory( ) const
{
	return m_directory;
}

const std::stringstream& BeFile::getContent( )
{
	return m_content;
}

bool BeFile::loadFile( )
{
// 	std::cout << "loading " << m_fullfilename << std::endl;

	std::ifstream fstream( m_fullfilename.c_str(), std::ios::in | std::ios::binary );
	try
	{
		fstream.exceptions(fstream.failbit);
	}
	catch (const std::ios_base::failure& e)
	{
		std::cerr << "  ERROR::BeFile::loadFile::ios_base::failure: " << e.what() << std::endl;
		std::cerr << "  ERROR  ::filename: " << m_fullfilename << std::endl;
		return false;
	}
	
	if ( fstream.is_open() )
	{
		if ( fstream.good() )
		{
			boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
			
			bool is_compressed(false);
			if ( m_fullfilename.find(".bz2") != std::string::npos )
				is_compressed = true;

			if ( is_compressed )
			{
				try
				{
					in.push(boost::iostreams::bzip2_decompressor());
					in.push(fstream);
					boost::iostreams::copy(in, m_content);
				}
				catch(const boost::iostreams::bzip2_error& exception)
				{
					const int error = exception.error();

					if(error == boost::iostreams::bzip2::data_error)
					{
						std::cerr << "  ERROR::BeFile::loadFile::bzip2: compressed data stream is corrupted" << std::endl;
					}
					else if(error == boost::iostreams::bzip2::data_error_magic)
					{
						std::cerr << "  ERROR::BeFile::loadFile::bzip2: compressed data stream does not begin with the 'magic' sequence 'B' 'Z' 'h'" << std::endl;
					}
					else if(boost::iostreams::bzip2::config_error) {
						std::cerr << "  ERROR::BeFile::loadFile::bzip2: libbzip2 has been improperly configured for the current platform" << std::endl;
					}
					std::cerr << "  ERROR  ::filename: " << m_fullfilename << std::endl;

					fstream.close();
					return false;
				}
			}
			else
			{
				in.push(fstream);
				boost::iostreams::copy(in, m_content);
			}


			fstream.close();
			return true;
		}
		fstream.close();
	}

	return false;
}

bool BeFile::getLine( std::string& line )
{
	if ( !m_content.eof() )
	{
		char str[500000];
// 		memset(str, 0, 500000);
		
		m_content.getline(str,500000);
		line = str;

		if ( !line.empty() )
		{
			const auto last_pos( line.size()-1 );
			if ( line.substr( last_pos, 1 ) == "\r" )
				line = line.substr( 0, last_pos );
			if ( line.size() > 0 )
				if ( line.substr( last_pos, 1 ) == "\0" )
					line = line.substr( 0, last_pos );
		}
		return true;
	}
	return false;
}

void BeFile::appendLine( const std::string& append )
{
	m_content << append;
}

void BeFile::prependLine( const std::string& prepend )
{
	std::string content_copy(m_content.str());
	
	m_content.str(prepend + content_copy);
// 	m_content.clear();
// 	m_content << prepend << content_copy;
}

BeFile::~BeFile()
{
}
