//         //update shape sizes
//         for (unsigned i = 0; i < numNeurons; ++i) {
//             btCollisionShape* bt = neuronShape[i];
//             float s = neuronSize;
//             float w = s * (1.0 + (fabs(brain->neurons[i]->getOutput())));
//             float h = s * (1.0 + sqrt(fabs(brain->neurons[i]->potential)));
//             bt->setLocalScaling(btVector3(w, h, (w + h) / 2.0));
//         }

// FIXME benchmark NOT checking distance for every bodypart.
// FIXME when picking body, not mouving mouse but moving, bad cam->screendirection

#include "filesystem/be_filesystem.h"
#include "worldb.h"
#include "graphics/sc_graphics.h"

#include "common/be_mouse_picker.h"
#include "statsbuffer.h"
#include "gui/logbuffermessage.h"
#include "dirlayout.h"
#include "../gui/textverbosemessage.h"
#include "critterselection.h"
#include "food.h"
#include "wall.h"
#include "critterb.h"
#include "resource/sc_client_map_resource.h"
#include "genotype.h"

#ifdef HAVE_OPENMP
	#include <omp.h>
#endif
#include <iomanip>


using namespace std;

void WorldB::CollisionNearOverride(btBroadphasePair& collisionPair, btCollisionDispatcher& dispatcher, const btDispatcherInfo& dispatchInfo)
{
	btRigidBody* b1 = static_cast<btRigidBody*>(collisionPair.m_pProxy0->m_clientObject);
	btRigidBody* b2 = static_cast<btRigidBody*>(collisionPair.m_pProxy1->m_clientObject);
	Entity* e1 = static_cast<Entity*>(b1->getUserPointer());
	Entity* e2 = static_cast<Entity*>(b2->getUserPointer());
	if(e1 == NULL || e2 == NULL)
	{
		//cout << "Entity cast error in collide callback." << endl;
// 		return dispatcher.defaultNearCallback(collisionPair, dispatcher, dispatchInfo);
// 		dispatcher.defaultNearCallback(collisionPair, dispatcher, dispatchInfo);
	}
// 	else if(e1->type == CRITTER && e2->type == CRITTER)
// 	{
		else if(e1 == e2)
		{
			//cout << "Not colliding." << endl;
			return;
		}
	//cout << "Colliding." << endl;
// 	}
	dispatcher.defaultNearCallback(collisionPair, dispatcher, dispatchInfo);
}

WorldB::WorldB(  boost::shared_ptr<BeGraphicsSystem> system, BeFilesystem& fileSystem, boost::shared_ptr<Textverbosemessage> textverbosemessage, const bool lock_axis )
 : m_camera(SIMD_HALF_PI/2, 1024.0f/768.0f, 0.2f, 1800.0f)
 , settings(Settings::Instance())
 , retinasperrow(119)
 , m_threads_last(1)
 , m_graphicsSystem(system)
 , m_panel_textverbosemessage(textverbosemessage)
 , m_fileSystem(fileSystem)
//  , m_skyBoxAngle(0)
 , mouseRayTo(0.0f, 0.0f, 0.0f)
//  , m_color_mouth(1.0f, 0.0f, 0.0f, 0.0f)
//  , m_color_wall(0.28f, 0.18f, 0.08f, 0.0f)
//  , m_initial_worldsizeX( settings->getCVar("worldsizeX") )
//  , m_initial_worldsizeY( settings->getCVar("worldsizeY") )
//  , m_initial_worldsizeZ( settings->getCVar("worldsizeZ") )
 , m_initial_worldsizeX( 260 )
 , m_initial_worldsizeY( 100 )
 , m_initial_worldsizeZ( 140 )
 , m_skybox_offset(0,0,0)
 , m_follow_critterP(0)
//  , sunCounter(0)
 , m_skyBoxAngles(2.0f,2.0f,87.0f)
 , m_follow_critter_view3_roty(0)
 , m_follow_critter_view3_rotz(0)
 , drawscene(settings->getCVarPtr("drawscene"))
 , drawdebug(settings->getCVarPtr("drawdebug"))
 , m_sceneNodeCamera(lock_axis)
 , m_sceneNodeCamera_follow1(true)
 , m_sceneNodeCamera_follow2(true)
 , m_sceneNodeCamera_follow3(true)

//  , m_currentSceneNode(m_sceneNodeCamera)
//  , m_initial_worldsizeY( 140 )
//  , m_initial_worldsizeZ( 100 )
 
//  , m_color_default(1.0f, 1.0f, 1.0f, 0.0f)
 

 
//  , m_extinctionmode(false)
//  , m_extinctionmode_until(0)
{
	
	const unsigned int m_starttime(time(0));

	// hostname
	char hn[256] = "";
	gethostname(hn, sizeof(hn));

	std::stringstream ident;
	ident << hn << "(" << m_starttime << ")";
	m_starttime_s = ident.str();
	
// 	std::stringstream s;
// 	s << ident.str();
// 	m_starttime_s = s.str();

	m_camera.setSceneNode(&m_sceneNodeCamera);

	// settings and pointers
// 		retinasperrow = settings->getCVarPtr("retinasperrow");
		m_glsl = settings->getCVarPtr("glsl");
		m_hdr = settings->getCVarPtr("hdr");

		critter_maxlifetime = settings->getCVarPtr("critter_maxlifetime");
		critter_maxenergy = settings->getCVarPtr("critter_maxenergy");
		critter_autosaveinterval = settings->getCVarPtr("critter_autosaveinterval");
		critter_autoexchangeinterval = settings->getCVarPtr("critter_autoexchangeinterval");
		population_double = settings->getCVarPtr("population_double");
		critter_retinasize = settings->getCVarPtr("critter_retinasize");
		critter_sightrange = settings->getCVarPtr("critter_sightrange");
// 		critter_raycastvision = settings->getCVarPtr("critter_raycastvision");
		critter_enableomnivores = settings->getCVarPtr("critter_enableomnivores");
		critter_startenergy = settings->getCVarPtr("critter_startenergy");
		setting_critter_mutate_maxlifetime = settings->getCVarPtr("critter_mutate_maxlifetime");

		brain_mutationrate = settings->getCVarPtr("brain_mutationrate");
		body_mutationrate = settings->getCVarPtr("body_mutationrate");
		
		m_camera_mode = settings->getCVarPtr("camera_mode");
		m_camera_smoothfactor = settings->getCVarPtr("camera_smoothfactor");

		food_maxlifetime = settings->getCVarPtr("food_maxlifetime");
		food_maxenergy = settings->getCVarPtr("food_maxenergy");
		energy = settings->getCVarPtr("energy");
// 		headless = settings->getCVarPtr("headless");
		threads = settings->getCVarPtr("threads");
		mincritters = settings->getCVarPtr("mincritters");
		insertcritterevery = settings->getCVarPtr("critter_insertevery");
		worldsizeX = settings->getCVarPtr("worldsizeX");
		worldsizeY = settings->getCVarPtr("worldsizeY");
		worldsizeZ = settings->getCVarPtr("worldsizeZ");

		skybox_rotationX = settings->getCVarPtr("skybox_rotationX");
		skybox_rotationY = settings->getCVarPtr("skybox_rotationY");
		skybox_rotationZ = settings->getCVarPtr("skybox_rotationZ");
		m_skybox_scale = settings->getCVarPtr("skybox_scale");
		

		population_limit_energy = settings->getCVarPtr("population_limit_energy");
		population_limit_energy_percent = settings->getCVarPtr("population_limit_energy_percent");

		population_eliminate_portion = settings->getCVarPtr("population_eliminate_portion");
		population_eliminate_portion_percent = settings->getCVarPtr("population_eliminate_portion_percent");
		population_eliminate_portion_decrenergy = settings->getCVarPtr("population_eliminate_portion_decrenergy");
		population_eliminate_portion_incrworldsizeX = settings->getCVarPtr("population_eliminate_portion_incrworldsizeX");
		population_eliminate_portion_incrworldsizeY = settings->getCVarPtr("population_eliminate_portion_incrworldsizeY");
		population_eliminate_portion_incrworldsizeZ = settings->getCVarPtr("population_eliminate_portion_incrworldsizeZ");
		population_eliminate_portion_decrmaxlifetimepct = settings->getCVarPtr("population_eliminate_portion_decrmaxlifetimepct");

		population_eliminate_portion_brainmutationratechange = settings->getCVarPtr("population_eliminate_portion_brainmutationratechange");
		population_eliminate_portion_brainmutationratemin = settings->getCVarPtr("population_eliminate_portion_brainmutationratemin");
		population_eliminate_portion_brainmutationratemax = settings->getCVarPtr("population_eliminate_portion_brainmutationratemax");
		population_eliminate_portion_bodymutationratechange = settings->getCVarPtr("population_eliminate_portion_bodymutationratechange");
		population_eliminate_portion_bodymutationratemin = settings->getCVarPtr("population_eliminate_portion_bodymutationratemin");
		population_eliminate_portion_bodymutationratemax = settings->getCVarPtr("population_eliminate_portion_bodymutationratemax");

	statsBuffer = Statsbuffer::Instance();
	critterselection = Critterselection::Instance();
	// home & program directory
	dirlayout = Dirlayout::Instance();

	m_freeEnergy = *food_maxenergy * *energy;
		
	currentCritterID	= 1;
	insertCritterCounter	= 0;
	foodIntervalCounter = 0;
	critterIntervalCounter = 0;
	autosaveCounter		= 0.0f;
	autoexchangeCounter	= 0.0f;
	insertHeight		= 20.0f;
	pause = false;
	mouselook = false;
	mousex = -100;
	mousey = -100;

	
	// vision retina allocation
	items = 4 * 2048 * 2048;
	retina = (unsigned char*)malloc(items);
	memset(retina, 0, items);

	// THREADED BULLET
// 	int maxNumOutstandingTasks = 6;
// 	m_collisionConfiguration = new btDefaultCollisionConfiguration();
// 	PosixThreadSupport::ThreadConstructionInfo constructionInfo("collision", processCollisionTask, createCollisionLocalStoreMemory, maxNumOutstandingTasks);
// 	m_threadSupportCollision = new PosixThreadSupport(constructionInfo);
// 	m_dispatcher = new SpuGatheringCollisionDispatcher(m_threadSupportCollision,maxNumOutstandingTasks,m_collisionConfiguration);
// 	
// 	btVector3 worldAabbMin(-10000,-10000,-10000);
// 	btVector3 worldAabbMax(10000,10000,10000);
// 	m_broadphase = new btAxisSweep3 (worldAabbMin, worldAabbMax);
// 	m_broadphase->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
// // 	m_solver = new btSequentialImpulseConstraintSolver;
// // 	m_solver = new SpuMinkowskiPenetrationDepthSolver();
// 	m_solver = new btParallelConstraintSolver();
// 	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher,m_broadphase,m_solver,m_collisionConfiguration);
// 
// // 		m_dynamicsWorld->getSolverInfo().m_numIterations = 10;
// // 		m_dynamicsWorld->getSolverInfo().m_solverMode = SOLVER_SIMD+SOLVER_USE_WARMSTARTING;
// 
// // 		m_dynamicsWorld->getDispatchInfo().m_enableSPU = true;
	// stop threaded bullet

	// NOT THREADED
	m_collisionConfiguration = new btDefaultCollisionConfiguration();
	m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
	if(settings->getCVar("body_selfcollisions") == 0)
		m_dispatcher->setNearCallback(&WorldB::CollisionNearOverride);
	
	btVector3 worldAabbMin(-1000,-1000,-1000);
	btVector3 worldAabbMax(1000,1000,1000);
	m_broadphase = new btAxisSweep3 (worldAabbMin, worldAabbMax);
	m_ghostpaircallback = new btGhostPairCallback();
	m_broadphase->getOverlappingPairCache()->setInternalGhostPairCallback(m_ghostpaircallback);
	m_solver = new btSequentialImpulseConstraintSolver;

	
// 	m_dynamicsWorldSPtr.reset( new btDiscreteDynamicsWorld(m_dispatcher,m_broadphase,m_solver,m_collisionConfiguration) );
// 	m_dynamicsWorld = m_dynamicsWorldSPtr.get();
	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher,m_broadphase,m_solver,m_collisionConfiguration);
	// END NOT THREADED
	
// 	m_dynamicsWorld->setGravity( btVector3(0.0f, -50.0f, 0.0f) );

// 	m_dynamicsWorld->getSolverInfo().m_solverMode = SOLVER_USE_WARMSTARTING | SOLVER_SIMD;
	m_dynamicsWorld->getSolverInfo().m_solverMode = SOLVER_USE_WARMSTARTING | SOLVER_SIMD | SOLVER_ENABLE_FRICTION_DIRECTION_CACHING;
	m_dynamicsWorld->getSolverInfo().m_numIterations = 8;

	// raycast
	raycast = new Raycast(m_dynamicsWorld);

	// mousepicker
	mousepicker = new BeMousePicker(m_dynamicsWorld);

	// determine vision width
	picwidth = retinasperrow * (*critter_retinasize+1);

	// threading locks
// 	omp_init_lock(&my_lock1);
// 	omp_init_lock(&my_lock2);

	// create the needed ext bundings for fbos
// 	if ( !*headless )
	{
		// generate namespace for the frame buffer, colorbuffer and depthbuffer
		glGenFramebuffersEXT(1, &fb);
		glGenTextures(1, &color_tex); 
		glGenRenderbuffersEXT(1, &depth_rb);

		//switch to our fbo so we can bind stuff to it
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb);
		//create the colorbuffer texture and attach it to the frame buffer
		glBindTexture(GL_TEXTURE_2D, color_tex);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 2048, 2048, 0, GL_RGBA, GL_INT, NULL);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, color_tex, 0); 
		
		// create a render buffer as our depthbuffer and attach it
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depth_rb);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, 2048, 2048);
// 		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, 2048, 2048);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depth_rb);
		
		// Go back to regular frame buffer rendering
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		
		
