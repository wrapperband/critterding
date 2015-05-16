#ifndef SC_CLIENT_MAP_RESOURCE_H_INCLUDED
#define SC_CLIENT_MAP_RESOURCE_H_INCLUDED

#include "graphics/be_texture.h"
// #include "thread/be_worker_pool.h"
// #include "boost/threadpool/future.hpp"

#include "scenes/critterding/entities/map.h"

class ScClientMapResource
{
public:

// 	enum Status
// 	{
// 		eStatusLoading,
// 		eStatusError,
// 		eStatusReady
// 	};

	ScClientMapResource(BeFilesystem& filesystem, const std::string& path, boost::shared_ptr<BeGraphicsSystem> graphicsSystem, boost::shared_ptr<BeGraphicsModelSystem> modelSystem)
	{
// 		m_future=threadPool->schedule< boost::shared_ptr<ClientMap> >(boost::bind(&ScClientMapResource::load, this, filesystem, path));
// 		std::cout << "upload " << path << std::endl;
// 		load(filesystem, path);
		m_map.reset(new ClientMap(filesystem, path));		
		m_map->upload(graphicsSystem, modelSystem);
// 		m_status=eStatusReady;
// 		std::cout << "  ok" << std::endl;
	}

	boost::shared_ptr<ClientMap> get()
	{
		return isReady() ? m_map : boost::shared_ptr<ClientMap>();
	}

	bool isReady()
	{
		return true;
	}
// 	bool isReady()
// 	{
// 		update();
// 		return m_status==eStatusReady;
// 	}

private:

// 	boost::shared_ptr<ClientMap> load(BeFilesystem& filesystem, const std::string& path)
// 	{
// 		m_map.reset(new ClientMap(filesystem, path));
// 		return m_map;
// 	}

// 	Status m_status;
// 	boost::shared_future< boost::shared_ptr<ClientMap> > m_future;
	boost::shared_ptr<ClientMap> m_map;
// 	boost::shared_ptr<BeGraphicsSystem> m_graphicsSystem; 
// 	boost::shared_ptr<BeGraphicsModelSystem> m_modelSystem;

};

#endif