#ifndef TEXTURESTORE_H_INCLUDED
#define TEXTURESTORE_H_INCLUDED

#include <map>
#include <string>
#include "graphics/be_texture.h"
#include "resource/be_texture_2d_resource.h"
#include <boost/shared_ptr.hpp>

class Texturestore
{
	public:
		Texturestore() {};
		~Texturestore() {};
		boost::shared_ptr<BeTexture2DResource> load(const std::string& filename);
// 		boost::shared_ptr<BeWorkerPool> m_threadPool; 
// 		std::map<std::string, boost::shared_ptr<BeTexture2DResource> > m_resources;
};

#endif
