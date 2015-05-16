#include "be_parser.h"
#include "be_obj_loader.h"

#include "kernel/be_profiler.h"

BeObjLoader::BeObjLoader( BeFilesystem& filesystem, BeFile& befile, const btVector3& scale, const btTransform& transform )
 :	m_numberVertices(0),
	m_numberNormals(0),
	m_numberTexCoor(0),
	m_numberVIndices(0),
	m_numberTIndices(0),
	m_numberNIndices(0),
	m_logDebug("OBJ")
{
	char profilerId[2048];
	sprintf( profilerId, "%s %s", BE_FUNCTION_ID, befile.getFilename().c_str() );
	BeProfiler profiler(profilerId);
	m_filename = befile.getFilename();

	gVertices = new btScalar[5000000];
	gNormals = new float[5000000];
	gTexCoor = new float[5000000];
	gVIndices = new int[5000000];
	gTIndices = new int[5000000];
	gNIndices = new int[5000000];

    const bool hasScale=scale!=btVector3(1,1,1);
    const bool hasTransform=!(transform==btTransform::getIdentity());

// 	cerr << "loading model: " << filename << endl;
// 	std::cout << "BeObjLoader::scale " << scale.x() << "  " << scale.y() << "  " << scale.z() << std::endl;
	
	
	std::string line;
	BeParser parseH;
	while ( befile.getLine(line) )
	{
		parseH.reset();
// 		std::cerr << line << std::endl;
		// trim spaces
		parseH.trimWhitespaces(line);
		// remove comment lines
		parseH.removeCommentlines(line);

		if ( parseH.position_index < line.length() )
		{
// 				unsigned int type;
			const std::string& otype = parseH.returnUntillStrip( ' ', line );
			if ( !otype.empty() )
			{
				if ( otype == "v" )
				{
					btVector3 position;
                    parseH.parseVector3(line, position);

                    if(hasTransform)
                    {
					    position = transform * position;
                    }
                    if(hasScale)
                    {
                        position = scale * position;
                    }

					gVertices[ m_numberVertices++ ] = position.x();
					gVertices[ m_numberVertices++ ] = position.y();
					gVertices[ m_numberVertices++ ] = position.z();
// 						std::cerr << "vectors: " << m_numberVertices << std::endl;
// 						std::cerr << "x: " << x << std::endl;
// 						std::cerr << "y: " << y << std::endl;
// 						std::cerr << "z: " << z << std::endl;
//					}
//					else
//						m_logDebug << "::OBJ warning: this line sucks: '" << line << "'\n";
// 						gVertices.push_back(v);
				}

				else if ( otype == "vt" )
				{
					float u=0.0f, v=0.0f;

					// u
					parseH.trimWhitespaces(line);
					//std::string us = parseH.returnUntillStrip( ' ', line );
					//u = atof( us.c_str() );
                    parseH.parseFloat(line, u);

					// v
					parseH.trimWhitespaces(line);
					//std::string vs = parseH.returnUntillStrip( ' ', line );
					//v = atof( vs.c_str() );
                    parseH.parseFloat(line, v);

					//if ( !us.empty() && !vs.empty() )  // && line == ""
					{
// 							cerr << "got a texture coordinate here" << endl;
						gTexCoor[m_numberTexCoor++] = u;
// 							if ( u < 0.0f )
// 							cerr << "u: " << m_numberTexCoor << ": '" << u << "'" << endl;
						gTexCoor[m_numberTexCoor++] = v;
// 							if ( v < 0.0f )
// 							cerr << "v: " << m_numberTexCoor << ": '" << v << "'" << endl;
					}
				}

				else if ( otype == "vn" )
				{

                    btVector3 normal;
					
                    parseH.parseVector3(line, normal);

// 						if ( !xs.empty() && !ys.empty() && !zs.empty() )
// 						{
// 							std::cerr << "got a normal here" << std::endl;
// 							std::cerr << "x: " << x << std::endl;
// 							std::cerr << "y: " << y << std::endl;
// 							std::cerr << "z: " << z << std::endl;
                    if(hasTransform)
                    {
						normal = transform * normal;
                        normal.normalize();
                    }
                                                            //Dont think we want to scale vectors
						gNormals[ m_numberNormals++ ] = normal.x();// * scale.x();
						gNormals[ m_numberNormals++ ] = normal.y();//* scale.y();
						gNormals[ m_numberNormals++ ] = normal.z();// * scale.z();
// 						gNormals[ m_numberNormals++ ] = x;
// 						gNormals[ m_numberNormals++ ] = y;
// 						gNormals[ m_numberNormals++ ] = z;
// 						}
				}

				else if ( otype == "f" )
				{
					std::string vindex1, vindex2, vindex3, vindex4;
					std::string tindex1, tindex2, tindex3, tindex4;
					std::string nindex1, nindex2, nindex3, nindex4;
					int vi1 = 0, vi2 = 0, vi3 = 0, vi4 = 0;
					int ti1 = 0, ti2 = 0, ti3 = 0, ti4 = 0;
					int ni1 = 0, ni2 = 0, ni3 = 0, ni4 = 0;

// 						cerr << line << endl;
					// 1
					parseH.trimWhitespaces(line);
					
					vindex1 = parseH.returnUntillStrip( '/', line );
					if ( !vindex1.empty() ) vi1 = atoi( vindex1.c_str() );
					tindex1 = parseH.returnUntillStrip( '/', line );
					if ( !tindex1.empty() ) ti1 = atoi( tindex1.c_str() );
					nindex1 = parseH.returnUntillStrip( ' ', line );
					if ( !nindex1.empty() ) ni1 = atoi( nindex1.c_str() );

					// 2
					parseH.trimWhitespaces(line);

					vindex2 = parseH.returnUntillStrip( '/', line );
					if ( !vindex2.empty() ) vi2 = atoi( vindex2.c_str() );
					tindex2 = parseH.returnUntillStrip( '/', line );
					if ( !tindex2.empty() ) ti2 = atoi( tindex2.c_str() );
					nindex2 = parseH.returnUntillStrip( ' ', line );
					if ( !nindex2.empty() ) ni2 = atoi( nindex2.c_str() );

					// 3
					parseH.trimWhitespaces(line);

// 					std::string s3 = parseH.returnUntillStrip( " ", line );
					vindex3 = parseH.returnUntillStrip( '/', line );
					if ( !vindex3.empty() ) vi3 = atoi( vindex3.c_str() );
					tindex3 = parseH.returnUntillStrip( '/', line );
					if ( !tindex3.empty() ) ti3 = atoi( tindex3.c_str() );
					nindex3 = parseH.returnUntillStrip( ' ', line );
					if ( !nindex3.empty() ) ni3 = atoi( nindex3.c_str() );

					// 4
					parseH.trimWhitespaces(line);

// 					std::string s4 = parseH.returnUntillStrip( " ", line );
					vindex4 = parseH.returnUntillStrip( '/', line );
					if ( !vindex4.empty() ) vi4 = atoi( vindex4.c_str() );
					tindex4 = parseH.returnUntillStrip( '/', line );
					if ( !tindex4.empty() ) ti4 = atoi( tindex4.c_str() );
					nindex4 = parseH.returnUntillStrip( ' ', line );
					if ( !nindex4.empty() ) ni4 = atoi( nindex4.c_str() );

// 						std::cerr << vi1 << std::endl;
// 						assert(vi1);
// 						assert(viy);
// 						assert(viz);

					
// 						string last = parseH.returnUntillStrip( " ", line );
// 						if ( last.size() > 1 )
// 						{
// 							cerr << "4 faces dammit '" << last << "'" << endl;
// 							exit(1);
// 						}
					
					
					if ( vi1 != 0 && vi2 != 0 && vi3 != 0 )
					{
						// TODO REMOVE T & N NUMS, unneeded, always the same yo

						if ( vi1 > 0 )	gVIndices[m_numberVIndices++] = vi1;
						else		gVIndices[m_numberVIndices++] = (m_numberVertices/3) + vi1 + 1;
						if ( vi2 > 0 )	gVIndices[m_numberVIndices++] = vi2;
						else		gVIndices[m_numberVIndices++] = (m_numberVertices/3) + vi2 + 1;
						if ( vi3 > 0 )	gVIndices[m_numberVIndices++] = vi3;
						else		gVIndices[m_numberVIndices++] = (m_numberVertices/3) + vi3 + 1;

						if ( ti1 > 0 )	gTIndices[m_numberTIndices++] = ti1;
						else		gTIndices[m_numberTIndices++] = (m_numberTexCoor/2) + ti1 + 1;
						if ( ti2 > 0 )	gTIndices[m_numberTIndices++] = ti2;
						else		gTIndices[m_numberTIndices++] = (m_numberTexCoor/2) + ti2 + 1;
						if ( ti3 > 0 )	gTIndices[m_numberTIndices++] = ti3;
						else		gTIndices[m_numberTIndices++] = (m_numberTexCoor/2) + ti3 + 1;

						if ( ni1 > 0 )	gNIndices[m_numberNIndices++] = ni1;
						else		gNIndices[m_numberNIndices++] = (m_numberNormals/3) + ni1 + 1;
						if ( ni2 > 0 )	gNIndices[m_numberNIndices++] = ni2;
						else		gNIndices[m_numberNIndices++] = (m_numberNormals/3) + ni2 + 1;
						if ( ni3 > 0 )	gNIndices[m_numberNIndices++] = ni3;
						else		gNIndices[m_numberNIndices++] = (m_numberNormals/3) + ni3 + 1;

						if ( vi4 != 0 )
						{
							if ( vi4 > 0 )	gVIndices[m_numberVIndices++] = vi4;
							else		gVIndices[m_numberVIndices++] = (m_numberVertices/3) + vi4 + 1;
							if ( vi1 > 0 )	gVIndices[m_numberVIndices++] = vi1;
							else		gVIndices[m_numberVIndices++] = (m_numberVertices/3) + vi1 + 1;
							if ( vi3 > 0 )	gVIndices[m_numberVIndices++] = vi3;
							else		gVIndices[m_numberVIndices++] = (m_numberVertices/3) + vi3 + 1;

							if ( ti4 > 0 )	gTIndices[m_numberTIndices++] = ti4;
							else		gTIndices[m_numberTIndices++] = (m_numberTexCoor/2) + ti4 + 1;
							if ( ti1 > 0 )	gTIndices[m_numberTIndices++] = ti1;
							else		gTIndices[m_numberTIndices++] = (m_numberTexCoor/2) + ti1 + 1;
							if ( ti3 > 0 )	gTIndices[m_numberTIndices++] = ti3;
							else		gTIndices[m_numberTIndices++] = (m_numberTexCoor/2) + ti3 + 1;

							if ( ni4 > 0 )	gNIndices[m_numberNIndices++] = ni4;
							else		gNIndices[m_numberNIndices++] = (m_numberNormals/3) + ni4 + 1;
							if ( ni1 > 0 )	gNIndices[m_numberNIndices++] = ni1;
							else		gNIndices[m_numberNIndices++] = (m_numberNormals/3) + ni1 + 1;
							if ( ni3 > 0 )	gNIndices[m_numberNIndices++] = ni3;
							else		gNIndices[m_numberNIndices++] = (m_numberNormals/3) + ni3 + 1;
						}

					}
// 					exit(0);
// 						cerr << m_numberTIndices<< endl;
					assert ( m_numberNIndices ==  m_numberVIndices && m_numberVIndices == m_numberTIndices  );

				}

				else if ( otype == "mtllib" ) // material library
				{
					parseH.trimWhitespaces(line);
					const std::string& materialfile = parseH.returnUntillStrip( ' ', line );
					loadMaterialfile( filesystem, befile.getDirectory(), materialfile );
				}

				else if ( otype == "usemtl" ) // material library
				{
					parseH.trimWhitespaces(line);
					const std::string& usematerial = parseH.returnUntillStrip( ' ', line );

					matit = matlist.find( usematerial ); // Search for "Test" key
					if ( matit != matlist.end() )
					{
						Facematerial facematerial;

						facematerial.faceindex_first = m_numberVIndices;
						facematerial.faceindex_number = 0;
						facematerial.material = usematerial;
						facematerials.push_back( facematerial );
					}
				}
			}
		}
	}

	const size_t size=facematerials.size();

	// calculate facematerial triangle numbers
	for ( unsigned int i=1; i < size; ++i )
	{
		Facematerial* previousFM = &facematerials[ i-1 ];
		previousFM->faceindex_number = facematerials[i].faceindex_first - previousFM->faceindex_first;
	}
	// to calculate the last one we need to use m_numberVIndices
	Facematerial* previousFM = &facematerials[ size-1 ];
	previousFM->faceindex_number = m_numberVIndices - previousFM->faceindex_first;

// 	cerr << "end loading obj" << endl;
}