// 		//RGBA8 2D texture, 24 bit depth texture, 256x256
// 		glGenTextures(1, &color_tex);
// 		glBindTexture(GL_TEXTURE_2D, color_tex);
// // 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
// // 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
// // 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
// // 		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
// 		//NULL means reserve texture memory, but texels are undefined
// // 		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 256, 256, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);
// 		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 2048, 2048, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
// 		//-------------------------
// 		glGenFramebuffersEXT(1, &fb);
// 		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb);
// 		//Attach 2D texture to this FBO
// 		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, color_tex, 0);
// 		//-------------------------
// 		glGenRenderbuffersEXT(1, &depth_rb);
// 		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depth_rb);
// 		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, 2048, 2048);
// 		//-------------------------
// 		//Attach depth buffer to FBO
// 		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depth_rb);
// 		//-------------------------




		//Does the GPU support current FBO configuration?
		GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
		switch(status)
		{
			case GL_FRAMEBUFFER_COMPLETE_EXT:
// 				std::cout << "FRAMEBUFFER : GOOD" << std::endl;
				break;
			default:
				std::cout << "FRAMEBUFFER : NOT GOOD" << std::endl;
				break;
		}
	}

// 	// threads
// 	unsigned hardwareConcurrency = settings->getCVar("threads") + 1;
// 	// 0 : autodetect
// 	if ( hardwareConcurrency == 0 )
// 	{
// 		hardwareConcurrency = boost::thread::hardware_concurrency();
// 		std::cout << "::WORKERPOOL threads " << hardwareConcurrency << "\n";
// 	}
// 	// >0 : threads - 1 (excluding main thread)
// 	else
// 	{
// 		hardwareConcurrency -= 1;
// 		std::cout << "::WORKERPOOL threads " << hardwareConcurrency+1 << "\n";
// 	}
// 
// 	m_workerPool = boost::shared_ptr<BeWorkerPool>(new BeWorkerPool(hardwareConcurrency));
    m_modelSystem.reset(new BeGraphicsModelSystem(m_fileSystem));


	
	// FIXME move to physics?
	m_geometryModelSystem.reset(new BeGeometrySystem);
	
	
	m_render_offset2.setIdentity();
	m_render_offset2.getBasis().setEulerZYX( 0.0f, 0.0f, SIMD_HALF_PI );

// 	videocap = Videocap::Instance();
		
// 	m_follow_critter_transform.setIdentity();
// 	m_follow_critter_transform.setOrigin( btVector3(0,50,0) );

	sun_position.setIdentity();

// 	static_sun_pos.setOrigin( btVector3( 0.0f, 7.0f * *worldsizeX, 0.0f ) );
	
	btVector3 sundirection( 500.0f, -450.0, 0.0f );
	sundirection = sundirection.normalized();
	const float distance(818.0f);
// 	const float distance(681.66f * m_skybox_scale);
	
	static_sun_pos.setIdentity();
	static_sun_pos.setOrigin( sundirection * distance );

	m_follow_critter_transform_prev.setIdentity();
// 	sun_offset.setIdentity();


	m_sceneNodeCamera_follow2.setOrigin(btVector3(0,10,0));
	btTransform t;
	t.setIdentity();
	t.getBasis().setEulerZYX( -0.5f, 0.0f, 0.0f );
	m_sceneNodeCamera_follow2.setRotation(t.getRotation());
// 	m_follow_critter_view1.setIdentity();
// 	m_follow_critter_view1.setOrigin(btVector3(0,10,0));
// 	m_follow_critter_view1.getBasis().setEulerZYX( -0.5f, 0.0f, 0.0f );
	
	
// 	m_follow_critter_view2.setIdentity();
	m_follow_critter_view3.setIdentity();
	m_follow_critter_view3.getBasis().setEulerZYX( -0.5f, 0.0f, 0.0f );

	m_startpos_t_follow_critter.setIdentity();
	m_startpos_t_follow_critter.setOrigin(btVector3(0,0,20));
	
	m_null_transform.setIdentity();

	// ALL OK UPSIDE DOWN
		m_follow_critter_view3kink.setIdentity();
		m_follow_critter_view3kink.getBasis().setEulerZYX( SIMD_HALF_PI, 0.0f, 0.0f );
	

	if ( !m_model_critter )
	{
		load(m_fileSystem, "critter");
		
		const auto& m = m_modelSystem->load(m_path, m_graphicsSystem, m_modelSystem, btVector3(1,1,1), m_null_transform);
		m_model_critter = m->get();
	}
	if ( !m_model_food )
	{
		load(m_fileSystem, "food");
		
		const auto& m = m_modelSystem->load(m_path, m_graphicsSystem, m_modelSystem, btVector3(1.81f,1.81f,1.81f), m_null_transform);
		m_model_food = m->get();
	}
	
	
}

void WorldB::load(BeFilesystem& filesystem, const std::string& path)
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



void WorldB::init()
{
	makeFloor();

	if ( settings->getCVar("autoload") )
		loadAllCritters();
	if ( settings->getCVar("autoloadlastsaved") )
		loadAllLastSavedCritters();
	
	m_skyTransform.setIdentity();
	
	// reset cam
		resetCamera();
	
}

// void WorldB::initShared()
// {
// 	// reset cam
// 		resetCamera();
// }

void WorldB::castMouseRay()
{
// 	cout << "casting" << endl;

	mouseRay = raycast->cast( getCameraTransform().getOrigin(), mouseRayTo );
// 	mouseRay = raycast->cast( camera.position.getOrigin(), mouseRayTo );
	mouseRayHit = false;
	if ( mouseRay.m_hit )
	{
		if ( !( mouseRay.m_hitBody->isStaticObject()/* || mouseRay.m_hitBody->isKinematicObject()*/ ) )
		{
			Entity* e = static_cast<Entity*>(mouseRay.m_hitBody->getUserPointer());
			if ( e->type == FOOD || e->type == CRITTER )
			{
				
				mouseRayHit = true;
				mouseRayHitEntity = e;
			}
		}
	}
}

void WorldB::calcMouseDirection()
{
// 	cout << "updating mouserayto" << endl;
	mouseRayTo = m_camera.getScreenDirection(mousex, mousey, getCameraTransform());
}

// void WorldB::moveInMouseDirection(bool towards)
// {
// // 	cout << "updating mouserayto" << endl;
// // 	if ( towards )
// // 		camera.moveTowards(mouseRayTo.normalized());
// // 	else
// // 		camera.moveAwayFrom(mouseRayTo.normalized());
// }

void WorldB::unpickBody()
{
	if ( *drawscene == 1 || *drawdebug > 0 )
	{
		mousepicker->detach();
	}
}

void WorldB::pickBody()
{
	if ( *drawscene == 1 || *drawdebug > 0 )
	{
		mousepicker->detach();

		if ( mouseRayHit )
		{
			if ( mouseRayHitEntity->type == FOOD || mouseRayHitEntity->type == CRITTER )
			{
				const btRigidBody* b = static_cast<const btRigidBody*>(mouseRay.m_hitBody);

	// 			// if critter, and it's the head's ghostobject we touch, overwrite with head's body
	// 			if ( mouseRayHitEntity->type == CRITTER )
	// 			{
	// 				const btCollisionObject* co = static_cast<const btCollisionObject*>(b);
	// 				CritterB* c = static_cast<CritterB*>(mouseRayHitEntity);
	// 				if ( co == c->body.mouths[0]->ghostObject )
	// 				{
	// 					b = c->body.mouths[0]->body;
	// 				}
	// 			}
				
				btRigidBody* new_b(0);
	// 			for( unsigned int j(0); j < entities.size() && new_b == 0; ++j)
	// 			{	
	// 				if ( entities[j]->type == FOOD )
	// 				{
	// 					Food* f = static_cast<Food*>( entities[j] );
	// 					for( unsigned int i(0); i < f->body.bodyparts.size(); ++i)
	// 					{
	// 						if ( f->body.bodyparts[i]->body == b )
	// 						{
	// 							new_b = f->body.bodyparts[i]->body;
	// 							break;
	// 						}
	// 					}
	// 				}
	// 			}
				for( unsigned int j(0); j < food_units.size() && new_b == 0; ++j)
				{	
					Food* f(food_units[j]);
					for( unsigned int i(0); i < f->body.bodyparts.size(); ++i)
					{
						if ( f->body.bodyparts[i]->body == b )
						{
							new_b = f->body.bodyparts[i]->body;
							break;
						}
					}
				}
				for( unsigned int j=0; j < critters.size() && new_b == 0; ++j)
				{
					CritterB* f = critters[j];
					for( unsigned int i(0); i < f->body.bodyparts.size(); ++i)
					{
						if ( f->body.bodyparts[i]->body == b )
						{
							new_b = f->body.bodyparts[i]->body;
							break;
						}
						else
						{
							const btCollisionObject* co = static_cast<const btCollisionObject*>(b);
							if ( co == f->body.mouths[0]->ghostObject )
							{
								new_b = f->body.mouths[0]->body;
								break;
							}
							
						}
					}
				}
				
				if ( new_b )
				{
					mousepicker->attach( new_b, mouseRay.m_hitPosition, getCameraTransform().getOrigin() );
	// 				mousepicker->attach( new_b, mouseRay.m_hitPosition, m_sceneNodeCamera.getOrigin(), mouseRayTo );
	// 				mousepicker->attach( new_b, mouseRay.m_hitPosition, m_sceneNodeCamera.getOrigin(), mouseRay.m_hitPosition );
					mousepicker->m_pickedBoolP = &mouseRayHitEntity->isPicked;
					*mousepicker->m_pickedBoolP = true;
		// 			cout << "picked body" << endl;
				}
			}
		}
	}
}

int WorldB::selectBody()
{
	if ( mouseRayHit )
		if ( mouseRayHitEntity->type == CRITTER )
		{
			CritterB* c = static_cast<CritterB*>(mouseRayHitEntity);
			if (critterselection->registerCritter(c))
			{
				for ( unsigned int i=0; i < critterselection->clist.size(); ++i )
					if ( critterselection->clist[i]->critterID == c->critterID )
						return i;
			}
			else
				critterselection->unregisterCritter(c);
				
		}
	return -1;
}

void WorldB::deselectBody()
{
	if ( mouseRayHit )
		if ( mouseRayHitEntity->type == CRITTER )
			critterselection->unregisterCritter(static_cast<CritterB*>(mouseRayHitEntity));
}

void WorldB::movePickedBodyTo()
{
	if ( mousepicker->m_active )
	{
		calcMouseDirection();
		mousepicker->moveTo( getCameraTransform().getOrigin(), mouseRayTo );
	}
}

void WorldB::movePickedBodyFrom()
{
	if ( mousepicker->m_active )
	{
		calcMouseDirection();
		mousepicker->moveFrom( getCameraTransform().getOrigin() );
	}
}

void WorldB::calcFreeEnergy()
{
	
	// determine percentage of energy we want to maintain
	
	// normal ratio when population is lower than limit
	float m_energy_ratio(1.0f);
	
	// if nr critters is over the population_limit_energy, use population_limit_energy_percent
	if ( critters.size() >= (unsigned int)*population_limit_energy && *population_limit_energy > 0 )
	{
		m_energy_ratio = 0.01f * *population_limit_energy_percent;
// 		std::cout << m_energy_ratio << std::endl;
	}
	
	m_freeEnergy = m_energy_ratio * *food_maxenergy * *energy;

	const auto& end(critters.end());
	for( auto it(critters.begin()); it != end; ++it)
	{
		m_freeEnergy -= (*it)->energyLevel;
	}

	const auto& e_end(food_units.end());
	for( auto it(food_units.begin()); it != e_end; ++it)
	{
		m_freeEnergy -= (*it)->energyLevel;
	}
	
	
}

void WorldB::updateSkyboxAngles()
{
// 		m_skyBoxAngle += 0.15f * timer.getSeconds();
// 		m_skyBoxAngle += 0.002f;
// 		m_skyBoxAngle += 0.0f;
		
		const double factor( 0.0174532925199 ); // (1.0/180) * SIMD_PI;

		btVector3 skyboxAngles(*skybox_rotationX, *skybox_rotationY, *skybox_rotationZ);
		
// 		m_skyBoxAngleX += 0.0001f * *skybox_rotationX;
// 		m_skyBoxAngleY += 0.0001f * *skybox_rotationY;
// 		m_skyBoxAngleZ += 0.0001f * *skybox_rotationZ;

		
		m_skyBoxAngles += skyboxAngles * 0.0001f;
		
		if ( m_skyBoxAngles.x() > 360.0f )
			m_skyBoxAngles.setX(m_skyBoxAngles.x()-360.0f);
		if ( m_skyBoxAngles.y() > 360.0f )
			m_skyBoxAngles.setY(m_skyBoxAngles.y()-360.0f);
		if ( m_skyBoxAngles.z() > 360.0f )
			m_skyBoxAngles.setZ(m_skyBoxAngles.z()-360.0f);
		
		m_skyTransform.getBasis().setEulerZYX( factor*m_skyBoxAngles.x(), factor*m_skyBoxAngles.y(), factor*m_skyBoxAngles.z() );

}

