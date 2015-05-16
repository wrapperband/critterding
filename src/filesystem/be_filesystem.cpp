// boost::shared_ptr<BeGraphicsModel> m(new BeGraphicsModel( m_fileSystem.getDataRoot(path) ));
#include "be_filesystem.h"

using namespace boost::filesystem;

#include <iostream>
#include <fstream>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/bzip2.hpp>

#if defined(WIN32)
#	include <shlobj.h> // for SHGetFolderPath and CSIDL_PERSONAL
#endif


BeFilesystem::BeFilesystem() : m_logDebug("FILE")
{
}

		std::string BeFilesystem::getHomedir( const std::string& scene_name )
		{
			std::string home;
			#if !defined(WIN32)
				home = getenv("HOME");
				home.append( "/."+scene_name+"/" );
			#else
				char mydoc[MAX_PATH];
				memset(mydoc, 0, sizeof(mydoc));
				SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, mydoc);
				home = std::string(mydoc) + "\\"+scene_name+"\\";
			#endif
				std::cout << home << std::endl;
			return home;
		}

bool BeFilesystem::load( BeFile& befile, const std::string& file )
{
	// search root paths for file
	for ( unsigned int i=0; i < berootpaths.list.size(); i++ )
	{
		std::string fullfilename( berootpaths.list[i] );
		fullfilename.append( file );

		m_logDebug << "::FILE checking " << fullfilename << "\n";
// 		std::cerr << "checking " << fullfilename << std::endl;
		if ( boost::filesystem::exists( fullfilename ) )
		{
			m_logDebug << "::FILE loading " << fullfilename << "\n";
			if ( befile.setFilename( fullfilename ) )
			{
				return true;
			}
			return false;
		}
	}

	m_logDebug << "::FILE warning: cannot find file '" << file << "'\n";
	return false;
}

std::string BeFilesystem::getPath( const std::string& file )
{
	// search root paths for file
	for ( unsigned int i=0; i < berootpaths.list.size(); i++ )
	{
		std::string fullfilename( berootpaths.list[i] );
		fullfilename.append( file );
		
// 		std::cout << "testing " << fullfilename << std::endl;
		if ( boost::filesystem::exists( fullfilename ) )
		{
			return fullfilename;
		}
	}

	m_logDebug << "::FILE warning: cannot find file '" << file << "'\n";
	return "";
}

bool BeFilesystem::save(const std::string &filename, const std::string& content)
{
	std::fstream file_op(filename.c_str(),std::ios::out);

	try
	{
		file_op.exceptions(file_op.failbit);
	}
	catch (const std::ios_base::failure& e)
	{
		std::cerr << "  ERROR::BeFilesystem::save::ios_base::failure: " << e.what() << std::endl;
		std::cerr << "  ERROR  ::filename: " << filename << std::endl;
		return false;
	}
	
	if (file_op.is_open())
	{
		if (file_op.good())
		{
			file_op << content;
			file_op.close();
			return true;
		}
		file_op.close();
	}
	return false;
}

bool BeFilesystem::save_bz2(const std::string& filename, const std::string& content)
{
// 	std::cout << "saving" << filename << ".bz2" << std::endl;
// 				try{
// 					boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
// 					in.push(boost::iostreams::bzip2_decompressor());
// 					in.push(fstream);
// 					boost::iostreams::copy(in, m_content);
// 				}
// 				catch(boost::iostreams::bzip2_error& error)
// 				{
	
	
	std::istringstream uncompressed_string(content);
	if (uncompressed_string.good())
	{
		const std::string& newfilename( filename+".bz2" );

		std::ostringstream compressed_string;
		try
		{
			boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
			in.push(boost::iostreams::bzip2_compressor());
			in.push(uncompressed_string);

			boost::iostreams::copy(in, compressed_string);
		}
		catch(const boost::iostreams::bzip2_error& exception)
		{
			const int error = exception.error();

			if(error == boost::iostreams::bzip2::data_error)
			{
				std::cerr << "  ERROR::BeFilesystem::save_bz2::bzip2: compressed data stream is corrupted" << std::endl;
			}
			else if(error == boost::iostreams::bzip2::data_error_magic)
			{
				std::cerr << "  ERROR::BeFilesystem::save_bz2::bzip2: compressed data stream does not begin with the 'magic' sequence 'B' 'Z' 'h'" << std::endl;
			}
			else if(boost::iostreams::bzip2::config_error) {
				std::cerr << "  ERROR::BeFilesystem::save_bz2::bzip2: libbzip2 has been improperly configured for the current platform" << std::endl;
			}
			std::cerr << "  ERROR  ::filename: " << newfilename << std::endl;

			return false;
		}

		if (compressed_string.good())
		{
			std::fstream file_op(newfilename.c_str(),std::ios::out | std::ios::binary);

			try
			{
				file_op.exceptions(file_op.failbit);
			}
			catch (const std::ios_base::failure& e)
			{
				std::cerr << "  ERROR::BeFilesystem::save_bz2::ios_base::failure: " << e.what() << std::endl;
				std::cerr << "  ERROR  ::filename: " << newfilename << std::endl;
				return false;
			}

			if (file_op.is_open())
			{
				if (file_op.good())
				{
					file_op << compressed_string.str();
					file_op.close();
					return true;
				}
				file_op.close();
			}
		}

	}

	return false;	
}

void BeFilesystem::rename(const std::string& filename, const std::string& filename_new)
{
	boost::filesystem::rename( filename, filename_new );
	
}

void BeFilesystem::rm(const std::string& filename)
{
	if( remove( filename.c_str() ) != 0 )
		m_logDebug << "::FILE error: deleting file " << filename << "\n";
// 	std::cerr << "Error deleting file" << std::endl;
// 	else
// 		cerr << "file succesfully deleted" << endl;
}

BeFilesystem::~BeFilesystem()
{
}