void BeObjLoader::loadMaterialfile( BeFilesystem& filesystem, const std::string& mpath, const std::string& file )
{
	std::string fullfilename(mpath);
	fullfilename.append(file);

	BeFile materialbefile;
	
	if ( filesystem.load( materialbefile, fullfilename ) )
	{
		
		Material* material = 0;
		std::string line;
		std::string name;
		BeParser parseH;
		
		while ( materialbefile.getLine(line) )
		{
			parseH.reset();
	// 		std::cerr << line << std::endl;
			// trim spaces
			parseH.trimWhitespaces(line);
			// remove comment lines
			parseH.removeCommentlines(line);

			if ( parseH.position_index != std::string::npos )
			{

	// 				cerr << line << endl;

	// 				unsigned int type;
				const std::string& otype = parseH.returnUntillStrip( ' ', line );
				if ( !otype.empty() )
				{

					if ( otype == "newmtl" )
					{
						name = parseH.returnUntillStrip( ' ', line );
	// 						cerr << "new material, name: " << name << endl;
						matlist[name] = Material();
						material = &matlist[name];
					}

					else if ( otype == "map_Kd" )
					{
						std::string texturename = mpath;
// 						texturename.append( "textures/" );
						texturename.append( parseH.returnUntillStrip( ' ', line ) );
	// 						matlist[currentname].textureid = Texturestore::Instance()->load( texturename );
						material->m_imagePath=texturename;

	// 						cerr << "material texture: " << texturename  << " ! " << matlist[currentname].textureid << endl << endl;
					}

					else if ( otype == "map_bump" )
					{
						std::string texturename = mpath;
// 						texturename.append( "textures/" );
						texturename.append( parseH.returnUntillStrip( ' ', line ) );
	// 						matlist[currentname].textureid = Texturestore::Instance()->load( texturename );
						material->m_imagePathBump=texturename;

	// 						cerr << "material texture: " << texturename  << " ! " << matlist[currentname].textureid << endl << endl;
					}

					else if ( otype == "Ka" )
					{
                        btVector3 ambient;
                        parseH.parseVector3(line, ambient);
// 						material->m_material.setAmbient(ambient);
						material->m_material.setAmbient(BeColor(ambient.x(), ambient.y(), ambient.z(), 1.0f));
	// 						cerr << x << endl;
	// 						cerr << y << endl;
	// 						cerr << z << endl;
					}

					else if ( otype == "Kd" )
					{
                        btVector3 diffuse;
                        parseH.parseVector3(line, diffuse);
						material->m_material.setDiffuse(BeColor(diffuse.x(), diffuse.y(), diffuse.z(), 1.0f));
// 												std::cerr << x << std::endl;
// 												std::cerr << y << std::endl;
// 												std::cerr << z << std::endl;

						//GL workaround for ambient set to black in obj materials
						if( 
							material->m_material.getAmbient().r() == 0.0f
							&& material->m_material.getAmbient().g() == 0.0f
							&& material->m_material.getAmbient().b() == 0.0f
							
						)
						{
							material->m_material.setAmbient( material->m_material.getDiffuse() );
							m_logDebug << "::OBJ warning: " << file << " (" << name << ")" << ": Ambient 0, setting to diffuse color" << "\n";
// 							material->m_material.setAmbient( Vector4f(0.4,0.4,0.4,1) );
						}


					}
					else if ( otype == "Ks" )
					{
						btVector3 specular;
                        parseH.parseVector3(line, specular);
						material->m_material.setSpecular(BeColor(specular.x(), specular.y(), specular.z(), 1.0f));
						// 						cerr << x << endl;
						// 						cerr << y << endl;
						// 						cerr << z << endl;
					}
					else if( otype == "Ns" )
					{
						float shininess = 0.0f;
                        //atof(parseH.returnUntillStrip( ' ', line ).c_str())
                        parseH.parseFloat(line, shininess);
                        shininess *= 0.128;
	// 						cerr << "ns yo " << Ns << currentname <<  endl;
						material->m_material.setShininess(shininess);
	// 						assert( material.Ns <= 128.0f ); //Ensure its in valid gl range
					}

					else if( otype == "d" || otype == "Tr" )
					{
						//Transparency, so will store in diffuse alpha component
						//Apparently some materials use d, others use Tr
	// 						material.d = atof(parseH.returnUntillStrip( " ", line ).c_str());
						float alpha = 0.0f;
                        //atof(parseH.returnUntillStrip( ' ', line ).c_str());
                        parseH.parseFloat(line, alpha);
						material->m_material.getDiffuse().setA(alpha);
						material->m_material.getAmbient().setA(alpha);
						material->m_material.getSpecular().setA(alpha);
						
// 						std::cout << alpha << std::endl;
						
	// 						assert( material.d <= 1.0f ); //Ensure its in valid gl range
					}
				}
			}
		}
		
		// HACK TODO FIXME TO MAKE THE LAST MATERIAL WORK
		if ( matlist.size() > 1 )
		{
			matlist["cdfixtmaterial"] = Material();
// 			material = &matlist[name];
		}
		
		
	}
// 		cerr << "loaded" << endl;
}

BeObjLoader::~BeObjLoader()
{
	delete[] gVertices;
	delete[] gNormals;
	delete[] gTexCoor;
	delete[] gVIndices;
	delete[] gTIndices;
	delete[] gNIndices;
}


