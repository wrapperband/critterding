#include "roundworld.h"
#ifdef HAVE_OPENMP
	#include <omp.h>
#endif

#include "scenes/critterding/entities/food.h"
#include "scenes/critterding/entities/critterb.h"
#include "resource/sc_client_map_resource.h"

using namespace std;

Roundworld::Roundworld(  boost::shared_ptr<BeGraphicsSystem> system, BeFilesystem& fileSystem, boost::shared_ptr<Textverbosemessage> textverbosemessage )
 : WorldB( system, fileSystem, textverbosemessage, false )
{
	// reset cam
		resetCamera();

	
}

void Roundworld::init()
{
	//makeFloor();

// 	groundTransform.setIdentity();
// 	groundTransform.setOrigin( btVector3(0,0,0) );
// 
// 	groundShape = new btSphereShape(btScalar( *worldsizeX ));
// 	
// 	fixedGround = new btCollisionObject();
// 	fixedGround->setUserPointer(this);
// 	fixedGround->setCollisionShape(groundShape);
// 	fixedGround->setWorldTransform(groundTransform);
// 	m_dynamicsWorld->addCollisionObject(fixedGround);
//     fixedGround->setFriction(0.8f);
//     fixedGround->setRestitution(0.6f);

	
	if ( settings->getCVar("autoload") )
		loadAllCritters();
	if ( settings->getCVar("autoloadlastsaved") )
		loadAllLastSavedCritters();

	m_skyTransform.setIdentity();
	
/*	
	if ( !m_model )
	{
		load(m_fileSystem, "planet");
		
		btTransform t;
		t.setIdentity(); 
		m_model=m_modelSystem->load(m_path, m_graphicsSystem, m_modelSystem, btVector3(1,1,1), t);
	}*/

	makeFloor();
}

void Roundworld::process(const BeTimer& timer)
{
	if ( !pause )
	{
		updateSkyboxAngles();

		// recalc freeEnergy
			calcFreeEnergy();

		killHalforDouble();
		expireFood();
		autoinsertFood();
		expireCritters();
		autoexchangeCritters(timer);
		autosaveCritters(timer);
		autoinsertCritters();
		// adjust gravity vectors of all entities' rigid bodies
		unsigned int j, b;
// 		Food* f;
// 		CritterB* bod;
		btRigidBody* bo;
		
		for( j=0; j < food_units.size(); ++j)
		{	
// 			if ( entities[j]->type == FOOD )
			{
// 				f = food[j];
				Food* f( food_units[j] );
				for( b=0; b < f->body.bodyparts.size(); ++b)
				{
					bo = f->body.bodyparts[b]->body;
					bo->setGravity( -(bo->getCenterOfMassPosition().normalized()*10) );
				}
			}
		}
		for( j=0; j < critters.size(); ++j)
		{
			CritterB* bod = critters[j];
			for( b=0; b < bod->body.bodyparts.size(); ++b)
			{
				bo = bod->body.bodyparts[b]->body;
				bo->setGravity( -(bo->getCenterOfMassPosition().normalized()*10) );
			}
			
			// catch by Ethical
			for( b=0; b < bod->body.mouths.size(); ++b)
			{
				bo = bod->body.mouths[b]->body;
				bo->setGravity( -(bo->getCenterOfMassPosition().normalized()*10) );
			}
		}
		
// 		if ( *critter_raycastvision == 0 )
		{
			renderVision();
			grabVision();
		}

		// do a bullet step
			m_dynamicsWorld->stepSimulation(0.016667f, 0, 0.016667f);
	// 		m_dynamicsWorld->stepSimulation(Timer::Instance()->bullet_ms / 1000.f);

		unsigned int i;
		const unsigned int lmax = (int)critters.size();
// 		CritterB *c;
		float freeEnergyc = 0.0f;

// 		// FIXME USE FROM WORLDB
// 		for( int i=0; i < lmax; ++i)
// 		{
// 			c = critters[i];
// 			
// 				checkCollisions(  c );
// 
// 			// process
// 				c->process();
// 
// 			// record critter used energy
// 				freeEnergyc += c->energyUsed;
// 
// 			// process Output Neurons
// 				eat(c);
// 
// 			// procreation if procreation energy trigger is hit
// 				procreate(c);
// 		}

		
		
		for( i=0; i < lmax; ++i)
			checkCollisions( critters[i] );
		
#ifdef HAVE_OPENMP
	// process
		if ( m_threads_last != *threads )
		{
			m_threads_last = *threads;
			omp_set_num_threads(m_threads_last);
		}

		if ( m_threads_last > 1 )
		{
// 			unsigned int t_lmax = lmax;
			#pragma omp parallel for private(i)
			for( i=0; i < lmax; ++i)
			{
				critters[i]->process();
// 				freeEnergyc += c->energyUsed;
			}
		}
		else
#endif
		{
			for( i=0; i < lmax; ++i)
				critters[i]->process();
		}

		for( i=0; i < lmax; ++i)
		{
			CritterB* c = critters[i];

			// record critter used energy
				freeEnergyc += c->energyUsed;

			// process Output Neurons
				eat(c);

			// procreation if procreation energy trigger is hit
				procreate(c);
				
				c->beingTouched = false;
				c->beingEaten   = false;
		}
		
		
		
		
		
		m_freeEnergy += freeEnergyc;

		getGeneralStats();
	}
}

