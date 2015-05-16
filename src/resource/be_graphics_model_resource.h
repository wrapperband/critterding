#ifndef BE_GRAPHICS_MODEL_RESOURCE_H_INCLUDED
#define BE_GRAPHICS_MODEL_RESOURCE_H_INCLUDED

#include "graphics/be_texture.h"
// #include "thread/be_worker_pool.h"
// #include "boost/thread/future.hpp"
// #include "boost/threadpool/future.hpp"
#include "common/be_model_system.h"

template<typename T>
class BeUnalign
{
public:
	BeUnalign(const T& value)
	{
		m_value = new T;
		*m_value = value;
	}
	BeUnalign(const BeUnalign& other)
	{
		m_value = new T;
		*m_value = *other.m_value;
	}
	~BeUnalign()
	{
		delete m_value;
	}
	operator T&()
	{
		return *m_value;
	}
private:
	T* m_value;
};

class BeGraphicsModelResource
{
public:

	enum Status
	{
		eStatusLoading,
		eStatusError,
		eStatusReady
	};

	BeGraphicsModelResource(BeFilesystem& filesystem, const std::string& path, boost::shared_ptr<BeGraphicsSystem> graphicsSystem, boost::shared_ptr<BeGraphicsModelSystem> graphicsModelSystem, const btVector3& scale, const btTransform& transform) : m_status(eStatusLoading)
	{
// 		std::cout << "BeGraphicsModelResource upload " << path << std::endl;
// 		m_futureGraphicsModel=threadPool->schedule< boost::shared_ptr<BeGraphicsModel> >(boost::bind(&BeGraphicsModelResource::loadModel, this, graphicsSystem, filesystem, path, BeUnalign<btVector3>(scale), BeUnalign<btTransform>(transform)));
		
		m_graphicsModel=loadModel(graphicsSystem, filesystem, path, BeUnalign<btVector3>(scale), BeUnalign<btTransform>(transform));
// 		m_graphicsModel=loadModel(graphicsSystem, filesystem, path, BeUnalign<btVector3>(scale), BeUnalign<btTransform>(transform));
		m_graphicsModel->upload(graphicsModelSystem->m_textureStore);
		
		m_status=eStatusReady;
// 		std::cout << "  ok" << std::endl;
	}

	boost::shared_ptr<BeGraphicsModel> get()
	{
		return isReady() ? m_graphicsModel : boost::shared_ptr<BeGraphicsModel>();
	}

// 	void update()
// 	{
// // 		std::cout << "a" << std::endl;
// 		if(m_status==eStatusLoading)
// 		{
// // 			std::cout << "b" << std::endl;
// // 			if(m_futureGraphicsModel.is_ready())
// 			{
// // 				if ( m_futureGraphicsModel )
// 				{
// 					boost::shared_ptr<BeGraphicsModel> testGraphicsModel=m_futureGraphicsModel.get();
// 					if ( testGraphicsModel )
// 					{
// 
// // 						std::cout << "c" << std::endl;
// 						m_graphicsModel=testGraphicsModel;
// // 						std::cout << "d" << std::endl;
// 						m_graphicsModel->upload(m_graphicsModelSystem->m_textureStore);
// // 						std::cout << "e" << std::endl;
// 						m_status=eStatusReady;
// // 						std::cout << "f" << std::endl;
// 					}
// 					else
// 					{
// 						m_status=eStatusError;
// // 						std::cout << "client model: loading failed" << std::endl;
// 					}
// 				}
// 			}
// // 			std::cout << "g" << std::endl;
// 		}
// // 		std::cout << "h" << std::endl;
// 	}

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

	boost::shared_ptr<BeGraphicsModel> loadModel( boost::shared_ptr<BeGraphicsSystem> graphicsSystem, BeFilesystem& filesystem, const std::string& path, const btVector3& scale, const btTransform& transform )
	{
		BeFile objBeFile;
		if ( filesystem.load(objBeFile, path) )
		{
			BeObjLoader obj( filesystem, objBeFile, scale, transform );
			boost::shared_ptr<BeGraphicsModel> m(new BeGraphicsModel( graphicsSystem, path ));

			// 	cerr << "model: " << m->name << endl;
			// 	cerr << "number of triangles: " << (m->m_numberVIndices/3) << endl;

			// copy materials
			m->matlist = obj.matlist;
			// copy face materials
			m->facematerials = obj.facematerials;

			m->setNumberVertices( obj.getNumberVIndices()*3 );
			m->setNumberNormals( obj.getNumberVIndices()*3 );
			m->setNumberTexCoor( obj.getNumberVIndices()*2 );
			m->setNumberVIndices( obj.getNumberVIndices() );

			for ( unsigned int i=0; i < obj.getNumberVIndices(); i++ )
			{
				m->indices[i] = i;
				if ( obj.gVIndices[i] > 0 )
				{
					const int index = (obj.gVIndices[i] - 1) * 3;
					memcpy(&m->vertices[(i*3)], &obj.gVertices[index], sizeof(float)*3);
				}
				else
				{
					memset(&m->vertices[(i*3)], 0, sizeof(float)*3);
				}
				if ( obj.gTIndices[i] > 0 )
				{
					const int tindex = (obj.gTIndices[i] - 1) * 2;
					memcpy(&m->texcoor[(i*2)], &obj.gTexCoor[(tindex)], sizeof(float)*2);
				}
				else
				{
					memset(&m->texcoor[(i*2)], 0, sizeof(float)*2);
				}
				if ( obj.gNIndices[i] > 0 )
				{
					const int nindex = (obj.gNIndices[i] - 1) * 3;
					memcpy(&m->normals[(i*3)], &obj.gNormals[nindex], sizeof(float)*3);
				}
				else
				{
					memset(&m->normals[(i*3)], 0, sizeof(float)*3);
				}
			}
			return m;
		}
		else
		{
			std::cout << "client model: loading failed: '" << path << "'" << std::endl;
		}
		
		m_status = eStatusError;
		return boost::shared_ptr<BeGraphicsModel>();
	}

	Status m_status;
	//boost::shared_ptr<BeTexture2D> m_texture;
// 	boost::shared_future< boost::shared_ptr<BeGraphicsModel> > m_futureGraphicsModel;
	boost::shared_ptr<BeGraphicsModel> m_graphicsModel;
// 	boost::shared_ptr<BeGraphicsModelSystem> m_graphicsModelSystem;

};

#endif