void WorldB::process(const BeTimer& timer)
{
	if ( !pause )
	{
// 		m_snd_sys.process();

			updateSkyboxAngles();

		// recalc freeEnergy
			calcFreeEnergy();
		
		#ifdef HAVE_OPENCL
// 			nbody.process();
		#endif
	
		// kill half?
			killHalforDouble();

		// Remove food
			expireFood();

		// Autoinsert Food
			autoinsertFood();

		// remove all dead critters
			expireCritters();

		// Autosave Critters?
			autosaveCritters(timer);

		// Autoexchange Critters?
			autoexchangeCritters(timer);

		// Autoinsert Critters?
			autoinsertCritters();

// 		videocap->grab();
// 		for ( unsigned int j=0; j < videocap->image_cur.v.size(); j++ )
// 			std::cout << (int)videocap->image_cur.v[j] << " ";
// 		std::cout << std::endl;
// 		std::cout << std::endl;

// 		if ( *critter_raycastvision == 0 )
		{
			renderVision();
			grabVision();
		}

		// do a bullet step
// 			m_dynamicsWorld->stepSimulation(0.016667f, 0, 0.016667f);


			m_dynamicsWorld->stepSimulation(0.016667f, 3, 0.00833334f);
			
			
			
	// 		m_dynamicsWorld->stepSimulation(Timer::Instance()->bullet_ms / 1000.f);
	// cout << Timer::Instance()->bullet_ms << " : " << Timer::Instance()->bullet_ms / 1000.f << endl;

			
			
			
			
			
			
			
			
		// process all critters
		unsigned int lmax = critters.size();
		unsigned int i;
		float freeEnergyc = 0.0f;

		
// 		for( i=0; i < lmax; ++i)
// 		{
// 			CritterB* c = critters[i];
// 			
// 				checkCollisions( c );
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
// 			procreate(c);
// 			c->beingTouched = false;
// 			c->beingEaten   = false;
// 		}
		
		
		
		for( i=0; i < lmax; ++i)
			checkCollisions( critters[i] );
		
#ifdef HAVE_OPENMP
// 		std::cout << "found" << std::endl;
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
// 			omp_set_lock(&my_lock1);

void WorldB::childPositionOffset(btTransform* v)
{
// 	v->getOrigin().setY(insertHeight);
	v->getOrigin().setY(12.0f);
}

void WorldB::procreate( CritterB* c )
{
	if ( c->procreate && c->canProcreate )
	{
		bool brainmutant = false;
		bool bodymutant = false;
		
		int brainMutationrate = *brain_mutationrate;
// 		int brainMutationrate = c->genotype->brainzArch->m_mutationrate;
		int bodyMutationrate = *body_mutationrate;
// 		int bodyMutationrate = c->genotype->bodyArch->m_mutationrate;
		
		
		// mutationratemax - ((( numcritters-critterrange ) / critterrange) * mutationraterange)
		if ( *population_eliminate_portion_bodymutationratechange && *population_eliminate_portion_bodymutationratemax > *population_eliminate_portion_bodymutationratemin )
		{
			float critter_range = 0.5f * *population_eliminate_portion;
			const int mutation_range = *population_eliminate_portion_bodymutationratemax - *population_eliminate_portion_bodymutationratemin;
			brainMutationrate = *population_eliminate_portion_bodymutationratemax - ((((float)critters.size()-critter_range)/critter_range)*mutation_range);
			// control
			if ( brainMutationrate > *population_eliminate_portion_bodymutationratemax )
				brainMutationrate = *population_eliminate_portion_bodymutationratemax;
			if ( brainMutationrate < *population_eliminate_portion_bodymutationratemin )
				brainMutationrate = *population_eliminate_portion_bodymutationratemin;
// 			cout << "brain:" << brainMutationrate << endl;
		}
		
		if ( *population_eliminate_portion_brainmutationratechange && *population_eliminate_portion_brainmutationratemax > *population_eliminate_portion_brainmutationratemin )
		{
			float critter_range = 0.5f * *population_eliminate_portion;
			const int mutation_range = *population_eliminate_portion_brainmutationratemax - *population_eliminate_portion_brainmutationratemin;
			bodyMutationrate = *population_eliminate_portion_brainmutationratemax - ((((float)critters.size()-critter_range)/critter_range)*mutation_range);
			// control
			if ( bodyMutationrate > *population_eliminate_portion_brainmutationratemax )
				bodyMutationrate = *population_eliminate_portion_brainmutationratemax;
			if ( bodyMutationrate < *population_eliminate_portion_brainmutationratemin )
				bodyMutationrate = *population_eliminate_portion_brainmutationratemin;
// 			cout << "body:" << bodyMutationrate << endl;
		}

		if ( randgen->Instance()->get(1,1000) <= brainMutationrate )
			brainmutant = true;

		if ( randgen->Instance()->get(1,1000) <= bodyMutationrate )
			bodymutant = true;

		btTransform np = c->body.bodyparts[0]->myMotionState->m_graphicsWorldTrans;

		// position offset
		childPositionOffset(&np);
// 		np.setY(insertHeight);


// 		if ( np.getX() > *worldsizeX/2 )
// 			np.setX(np.getX()-1.0f);
// 		else
// 			np.setX(np.getX()+1.0f);

		CritterB *nc = new CritterB(*c, currentCritterID++, np, brainmutant, bodymutant);
		//CritterB *nc = new CritterB(*c, currentCritterID++, findPosition(), mutant);

		// display message of birth
			stringstream buf;
			buf << setw(4) << c->critterID << " : " << setw(4) << nc->critterID;
			buf << " ad: " << setw(4) << nc->genotype->adamdist;
			buf << " n: " << setw(4) << nc->brain.totalNeurons << " s: " << setw(5) << nc->brain.totalSynapses;

			if ( brainmutant || bodymutant )
			{
				buf << " ";
				if ( brainmutant ) buf << "brain";
				if ( brainmutant && bodymutant ) buf << "+";
				if ( bodymutant ) buf << "body";
				buf << " mutant";
			}

			m_panel_textverbosemessage->addBirth(buf);

// 			if (*headless)
// 				cout << buf.str()<< endl;

		// split energies in half
			nc->energyLevel = c->energyLevel/2.0f;
			c->energyLevel -= nc->energyLevel;

		// reset procreation energy count
			critters.push_back( nc );
			nc->calcFramePos(critters.size()-1);
			
		// rejuvenate
			c->totalFrames *= 0.5f;

			c->procreateTimeCount = 0;
	}
}

void WorldB::eat( CritterB* c )
{
	if ( c->eat )
	{
		if ( c->touchingFood )
		{
			Food* f = static_cast<Food*>(c->touchedEntity);
			float eaten = 0.05f * *critter_maxenergy;
			if ( c->energyLevel + eaten > *critter_maxenergy )
				eaten -= (c->energyLevel + eaten) - *critter_maxenergy;
			if ( f->energyLevel - eaten < 0 )
				eaten = f->energyLevel;

			c->energyLevel += eaten;
			f->energyLevel -= eaten;
		}
		else if ( c->touchingCritter && *critter_enableomnivores )
		{
			CritterB* ct = static_cast<CritterB*>(c->touchedEntity);
			float eaten = 0.05f * *critter_maxenergy;
			if ( c->energyLevel + eaten > *critter_maxenergy )
				eaten -= (c->energyLevel + eaten) - *critter_maxenergy;
			if ( ct->energyLevel - eaten < 0 )
				eaten = ct->energyLevel;

			c->energyLevel += eaten;
			ct->energyLevel -= eaten;
			ct->eaten = true;  // This is unused. :P
			ct->beingEaten = true;
		}
	}
}

void WorldB::killHalforDouble()
{
	if ( *population_double != 0 && critters.size() <= (unsigned int)*population_double )
	{
		unsigned int number_of_critters = critters.size();
		for ( unsigned int c = 0; c < number_of_critters; c++ )
			duplicateCritter(c, false, false);
	}
	else if ( critters.size() >= (unsigned int)*population_eliminate_portion )
	{
		killHalfOfCritters();
		
		// reduce energy
// 		if ( *population_eliminate_portion_decrenergypct > 0 )
// 		{
// 			float dec_amount = ((*food_maxenergy * *energy) - *food_maxenergy) / *food_maxenergy;
// 			if ( dec_amount >= 0.0f )
// 			{
// 				int dec = (dec_amount / 100) * *population_eliminate_portion_decrenergypct;
// 				settings->setCVar("energy", *energy - dec );
// 				//*energy -= dec;
// 				freeEnergy -= dec * *food_maxenergy;
// 			}
// 		}

		if ( *population_eliminate_portion_decrenergy > 0 )
		{
			settings->setCVar("energy", *energy - *population_eliminate_portion_decrenergy );
			//*energy -= dec;
			m_freeEnergy -= *population_eliminate_portion_decrenergy * *food_maxenergy;
		}

		// INC FLOOR SIZES
			bool do_makeFloor(false);
			// increase worldsizes
			if ( *population_eliminate_portion_incrworldsizeX > 0 )
			{
				settings->increaseCVar("worldsizeX", *population_eliminate_portion_incrworldsizeX);
				do_makeFloor = true;
			}
			if ( *population_eliminate_portion_incrworldsizeY > 0 )
			{
				settings->increaseCVar("worldsizeY", *population_eliminate_portion_incrworldsizeY);
				do_makeFloor = true;
			}
			if ( *population_eliminate_portion_incrworldsizeZ > 0 )
			{
				settings->increaseCVar("worldsizeZ", *population_eliminate_portion_incrworldsizeZ);
				do_makeFloor = true;
			}
			if ( do_makeFloor )
				makeFloor();
		
		// decrease critter_maxlifetime
		if ( *population_eliminate_portion_decrmaxlifetimepct > 0 )
		{
			int dec = (*critter_maxlifetime / 100) * *population_eliminate_portion_decrmaxlifetimepct;
			settings->setCVar("critter_maxlifetime", *critter_maxlifetime-dec );
		}
	}
}

void WorldB::autoinsertCritters()
{
	
	// insert critter if < minimum
	if ( *mincritters > 0 )
	{
		unsigned int interval( *critter_maxlifetime / *mincritters );
		if ( interval > 5000 )
			interval = 5000;
		
		if ( ++critterIntervalCounter >= interval / 3 )
		{
			unsigned int number_to_insert = 1;

			if ( interval == 0 )
				number_to_insert += *mincritters / *critter_maxlifetime;
			
			for ( unsigned int i(0); i < number_to_insert; ++i )
			{
				if ( critters.size() < (unsigned int)*mincritters )
					insertCritter();
			}
			critterIntervalCounter = 0;
		}
	}

// 	// insert critter if < minimum
// 	if ( critters.size() < (unsigned int)*mincritters )
// 		insertCritter();

	// insert critter if insertcritterevery is reached
	if ( *insertcritterevery > 0 )
	{
		if ( insertCritterCounter >= (unsigned int)*insertcritterevery )
		{
			insertCritter();
			insertCritterCounter = 0;
		}
		else
		{
			insertCritterCounter++;
		}
	}
}

void WorldB::autosaveCritters(const BeTimer& timer)
{
	if ( *critter_autosaveinterval > 0 )
	{
		autosaveCounter += timer.getSeconds();
		if ( autosaveCounter > *critter_autosaveinterval )
		{
			autosaveCounter = 0.0f;
			saveAllCritters();
		}
	}
}

void WorldB::autoexchangeCritters(const BeTimer& timer)
{
	if ( *critter_autoexchangeinterval > 0 )
	{
		autoexchangeCounter += timer.getSeconds();
		if ( autoexchangeCounter > *critter_autoexchangeinterval )
		{
			autoexchangeCounter = 0.0f;

			// determine exchange directory
			
			// save or load? :)
			const unsigned int mode = randgen->Instance()->get( 0, 10001 );
			
			// always load if critters == 0
			if ( critters.size() == 0 || mode < 6001 )
			{
				vector<string> files;
				dirH.listContentsFull(dirlayout->exchangedir, files);

// 				std::cout << std::endl << "exchange load: " << std::endl;
				
				if ( files.size() > 0 )
				{
					unsigned int t_sum( 0 );
					unsigned int t_attempts( 0 );
					const unsigned int load_max( 1+ (files.size() / 10) );

					for ( int i = 0; i < (int)files.size() && t_sum < load_max && t_attempts++ < 2*load_max; ++i )
					{
						const unsigned int loadf(randgen->Instance()->get( 0, files.size()-1 ));
						auto& f(files[loadf]);
						
						if ( ( parseH.endMatches( ".cr", f ) || parseH.endMatches(".cr.bz2", f) ) && f.find(m_starttime_s) == std::string::npos )
						{
							BeFile befileCritter;
							if ( m_fileSystem.load( befileCritter, f ) )
							{
								std::string content( befileCritter.getContent().str() );
								if ( !content.empty() )
								{
									btTransform t;
									t.setIdentity();
									t.setOrigin(findPosition());
									
									CritterB *c = new CritterB(content, m_dynamicsWorld, t, retina);

									if ( !c->loadError)
									{
										m_fileSystem.rm( f );

										critters.push_back( c );

										c->critterID = currentCritterID++;
										c->calcFramePos(critters.size()-1);

										// start energy
										m_freeEnergy -= c->energyLevel;
										
										++t_sum;
									}
									else
										delete c;
								}
							}
							else
							{
								const string buf2("WARNING: autoexchange: loading critter failed");
// 								if ( !*headless )
									m_logBuffer->add(buf2);
// 								else
// 									cout << buf2<< endl;
							}
							
						}
// 						else
// 							std::cout << "NOT OK" << std::endl;
						
						files.erase( files.begin()+loadf );
						i = -1;

					}
					if ( t_sum > 0 )
					{
						stringstream buf;
		// 				buf << "Loaded critters from " << exchangedir;
						buf << "autoexchange: loaded critters (" << t_sum << ")";
// 						if ( !*headless )
// 						{
							m_logBuffer->add(buf.str());
// 						}
// 						else
// 							cout << buf.str()<< endl;
					}
					else
						autoExchangeSaveCritter();
				}
			}
			else
			{
				autoExchangeSaveCritter();
			}
		}
	}
}

void WorldB::autoExchangeSaveCritter()
{
	if ( critters.size() > 0 )
	{
		// pick one to save
		const unsigned int savec(randgen->Instance()->get( 0, critters.size()-1 ));

		stringstream buf;
		buf << dirlayout->exchangedir << "/" << m_starttime_s << "." << time(0) << ".cr";
		const string& filename(buf.str());

		// save critter
		if ( m_fileSystem.save_bz2(filename, critters[savec]->genotype->saveGenotype()) )
		{
			const string buf2("autoexchange: saved critter");
// 			if ( !*headless )
				m_logBuffer->add(buf2);
// 			else
// 				cout << buf2<< endl;

			removeCritter(savec);
		}
		else
		{
			const string buf2("WARNING: autoexchange: saving critter failed");
// 			if ( !*headless )
				m_logBuffer->add(buf2);
// 			else
// 				cout << buf2<< endl;
		}
	}
}

void WorldB::autoinsertFood()
{
// 	200     @     100    = 0.5			>   1/0.5 = 2
// 	200     @     32000  = 160			>   1/160
	
	if ( *food_maxenergy > 0 && *food_maxlifetime > 0 )
	{
		const int queue(m_freeEnergy / *food_maxenergy);
		
		if ( *energy > 0 && queue > 0 )
		{
			unsigned int exp_interval_div;
			const unsigned int interval( *food_maxlifetime / *energy );
			if ( currentCritterID > 100 )
			{
// 				std::cout << "a " << currentCritterID << std::endl;
				const unsigned int exp_interval_div_t = (float)interval / (((float)queue+21) / 20);
				const unsigned int t_max(12);
				exp_interval_div = min(exp_interval_div_t,t_max);
			}
			else
			{
// 				std::cout << "b " << currentCritterID << std::endl;
				exp_interval_div = interval;
			}
			
// 			const unsigned int exp_interval_div_t = (float)interval / (((float)queue+51) / 50);
// 			const unsigned int t_max(12);
// 			const auto exp_interval_div = min(exp_interval_div_t,t_max);
			
			
			if ( ++foodIntervalCounter >= exp_interval_div || queue <= 10 )
			{
				if ( m_freeEnergy >= *food_maxenergy )
				{
			// 		if ( exp_interval_div > 10 )
			// 			exp_interval_div = 10;

		// 			if ( ++foodIntervalCounter >= exp_interval_div || queue <= 2 )
					{
						// std::cout  << exp_interval_div << " queue: " << m_freeEnergy / *food_maxenergy << std::endl;

						unsigned int number_to_insert = 1;

						if ( interval == 0 )
							number_to_insert += *energy / *food_maxlifetime;

						for ( unsigned int i(0); i < number_to_insert; ++i )
						{
							if ( m_freeEnergy >= *food_maxenergy )
							{
								insertRandomFood(1, *food_maxenergy);
								m_freeEnergy -= *food_maxenergy;

								foodIntervalCounter = 0; // FIXME MOVE TO OUTER LOOP?
							}
						}

					}
				}
			}
		}
	}
}

void WorldB::expireCritters()
{
	for( int i=0; i < critters.size(); ++i)
	{
		const CritterB* c(critters[i]);
	
		// see if energy level isn't below 0 -> die, or die of old age
		if ( c->energyLevel <= 0.0f )
		{
			stringstream buf;
			if ( c->eaten )
				buf << setw(4) << c->critterID << " got eaten";
			else
				buf << setw(4) << c->critterID << " starved";
			m_panel_textverbosemessage->addDeath(buf);
			
// 			if (*headless)
// 				cout << buf.str()<< endl;

			removeCritter(i);
			i--;
		}
		else
		{
			// die of old age
			
			unsigned int actual( *critter_maxlifetime );
			if ( *setting_critter_mutate_maxlifetime > 0 )
				actual = c->genotype->bodyArch->m_maxlifetime;
			
			else if ( c->totalFrames > actual )
	// 		else if ( c->totalFrames > (unsigned int)*critter_maxlifetime )
			{
				stringstream buf;
				buf << setw(4) << c->critterID << " died of age";
				m_panel_textverbosemessage->addDeath(buf);

	// 			if (*headless)
	// 				cout << buf.str()<< endl;

				removeCritter(i);
				i--;
			}
			// die if y < 200
			else
			{
				btVector3 pos = c->body.bodyparts[0]->myMotionState->m_graphicsWorldTrans.getOrigin();
				
				if ( pos.getY() < -200.0f )
				{
					stringstream buf;
					buf << setw(4) << c->critterID << " fell in the pit";
					m_panel_textverbosemessage->addDeath(buf);

	// 				if (*headless)
	// 					cout << buf.str()<< endl;

					removeCritter(i);
					i--;
				}
			}
		}
	}
}

void WorldB::expireFood()
{
// 	for( unsigned int i=0; i < food.size(); ++i)
	for( int i=0; i < food_units.size(); ++i)
	{
		Food* f = food_units[i];
		// food was eaten
		if ( f->energyLevel <= 0 )
		{
			m_freeEnergy += f->energyLevel;
			if ( f->isPicked )
				mousepicker->detach();
			delete f;
			food_units.erase(food_units.begin()+i);
			i--;
		}
		// old food, this should remove stuff from corners
		else if ( ++f->totalFrames >= (unsigned int)*food_maxlifetime )
		{
			m_freeEnergy += f->energyLevel;
			if ( f->isPicked )
				mousepicker->detach();
			delete f;
			food_units.erase(food_units.begin()+i);
			i--;
		}
		// die if y < 200
		else
		{
			btVector3 pos = f->body.bodyparts[0]->myMotionState->m_graphicsWorldTrans.getOrigin();

			if ( pos.getY() < -200.0f )
			{
				m_freeEnergy += f->energyLevel;
				if ( f->isPicked )
					mousepicker->detach();
				delete f;
				food_units.erase(food_units.begin()+i);
				i--;
			}
		}
	}
}

void WorldB::getGeneralStats()
{
// 	settings->info_totalNeurons = 0;
// 	settings->info_totalSynapses = 0;
// 	settings->info_totalAdamDistance = 0;
// 	settings->info_totalBodyparts = 0;
// 	settings->info_totalWeight = 0;

// 	settings->info_critters = critters.size();
// 	settings->info_food = food.size();

// 	int info_totalNeurons = 0;
// 	int info_totalSynapses = 0;
// 	int info_totalAdamDistance = 0;
// 	int info_totalBodyparts = 0;
// 	int info_totalWeight = 0;
// 	CritterB* c;

// #pragma omp parallel for shared (info_totalNeurons, info_totalSynapses, info_totalAdamDistance, info_totalBodyparts, info_totalWeight, crittersize) private(i, c)
// 	for( unsigned int i=0; i < critters.size(); ++i )
// 	{
// 		c = critters[i];
// 		info_totalNeurons		+= c->brain.totalNeurons;
// 		info_totalSynapses		+= c->brain.totalSynapses;
// 		info_totalAdamDistance		+= c->genotype->adamdist;
// 		info_totalBodyparts		+= c->body.bodyparts.size();
// 		info_totalWeight		+= c->body.totalWeight;
// 	}
// 
// 	settings->info_totalNeurons		+= info_totalNeurons;
// 	settings->info_totalSynapses		+= info_totalSynapses;
// 	settings->info_totalAdamDistance	+= info_totalAdamDistance;
// 	settings->info_totalBodyparts		+= info_totalBodyparts;
// 	settings->info_totalWeight		+= info_totalWeight;

	statsBuffer->add( critters, food_units );
}

void WorldB::checkCollisions( CritterB* c )
{
	// set inputs to false and recheck
		c->touchingFood = false;
		c->touchingCritter = false;

	if ( c->body.mouths.size() > 0 )
	{
		btBroadphasePairArray& pairArray = c->body.mouths[0]->ghostObject->getOverlappingPairCache()->getOverlappingPairArray();
		int numPairs = pairArray.size();

		for ( int i=0; i < numPairs; ++i )
		{
			manifoldArray.clear();

			const btBroadphasePair& pair = pairArray[i];

			//unless we manually perform collision detection on this pair, the contacts are in the dynamics world paircache:
			btBroadphasePair* collisionPair = m_dynamicsWorld->getPairCache()->findPair(pair.m_pProxy0,pair.m_pProxy1);
			if (!collisionPair)
				continue;

			if (collisionPair->m_algorithm)
				collisionPair->m_algorithm->getAllContactManifolds(manifoldArray);

			for ( int j = 0; j < manifoldArray.size(); j++ )
			{
				btPersistentManifold* manifold = manifoldArray[j];
				
				const btCollisionObject* object1 = static_cast<const btCollisionObject*>(manifold->getBody0());
				const btCollisionObject* object2 = static_cast<const btCollisionObject*>(manifold->getBody1());

				if ( object1->getUserPointer() == c && object2->getUserPointer() == c )
					continue;

				for ( int p = 0; p < manifold->getNumContacts(); p++ )
				{
					const btManifoldPoint &pt = manifold->getContactPoint(p);
					if ( pt.getDistance() < 0.0f )
					{
						void* Collidingobject;
						if ( object1->getUserPointer() != c && object1->getUserPointer() != 0 )
							Collidingobject = object1->getUserPointer();
						else if ( object2->getUserPointer() != c && object2->getUserPointer() != 0 )
							Collidingobject = object2->getUserPointer();
						else 
							continue;

						// Touching Food
						Entity* e = static_cast<Entity*>(Collidingobject);
						if ( e->type == FOOD )
						{
// 							cout << "touches food" << endl;
							c->touchingFood = true;
							c->touchedEntity = e;
							return;
						}
						// Touching Critter
						else if ( e->type == CRITTER )
						{
// 							cout << "touches critter" << endl;
							c->touchingCritter = true;
							c->touchedEntity = e;
              CritterB* ct = static_cast<CritterB*>(e);
              ct->beingTouched = true;
							return;
						}

/*						// Touching Food
						Food* f = static_cast<Food*>(Collidingobject);
						if ( f > -1 )
						{
							if ( f->type == 1 )
							{
								stop = true;
								c->touchingFood = true;
								c->touchedFoodID = f;
							}
							else
							{
								// Touching Critter
								CritterB* b = static_cast<CritterB*>(Collidingobject);
								if ( b->type == 0 )
								{
									stop = true;
									c->touchingCritter = true;
									c->touchedCritterID = b;
								}
							}
						}*/
					}
				}
			}
		}
	}
}

void WorldB::insertRandomFood(int amount, float energy)
{
	for ( int i=0; i < amount; ++i )
	{
		Food *f = new Food();
		f->energyLevel = energy;
		//f->resize();
		f->createBody( m_dynamicsWorld, findPosition() );

// 		f->m_model = m_model_food;
		
// 		entities.push_back( f );
		food_units.push_back( f );
	}
}

void WorldB::insertCritter()
{
	btTransform t;
	t.setIdentity();
	t.setOrigin(findPosition());
	
	CritterB *c = new CritterB(m_dynamicsWorld, currentCritterID++, t, retina);
	critters.push_back( c );
	c->calcFramePos(critters.size()-1);

	// start energy
	m_freeEnergy -= c->energyLevel;
}

btVector3 WorldB::findPosition()
{
	btVector3 resizing_offset( m_initial_worldsizeX - *worldsizeX, insertHeight, m_initial_worldsizeZ - *worldsizeZ );
	return btVector3( (float)randgen->Instance()->get( 0, 100**worldsizeX ) / 100, insertHeight, (float)randgen->Instance()->get( 0, 100**worldsizeZ ) / 100 ) + ( resizing_offset * 0.5f );
}

void WorldB::removeCritter(unsigned int cid)
{
	m_freeEnergy += critters[cid]->energyLevel;

	if ( critters[cid]->isPicked )
		mousepicker->detach();
	
	if ( m_follow_critterP == critters[cid] )
	{
		m_follow_critterP = 0;
// 		m_follow_critterP = *critters.rbegin();
		m_camera.setSceneNode(&m_sceneNodeCamera);
	}
	
	critterselection->unregisterCritterID(critters[cid]->critterID);
	critterselection->deselectCritter(critters[cid]->critterID);

	delete critters[cid];
	critters.erase(critters.begin()+cid);

	// update higher retina frame positions
	int c;
// 	omp_set_num_threads( *threads );
// 	#pragma omp parallel for private(c)
	for ( c = cid; c < (int)critters.size(); c++ )
		critters[c]->calcFramePos(c);
}

void WorldB::killHalfOfCritters()
{
// 	for ( unsigned int c = 0; c < critters.size(); c++ )
// 		removeCritter(c);

	// kill oldest
	if ( critters.size() > 0 )
	{
// 		// initialize sort indices
// 			vector<int> indices ( critters.size(), 0 );
// 			for ( unsigned int i = 0; i < critters.size(); ++i )
// 				indices[i] = i;
// 
// 		// sort results
// 			for ( int i = critters.size(); i>0; i--  )
// 				for ( int j = 0; j < i-1; j++  )
// 					if ( critters[indices[j]]->totalFrames < critters[indices[j+1]]->totalFrames )
// 					{
// 						unsigned keepI	= indices[j];
// 						indices[j]	= indices[j+1];
// 						indices[j+1]	= keepI;
// 					}
		
		
		
		
		
		
		// calculate % to eliminate, minimal 1
		const unsigned int eliminate_amount = 1 + ((0.01f * critters.size()) * *population_eliminate_portion_percent);
		for ( unsigned int c = 0; c < eliminate_amount; ++c )
		{
			// find oldest
			unsigned int highest_age( 0 );
			unsigned int highest_age_key( 0 );
			for ( unsigned int i = 0; i < critters.size(); ++i )
			{
				if ( critters[i]->totalFrames > highest_age )
				{
					highest_age = critters[i]->totalFrames;
					highest_age_key = i;
				}
			}
			
			removeCritter(highest_age_key);
		}
	}
}

void WorldB::renderVision()
{
	// render critter vision
// 	if ( !*critter_raycastvision )
	{
		auto t_v_inv( m_graphics->m_v_inv );
		if ( *m_glsl == 0 )
			t_v_inv = -1;
		
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb); 
		glClearColor (0.0f, 0.0f, 0.0f, 0.0f);
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// 		const auto& end(critters.end());
// 		for( auto it(critters.begin()); it != end; ++it)
		for( unsigned int i=0; i < critters.size(); ++i)
		{
			CritterB* c( critters[i] );
			if ( c->body.mouths.size() > 0 )
			{
				c->place( t_v_inv );
// 				drawWithinCritterSight( i );
				drawWithinCritterSight(c);
				
// // 				critters[i]->releaseFBO();
			}
		}
	}
}

void WorldB::grabVision()
{
	// Read pixels into retina
	if ( /*!*critter_raycastvision &&*/ !critters.empty() )
	{
		// determine height
		unsigned int picheight = *critter_retinasize;
		int rows = critters.size();
		while ( rows > retinasperrow )
		{
			picheight += *critter_retinasize;
			rows -= retinasperrow;
		}
// 		glReadBuffer(GL_BACK);
		glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
		glReadPixels(0, 0, picwidth, picheight, GL_RGBA, GL_UNSIGNED_BYTE, retina);
	}

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); 
// 	glBindTexture(GL_TEXTURE_2D, color_tex);
}

