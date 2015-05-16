#include "filesystem/be_filesystem.h"
#include "map.h"
#include "resource/be_graphics_model_resource.h"

ClientMap::ClientMap(BeFilesystem& filesystem, const std::string& path)
{
	load(filesystem, path);
}

ClientMap::~ClientMap()
{
// 	cerr << "exiting map" << endl;
};

void ClientMap::load(BeFilesystem& filesystem, const std::string& path)
{
// 	std::string mapfile = "maps/";
	std::string mapfile;
	mapfile.append(path);
	mapfile.append("/map");

	BeFile befileMap;

	// open and parse file
	if ( filesystem.load(befileMap, mapfile) )
	{
		std::string maptype;
		std::string obj;

        btVector3 scale(1,1,1);
        btVector3 position(0,0,0);
        btVector3 euler(0,0,0);

		BeParser parseH;
		std::string line;
		while ( befileMap.getLine(line) )
		{
			parseH.reset();
			// trim spaces
			parseH.trimWhitespaces(line);
			// remove comment lines
			parseH.removeCommentlines(line);

			if (  parseH.position_index != std::string::npos  )
			{

				std::string param = parseH.returnUntillStrip( '=', line );

// 				// trim spaces at back TODO
				while ( parseH.endMatchesStrip( " ", param ) ) {};
				while ( parseH.endMatchesStrip( "\t", param ) ) {};

				// trim spaces from begin of line again
				parseH.trimWhitespaces(line);
				if ( param == "maptype" )
                {
					parseH.parseString(line, maptype);
                }
				else if ( param == "obj" )
                {
                    parseH.parseString(line, obj);
                }
				else if ( param == "scale" )
				{
					parseH.parseVector3(line, scale);
				}
				else if ( param == "position" )
				{
					parseH.parseVector3(line, position);
				}
				else if ( param == "rotation" )
				{
					parseH.parseVector3(line, euler);
				}
			}
		}
		// load OBJ file
		if ( maptype == "obj" )
		{
			m_path=befileMap.getDirectory();
            m_path.append(obj);
            m_scale=scale;
			m_transform.setOrigin( position );
			m_transform.getBasis().setEulerZYX( euler.x(), euler.y(), euler.z() );
		}

	}
	else
		std::cout << "load map failed : '" << mapfile << "'" << std::endl;
}


void ClientMap::draw(const btVector3& scale)
{
	if ( m_model )
	{
		if ( m_model->isReady() )
		{
			const unsigned int t_num_materials( m_model->get()->numberOfMaterials() );

			for ( unsigned int t_current_material(0); t_current_material < t_num_materials; ++t_current_material )
			{
// 				std::cout << "      2" << std::endl;
				m_model->get()->setContext( t_current_material );
				m_model->get()->draw( t_current_material, m_transform, m_scale * scale );
			}
		}
	}
}

// void ClientMap::drawSkyBox()
// {
// 	if( m_skyBoxModel && m_skyBoxModel->isReady() )
// 	{
// 		btTransform t;
// 		t.setIdentity();
// 		t.getBasis().setEulerZYX(0,m_skyBoxAngle*SIMD_PI/180.0f,0);
// 		m_skyBoxModel->get()->draw(m_transform*m_skyBoxTransform*t);
// 	}
// }








ServerMap::ServerMap(BeFilesystem& filesystem, const std::string& path, boost::shared_ptr<BeGeometrySystem> modelSystem, btDynamicsWorld* dynamicsWorld/*, const btVector3& tscale*/) :
finish_width(0.1f),
finish_height(0.1f),
m_body(dynamicsWorld),
m_logDebug("MAP")
{
	load(filesystem, path, modelSystem/*, scale*/);
}

ServerMap::~ServerMap()
{
// 	cerr << "exiting map" << endl;
};