void Roundworld::makeFloor()
{
	
// 	m_dynamicsWorld->removeCollisionObject(fixedGround);
// 	delete groundShape;
// 	delete fixedGround;
// 
// 	groundShape = new btSphereShape(btScalar( *worldsizeX ));
// 	
// 	fixedGround = new btCollisionObject();
// 	fixedGround->setUserPointer(this);
// 	fixedGround->setCollisionShape(groundShape);
// 	fixedGround->setWorldTransform(groundTransform);
// 	m_dynamicsWorld->addCollisionObject(fixedGround);

// 	const auto& mapName = settings->getCVarS("map").c_str();
	const auto& mapName = std::string("maps/")+settings->getCVarS("map").c_str();
	if ( !m_map )
	{
		m_map = boost::shared_ptr<ScClientMapResource>(new ScClientMapResource(m_fileSystem, mapName, m_graphicsSystem, m_modelSystem));
	}

	if ( m_physics_map )
		m_physics_map.reset();
	
// 	if ( !m_physics_map )
// 	{
// 		const float scale(0.001f * *worldsizeX);
// 		
// 		m_physics_map = boost::shared_ptr<ServerMap>(new ServerMap( m_fileSystem, mapName, m_geometryModelSystem, m_dynamicsWorld ));
// 		for ( auto it(m_physics_map->body().m_bodyparts.begin()); it != m_physics_map->body().m_bodyparts.end(); ++it )
// 		{
// 			(*it)->getBody()->getCollisionShape()->setLocalScaling(btVector3(scale,scale,scale));
// 			(*it)->getBody()->setFriction(0.8f);
// 			(*it)->getBody()->setRestitution(0.6f);
// 		}
// 	}

	if ( !m_physics_map )
	{
// 		m_map_scale.setX((float)*worldsizeX / m_initial_worldsizeX);
// 		m_map_scale.setY((float)*worldsizeY / m_initial_worldsizeY);
// 		m_map_scale.setZ((float)*worldsizeZ / m_initial_worldsizeZ);

		m_map_scale.setX((float)*worldsizeX);
		m_map_scale.setY((float)*worldsizeY);
		m_map_scale.setZ((float)*worldsizeZ);
		
// 		m_map_scale.setX(sqrt(*worldsizeX));
// 		m_map_scale.setY(sqrt(*worldsizeY));
// 		m_map_scale.setZ(sqrt(*worldsizeZ));
		
		m_map_scale = m_map_scale * 0.0001f;
		
// 		const float scale(0.001f * *worldsizeX);
		
		m_physics_map = boost::shared_ptr<ServerMap>(new ServerMap( m_fileSystem, mapName, m_geometryModelSystem, m_dynamicsWorld ));
		for ( auto it(m_physics_map->body().m_bodyparts.begin()); it != m_physics_map->body().m_bodyparts.end(); ++it )
		{
			(*it)->getBody()->getCollisionShape()->setLocalScaling(m_map_scale);
// 			(*it)->getBody()->setFriction(0.8f);
// 			(*it)->getBody()->setRestitution(0.6f);
		}
	}
	
// 	if ( !m_skyBoxModel )
// 	{
// 		const auto& mapName = std::string("skies/")+settings->getCVarS("skybox").c_str();
// // 		std::string mapName = "skys/round/skydome3.obj";
// 		
// 		btTransform t;
// 		t.setIdentity();
// 		
// 		m_skyBoxModel=m_modelSystem->load(mapName, m_graphicsSystem, m_modelSystem, btVector3(m_skybox_scale,m_skybox_scale,m_skybox_scale), t);
// // 		m_skybox = boost::shared_ptr<ScClientMapResource>(new ScClientMapResource(m_fileSystem, mapName, m_graphicsSystem, m_modelSystem));
// 	}
	makeSkybox();
	
	activateFood();
}