// void WorldB::drawWithoutFaces()
// {
// 	cout << "drawWithoutFaces" << endl;
// 	drawfloor();
// 
// 	const auto& end(critters.end());
// 	for( auto it(critters.begin()); it != end; ++it)
// 		(*it)->draw(false);
// // 	for( unsigned int i=0; i < critters.size(); ++i)
// // 		critters[i]->draw(false);
// 
// // 	for( unsigned int i=0; i < food.size(); ++i)
// // 		food[i]->draw();
// 	const auto& e_end(entities.end());
// 	for( auto it(entities.begin()); it != e_end; ++it)
// 		(*it)->draw();
// // 	for( unsigned int i=0; i < entities.size(); ++i)
// // 		entities[i]->draw();
// }

const btTransform& WorldB::getCameraTransform() const
{
	return m_follow_critter_transform_prev;
}

void WorldB::slerp( const btTransform& source, btTransform& target, const double factor )
{
	if ( factor > 0.0f )
	{
		
		float magnitude = btSqrt(source.getRotation().length2() * target.getRotation().length2()); 		
		float product = fabs(target.getRotation().dot(source.getRotation()) / magnitude);
// 		std::cout << "p: " << product << std::endl;

// 		double smooth_factor2 = factor;
		
		
// 		if ( smooth_factor2 >= 1.0 )
// 			smooth_factor2 = 0.9999999;
		
// 		if ( magnitude > 0.0f )
		
// 		double fps( 1.0/factor );
		
// 		double change_factor( 1.0 - (0.0005 * factor) );
		double change_factor( factor / (product * product) );
		
		
// 		change_factor = min(max(change_factor, 0.0), 1.0);
		change_factor = min(max(change_factor, 0.0), 0.9999999);

// 		std::cout << "factor: " << change_factor << std::endl;
		
		if ( change_factor > 0.0 )
		{
// 			if ( change_factor == 1.0 )
// 			{
// 				target = source;
// 			}
// 			else
			{
				if ( source.getOrigin() != target.getOrigin() )
				{
// 					btVector3 n_pos( (source.getOrigin() * (change_factor)) + (target.getOrigin() * (1.0-change_factor) ) );
					btVector3 n_pos( target.getOrigin().lerp( source.getOrigin(), change_factor) );
// 					btVector3 n_pos( source.getOrigin().lerp( target.getOrigin(), change_factor) );
					
					
					if ( n_pos == target.getOrigin() )
					{
// 						std::cout << "pos no change" << std::endl;
						target.setOrigin(source.getOrigin());
					}
					else if ( n_pos == n_pos )
					{
// 						std::cout << "pos yes " << change_factor << std::endl;
// 						std::cout << source.getOrigin().distance( target.getOrigin() ) << std::endl;
						target.setOrigin(n_pos);
					}
					else
					{
						target.setOrigin(source.getOrigin());
// 						std::cout << "pos no : fail" << std::endl;
					}
				}
// 				else
// 					std::cout << "pos no" << std::endl;
				
				if ( product != 1.0f )
				{
// 					btQuaternion n_rot( (source.getRotation() * (change_factor)) + (target.getRotation() * (1.0-change_factor) ) );
					
					
					btQuaternion n_rot( target.getRotation().slerp( source.getRotation(), change_factor ) );
// 					btQuaternion n_rot( source.getRotation().normalized().slerp( target.getRotation().normalized(), change_factor ) );
					
					if ( n_rot == target.getRotation() )
					{
// 						std::cout << "rot no change" << std::endl;
						target.setRotation(source.getRotation());
					}
					else if ( n_rot == n_rot )
					{
// 						std::cout << "rot yes success" << std::endl;
// 						std::cout << source.getRotation().angle( target.getRotation() ) << std::endl;
						target.setRotation(n_rot);
					}
					else
					{
// 						std::cout << "rot no : fail" << std::endl;
						target.setRotation(source.getRotation());
					}
				}
// 				else
// 					std::cout << "rot no" << std::endl;
			}
		}
	}
}

