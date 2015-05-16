#include "be_model_system.h"
#include "utils/texturestore.h"
#include "resource/be_graphics_model_resource.h"
#include <boost/smart_ptr/make_shared.hpp>

BeGraphicsModel::BeGraphicsModel( boost::shared_ptr<BeGraphicsSystem> system, const std::string& filename ) :
	m_filename(filename),
	m_numberVertices(0),
	m_numberNormals(0),
	m_numberTexCoor(0),
	m_numberVIndices(0),
	m_arrayBuffer(0),
	m_elementArrayBuffer(0),
	m_vertexArray(0),
	m_system(system),
	m_glint_size(sizeof(GLint)),
	m_glsl(Settings::Instance()->getCVarPtr("glsl"))
{
}

void BeGraphicsModel::upload(Texturestore &textureStore)
{
	
	buildElementArrayBuffer();
		assert(m_arrayBuffer==0);
		m_arrayBuffer.reset(new BeArrayBuffer);
		assert(m_arrayBuffer);

		m_arrayBuffer->data((m_numberVertices*sizeof(btScalar))+((m_numberNormals+m_numberTexCoor)*sizeof(GLfloat)), NULL, GL_STATIC_DRAW_ARB);
		m_arrayBuffer->subData(0, m_numberVertices*sizeof(btScalar), vertices.get());
		m_arrayBuffer->subData(m_numberVertices*sizeof(btScalar), m_numberNormals*sizeof(GLfloat), normals.get());
		m_arrayBuffer->subData((m_numberVertices*sizeof(btScalar))+(m_numberNormals*sizeof(GLfloat)), m_numberTexCoor*sizeof(GLfloat), texcoor.get());

		assert(m_elementArrayBuffer==0);
		m_elementArrayBuffer.reset(new BeElementArrayBuffer);
		assert(m_elementArrayBuffer);
		m_elementArrayBuffer->data(elementArrayBuffer.size()*m_glint_size, &elementArrayBuffer[0], GL_STATIC_DRAW);

		m_vertexArray.reset(new BeVertexArray);
		m_vertexArray->setElementArrayBuffer(m_elementArrayBuffer.get());
		m_vertexArray->vertexPointer( *m_arrayBuffer, 3, GL_FLOAT, 0, 0 );
		m_vertexArray->normalPointer( *m_arrayBuffer, GL_FLOAT, 0, m_numberVertices*sizeof(btScalar) );
		m_vertexArray->texCoordPointer( *m_arrayBuffer, 2, GL_FLOAT, 0, ( (m_numberVertices*sizeof(btScalar)) + (m_numberNormals*sizeof(GLfloat)) ) );
		m_vertexArray->enableClientState(GL_VERTEX_ARRAY);
		m_vertexArray->enableClientState(GL_NORMAL_ARRAY);
		m_vertexArray->enableClientState(GL_TEXTURE_COORD_ARRAY);

		// upload textures
		for ( matit = matlist.begin(); matit != matlist.end(); ++matit )
		{
			if ( matit->second.m_imagePath.size() )
			{
				matlist[ matit->first ].m_imageTexture2D = textureStore.load(matit->second.m_imagePath);
			}
			if ( matit->second.m_imagePathBump.size() )
			{
				matlist[ matit->first ].m_imageTexture2DBump = textureStore.load(matit->second.m_imagePathBump);
			}
		}
}

void BeGraphicsModel::buildElementArrayBuffer()
{
	matlist_iterator it=matlist.begin();
	for ( unsigned int i=0; i < matlist.size(); ++i)
	{
		std::pair<const std::string, Material> pair=*it;
		unsigned int first=elementArrayBuffer.size();
		unsigned int count=0;
		for ( unsigned int j=0; j < facematerials.size(); j++ )
		{
			const Facematerial& facematerial = facematerials[j];
			if(facematerial.material==pair.first)
			{
				for( unsigned int k=0; k < facematerial.faceindex_number; ++k)
				{
					elementArrayBuffer.push_back(indices[facematerial.faceindex_first+k]);
					++count;
				}
			}
		}
		DrawCall drawCall;
		drawCall.m_first=first;
		drawCall.m_count=count;
		m_drawCalls.push_back(drawCall);
		++it;
	}
}