// void Roundworld::drawWithGrid()
// {
// // 	glClear(GL_STENCIL_BUFFER_BIT);
// // 	glEnable(GL_CULL_FACE);
// // 	glCullFace(GL_BACK);
// 
// 	glPushMatrix(); 
// 		glTranslatef(0,0,0);
// 		glColor4f( 0.07f, 0.045f, 0.02f, 0.0f );
// 		drawSphere(
// 			*worldsizeX,
// 			(12.56637* *worldsizeX / 16),
// 			(12.56637* *worldsizeX / 8)
// 		);
// 	glPopMatrix(); 
// 	for( unsigned int i=0; i < critters.size(); ++i)
// 		critters[i]->draw(true);
// 
// 	for( unsigned int i=0; i < entities.size(); ++i)
// 		entities[i]->draw();
// 
// // 	glDisable(GL_STENCIL_TEST);
// // 	glDisable(GL_CULL_FACE);
// }

void Roundworld::childPositionOffset(btTransform* v)
{
// 	*v+= (v->normalized()*insertHeight);
	
// 	t.setOrigin( o.getOrigin().normalized() * 3.0f );
	
	
	// offset by one up
	btTransform o(*v);
	btTransform t;
	t.setIdentity();
	t.setOrigin( o.getOrigin().normalized() * insertHeight );
	
	*v = t * o;
	
	
}