void WorldB::updateCameraTransform( const float timeDelta )
{
// 	0 : focus on critter
// 	1 : no focus on critter
// 	2 : critter viewpoint
// 	3 : snap loose from critter
// 	4 : general view
	
	btTransform t2;
	float factor(0.0001);
	
	if ( *m_camera_mode == 3 )
	{
		m_camera.setSceneNode(&m_sceneNodeCamera);

// 		std::cout << "slerping" << std::endl;
// 		slerp( m_camera.getSceneNode()->getTransform(), m_follow_critter_transform_prev, smooth_factor );
		t2 = m_camera.getSceneNode()->getTransform();
	}
	else
	{
		if ( m_follow_critterP == 0 )
		{
			m_camera.setSceneNode(&m_sceneNodeCamera);
// 			slerp( m_camera.getSceneNode()->getTransform(), m_follow_critter_transform_prev, smooth_factor );
			t2 = m_camera.getSceneNode()->getTransform();
		}
		else
		{
	// 		// CRITTERPOSITION * CAMERA + LOOKAT
			if ( *m_camera_mode == 0 )
			{
				m_camera.setSceneNode(&m_sceneNodeCamera_follow1);
				
				// CRITTER POSITION
					const btVector3 critter_pos_vector( m_follow_critterP->body.mouths[0]->ghostObject->getWorldTransform().getOrigin() );
					btTransform critter_pos_transform;
					critter_pos_transform.setIdentity();
					critter_pos_transform.setOrigin( critter_pos_vector );
				
					m_follow_critter_view3.setOrigin( m_follow_critter_view3.getOrigin() + m_camera.getSceneNode()->getTransform().getOrigin() );
					m_follow_critter_view3.setRotation( m_follow_critter_view3.getRotation() * m_camera.getSceneNode()->getTransform().getRotation() );
					m_camera.getSceneNode()->setTransform(m_null_transform);
					
				// RELATIVE CAMERA POSITION
					btTransform camera_pos_transform;
					camera_pos_transform.setIdentity();
					camera_pos_transform.setOrigin( btVector3(0, m_follow_critter_view3.getOrigin().z(), 0) );

					// LOOK DOWN
						camera_pos_transform.getBasis().setEulerZYX( -SIMD_HALF_PI, 0.0f, 0.0f );

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
						turn_y.getBasis().setEulerZYX( m_follow_critter_view3_rotz, m_follow_critter_view3_roty, 0.0f );

// 					btTransform t2( critter_pos_transform * /*turn_x * */turn_y * /*turn_z * */m_startpos_t_follow_critter * m_follow_critter_view3kink * camera_pos_transform/* * look_down_transform)*/ );
					t2 = critter_pos_transform * turn_y * m_startpos_t_follow_critter * m_follow_critter_view3kink * camera_pos_transform;
					
// 					slerp( t2, m_follow_critter_transform_prev, smooth_factor );
			}
// 			if ( *m_camera_mode == 0 )
// 			{
// 				// CRITTER POSITION
// 					const btVector3 critter_pos_vector( m_follow_critterP->body.mouths[0]->ghostObject->getWorldTransform().getOrigin() );
// 					btTransform critter_pos_transform;
// 					critter_pos_transform.setIdentity();
// 					critter_pos_transform.setOrigin( critter_pos_vector );
// 				
// 					m_follow_critter_view3.setOrigin( m_follow_critter_view3.getOrigin() + m_camera.getSceneNode()->getTransform().getOrigin() );
// 					m_follow_critter_view3.setRotation( m_follow_critter_view3.getRotation() * m_camera.getSceneNode()->getTransform().getRotation() );
// 					m_camera.getSceneNode()->setTransform(m_null_transform);
// 					
// 				// RELATIVE CAMERA POSITION
// 					btTransform camera_pos_transform;
// 					camera_pos_transform.setIdentity();
// 					camera_pos_transform.setOrigin( btVector3(0, m_follow_critter_view3.getOrigin().z(), 0) );
// 
// 					// LOOK DOWN
// 						camera_pos_transform.getBasis().setEulerZYX( -SIMD_HALF_PI, 0.0f, 0.0f );
// 
// 				// TURN Y & Z ANGLES
// 					
// 					// FETCH ANGLES FROM VIEW
// 						btScalar rotation_x(0), rotation_y(0), rotation_z(0);
// 						m_follow_critter_view3.getBasis().getEulerYPR(rotation_x, rotation_y, rotation_z);
// 
// 					// UPDATE ROTATIONS & CAP
// 						m_follow_critter_view3_roty += rotation_y;
// 						m_follow_critter_view3_rotz += rotation_z;
// 						
// 						if ( m_follow_critter_view3_roty > 6.30f )
// 							m_follow_critter_view3_roty -= SIMD_2_PI;
// 						else if ( m_follow_critter_view3_roty < -6.30f )
// 							m_follow_critter_view3_roty += SIMD_2_PI;
// 						if ( m_follow_critter_view3_rotz > 6.30f )
// 							m_follow_critter_view3_rotz -= SIMD_2_PI;
// 						else if ( m_follow_critter_view3_rotz < -6.30f )
// 							m_follow_critter_view3_rotz += SIMD_2_PI;
// 						
// 					// RESET VIEW ROTATION
// 						m_follow_critter_view3.setRotation( m_null_transform.getRotation() );
// 
// 					// TRANSFORM
// 						btTransform turn_y;
// 						turn_y.setIdentity();
// 						turn_y.getBasis().setEulerZYX( m_follow_critter_view3_rotz, m_follow_critter_view3_roty, 0.0f );
// 
// 					btTransform t2( critter_pos_transform * /*turn_x * */turn_y * /*turn_z * */m_startpos_t_follow_critter * m_follow_critter_view3kink * camera_pos_transform/* * look_down_transform)*/ );
// 					
// 					slerp( t2, m_follow_critter_transform_prev, timeDelta );
// 			}
			
	// 			// LOOK AT CRITTER
	// 				btTransform cam_to_critter_t;
	// 				{
	// 					cam_to_critter_t.setIdentity();
	// // 					const btVector3 up(m_camera.getSceneNode()->getTransform().getBasis()[0][0], m_camera.getSceneNode()->getTransform().getBasis()[1][0], m_camera.getSceneNode()->getTransform().getBasis()[2][0]);
	// // 					const btVector3 up(0, 0, 1.0);
	// 					const btVector3 up(btVector3(rel_cam_tranform.getBasis()[0][0], rel_cam_tranform.getBasis()[1][0], rel_cam_tranform.getBasis()[2][0]).normalized());
	// // 					const btVector3 direction = rel_cam_tranform.getOrigin().normalized();
	// 					const btVector3 direction = -(t2.getOrigin() - camera_pos_transform.getOrigin()).normalized();
	// 					const btScalar angle = acos(up.dot(direction));
	// 					const btVector3 axis = up.cross(direction);
	// 					btQuaternion cam_to_critter_t_rot(axis, angle);
	// // 					cam_to_critter_t.setOrigin(camera_position);
	// 					cam_to_critter_t.setRotation(cam_to_critter_t_rot);
	// 				}
	// 				t2 *= cam_to_critter_t;
			

			// CRITTERPOSITION * CAMERA
				else if ( *m_camera_mode == 1 )
				{
					m_camera.setSceneNode(&m_sceneNodeCamera_follow2);
					// CRITTER POSITION
						btTransform t;
						t.setIdentity();
						t.setOrigin(m_follow_critterP->body.mouths[0]->ghostObject->getWorldTransform().getOrigin());

		// 			// CAMERA TRANSFORM OFFSET
// 						btTransform t2(m_startpos_t_follow_critter * t * m_camera.getSceneNode()->getTransform()/* * m_follow_critter_view1*/);
// 						slerp( t2, m_follow_critter_transform_prev, smooth_factor );
						t2 = m_startpos_t_follow_critter * t * m_camera.getSceneNode()->getTransform();
				}

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
						
						factor = 0.0005;
				}
		}
// 						slerp( t2, m_follow_critter_transform_prev, smooth_factor * 5 );
	}
	