void BeGraphicsModel::draw( unsigned int current_material, const btTransform& transform )
{
// 	btScalar m[16];
	transform.getOpenGLMatrix(m_matrix);
	
	glPushMatrix();
// 	m_system->matrixPush(GL_MODELVIEW);
	
		m_system->matrixMult(GL_MODELVIEW, m_matrix);
	// 	m_system->matrixLoad(GL_MODELVIEW, m);
		draw(current_material);

	glPopMatrix();
// 	m_system->matrixPop(GL_MODELVIEW);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
}

void BeGraphicsModel::draw( unsigned int current_material, const btTransform& transform, const btVector3& scale )
{
// 	btScalar m[16];
	transform.getOpenGLMatrix(m_matrix);
// 	m_system->matrixPush(GL_MODELVIEW);
	glPushMatrix();

// 	m_system->matrixMult(GL_MODELVIEW, m_matrix);
	m_system->matrixMult(m_matrix);
// 	m_system->matrixLoad(GL_MODELVIEW, m_matrix);
	
// 		m_system->matrixPush(GL_MODELVIEW);
// 		glPushMatrix();

			if ( scale.x() != 1.0f || scale.y() != 1.0f || scale.z() != 1.0f )
				m_system->matrixScale(GL_MODELVIEW, scale.x(), scale.y(), scale.z());
			
			draw(current_material);

// 		glPopMatrix();
// 		m_system->matrixPop(GL_MODELVIEW);

// 	m_system->matrixPop(GL_MODELVIEW);
	glPopMatrix();

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
}

void BeGraphicsModel::drawToDepth( const btTransform& transform )
{
// 	btScalar m[16];
	transform.getOpenGLMatrix(m_matrix);
	m_system->matrixPush(GL_MODELVIEW);
	m_system->matrixMult(GL_MODELVIEW, m_matrix);
	drawToDepth();

	m_system->matrixPop(GL_MODELVIEW);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
}

void BeGraphicsModel::draw( unsigned int current_material, const float* transform )
{
	glPushMatrix();
// 	m_system->matrixPush(GL_MODELVIEW);
// 		m_system->matrixMult(GL_MODELVIEW, transform);
		m_system->matrixMult(transform);
		draw(current_material);
	glPopMatrix();
// 	m_system->matrixPop(GL_MODELVIEW);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
}

// void BeGraphicsModel::draw( const float* transform, const float x, const float y, const float z )
void BeGraphicsModel::draw(unsigned int current_material, const float* transform, const btVector3& scale )
{
	glPushMatrix();
// 	m_system->matrixPush(GL_MODELVIEW);
// 	std::cout << "scale" << std::endl;
// 		m_system->matrixMult(GL_MODELVIEW, transform);
		m_system->matrixMult(transform);
		if ( scale.x() != 1.0f || scale.y() != 1.0f || scale.z() != 1.0f )
			m_system->matrixScale(GL_MODELVIEW, scale.x(), scale.y(), scale.z());
		draw(current_material);
	glPopMatrix();
// 	m_system->matrixPop(GL_MODELVIEW);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
}

// void BeGraphicsModel::draw( const float* transform, const float x, const float y, const float z )
void BeGraphicsModel::drawToDepth(const float* transform, const btVector3& scale )
{
	m_system->matrixPush(GL_MODELVIEW);
// 	std::cout << "scale" << std::endl;
	m_system->matrixMult(GL_MODELVIEW, transform);
	if ( scale.x() != 1.0f || scale.y() != 1.0f || scale.z() != 1.0f )
		m_system->matrixScale(GL_MODELVIEW, scale.x(), scale.y(), scale.z());
	drawToDepth();
	m_system->matrixPop(GL_MODELVIEW);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
}

unsigned int BeGraphicsModel::numberOfMaterials()
{
	return matlist.size();
}