void Roundworld::updateCameraTransform( const float timeDelta )
{
// 	if ( m_follow_critterP == 0 )
// 	{
// 		slerp( m_camera.getSceneNode()->getTransform(), m_follow_critter_transform_prev, timeDelta );
// 	}
// 	else
// 	{

	btTransform t2;
	float factor(0.0001);

	if ( *m_camera_mode == 3 )
	{
		m_camera.setSceneNode(&m_sceneNodeCamera);

// 		std::cout << "slerping" << std::endl;
// 		slerp( m_camera.getSceneNode()->getTransform(), m_follow_critter_transform_prev, timeDelta );
		t2 = m_camera.getSceneNode()->getTransform();
	}
	else
	{
		if ( m_follow_critterP == 0 )
		{
	// 		std::cout << "slerping" << std::endl;
// 			slerp( m_camera.getSceneNode()->getTransform(), m_follow_critter_transform_prev, timeDelta );
			t2 = m_camera.getSceneNode()->getTransform();
		}
		else
		{

	// 		// CRITTERPOSITION * CAMERA + LOOKAT
			if ( *m_camera_mode == 0 )
			{
				m_camera.setSceneNode(&m_sceneNodeCamera_follow1);
				
// 					const auto v_critter(m_follow_critterP->body.mouths[0]->ghostObject->getWorldTransform().getOrigin());
// 					const btVector3 v_critter_normalized( v_critter.normalized() );

				// CRITTER POSITION
					const btVector3 critter_pos_vector( m_follow_critterP->body.mouths[0]->ghostObject->getWorldTransform().getOrigin() );
					btTransform critter_pos_transform;
					critter_pos_transform.setIdentity();
					critter_pos_transform.setOrigin( critter_pos_vector );

					m_follow_critter_view3.setOrigin( m_follow_critter_view3.getOrigin() + m_camera.getSceneNode()->getTransform().getOrigin() );
					m_follow_critter_view3.setRotation( m_follow_critter_view3.getRotation() * m_camera.getSceneNode()->getTransform().getRotation() );
					m_camera.getSceneNode()->setTransform(m_null_transform);
					
				// RELATIVE CAMERA POSITION // FIXME somehow, we need this
					btTransform camera_pos_transform;
					camera_pos_transform.setIdentity();

				// RELATIVE CAMERA ROTATION
					btTransform camera_rot_transform;
					camera_rot_transform.setIdentity();
					camera_rot_transform.setRotation( m_follow_critter_view3.getRotation() );

				// LOOK DOWN
					btTransform look_down_transform;
					look_down_transform.setIdentity();
					look_down_transform.getBasis().setEulerZYX( -SIMD_HALF_PI, 0.0f, 0.0f );

				// TURN Y & Z ANGLES
					
					// FETCH ANGLES FROM VIEW
						btScalar rotation_x(0), rotation_y(0), rotation_z(0);
						m_follow_critter_view3.getBasis().getEulerYPR(rotation_x, rotation_y, rotation_z);

					// UPDATE ROTATIONS & CAP
						m_follow_critter_view3_roty += rotation_y;
						m_follow_critter_view3_rotz += rotation_z;
						
						if ( m_follow_critter_view3_roty > 6.30f )
							m_follow_critter_view3_roty -= SIMD_2_PI;
						else if ( m_follow_critter_view3_roty < -6.30f )
							m_follow_critter_view3_roty += SIMD_2_PI;
						if ( m_follow_critter_view3_rotz > 6.30f )
							m_follow_critter_view3_rotz -= SIMD_2_PI;
						else if ( m_follow_critter_view3_rotz < -6.30f )
							m_follow_critter_view3_rotz += SIMD_2_PI;
						
					// RESET VIEW ROTATION
						m_follow_critter_view3.setRotation( m_null_transform.getRotation() );

					// TRANSFORM
						btTransform turn_y;
						turn_y.setIdentity();
						turn_y.getBasis().setEulerZYX( m_follow_critter_view3_rotz, 0.0f, m_follow_critter_view3_roty );

					btTransform cam_to_critter_t;
					{
						cam_to_critter_t.setIdentity();
						const btVector3 up(0,0,1);
						const btVector3 direction = critter_pos_vector.normalized();

						const btScalar angle = acos(up.dot(direction));
						const btVector3 axis = up.cross(direction);
						btQuaternion cam_to_critter_t_rot(axis, angle);
						
						cam_to_critter_t.setRotation(cam_to_critter_t_rot);
					}

					btTransform length;
					length.setIdentity();
					length.setOrigin( btVector3( 0, 0, m_follow_critter_view3.getOrigin().getZ() ) );
// 					btTransform t2( critter_pos_transform * camera_pos_transform * cam_to_critter_t * turn_y * m_follow_critter_view3kink * m_startpos_t_follow_critter * length );

// 					slerp( t2, m_follow_critter_transform_prev, timeDelta );
					t2 = critter_pos_transform * camera_pos_transform * cam_to_critter_t * turn_y * m_follow_critter_view3kink * m_startpos_t_follow_critter * length;
			}
			
	
			// CRITTERPOSITION * CAMERA
			else if ( *m_camera_mode == 1 )
			{
				m_camera.setSceneNode(&m_sceneNodeCamera_follow2);
// 				// CRITTER POSITION
// 					btTransform t;
// 					t.setIdentity();
// 					t.setOrigin(m_follow_critterP->body.mouths[0]->ghostObject->getWorldTransform().getOrigin());
// 
// 				// CAMERA TRANSFORM OFFSET
// 					btTransform t2 = t * m_camera.getSceneNode()->getTransform();
// 					slerp( t2, m_follow_critter_transform_prev, timeDelta );
				
				
				
				// CRITTER POSITION
					const btVector3 critter_pos_vector( m_follow_critterP->body.mouths[0]->ghostObject->getWorldTransform().getOrigin() );
					btTransform critter_pos_transform;
					critter_pos_transform.setIdentity();
					critter_pos_transform.setOrigin( critter_pos_vector );

// 					m_follow_critter_view1.setOrigin( m_follow_critter_view1.getOrigin() + m_camera.getSceneNode()->getTransform().getOrigin() );
// 					m_follow_critter_view1.setRotation( m_follow_critter_view1.getRotation() * m_camera.getSceneNode()->getTransform().getRotation() );
// 					m_camera.getSceneNode()->setTransform(m_null_transform);
					
				// RELATIVE CAMERA POSITION // FIXME somehow, we need this
					btTransform camera_pos_transform;
					camera_pos_transform.setIdentity();

				// RELATIVE CAMERA ROTATION
					btTransform camera_rot_transform;
					camera_rot_transform.setIdentity();
					camera_rot_transform.setRotation( m_camera.getSceneNode()->getTransform().getRotation() );

				// LOOK DOWN
					btTransform look_down_transform;
					look_down_transform.setIdentity();
					look_down_transform.getBasis().setEulerZYX( -SIMD_HALF_PI, 0.0f, 0.0f );

				// TURN Y & Z ANGLES
					
// 					// FETCH ANGLES FROM VIEW
// 						btScalar rotation_x(0), rotation_y(0), rotation_z(0);
// 						m_follow_critter_view1.getBasis().getEulerYPR(rotation_x, rotation_y, rotation_z);
// 
// 					// UPDATE ROTATIONS & CAP
// 						m_follow_critter_view1_roty += rotation_y;
// 						m_follow_critter_view1_rotz += rotation_z;
// 						
// 						if ( m_follow_critter_view1_roty > 6.30f )
// 							m_follow_critter_view1_roty -= SIMD_2_PI;
// 						else if ( m_follow_critter_view1_roty < -6.30f )
// 							m_follow_critter_view1_roty += SIMD_2_PI;
// 						if ( m_follow_critter_view1_rotz > 6.30f )
// 							m_follow_critter_view1_rotz -= SIMD_2_PI;
// 						else if ( m_follow_critter_view1_rotz < -6.30f )
// 							m_follow_critter_view1_rotz += SIMD_2_PI;
// 						
// 					// RESET VIEW ROTATION
// 						m_follow_critter_view1.setRotation( m_null_transform.getRotation() );

// 					// TRANSFORM
// 						btTransform turn_y;
// 						turn_y.setIdentity();
// 						turn_y.getBasis().setEulerZYX( m_follow_critter_view1_rotz, 0.0f, m_follow_critter_view1_roty );

					btTransform cam_to_critter_t;
					{
						cam_to_critter_t.setIdentity();
						const btVector3 up(0,0,1);
						const btVector3 direction = critter_pos_vector.normalized();

						const btScalar angle = acos(up.dot(direction));
						const btVector3 axis = up.cross(direction);
						btQuaternion cam_to_critter_t_rot(axis, angle);
						
						cam_to_critter_t.setRotation(cam_to_critter_t_rot);
					}

					btTransform length;
					length.setIdentity();
					length.setOrigin( m_camera.getSceneNode()->getTransform().getOrigin() );

// 					btTransform t2( critter_pos_transform * camera_pos_transform * cam_to_critter_t * m_follow_critter_view3kink * m_startpos_t_follow_critter * camera_rot_transform * length );
// 					slerp( t2, m_follow_critter_transform_prev, timeDelta );
					t2 = critter_pos_transform * camera_pos_transform * cam_to_critter_t * m_follow_critter_view3kink * m_startpos_t_follow_critter * camera_rot_transform * length;
			}
// 
			// CRITTERPOSITION+ROTATION * CAMERA = critter viewpoint
				else if ( *m_camera_mode == 2 )
				{
						m_camera.setSceneNode(&m_sceneNodeCamera_follow3);
					// CRITTER POSITION
						btTransform t;
						t.setIdentity();
						t.getBasis().setEulerZYX( 0.0f, -SIMD_HALF_PI, 0.0f );

					// CAMERA TRANSFORM OFFSET
// 						btTransform t2(m_follow_critterP->body.mouths[0]->ghostObject->getWorldTransform() * t * m_camera.getSceneNode()->getTransform());
// 						slerp( t2, m_follow_critter_transform_prev, smooth_factor * 5 );

						t2 = m_follow_critterP->body.mouths[0]->ghostObject->getWorldTransform() * t * m_camera.getSceneNode()->getTransform();
// 						btTransform t2(m_follow_critterP->body.mouths[0]->ghostObject->getWorldTransform() * t * m_camera.getSceneNode()->getTransform());
// 						slerp( t2, m_follow_critter_transform_prev, timeDelta*10 );
						factor = 0.0005;
				}
// 			// CRITTERPOSITION+ROTATION * CAMERA = critter viewpoint
// 			else if ( *m_camera_mode == 2 )
// 			{
// 				m_camera.setSceneNode(&m_sceneNodeCamera_follow3);
// 				// CRITTER POSITION
// 					btTransform t;
// 					t.setIdentity();
// 					t.getBasis().setEulerZYX( 0.0f, -SIMD_HALF_PI, 0.0f );
// 
// 					m_follow_critter_view2.setOrigin( m_follow_critter_view2.getOrigin() + m_camera.getSceneNode()->getTransform().getOrigin() );
// 					m_follow_critter_view2.setRotation( m_follow_critter_view2.getRotation() * m_camera.getSceneNode()->getTransform().getRotation() );
// 					m_camera.getSceneNode()->setTransform(m_null_transform);
// 					
// 				
// 				// CAMERA TRANSFORM OFFSET
// 					btTransform t2(m_follow_critterP->body.mouths[0]->ghostObject->getWorldTransform() * t * m_follow_critter_view2);
// 					slerp( t2, m_follow_critter_transform_prev, timeDelta*10 );
// 			}
			
		}

	}

	const float smooth_factor( factor * timeDelta * ( (1001 - *m_camera_smoothfactor) ) );
// 	std::cout << smooth_factor << std::endl;
	if ( *m_camera_smoothfactor == 0 || smooth_factor >= 1.0 )
	{
		m_follow_critter_transform_prev = t2;
	}
	else
	{
		slerp( t2, m_follow_critter_transform_prev, smooth_factor );
	}
	// 	std::cout << "cam updated" << std::endl;
}