// 	float magnitude = btSqrt(t2.getRotation().length2() * m_follow_critter_transform_prev.getRotation().length2()); 		
// 	float product = m_follow_critter_transform_prev.getRotation().dot(t2.getRotation()) / magnitude;
// 	std::cout << "p: " << product << std::endl;

// 	double smooth_factor( (double)(1001 - *m_camera_smoothfactor) * timeDelta * factor );
	const float smooth_factor( factor * timeDelta * ( (1001 - *m_camera_smoothfactor) ) );
	
// 	smooth_factor /= fabs(product*product);
// 	
// 	std::cout << "sm: " << smooth_factor << std::endl;
// 	
// 	if ( smooth_factor >= 1.0 )
// 		smooth_factor = 0.99999;
	
	if ( *m_camera_smoothfactor == 0 )
	{
		m_follow_critter_transform_prev = t2;
	}
	else
	{
		
// 		if ( magnitude > 0.0f )
		{
			slerp( t2, m_follow_critter_transform_prev, smooth_factor );
		}
	}
	
	
}


void WorldB::drawWithGrid()
{
	m_skyTransform.setOrigin(getCameraTransform().getOrigin() + m_skybox_offset);

	drawfloor();
	drawCritters();

	drawFood();
}


void WorldB::drawfloor()
{
	sun_position = m_skyTransform * static_sun_pos;
	
	setLights();

// 	glColor4f( 0.34f, 0.25f, 0.11f, 0.0f );
	for( unsigned int i(0); i < wall_units.size(); ++i)
	{
		wall_units[i]->draw(false, true);
	}

	if (m_monolithModel)
	{
		m_graphicsSystem->color( BeColor(0.0f, 0.0f, 0.0f, 0.0f) );
		btTransform t;
		t.setIdentity();
// 		t.setOrigin(btVector3(130, 9, 70));
		t.setOrigin(btVector3(120, 9, 80));
		
		m_monolithModel->get()->setContext( 0 );
		m_monolithModel->get()->draw(0, t);
	}
	
	
// 	m_graphicsSystem->color( BeColor(1.0f, 1.0f, 1.0f, 0.0f) );
// 	glColor4f( 1.0f, 1.0f, 1.0f, 0.0f );
	if ( m_map && m_map.get() && m_map->isReady() && m_map.get()->get() )
	{
// 		glPushMatrix(); 
		
		m_map->get()->draw(m_map_scale);

// 		glPopMatrix();
	}

	
	if (m_skyBoxModel)
	{
		const auto prog(m_graphicsSystem->getActiveProgram());

		glDisable(GL_LIGHTING);

		if ( prog != 0 )
			m_graphicsSystem->useProgram(0);
		
		
			m_skyBoxModel->get()->setContext( 0 );
			m_skyBoxModel->get()->draw(0, m_skyTransform);
			
		if ( prog != 0 )
			m_graphicsSystem->useProgram(prog);

// 			// DRAW SUN
// 			glColor4f( 1.0f, 1.0f, 1.0f, 0.0f );
// 		
// 				GLfloat ambientLight[] = {1.0f, 0.0f, 1.0f, 1.0f};
// 				glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
// 
// 				GLfloat lightAmbient[] = {1.0f, 0.0f, 1.0f, 1.0f};
// 				glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
// 		
// 			m_model_food->setContext( 0 );
// 			m_model_food->draw( 0, sun_position );
// 
// 				GLfloat ambientLight2[] = {0.05f, 0.05f, 0.05f, 1.0f};
// 				glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight2);
// 
// 				GLfloat lightAmbient2[] = {0.025f, 0.025f, 0.025f, 1.0f};
// 				glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient2);

			
		glEnable(GL_LIGHTING);

	}
}