void BeGraphicsModel::setContext( unsigned int current_material )
{
	m_system->bindVertexArray(m_vertexArray.get());

	if ( matlist.size() > 0 )
	{
		size_t i=0;
		matlist_iterator it=matlist.begin();
		while(it!=matlist.end())
		{
			if ( i == current_material )
			{
				const Material& material=it->second;

				if ( material.m_imageTexture2D )
				{
					m_system->bindTexture2D(material.m_imageTexture2D->get().get());
				}
				else
				{
					m_system->bindTexture2DWhitePixel();
				}
				
				if ( *m_glsl == 1 )
				{
					if ( material.m_imageTexture2DBump )
					{
						glUniform1i(m_system->getNormalMappingTexLoc(), 3); // 91 == tex1Loc in qt.cpp // FIXME MULTITEXTURE FIXME
						glUniform1i(m_system->getNormalMappingDo(), 1); // 89 == normal_map_do in qt.cpp // FIXME MULTITEXTURE FIXME
						m_system->bindMultiTexture2D(GL_TEXTURE3, material.m_imageTexture2DBump->get().get());
					}
					else
					{
						if ( m_system->getActiveProgram() != 0 )
							glUniform1i(m_system->getNormalMappingDo(), 0); // 91 == tex1Loc in qt.cpp // FIXME MULTITEXTURE FIXME
							
						m_system->bindMultiTexture2D(GL_TEXTURE3, 0);
					}
				}

				
				m_system->applyMaterial(GL_FRONT_AND_BACK, &material.m_material);
				return;
			}
			++it;
			++i;
		}
	}
	else
	{
		m_system->bindTexture2DWhitePixel();
	}
}

void BeGraphicsModel::draw(  unsigned int current_material )
{
	if ( matlist.size() > 0 )
	{
		const DrawCall& drawCall = m_drawCalls[current_material];
	// 	if ( drawCall.m_count>0 )
			glDrawElements(GL_TRIANGLES, drawCall.m_count, GL_UNSIGNED_INT, (char *)NULL + ((drawCall.m_first)*m_glint_size) );
	}
	else
	{
		glDrawElements(GL_TRIANGLES, elementArrayBuffer.size(), GL_UNSIGNED_INT, NULL);
	}
}




void BeGraphicsModel::disableContext()
{
	m_system->bindVertexArray(0);
	m_system->bindTexture2D(0);
// 	m_graphicsSystem.bindTexture2D(0);
// 	m_system->disable(GL_TEXTURE_2D);
}