void Roundworld::resetCamera()
{
	int biggest = *worldsizeX;
	if ( *worldsizeY > biggest )
		biggest = *worldsizeY;
	
	
	if ( &m_sceneNodeCamera == m_camera.getSceneNode() )
	{
		btTransform t;
		t.setIdentity();
		t.setOrigin( btVector3(0.0f, 0.0f, 0.31f * biggest) );
		m_camera.getSceneNode()->setTransform(t);
	}
	else
	{
		reset_follow_location();		
// 			m_camera.getSceneNode()->setTransform(m_null_transform);
	}
	
	
	
	
	
	
	
// 	m_camera.getSceneNode()->setOrigin( btVector3( 0.0f, 0.0f, 0.0f ) );
// 	m_camera.getSceneNode()->setOrigin( btVector3( 0.0f, 0.35f * biggest, 0.0f ) );
	
	
// 	camera.position.setRotation(btQuaternion(btVector3(1, 0, 0), btScalar(90)));
// 	m_camera.getSceneNode()->pitch( -SIMD_HALF_PI ); // 1.5707f  (float)*energy/10
// 	m_camera.getSceneNode()->yaw( -SIMD_HALF_PI ); // 1.5707f  (float)*energy/10


// 	camera.position = btVector3( -0.5f**worldsizeX, -1.3f*biggest, -0.5f**worldsizeY);
// 	camera.rotation = Vector3f( 90.0f,  0.0f, 0.0f);
}


