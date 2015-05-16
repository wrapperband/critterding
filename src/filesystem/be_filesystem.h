#ifndef BE_FILESYSTEM_H_INCLUDED
#define BE_FILESYSTEM_H_INCLUDED

#include <boost/filesystem.hpp>
#include "kernel/be_log.h"
#include "be_rootpaths.h"
#include "be_file.h"

#include <string>

class BeFilesystem
{
	public:
		BeFilesystem();
		~BeFilesystem();

		void createDirs();

		std::string findInData( const std::string& file );
		bool load( BeFile& befile, const std::string& file );
		std::string getPath( const std::string& file );
		bool save(const std::string& filename, const std::string& content);
		bool save_bz2(const std::string& filename, const std::string& content);
		void rm(const std::string& filename);
		void rename(const std::string& filename, const std::string& filename_new);

		BeRootPaths& getRootPaths() { return berootpaths; }

		std::string getHomedir( const std::string& scene_name );
	private:
		static BeFilesystem* _instance;

		BeRootPaths berootpaths;
		BeLogDebug m_logDebug;
};

#endif