// void BeGraphicsModel::draw()
// {
// 	m_system->enable(GL_TEXTURE_2D);
// 
// 	m_system->bindVertexArray(m_vertexArray.get());
// 
// 	if ( matlist.size() > 0 )
// 	{
// 		int minPriority=0;
// 		int maxPriority=0;
// 
// // 		size_t i=0;
// 		matlist_iterator it=matlist.begin();
// 		while(it!=matlist.end())
// 		{
// 			const std::pair<const std::string, Material> pair=*it;
// 			int priority=0;
// 			const BeGraphicsMaterial* const material=m_system->getEffect(pair.first);
// 			if(material)
// 			{
// 				priority=material->getPriority();
// 			}
// 			if(priority<minPriority) minPriority = priority;
// 			if(priority>maxPriority) maxPriority = priority;
// 			
// 
// 			++it;
// // 			++i;
// 		}
// 	
// 		for(int priority=minPriority; priority<=maxPriority; ++priority)
// 		{
// 
// 			size_t i=0;
// 			matlist_iterator it=matlist.begin();
// 			while(it!=matlist.end())
// 			{
// 				const std::pair<const std::string, Material> pair=*it;
// 
// 				int pr = 0;
// // 				bool sort = false;
// 				int depthMask=1; // FIXME THESE ARE UNUSED
// 				int depthTest=1; // FIXME THESE ARE UNUSED
// 				int blend = 0;
// 				const BeGraphicsMaterial* const material=m_system->getEffect(pair.first);
// 				if(material)
// 				{
// // 					sort=material->getSort()!=0;
// 					pr=material->getPriority();
// 					depthMask=material->getDepthMask();
// 					depthTest=material->getDepthTest();
// 					blend=material->getBlend();
// 				}
// 
// 				if(pr==priority)
// 				{
// 
// 					glDepthMask(depthMask);
// 					if(depthTest)
// 					{
// 						glEnable(GL_DEPTH_TEST);
// 					}
// 					else
// 					{
// 						glDisable(GL_DEPTH_TEST);
// 					}
// 					if(blend)
// 					{
// 						glEnable(GL_BLEND);
// 						glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
// 					}
// 					else
// 					{
// 						glDisable(GL_BLEND);
// 						glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
// 					}
// 
// 					const Material& material=pair.second;
// 
// 					if ( material.m_imageTexture2D )
// 					{
// 						m_system->bindTexture2D(material.m_imageTexture2D->get().get());
// 						
// 	// 					if ( material.m_imageTexture2D->isReady() )
// 	// 								std::cout << "handle " << material.m_imageTexture2D->get()->handle() << std::endl;
// 									
// 					}
// // 					else
// // 					{
// 	// 					glColor4f(material.m_material.getDiffuse().x(), material.m_material.getDiffuse().y(), material.m_material.getDiffuse().z(), 1.0f);
// // 						m_system->bindTexture2D(0);
// // 					}
// 
// // 				BeEntity::ptr normal_mapping( get("normal_mapping", 1) );
// // 				if ( normal_mapping && normal_mapping->getProperty_Bool() )
// // 				{
// // 					btScalar modelview2[16];
// // 					t.getOpenGLMatrix(modelview2);
// // 					glUniformMatrix4fv(m_v_inv, 1, GL_FALSE, modelview2);
// // 				}
// 
// 
// 					if ( material.m_imageTexture2DBump )
// 					{
// // 						glActiveTexture(GL_TEXTURE3);
// 						glUniform1i(m_system->getNormalMappingTexLoc(), 3); // 91 == tex1Loc in qt.cpp // FIXME MULTITEXTURE FIXME
// 						glUniform1i(m_system->getNormalMappingDo(), 1); // 89 == normal_map_do in qt.cpp // FIXME MULTITEXTURE FIXME
// 						m_system->bindMultiTexture2D(GL_TEXTURE3, material.m_imageTexture2DBump->get().get());
// 					}
// 					else
// 					{
// // 						glActiveTexture(GL_TEXTURE3);
// // 						glUniform1i(m_system->getNormalMappingTexLoc(), 0); // 91 == tex1Loc in qt.cpp // FIXME MULTITEXTURE FIXME
// 						glUniform1i(m_system->getNormalMappingDo(), 0); // 91 == tex1Loc in qt.cpp // FIXME MULTITEXTURE FIXME
// 						m_system->bindMultiTexture2D(GL_TEXTURE3, 0);
// 					}
// 
// 					const DrawCall& drawCall = m_drawCalls[i];
// 
// // 								std::cout << "b" << std::endl;
// // 					m_system->applyMaterial(GL_FRONT_AND_BACK, &material.m_material);
// 						m_system->applyMaterial(GL_FRONT_AND_BACK, &material.m_material);
// // 								std::cout << "bb" << std::endl;
// 					
// 	// 				std::cout << "draw: materials OK c" << std::endl;
// 					glDrawElements(GL_TRIANGLES, drawCall.m_count, GL_UNSIGNED_INT, (char *)NULL + ((drawCall.m_first)*sizeof(GLint)) );
// 				}
// 
// 				++it;
// 				++i;
// 			}
// 
// 		}
// 	}
// 	else
// 	{
// // 		m_graphicsSystem.bindTexture2D(0);
// // 		m_system->applyMaterial(GL_FRONT_AND_BACK, 0);
// // 		std::cout << "draw: no materials d" << std::endl;
// 		glDrawElements(GL_TRIANGLES, elementArrayBuffer.size(), GL_UNSIGNED_INT, NULL);
// 	}
// 
// 	m_system->bindVertexArray(0);
// // 	m_graphicsSystem.bindTexture2D(0);
// 	m_system->disable(GL_TEXTURE_2D);
// }






