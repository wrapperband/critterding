#include "be_dir.h"
#include <iostream>
#include <sstream>

using namespace boost::filesystem;

Dir::Dir()
{
}

bool Dir::exists(const Path& directory) const
{
	const path directoryPath(directory);
	return boost::filesystem::exists(directoryPath) && is_directory(directoryPath);
}

void Dir::make(const Path& directory) const
{
	const path directoryPath(directory);
	create_directory(directoryPath);        
}

void Dir::listContents(const Path& dir, Paths& files, const bool fullPath) const
{
	if(exists(dir))
	{
		const Path directoryPath(dir);
		const directory_iterator end;	//default constructor constructs end iterator
		for( directory_iterator iter(directoryPath) ; iter != end ; ++iter )
		{
// 			if(is_regular_file(*iter))
// 			{
				if(fullPath)
				{
// 					Path finalPath(dir);
// 					finalPath += "/";
// 					finalPath += iter->leaf();
					
// 					std::stringstream fullSS;
// 					fullSS << iter->path().string();
// 					std::cout << iter->path().filename() << std::endl;
					
// 					finalPath += iter->path().filename();
					
// 					files.push_back(finalPath);
					files.push_back(iter->path().string());
// 					std::cout << fullSS.str() << std::endl;
				}
				else
				{
// 					files.push_back(iter->leaf());
				}
// 			}
		}
	}
}

void Dir::listContentsFull(const Path& dir, Paths& files) const
{
	listContents(dir, files, true);
}


Dir::~Dir()
{
}