void WorldB::setLights()
{
	sun_position = m_skyTransform * static_sun_pos;
	const btVector3& pos(sun_position.getOrigin());
	GLfloat lightPos[] = { pos.x(), pos.y(), pos.z(), 1.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
}



void WorldB::drawFood(const btVector3* cposi)
{
	const auto& e_size(food_units.size());
	if ( e_size > 0 )
	{
	// 	glColor4f( m_color_default.r(), m_color_default.g(), m_color_default.b(), 1.0f );
		
// 		if ( cposi != 0 )
		{
			bool switchedColor(false);
			const auto& e_size(food_units.size());
			for( unsigned int i=0; i < e_size; ++i)
			{
				Food* e = food_units[i];
					const auto& tr(e->myMotionState->m_graphicsWorldTrans);
					if ( cposi == 0 || cposi->distance( tr.getOrigin() ) < m_sightrange )
					{
						if ( !switchedColor )
						{
							switchedColor=true;
							
							if ( m_model_food )
								m_model_food->setContext(0);

// 							m_graphicsSystem->color( e->color );
// 							glColor4f( e->color.r(), e->color.g(), e->color.b(), e->color.a() );
						}

						m_model_food->draw( 0, tr );
					}

// 					if ( cposi->distance( f->myMotionState->m_graphicsWorldTrans.getOrigin() ) < m_sightrange )
// 					{
// 						if ( !switchedColor )
// 						{
// 							switchedColor=true;
// 							e->draw(true, true);
// 						}
// 						else
// 							e->draw(false, false);
// 					}
			}
		}
	}
}



void WorldB::drawCritters(const btVector3* cposi, CritterB* within_sight_of)
{
// 	const auto& c_size(critters.size());
// 	if ( c_size > 0 )
	{
		bool switchedColor(false);
		
		for( auto jit(critters.begin()); jit != critters.end(); ++jit)
// 		for( unsigned int j=0; j < c_size; ++j)
		{
// 			const CritterB* f = critters[j];
			const CritterB* f = (*jit);

			switchedColor = false;

			for( auto it(f->body.bodyparts.begin()); it != f->body.bodyparts.end(); ++it)
// 			for( unsigned int b=0; b < f->body.bodyparts.size(); ++b)
			{
				const Bodypart* b = (*it);
				const auto& tr(b->myMotionState->m_graphicsWorldTrans);
				
				
				if ( cposi == 0 || cposi->distance( tr.getOrigin() ) < m_sightrange )
				{
					if ( !switchedColor )
					{
						switchedColor = true;

						if ( m_model_critter )
							m_model_critter->setContext(0);

						
						if ( cposi == 0 && *f->colormode == 1 )
						{
							m_graphicsSystem->color( f->genotype->speciescolor );
// 							glColor4f( f->genotype->speciescolor.r(), f->genotype->speciescolor.g(), f->genotype->speciescolor.b(), 0.5f );
						}
						
						else if ( cposi != 0 && f->genotype == within_sight_of->genotype )
						{
							f->genotype->bodyArch->color.setA(0.25f);
							m_graphicsSystem->color( f->genotype->bodyArch->color );
							
// 							glColor4f( f->genotype->bodyArch->color.r(), f->genotype->bodyArch->color.g(), f->genotype->bodyArch->color.b(), 0.25f );
						}

						else
						{
							f->genotype->bodyArch->color.setA(0.5f);
							m_graphicsSystem->color( f->genotype->bodyArch->color );

// 							glColor4f( f->genotype->bodyArch->color.r(), f->genotype->bodyArch->color.g(), f->genotype->bodyArch->color.b(), 0.5f );
						}
					}

					if ( m_model_critter )
					{
// 						m_model_critter->draw( 0, b->myMotionState->m_graphicsWorldTrans, static_cast<const btBoxShape*>(b->shape)->getHalfExtentsWithMargin()*2.2 );
						m_model_critter->draw( 0, tr, b->m_dimensions_scaled );
					}
					else
					{
						tr.getOpenGLMatrix(drawposition);
						glPushMatrix(); 
							Displaylists::Instance()->glMultiMatrix(drawposition);
							const btVector3& halfExtent = static_cast<const btBoxShape*>(b->shape)->getHalfExtentsWithMargin();
							glScalef(halfExtent[0], halfExtent[1], halfExtent[2]);
							Displaylists::Instance()->call();
						glPopMatrix();
					}
				}
			}
		}

		// MOUTHS
		switchedColor = false;
		for( auto jit(critters.begin()); jit != critters.end(); ++jit)
// 		for( unsigned int j=0; j < c_size; ++j)
		{
// 			const CritterB* f = critters[j];
			const CritterB* f = (*jit);

// 			if ( c->critterID != f->critterID )
			{
				for( auto m_it(f->body.mouths.begin()); m_it != f->body.mouths.end(); ++m_it)
// 				for( unsigned int b=0; b < f->body.mouths.size(); ++b)
				{
					const auto* m = (*m_it);
					const auto& tr(m->ghostObject->getWorldTransform());

					if ( cposi == 0 || cposi->distance( tr.getOrigin() ) < m_sightrange )
					{
						if ( !switchedColor )
						{
// 							glColor4f( 1.0f, 0.0f, 0.0f, 0.0f );
// 							m_graphicsSystem->color( m_color_mouth );
							if ( m_model_critter )
								m_model_critter->setContext(0);

							m_graphicsSystem->color( BeColor(1.0f, 0.0f, 0.0f, 0.0f) );
							
							switchedColor = true;
						}

						if ( m_model_critter )
						{
							m_model_critter->draw( 0, tr, m->m_dimensions_scaled );
						}
						else
						{
							tr.getOpenGLMatrix(drawposition);
							glPushMatrix(); 
								Displaylists::Instance()->glMultiMatrix(drawposition);
								const btVector3& halfExtent = static_cast<const btBoxShape*>(m->shape)->getHalfExtentsWithMargin();
								glScalef(halfExtent[0], halfExtent[1], halfExtent[2]);
								Displaylists::Instance()->call();
							glPopMatrix();
						}
					}
				}
			}
		}
	}
}










void WorldB::drawShadow(btScalar* m,const btVector3& extrusion,const btCollisionShape* shape, Bodypart* bp,const btVector3& worldBoundsMin,const btVector3& worldBoundsMax)
{
	glPushMatrix(); 
	glMultMatrixf(m);
	if(shape->getShapeType() == UNIFORM_SCALING_SHAPE_PROXYTYPE)
	{
		const btUniformScalingShape* scalingShape = static_cast<const btUniformScalingShape*>(shape);
		const btConvexShape* convexShape = scalingShape->getChildShape();
		float	scalingFactor = (float)scalingShape->getUniformScalingFactor();
		btScalar tmpScaling[4][4]={ {scalingFactor,0,0,0}, {0,scalingFactor,0,0}, {0,0,scalingFactor,0}, {0,0,0,1} };
		drawShadow((btScalar*)tmpScaling,extrusion,convexShape,bp,worldBoundsMin,worldBoundsMax);
		glPopMatrix();
		return;
	}
	else if(shape->getShapeType()==COMPOUND_SHAPE_PROXYTYPE)
	{
		const btCompoundShape* compoundShape = static_cast<const btCompoundShape*>(shape);
		for (int i=compoundShape->getNumChildShapes()-1;i>=0;i--)
		{
			btTransform childTrans = compoundShape->getChildTransform(i);
			const btCollisionShape* colShape = compoundShape->getChildShape(i);
			btScalar childMat[16];
			childTrans.getOpenGLMatrix(childMat);
			drawShadow(childMat,extrusion*childTrans.getBasis(),colShape,bp,worldBoundsMin,worldBoundsMax);
		}
	}
// 	else
// 	{
// 	//	bool useWireframeFallback = true;
// 		if (shape->isConvex())
// 		{
// 			ShapeCache* sc = bp->cache((btConvexShape*)shape);
// 			btShapeHull* hull =&sc->m_shapehull;
// 			glBegin(GL_QUADS);
// 			for(int i=0;i<sc->m_edges.size();++i)
// 			{			
// 				const btScalar		d=btDot(sc->m_edges[i].n[0],extrusion);
// 				if((d*btDot(sc->m_edges[i].n[1],extrusion))<0)
// 				{
// 					const int		q=	d<0?1:0;
// 					const btVector3&	a=	hull->getVertexPointer()[sc->m_edges[i].v[q]];
// 					const btVector3&	b=	hull->getVertexPointer()[sc->m_edges[i].v[1-q]];
// 					glVertex3f(a[0],a[1],a[2]);
// 					glVertex3f(b[0],b[1],b[2]);
// 					glVertex3f(b[0]+extrusion[0],b[1]+extrusion[1],b[2]+extrusion[2]);
// 					glVertex3f(a[0]+extrusion[0],a[1]+extrusion[1],a[2]+extrusion[2]);
// 				}
// 			}
// 			glEnd();
// 		}
// 
// 	}
	glPopMatrix();

}

void WorldB::drawWithinCritterSight(CritterB *c)
{
	if ( c->body.mouths.size() > 0 )
	{
// 		glColor4f( m_color_default.r(), m_color_default.g(), m_color_default.b(), m_color_default.a() );	

		// draw everything in it's sight
		m_sightrange = 0.05f * *critter_sightrange; // FIXME MUTATION ONE (c->genotype->bodyArch->)

		const btTransform ctrans( c->body.mouths[0]->myMotionState->m_graphicsWorldTrans * m_render_offset2 );

		btVector3 direction (
			ctrans.getBasis()[0][1],
			ctrans.getBasis()[1][1],
			ctrans.getBasis()[2][1]
		);

		const btVector3 cposi = ctrans.getOrigin() - ( direction * (m_sightrange*0.95f) );


		m_skyTransform.setOrigin(ctrans.getOrigin());
		
		drawfloor();

		drawCritters( &cposi, c );

		drawFood( &cposi );
// 		glColor4f( m_color_default.r(), m_color_default.g(), m_color_default.b(), m_color_default.a() );	
	}
}

// same as before, but with exponential optimisation FIXME EXCEPT IT AIN'T WORKING
// void WorldB::drawWithinCritterSight(unsigned int cid)
// {
// 	CritterB *c = critters[cid];
// 
// 	if ( c->body.mouths.size() > 0 )
// 	{
// 		btVector3 cposi = c->body.mouths[0]->myMotionState->m_graphicsWorldTrans.getOrigin();
// 
// 		// draw everything in it's sight
// 		const float sightrange = (float)*critter_sightrange/10;
// 
// 		drawfloor();
// 
// 		for( auto it(entities.begin()); it != entities.end(); ++it)
// 		{
// 			if ( (*it)->type == FOOD )
// 			{
// 				Food* f = static_cast<Food*>(*it);
// 				if ( cposi.distance( f->myMotionState->m_graphicsWorldTrans.getOrigin() ) < sightrange )
// 					f->draw();
// 			}
// 		}
// // 		cout << "prerecorded " << c->crittersWithinRange.size() <<  endl;
// 
// 		// first process prechecked crittersWithinRange vector
// 		for( unsigned int p=0; p < c->crittersWithinRange.size(); p++)
// 		{
// 			CritterB *f = critters[ c->crittersWithinRange[p] ];
// 
// 			glColor4f( f->genotype->bodyArch->color.r, f->genotype->bodyArch->color.g, f->genotype->bodyArch->color.b, f->genotype->bodyArch->color.a );
// 			for( auto it(f->body.bodyparts.begin()); it != f->body.bodyparts.end(); ++it)
// 			{
// 				(*it)->myMotionState->m_graphicsWorldTrans.getOpenGLMatrix(drawposition);
// 				Displaylists::Instance()->glMultiMatrix(drawposition);
// 				btVector3 halfExtent = static_cast<const btBoxShape*>((*it)->shape)->getHalfExtentsWithMargin();
// 				glScalef(halfExtent[0], halfExtent[1], halfExtent[2]);
// 				Displaylists::Instance()->call();
// 			}
// 
// 			glColor4f( 1.0f, 0.0f, 0.0f, 0.0f );
// 
// 			for( auto m_it(f->body.mouths.begin()); m_it != f->body.mouths.end(); ++m_it)
// 			{
// 				(*m_it)->ghostObject->getWorldTransform().getOpenGLMatrix(drawposition);
// 				Displaylists::Instance()->glMultiMatrix(drawposition);
// 				btVector3 halfExtent = static_cast<const btBoxShape*>((*m_it)->shape)->getHalfExtentsWithMargin();
// 				glScalef(halfExtent[0], halfExtent[1], halfExtent[2]);
// 				Displaylists::Instance()->call();
// 			}
// 		}
// 		// clear crittersWithinRange vector
// 		c->crittersWithinRange.clear();
// 
// // 		cout << "not recorded " << endl;
// 		// now start from current critter to last, record new checks for later critters
// 		for( unsigned int j=cid; j < critters.size(); j++)
// 		{
// // 			cout << " checking distance of " << j << endl;
// 			CritterB *f = critters[j];
// 			
// 			// if the first head is within range, draw critters bodyparts and if not same critter, draw head.
// 			if ( c->critterID == f->critterID || cposi.distance( f->body.mouths[0]->ghostObject->getWorldTransform().getOrigin() ) < sightrange )
// 			{
// 				//draw bodies if within range
// 				glColor4f( f->genotype->bodyArch->color.r, f->genotype->bodyArch->color.g, f->genotype->bodyArch->color.b, f->genotype->bodyArch->color.a );
// 				for( auto it(f->body.bodyparts.begin()); it != f->body.bodyparts.end(); ++it)
// 				{
// // 					if ( cposi.distance( (*it)->myMotionState->m_graphicsWorldTrans.getOrigin() ) < sightrange )
// // 					{
// 					(*it)->myMotionState->m_graphicsWorldTrans.getOpenGLMatrix(drawposition);
// 					Displaylists::Instance()->glMultiMatrix(drawposition);
// 					btVector3 halfExtent = static_cast<const btBoxShape*>((*it)->shape)->getHalfExtentsWithMargin();
// 					glScalef(halfExtent[0], halfExtent[1], halfExtent[2]);
// 					Displaylists::Instance()->call();
// // 					}
// 				}
// 				//draw heads if within range, and not itself
// // 				if ( c->critterID != f->critterID )
// // 				{
// 					// record for future distance checks
// // 					f->crittersWithinRange.push_back(cid);
// 					c->crittersWithinRange.push_back(j);
// 					
// 					glColor4f( 1.0f, 0.0f, 0.0f, 0.0f );
// 					for( auto m_it(f->body.mouths.begin()); m_it != f->body.mouths.end(); ++m_it)
// 					{
// 						(*m_it)->ghostObject->getWorldTransform().getOpenGLMatrix(drawposition);
// 						Displaylists::Instance()->glMultiMatrix(drawposition);
// 						btVector3 halfExtent = static_cast<const btBoxShape*>((*m_it)->shape)->getHalfExtentsWithMargin();
// 						glScalef(halfExtent[0], halfExtent[1], halfExtent[2]);
// 						Displaylists::Instance()->call();
// 					}
// 			}
// 		}
// 	}
// 	
// 	
// 	
// 	
// }

void WorldB::loadAllCritters()
{
	vector<string> files;
	dirH.listContentsFull(dirlayout->loaddir, files);

	for ( unsigned int i = 0; i < files.size(); ++i )
	{
		auto& f(files[i]);
		if ( parseH.endMatches(".cr", f) || parseH.endMatches(".cr.bz2", f) )
		{
			BeFile befileCritter;
			if ( m_fileSystem.load( befileCritter, f ) )
			{
				
				stringstream buf;
				buf << "loading " << f;
				m_logBuffer->add(buf.str());
	// 			cout << buf.str() << endl;

				std::string content( befileCritter.getContent().str() );
				
				btTransform t;
				t.setIdentity();
				t.setOrigin(findPosition());
				CritterB *c = new CritterB(content, m_dynamicsWorld, t, retina);

				if ( !c->loadError)
				{
					critters.push_back( c );

					c->critterID = currentCritterID++;
					c->calcFramePos(critters.size()-1);

					// start energy
					m_freeEnergy -= c->energyLevel;
				}
				else
					delete c;
			}
		}
		
	}

	stringstream buf;
	buf << "Loaded critters from " << dirlayout->loaddir;
	m_logBuffer->add(buf.str());
	cout << buf.str() << endl;
}

void WorldB::loadAllLastSavedCritters() // FIXME overlap with previous function
{
	cout << "loading" << endl;
	vector<string> files;
	
	string filen = dirlayout->progdir;
	filen.append("/lastsaved");
	
	if ( boost::filesystem::exists(filen) )
	{
		BeFile befile;
		if ( m_fileSystem.load( befile, filen ) )
		{
			std::string lastsaveddir;
// 			std::string content;
			std::string line;
			while ( befile.getLine(line) )
			{
				lastsaveddir.append(line);
				lastsaveddir.append("\n");
			}

	// 		fileH.open( filen, lastsaveddir ); 
			
			lastsaveddir = lastsaveddir.substr(0, lastsaveddir.length() - 1);
			
		// 	lastsaveddir.append("/");
			cout << lastsaveddir << endl;

			dirH.listContentsFull(lastsaveddir, files);

			cout << "files: " << files.size() << endl;
			for ( unsigned int i = 0; i < files.size(); ++i )
			{
				if ( parseH.endMatches( ".cr", files[i] ) || parseH.endMatches(".cr.bz2", files[i]) )
				{
					stringstream buf;
					buf << "loading " << files[i];
					m_logBuffer->add(buf.str());

					BeFile befileCritter;
					if ( m_fileSystem.load( befileCritter, files[i] ) )
					{
						std::string content( befileCritter.getContent().str() );
						
						btTransform t;
						t.setIdentity();
						t.setOrigin(findPosition());
						CritterB *c = new CritterB(content, m_dynamicsWorld, t, retina);

						if ( !c->loadError)
						{
							critters.push_back( c );

							c->critterID = currentCritterID++;
							c->calcFramePos(critters.size()-1);

							// start energy
							m_freeEnergy -= c->energyLevel;
						}
						else
							delete c;
					}
				}
			}
			stringstream buf;
			buf << "Loaded critters from " << lastsaveddir;
			m_logBuffer->add(buf.str());
			cout << buf.str() << endl;
		}
	}
}

void WorldB::saveAllCritters()
{
	// determine save directory
	const string subprofiledir(dirlayout->savedir + "/default");
	
	stringstream buf;
	buf << subprofiledir << "/" << time(0);
	string subsavedir = buf.str();

	// make dirs
	if ( !dirH.exists(subprofiledir) )	dirH.make(subprofiledir);
	if ( !dirH.exists(subsavedir) )		dirH.make(subsavedir);

	for ( unsigned int i = 0; i < critters.size(); ++i )
	{
		// determine filename
		stringstream filename;
		filename << subsavedir << "/" << "critter" << i << ".cr";
	
		// save critters
// 		m_fileSystem.save(filename.str(), critters[i]->genotype->saveGenotype());
		m_fileSystem.save_bz2(filename.str(), critters[i]->genotype->saveGenotype());
// 		fileH.save(filename.str(), critters[i]->genotype->saveGenotype());
	}

	// save lastsaved file
	stringstream lastsaved;
	lastsaved << dirlayout->progdir << "/" << "lastsaved";
	
	m_fileSystem.save( lastsaved.str(), subsavedir );
	cout << "saved " << lastsaved.str() << " with " << subsavedir << endl;

 	//cout << endl << "Saved critters to " << subsavedir << endl << endl;
	stringstream buf2;
	buf2 << "Saved critters to " << subsavedir;
	m_logBuffer->add(buf2.str());
	cout << buf2.str() << endl;

}

void WorldB::resetCamera()
{
	if ( &m_sceneNodeCamera == m_camera.getSceneNode() )
	{
		btTransform t;
		t.setIdentity();
		m_sceneNodeCamera.setTransform(t);
		
		int biggest = *worldsizeX;
		const auto Z(1.4f * *worldsizeZ);
		if ( Z > biggest )
			biggest = Z;


		m_camera.getSceneNode()->setOrigin( btVector3( 0.5f * m_initial_worldsizeX, 1.f*biggest, 0.5f*m_initial_worldsizeZ) );
	// 	camera.position.setRotation(btQuaternion(btVector3(1, 0, 0), btScalar(90)));
		m_camera.getSceneNode()->pitch( -SIMD_HALF_PI ); // 1.5707f  (float)*energy/10
	}
	else
	{
		reset_follow_location();
// 			m_camera.getSceneNode()->setTransform(m_null_transform);
	}
}

void WorldB::reset_follow_location()
{
	btTransform t;
	t.setIdentity();
	if ( *m_camera_mode == 1 )
	{
		m_sceneNodeCamera_follow2.setOrigin(btVector3(0,10,0));
		btTransform t;
		t.setIdentity();
		t.getBasis().setEulerZYX( -0.5f, 0.0f, 0.0f );
		m_sceneNodeCamera_follow2.setRotation(t.getRotation());
		
		
// 		t.setOrigin(btVector3(0,10,0));
// 		t.getBasis().setEulerZYX( -0.5f, 0.0f, 0.0f );
// 		m_sceneNodeCamera_follow2.setTransform(t);
	}
	if ( *m_camera_mode == 2 )
	{
		m_sceneNodeCamera_follow3.setTransform(t);
	}
	else if ( *m_camera_mode == 0 )
	{
		m_follow_critter_view3.setIdentity();
		m_follow_critter_view3_roty = 0.0f;
		m_follow_critter_view3_rotz = 0.0f;
		m_follow_critter_view3.getBasis().setEulerZYX( -0.5f, 0.0f, 0.0f );
	}
}

// selected critter actions
int WorldB::findSelectedCritterID()
{
	if ( critterselection->selectedCritter == 0 )
		return -1;
	
	for ( unsigned int i=0; i < critters.size(); ++i )
		if ( critters[i]->critterID == critterselection->selectedCritter->critterID )
			return i;
	return -1;
}

int WorldB::findCritterID(unsigned int cid)
{
	for ( unsigned int i=0; i < critters.size(); ++i )
		if ( critters[i]->critterID == cid )
			return i;
	return -1;
}

void WorldB::removeSelectedCritter()
{
	int f(findSelectedCritterID());
	if ( f > -1 )
		removeCritter(f);
}

void WorldB::removeAllSelectedCritters()
{
	while ( critterselection->clist.size() > 0 )
		removeCritter( findCritterID(critterselection->clist[0]->critterID) );
}

void WorldB::duplicateCritter(int cid, bool brainmutant, bool bodymutant)
{
	if ( cid > -1 )
	{
		btTransform np = critters[cid]->body.bodyparts[0]->myMotionState->m_graphicsWorldTrans;

		// position offset
		childPositionOffset(&np);

		CritterB *nc = new CritterB(*critters[cid], currentCritterID++, np, brainmutant, bodymutant);

		// duplicate energy levels
		nc->energyLevel = *critter_startenergy;
		m_freeEnergy -= nc->energyLevel;

		critters.push_back( nc );
		nc->calcFramePos(critters.size()-1);
	}
}

void WorldB::duplicateSelectedCritter()
{
	duplicateCritter( findSelectedCritterID(), false, false );
}
void WorldB::spawnBrainMutantSelectedCritter()
{
	duplicateCritter( findSelectedCritterID(), true, false );
}
void WorldB::spawnBodyMutantSelectedCritter()
{
	duplicateCritter( findSelectedCritterID(), false, true );
}
void WorldB::spawnBrainBodyMutantSelectedCritter()
{
	duplicateCritter( findSelectedCritterID(), true, true );
}

void WorldB::feedSelectedCritter()
{
	int f(findSelectedCritterID());
	if ( f > -1 )
	{
		CritterB* c = critters[f];
		if ( c->energyLevel < *critter_startenergy )
		{
			float max_currentDiff = (float)*critter_startenergy - c->energyLevel;
			c->energyLevel += max_currentDiff;
			m_freeEnergy -= max_currentDiff;
		}
	}
}

void WorldB::resetageSelectedCritter()
{
	int f(findSelectedCritterID());
	if ( f > -1 )
		critters[f]->totalFrames = 0;
}

void WorldB::followCritter()
{
	int f(findSelectedCritterID());
	if ( f > -1 )
	{
		if ( m_follow_critterP == critters[f] )
		{
			m_follow_critterP = 0;
			m_camera.setSceneNode(&m_sceneNodeCamera);
		}
		else
		{
			m_follow_critterP = critters[f];
// 				m_camera.setSceneNode(&m_sceneNodeCamera_follow);
		}
	}
}

void WorldB::followCritterRaycast()
{
	if ( 
		(*drawscene == 1 || *drawdebug > 0)
		&& mouseRayHit
		&& mouseRayHitEntity->type == CRITTER
	)
	{
		CritterB* c = static_cast<CritterB*>(mouseRayHitEntity);

// 		if ( m_follow_critterP == c )
// 		{
// 			m_follow_critterP = 0;
// 			m_camera.setSceneNode(&m_sceneNodeCamera);
// 		}
// 		else
		{
			m_follow_critterP = c;
// 					m_camera.setSceneNode(&m_sceneNodeCamera_follow);
		}
	}
	else
	{
		m_follow_critterP = 0;
		m_camera.setSceneNode(&m_sceneNodeCamera);
	}
}

void WorldB::duplicateAllSelectedCritters()
{
	for ( unsigned int i=0; i < critterselection->clist.size(); i ++ )
		duplicateCritter( findCritterID(critterselection->clist[i]->critterID), false, false );
}
void WorldB::spawnBrainMutantAllSelectedCritters()
{
	for ( unsigned int i=0; i < critterselection->clist.size(); i ++ )
		duplicateCritter( findCritterID(critterselection->clist[i]->critterID), true, false );
}
void WorldB::spawnBodyMutantAllSelectedCritters()
{
	for ( unsigned int i=0; i < critterselection->clist.size(); i ++ )
		duplicateCritter( findCritterID(critterselection->clist[i]->critterID), false, true );
}
void WorldB::spawnBrainBodyMutantAllSelectedCritters()
{
	for ( unsigned int i=0; i < critterselection->clist.size(); i ++ )
		duplicateCritter( findCritterID(critterselection->clist[i]->critterID), true, true );
}

void WorldB::activateFood() const
{
	for( auto it(food_units.begin()); it != food_units.end(); ++it )
	{	
// 		if ( (*it)->type == FOOD )
		{
			Food* f( (*it) );
			for( auto bit(f->body.bodyparts.begin()); bit != f->body.bodyparts.end(); ++bit )
				(*bit)->body->activate();
		}
	}
}

void WorldB::makeSkybox()
{
	if ( !m_skyBoxModel )
	{
		const auto& mapName = std::string("skies/")+settings->getCVarS("skybox").c_str();
// 		std::string mapName = "skys/sky_temp/skydome3.obj";
// 		std::string mapName = "skys/default/skydome3.obj";
// 		std::string mapName = "skys/round/skydome3.obj";
		
		const float scale( 0.01f * *m_skybox_scale );
		m_skyBoxModel=m_modelSystem->load(mapName, m_graphicsSystem, m_modelSystem, btVector3(scale,scale,scale), m_null_transform);
// 		m_skybox = boost::shared_ptr<ScClientMapResource>(new ScClientMapResource(m_fileSystem, mapName, m_graphicsSystem, m_modelSystem));
	}
}

void WorldB::makeFloor()
{
	
	const auto& mapName = std::string("maps/")+settings->getCVarS("map").c_str();
	if ( !m_map )
	{
		m_map = boost::shared_ptr<ScClientMapResource>(new ScClientMapResource(m_fileSystem, mapName, m_graphicsSystem, m_modelSystem));
	}

	if ( m_physics_map )
		m_physics_map.reset();
	
	if ( !m_physics_map )
	{
		m_map_scale.setX((float)*worldsizeX / m_initial_worldsizeX);
		m_map_scale.setY((float)*worldsizeY / m_initial_worldsizeY);
		m_map_scale.setZ((float)*worldsizeZ / m_initial_worldsizeZ);
// 		m_map_scale.setY((float)*worldsizeZ / m_initial_worldsizeZ);
// 		m_map_scale.setZ((float)*worldsizeY / m_initial_worldsizeY);
		
// 		const float scale(0.001f * *worldsizeX);
		
		m_physics_map = boost::shared_ptr<ServerMap>(new ServerMap( m_fileSystem, mapName, m_geometryModelSystem, m_dynamicsWorld ));
		for ( auto it(m_physics_map->body().m_bodyparts.begin()); it != m_physics_map->body().m_bodyparts.end(); ++it )
		{
			(*it)->getBody()->getCollisionShape()->setLocalScaling(m_map_scale);
// 			(*it)->getBody()->setFriction(0.8f);
// 			(*it)->getBody()->setRestitution(0.6f);
		}
	}

	makeSkybox();
	
// 	if ( !m_monolithModel )
// 	{
// 		std::string mapName = "cube/cube.obj";
// 		
// 		btTransform t;
// 		t.setIdentity();
// 		
// 		m_monolithModel=m_modelSystem->load(mapName, m_graphicsSystem, m_modelSystem, btVector3(2.0f,4.5f,0.5f), t);
// // 		m_skybox = boost::shared_ptr<ScClientMapResource>(new ScClientMapResource(m_fileSystem, mapName, m_graphicsSystem, m_modelSystem));
// 	}

	
	activateFood();
	
// 	makeDefaultFloor();
// 	activateFood();
}

void WorldB::makeDefaultFloor()
{
// 	m_color_wall.setR(1.2f*0.28f);
// 	m_color_wall.setG(1.2f*0.18f);
// 	m_color_wall.setB(1.2f*0.08f);
// 	m_color_wall.setA(0.0f);

// 	if ( settings->getCVar("glsl")==1 )
// 		m_map = boost::shared_ptr<ScClientMapResource>(new ScClientMapResource(m_fileSystem, "e-road", m_graphicsSystem, m_modelSystem));

// 		const char * mapName = m_settings->getCVarS("map").c_str();
	if ( !m_map )
	{
		const auto& mapName = settings->getCVarS("map").c_str();
		
		m_map = boost::shared_ptr<ScClientMapResource>(new ScClientMapResource(m_fileSystem, mapName, m_graphicsSystem, m_modelSystem));
		m_physics_map = boost::shared_ptr<ServerMap>(new ServerMap( m_fileSystem, mapName, m_geometryModelSystem, m_dynamicsWorld ));
	}
	

	
// 	for ( int i=0; i < (int)entities.size(); ++i )
// 	{
// 		if ( entities[i]->type == WALL )
// 		{
// 			delete entities[i];
// 			entities.erase(entities.begin()+i);
// 			i--;
// 		}
// 	}
// 
// 	// Wall Constants
// 		const float WallWidth = 2.0f;
// 		const float WallHalfWidth = WallWidth/2.0f;
// 		const float WallHeight = 5.0f;
// 		const float WallHalfHeight = WallHeight/2.0f;
// 
// 	// Ground Floor
// 		
// 		btTransform t;
// 		t.setIdentity();
// 		t.setOrigin(btVector3( *worldsizeX/2.0f, -WallHalfWidth, *worldsizeY/2.0f ));
// 		
// 		Wall* w = new Wall( *worldsizeX, WallWidth, *worldsizeY, t, m_dynamicsWorld );
// 		w->color.setR(m_color_wall.r());
// 		w->color.setG(m_color_wall.g());
// 		w->color.setB(m_color_wall.b());
// 		w->m_model = m_model_food;
// 		entities.push_back(w);
// 	
// 	if ( settings->getCVar("worldwalls") )
// 	{
// 		// Left Wall
// 		{
// 			btTransform t;
// 			t.setIdentity();
// 			t.setOrigin(btVector3( 0.0f-WallHalfWidth, WallHalfHeight-WallWidth, *worldsizeY/2.0f ));
// 
// 			w = new Wall( WallWidth, WallHeight, *worldsizeY, t, m_dynamicsWorld );
// 			w->color.setR(m_color_wall.r());
// 			w->color.setG(m_color_wall.g());
// 			w->color.setB(m_color_wall.b());
// 			w->m_model = m_model_food;
// 			entities.push_back(w);
// 		}
// 		// Right Wall
// 		{
// 			btTransform t;
// 			t.setIdentity();
// 			t.setOrigin(btVector3( *worldsizeX+WallHalfWidth, WallHalfHeight-WallWidth, *worldsizeY/2.0f ));
// 
// 			w = new Wall( WallWidth, WallHeight, *worldsizeY, t, m_dynamicsWorld );
// 			w->color.setR(m_color_wall.r());
// 			w->color.setG(m_color_wall.g());
// 			w->color.setB(m_color_wall.b());
// 			w->m_model = m_model_food;
// 			entities.push_back(w);
// 		}
// 		// Top Wall
// 		{
// 			btTransform t;
// 			t.setIdentity();
// 			t.setOrigin(btVector3( *worldsizeX/2.0f, WallHalfHeight-WallWidth, 0.0f-WallHalfWidth ));
// 
// 			w = new Wall( *worldsizeX+(WallWidth*2), WallHeight, WallWidth, t, m_dynamicsWorld );
// 			w->color.setR(m_color_wall.r());
// 			w->color.setG(m_color_wall.g());
// 			w->color.setB(m_color_wall.b());
// 			w->m_model = m_model_food;
// 			entities.push_back(w);
// 		}
// 		// Bottom Wall
// 		{
// 			btTransform t;
// 			t.setIdentity();
// 			t.setOrigin(btVector3( *worldsizeX/2.0f, WallHalfHeight-WallWidth, *worldsizeY+WallHalfWidth ));
// 
// 			w = new Wall( *worldsizeX+(WallWidth*2), WallHeight, WallWidth, t, m_dynamicsWorld );
// 			w->color.setR(m_color_wall.r());
// 			w->color.setG(m_color_wall.g());
// 			w->color.setB(m_color_wall.b());
// 			w->m_model = m_model_food;
// 			entities.push_back(w);
// 		}
// 	}
}

// unsigned int WorldB::findFoodIndex( const unsigned int number ) const
// {
//   
// 	unsigned int found = 0;
// 	
// 	for ( unsigned int currentIndex = 0; currentIndex < food_units.size(); currentIndex++ )
// 	{
// 		if ( food_units[currentIndex]->type == et )
// 		{
// 			found ++;
// 			if ( found == number )
// 				return currentIndex;
// 		}
// 	}
// 
// 	cout << "findEntityIndex: number too high" << endl;
// 	exit(1);
// 
// }

void WorldB::togglePause()
{
	pause = !pause;
}

// void WorldB::toggleSleeper()
// {
// 	sleeper.swap();
// }

void WorldB::toggleMouselook()
{
	mouselook = !mouselook;
	if ( mouselook )
	{
#ifdef _WIN32
		SDL_WarpMouse(0,0);
#endif
		SDL_WM_GrabInput(SDL_GRAB_ON);
		SDL_ShowCursor(SDL_DISABLE);
		// clear remaining poll events
		{ SDL_Event e; while (SDL_PollEvent(&e)) {} };
		
		// release picked objects
		mousepicker->detach();
	}
	else
	{
		SDL_ShowCursor(SDL_ENABLE);
		SDL_WM_GrabInput(SDL_GRAB_OFF);
	}
}

WorldB::~WorldB()
{
	for ( unsigned int i=0; i < food_units.size(); ++i )
		delete food_units[i];
	// for ( unsigned int i=0; i < entities.size(); ++i )
	// 	delete entities[i];
	for ( unsigned int i=0; i < critters.size(); ++i )
		delete critters[i];
	for ( unsigned int i=0; i < wall_units.size(); ++i )
		delete wall_units[i];
	
	// has to be done before dynamicsworld frees
	if ( m_physics_map )
		m_physics_map.reset();

	free(retina);
	
	//Delete resources
	glDeleteTextures(1, &color_tex);
	glDeleteRenderbuffersEXT(1, &depth_rb);
	//Bind 0, which means render to back buffer, as a result, fb is unbound
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glDeleteFramebuffersEXT(1, &fb);	
	
	
	delete raycast;
	delete mousepicker;
	
	delete m_dynamicsWorld;

	delete m_collisionConfiguration;
	delete m_dispatcher;
	delete m_ghostpaircallback;
	delete m_broadphase;
	delete m_solver;

// 	omp_destroy_lock(&my_lock1);
// 	omp_destroy_lock(&my_lock2);
}