void BeGraphicsModel::drawToDepth()
{
	m_system->disable(GL_TEXTURE_2D);
	m_system->bindVertexArray(m_vertexArray.get());

// 	if ( matlist.size() > 0 )
// 	{
// 		int minPriority=0;
// 		int maxPriority=0;
// 
// // 		size_t i=0;
// 		matlist_iterator it=matlist.begin();
// 		while(it!=matlist.end())
// 		{
// 			const std::pair<const std::string, Material> pair=*it;
// 			int priority=0;
// 			const BeGraphicsMaterial* const material=m_system->getEffect(pair.first);
// 			if(material)
// 			{
// 				priority=material->getPriority();
// 			}
// 			if(priority<minPriority) minPriority = priority;
// 			if(priority>maxPriority) maxPriority = priority;
// 			
// 
// 			++it;
// // 			++i;
// 		}
// 	
// 		for(int priority=minPriority; priority<=maxPriority; ++priority)
// 		{
// 
// 			size_t i=0;
// 			matlist_iterator it=matlist.begin();
// 			while(it!=matlist.end())
// 			{
// 				const std::pair<const std::string, Material> pair=*it;
// 
// 				int pr = 0;
// // 				bool sort = false;
// 				int depthMask=1; // FIXME THESE ARE UNUSED
// 				int depthTest=1; // FIXME THESE ARE UNUSED
// 				int blend = 0;
// 				const BeGraphicsMaterial* const material=m_system->getEffect(pair.first);
// 				if(material)
// 				{
// // 					sort=material->getSort()!=0;
// 					pr=material->getPriority();
// 					depthMask=material->getDepthMask();
// 					depthTest=material->getDepthTest();
// 					blend=material->getBlend();
// 				}
// 
// 				if(pr==priority)
// 				{
// 
// 					glDepthMask(depthMask);
// // 					if(depthTest)
// // 					{
// 						glEnable(GL_DEPTH_TEST);
// // 					}
// // 					else
// // 					{
// // 						glDisable(GL_DEPTH_TEST);
// // 					}
// 					if(blend)
// 					{
// 						glEnable(GL_BLEND);
// 						glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
// 					}
// // 					else
// // 					{
// // 						glDisable(GL_BLEND);
// // 						glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
// // 					}
// 
// // 					const Material& material=pair.second;
// 
// // 					if ( material.m_imageTexture2DBump )
// // 					{
// // // 						glActiveTexture(GL_TEXTURE3);
// // 						glUniform1i(m_system->getNormalMappingTexLoc(), 3); // 91 == tex1Loc in qt.cpp // FIXME MULTITEXTURE FIXME
// // 						glUniform1i(m_system->getNormalMappingDo(), 1); // 89 == normal_map_do in qt.cpp // FIXME MULTITEXTURE FIXME
// // 						m_system->bindMultiTexture2D(GL_TEXTURE3, material.m_imageTexture2DBump->get().get());
// // 					}
// // 					else
// // 					{
// // // 						glActiveTexture(GL_TEXTURE3);
// // // 						glUniform1i(m_system->getNormalMappingTexLoc(), 0); // 91 == tex1Loc in qt.cpp // FIXME MULTITEXTURE FIXME
// // 						glUniform1i(m_system->getNormalMappingDo(), 0); // 91 == tex1Loc in qt.cpp // FIXME MULTITEXTURE FIXME
// // 						m_system->bindMultiTexture2D(GL_TEXTURE3, 0);
// // 					}
// 
// 					const DrawCall& drawCall = m_drawCalls[i];
// 
// 					glDrawElements(GL_TRIANGLES, drawCall.m_count, GL_UNSIGNED_INT, (char *)NULL + ((drawCall.m_first)*sizeof(GLint)) );
// 				}
// 
// 				++it;
// 				++i;
// 			}
// 
// 		}
// 	}
// 	else
	{
		glDrawElements(GL_TRIANGLES, elementArrayBuffer.size(), GL_UNSIGNED_INT, NULL);
	}

	m_system->bindVertexArray(0);
// 	m_graphicsSystem.bindTexture2D(0);
}


