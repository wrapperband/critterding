#ifndef BE_TEXTURE_2D_RESOURCE_H_INCLUDED
#define BE_TEXTURE_2D_RESOURCE_H_INCLUDED

#include "graphics/be_texture.h"
// #include "thread/be_worker_pool.h"
#include "filesystem/be_filesystem.h"
#include "utils/settings.h"
// #include <boost/thread/future.hpp>
// #include "boost/threadpool/future.hpp"

// #include <qt4/Qt/qimage.h>
// #include <QtGui>
// #include <QGLWidget>
#include <QGLWidget>


class BeTexture2DResource
{
public:
	
	enum Status
	{
		eStatusLoading,
		eStatusError,
		eStatusReady
	};

	BeTexture2DResource(BeFilesystem& filesystem, const std::string& path) : m_status(eStatusLoading)
	{
// 		m_futureImage=threadPool->schedule< boost::shared_ptr<QImage> >(boost::bind(&BeTexture2DResource::loadImage, this, path));
// 		m_texture=boost::shared_ptr<BeTexture2D>(new BeTexture2D);

// 		std::cout << "BeTexture2DResource upload " << path << std::endl;
	
		const std::string& fullfilename( filesystem.getPath(path) );
		if ( !fullfilename.empty() )
		{
				m_texture=boost::shared_ptr<BeTexture2D>(new BeTexture2D);
				boost::shared_ptr<QImage> image=loadImage(fullfilename);
				if(image->bits())
				{
                    m_texture->data(0, 4, image->width(), image->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image->bits());

					m_texture->parameter(GL_TEXTURE_MAX_LEVEL,4);
// 					m_texture->parameter(GL_TEXTURE_MAX_LOD,4);

                    m_texture->parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                    m_texture->parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                    m_texture->parameter(GL_TEXTURE_MAX_ANISOTROPY_EXT, 1.0f);
//                     m_texture->parameter(GL_TEXTURE_MAX_ANISOTROPY_EXT, Settings::Instance()->getCVar("texture_anisotropy"));
					
                    m_texture->generateMipmap();
					m_status=eStatusReady;
				}
				else
				{
					m_status=eStatusError;
				}
		}
		m_status=eStatusReady;
// 		std::cout << "  ok" << std::endl;
		
	}

	BeTexture2DResource(const std::string& path) : m_status(eStatusLoading)
	{
// 		m_futureImage=threadPool->schedule< boost::shared_ptr<QImage> >(boost::bind(&BeTexture2DResource::loadImage, this, path));
// 		m_texture=boost::shared_ptr<BeTexture2D>(new BeTexture2D);

// 		std::cout << "BeTexture2DResource upload " << path << std::endl;
	
				m_texture=boost::shared_ptr<BeTexture2D>(new BeTexture2D);
				boost::shared_ptr<QImage> image=loadImage(path);
				if(image->bits())
				{
                    m_texture->data(0, 4, image->width(), image->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image->bits());

					m_texture->parameter(GL_TEXTURE_MAX_LEVEL,4);
// 					m_texture->parameter(GL_TEXTURE_MAX_LOD,4);

                    m_texture->parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                    m_texture->parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                    m_texture->parameter(GL_TEXTURE_MAX_ANISOTROPY_EXT, 1.0f);
//                     m_texture->parameter(GL_TEXTURE_MAX_ANISOTROPY_EXT, Settings::Instance()->getCVar("texture_anisotropy"));
					
                    m_texture->generateMipmap();
					m_status=eStatusReady;
				}
				else
				{
					m_status=eStatusError;
				}
		m_status=eStatusReady;
// 		std::cout << "  ok" << std::endl;
		
	}

	boost::shared_ptr<BeTexture2D> get()
	{
		return isReady() ? m_texture : boost::shared_ptr<BeTexture2D>();
	}

	void update()
	{
// 		if(m_status==eStatusLoading)
// 		{
// 			if(m_futureImage.is_ready())
// 			{
// 				boost::shared_ptr<QImage> image=m_futureImage.get();
// 				if(image->bits())
// 				{
//                     m_texture->data(0, 4, image->width(), image->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image->bits());
// 					
// 					m_texture->parameter(GL_TEXTURE_MAX_LEVEL,4);
// // 					m_texture->parameter(GL_TEXTURE_MAX_LOD,4);
// 
//                     m_texture->parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//                     m_texture->parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
// 
// //                     m_texture->parameter(GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
//                     m_texture->parameter(GL_TEXTURE_MAX_ANISOTROPY_EXT, Settings::Instance()->getCVar("texture_anisotropy"));
// 					
//                     m_texture->generateMipmap();
// 					m_status=eStatusReady;
// 				}
// 				else
// 				{
// 					m_status=eStatusError;
// 				}
// 			}
// 		}
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

	boost::shared_ptr<QImage> loadImage(const std::string& path)
	{
        QImage imageA(path.c_str());
        QImage imageB = QGLWidget::convertToGLFormat(imageA);
        return boost::shared_ptr<QImage>(new QImage(imageB));
	}

	Status m_status;
	boost::shared_ptr<BeTexture2D> m_texture;

};

#endif