btVector3 Roundworld::findPosition()
{
	btVector3 v;
	v.setX((0.1**worldsizeX) + (1.1*insertHeight));
	v.setY((0.1**worldsizeY) + (1.1*insertHeight));
	v.setZ((0.1**worldsizeZ) + (1.1*insertHeight));

// 	v.setX(sqrt(*worldsizeX) + insertHeight);
// 	v.setY(sqrt(*worldsizeY) + insertHeight);
// 	v.setZ(sqrt(*worldsizeZ) + insertHeight);
	
// 	btVector3 v;
	return 
		btVector3(	((float)randgen->Instance()->get( 0, 200 )-100.0f) / 1000,
					((float)randgen->Instance()->get( 0, 200 )-100.0f) / 1000,
					((float)randgen->Instance()->get( 0, 200 )-100.0f) / 1000
	).normalized() * v ;
	
	
// 	return btVector3(	((float)randgen->Instance()->get( 0, 200 )-100.0f) / 1000,
// 				((float)randgen->Instance()->get( 0, 200 )-100.0f) / 1000,
// 				((float)randgen->Instance()->get( 0, 200 )-100.0f) / 1000
// 	).normalized() * ( *worldsizeX + insertHeight);
}


void Roundworld::drawfloor()
{
	sun_position = m_skyTransform * static_sun_pos;
	setLights();

	m_graphicsSystem->color( BeColor(1.0f, 0.0f, 0.0f, 0.0f) );
// 	glColor4f( 1.0f, 1.0f, 1.0f, 0.0f );
	if ( m_map )
	{
// 		glPushMatrix();
		m_map->get()->draw(m_map_scale);
// 		glPopMatrix();
	}	
// 	else
// 	{
// 		glPushMatrix(); 
// 			glColor4f( m_color_wall.r(), m_color_wall.g(), m_color_wall.b(), 0.0f );
// 			drawSphere(
// 				*worldsizeX,
// 				((12.56637* *worldsizeX) / 28),
// 				((12.56637* *worldsizeX) / 14)
// 			);
// 		glPopMatrix(); 
// 	}

	if (m_skyBoxModel)
	{
		const auto prog(m_graphicsSystem->getActiveProgram());

		if ( prog != 0 )
			m_graphicsSystem->useProgram(0);
		
		glDisable(GL_LIGHTING);
			m_skyBoxModel->get()->setContext( 0 );
			m_skyBoxModel->get()->draw(0, m_skyTransform);
		glEnable(GL_LIGHTING);

		if ( prog != 0 )
			m_graphicsSystem->useProgram(prog);
	}
}