boost::shared_ptr<BeGraphicsModelResource> BeGraphicsModelSystem::load(const std::string& filename, boost::shared_ptr<BeGraphicsSystem> graphicsSystem, boost::shared_ptr<BeGraphicsModelSystem> instance, const btVector3& scale, const btTransform& transform)
{
// 	// account for scale and transform
// 	btScalar rotX, rotY, rotZ;
// 	transform.getBasis().getEulerZYX(rotX, rotY, rotZ);
// 	std::stringstream sscacheName;
// 	sscacheName << filename << scale.x() << scale.y() << scale.z();
// 	sscacheName << transform.getOrigin().getX() << transform.getOrigin().getY() << transform.getOrigin().getZ();
// 	sscacheName << rotX << rotY << rotZ;
// 
// 	std::string cacheName(sscacheName.str());
// 	if ( m_resources[cacheName] )
// 	{
// 		return m_resources[cacheName];
// 	}
// 	else
	{
		boost::shared_ptr<BeGraphicsModelResource> m(new BeGraphicsModelResource( m_filesystem, filename, graphicsSystem, instance, scale, transform ));
// 		m_resources[cacheName]=m;
		return m;
	}
}

boost::shared_ptr<BeGeometry> BeGeometrySystem::load(BeFilesystem& filesystem, const std::string& filename, const btVector3& scale, const btTransform& transform)
{
// 	// check if models exists
// 	// 	cerr << "checking '" << filename << "'" << endl;
// 	if ( m_resources[filename] )
// 	{
// 		// 		cerr << "texture '" << filename << "' is present" << endl;
// 		return m_resources[filename];
// 	}
	BeFile objBeFile;
	if ( filesystem.load(objBeFile, filename) )
	{
		const BeObjLoader obj( filesystem, objBeFile, scale, transform );
		boost::shared_ptr<BeGeometry> m(boost::make_shared<BeGeometry>());

		// copy materials
		m->m_materialMap = obj.matlist;
		// copy face materials
		m->facematerials = obj.facematerials;
		m->setNumberVertices( obj.getNumberVertices()*3 );
		m->setNumberVIndices( obj.getNumberVIndices() );
		
// 		std::cout << "model: " << filename << std::endl;
// 		std::cout << "  number of faces: " << obj.getNumberVIndices()/3 << std::endl;

		
		
		for ( unsigned int i=0; i < obj.getNumberVIndices(); ++i )
		{
			m->indices[i] = obj.gVIndices[i] - 1;
		}
		
		memcpy(m->vertices.get(), obj.gVertices, obj.getNumberVertices()*sizeof(float)*3);
// 		memcpy(m->vertices.get(), obj.gVertices, obj.getNumberVertices()*sizeof(float));
		
		m->buildElementArrayBuffer();
// 		m_resources[filename]=m;
		return m;
	}
	return boost::shared_ptr<BeGeometry>();
}

void BeGeometry::buildElementArrayBuffer()
{
	for ( matlist_iterator it=m_materialMap.begin(); it != m_materialMap.end(); ++it)
	{
		unsigned int first=elementArrayBuffer.size();
		unsigned int count=0;
		for ( unsigned int j=0; j < facematerials.size(); ++j )
		{
			const Facematerial& facematerial = facematerials[j];
			if(facematerial.material==it->first)
			{
				for( unsigned int k=0; k < facematerial.faceindex_number; ++k)
				{
					elementArrayBuffer.push_back(indices[facematerial.faceindex_first+k]);
					++count;
				}
			}
		}
		DrawCall drawCall;
		drawCall.m_first=first;
		drawCall.m_count=count;
		m_drawCalls.push_back(drawCall);
	}
}