void ServerMap::load(BeFilesystem& filesystem, const std::string& path, boost::shared_ptr<BeGeometrySystem> modelSystem/*, const btVector3& tscale*/)
{
// 	std::string mapfile = "maps/";
	std::string mapfile;
	mapfile.append(path);
	mapfile.append("/map");

	BeFile befileMap;

	// open and parse file
	if ( filesystem.load(befileMap, mapfile) )
	{
		std::string maptype;
		std::string obj;

        btVector3 scale(1,1,1);
        btVector3 position(0,0,0);
        btVector3 euler(0,0,0);

		BeParser parseH;
		std::string line;
		while ( befileMap.getLine(line) )
		{
			parseH.reset();
			// trim spaces
			parseH.trimWhitespaces(line);
			// remove comment lines
			parseH.removeCommentlines(line);

			if ( parseH.position_index != std::string::npos )
			{

				std::string param = parseH.returnUntillStrip( '=', line );

// 				// trim spaces at back TODO
				while ( parseH.endMatchesStrip( " ", param ) ) {};
				while ( parseH.endMatchesStrip( "\t", param ) ) {};

				// trim spaces from begin of line again
				parseH.trimWhitespaces(line);

                if ( param == "maptype" )
                {
                    parseH.parseString(line, maptype);
                }
                else if ( param == "obj" )
                {
                    parseH.parseString(line, obj);
                }
                else if ( param == "scale" )
                {
                    parseH.parseVector3(line, scale);
                }
                else if ( param == "position" )
                {
                    parseH.parseVector3(line, position);
                }
                else if ( param == "rotation" )
                {
                    parseH.parseVector3(line, euler);
                }
			}
		}
		// load OBJ file
		if ( maptype == "obj" )
		{
			std::string objfilename( befileMap.getDirectory() );
			objfilename.append(obj);

            btTransform t;
            t.setIdentity();

			m_model = modelSystem->load( filesystem, objfilename, scale, t );

			if ( m_model != 0 )
			{
				btTransform transform;
                transform.setOrigin( position );
				transform.getBasis().setEulerZYX( euler.x(), euler.y(), euler.z() );

				//If there isnt physics_materials.xml create a single Trianglemesh with default physics material
				if( !loadPhysicsMaterials(befileMap.getDirectory() + "physics_materials.xml") || m_physicsMaterialMap.size() == 0 )
				{
					m_logDebug << "::MAP warning: no or invalid physics_materials.xml, using default physics material for entire map\n";
					boost::shared_ptr<BodypartRigidTrianglemesh> bodypart = m_body.addBodyPart_Rigid_Trianglemesh( m_model, transform, 0, 0, 0 );

					const BePhysicsMaterial mat;
					bodypart->setMaterial(mat);
				}
				else
				{
					//Otherwise create a Trianglemesh for each material
					btScalar* const vertices = m_model->vertices.get();
					unsigned int numVertices = m_model->getNumberVertices();
					int* const indices = &m_model->elementArrayBuffer[0];


					size_t i=0;
					BeGeometry::matlist_iterator it=m_model->m_materialMap.begin();
					while(it!=m_model->m_materialMap.end())
					{
						const std::pair<const std::string, Material> pair=*it;
						PhysicsMaterialMap::iterator it1 = m_physicsMaterialMap.find(pair.first);
						if( it1 != m_physicsMaterialMap.end() )
						{
							const BeGeometry::DrawCall& drawCall = m_model->m_drawCalls[i];
							const BePhysicsMaterial mat = it1->second;
							if(drawCall.m_count != 0 )
							{
								boost::shared_ptr<BodypartRigidTrianglemesh> bodypart = m_body.addBodyPart_Rigid_Trianglemesh((btVector3*)(vertices),numVertices,&indices[drawCall.m_first], drawCall.m_count, transform, 0, 0, 0 );
								bodypart->setMaterial(mat);
							}
						}
						
						++it;
						++i;
					}

				}

// 				startgrid.setOrigin( startPosition );
//  				startgrid.getBasis().setEulerZYX( startEuler.x(), startEuler.y(), startEuler.z() );
			}
		}
	}
// 	exit(0);
}

bool ServerMap::loadPhysicsMaterials(const std::string& path)
{
	//TODO: use BeFileSystem to find the correct path
	TiXmlDocument document;
	const bool result = document.LoadFile( path.c_str() );
	if(result)
	{
		const TiXmlElement* const rootElementPtr=document.RootElement();
		if(rootElementPtr)
		{
			const TiXmlElement& rootElement=(*rootElementPtr);
			const TiXmlElement* elementPtr=rootElement.FirstChildElement();
			while(elementPtr)
			{
				const TiXmlElement& element=(*elementPtr);
				if(element.ValueTStr()=="physics_material")
				{
					std::string texturePath;
					float friction = 1;
					float restitution = 0;
					BeXmlHelper xmlHelper;
					xmlHelper.addString("texture_path", texturePath);
					xmlHelper.addFloat("friction", friction);
					xmlHelper.addFloat("restitution", restitution);
					xmlHelper.parse(element);

					m_physicsMaterialMap[texturePath]=BePhysicsMaterial(friction,restitution);
				}
				elementPtr=element.NextSiblingElement();
			}
		}
		return true;
	}
	return false;
}