void Roundworld::drawSphere(btScalar radius, int lats, int longs) 
{
	int i, j;
	for(i = 0; i <= lats; ++i) {
		btScalar lat0 = SIMD_PI * (-btScalar(0.5) + (btScalar) (i - 1) / lats);
		btScalar z0  = radius*sin(lat0);
		btScalar zr0 =  radius*cos(lat0);

		btScalar lat1 = SIMD_PI * (-btScalar(0.5) + (btScalar) i / lats);
		btScalar z1 = radius*sin(lat1);
		btScalar zr1 = radius*cos(lat1);

		glBegin(GL_QUAD_STRIP);
		for(j = 0; j <= longs; ++j) {
			btScalar lng = 2 * SIMD_PI * (btScalar) (j - 1) / longs;
			btScalar x = cos(lng);
			btScalar y = sin(lng);
			glNormal3f(x * zr1, y * zr1, z1);
			glVertex3f(x * zr1, y * zr1, z1);
			glNormal3f(x * zr0, y * zr0, z0);
			glVertex3f(x * zr0, y * zr0, z0);
		}
		glEnd();
	}
}

// void Roundworld::setLights()
// {
// // 	if ( !pause )
// // 	{
// // 		++sunCounter;
// // 		sun_offset.getBasis().setEulerZYX( 0.00006 * sunCounter, 0.0f, 0.0f );
// // 	}
// // 
// // 	sun_position = sun_offset * static_sun_pos;
// // 	const btVector3& pos(sun_position.getOrigin());
// // 	
// // 	GLfloat lightPos[] = { pos.x(), pos.y(), pos.z(), 0.0f };
// 	
// 		sun_position = m_skyTransform * static_sun_pos;
// 	// 	sun_position = t * sun_offset * static_sun_pos;
// 		const btVector3& pos(sun_position.getOrigin());
// 		
// 		GLfloat lightPos[] = { pos.x(), pos.y(), pos.z(), 1.0f };
// 
// 		
// 		
// 		
// 		
// 	
// // 			GLfloat lightPos[] = { 0.25f * *worldsizeX, 0.5f * *worldsizeX, 0.25f * *worldsizeY, 1.0f };
// 	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
// 
// // 				GLfloat lightDirection[] = { -pos.x(), -pos.y(), -pos.z() }; // IF YOU USE THIS YOU HAVE TO NORMALIZE THIS
// // 				glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightDirection);
// 	
// 	GLfloat lightCutoff[] = { 180.0f }; // angle is 0 to 180
// 	glLightfv(GL_LIGHT0, GL_SPOT_CUTOFF, lightCutoff);
// 
// 	GLfloat lightExponent[] = { 128.0f }; // exponent is 0 to 128
// 	glLightfv(GL_LIGHT0, GL_SPOT_EXPONENT, lightExponent);
// 	
// 	GLfloat lightAttenuation[] = { 0.015f };
// 	glLightfv(GL_LIGHT0, GL_CONSTANT_ATTENUATION, lightAttenuation);
// 	
// 	GLfloat lightLAttenuation[] = { 0.0015f };
// 	glLightfv(GL_LIGHT0, GL_LINEAR_ATTENUATION, lightLAttenuation);
// 
// 	GLfloat lightQAttenuation[] = { 0.00002f };
// 	glLightfv(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, lightQAttenuation);
// }



Roundworld::~Roundworld()
{
// 	m_dynamicsWorld->removeCollisionObject(fixedGround);
// 	delete groundShape;
// 	delete fixedGround;
}
