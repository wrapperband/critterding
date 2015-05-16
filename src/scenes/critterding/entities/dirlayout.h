#ifndef DIRLAYOUT_H
#define DIRLAYOUT_H

#include "filesystem/be_dir.h"
#include <iostream>
#include <string>
#include <cstdlib>

class Dirlayout
{
	public:
		static Dirlayout* Instance();
		void createDirs();
		std::string getHomedir();

// 		std::string findInData( const std::string& file );

		std::string homedir;
		std::string progdir;
		std::string savedir;
		std::string loaddir;
		std::string exchangedir;

	protected:
		Dirlayout();
	private:
		static Dirlayout* _instance;

		std::vector<std::string> searchPathData;
		Dir dirH;
};

#endif
