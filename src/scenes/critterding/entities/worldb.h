#ifndef WORLDB_H
#define WORLDB_H

// #include "audio/be_audio_system2.h"
#include "physics/be_raycast.h"
#include "graphics/be_camera.h"
// #include <NxPhysics.h>

class ScGraphics;
class BeMousePicker;
class Statsbuffer;
class Dirlayout;
class Textverbosemessage;
class Critterselection;
class Food;
class Wall;
class CritterB;
class BeRand;
class btGhostPairCallback;
class Bodypart;
class Entity;
class BeGraphicsSystem;
class BeTimer;
class Settings;
class BeCameraPerspective;
class BeSceneNode;
class BeGraphicsModel;
class BeGraphicsModelSystem;
class ScClientMapResource;
class ServerMap;
class BeGraphicsModelResource;
class BeGeometrySystem;
class BeColor;

class WorldB
{
	class	btThreadSupportInterface*		m_threadSupportCollision;
	class	btThreadSupportInterface*		m_threadSupportSolver;

	public:

		WorldB( boost::shared_ptr<BeGraphicsSystem> system, BeFilesystem& fileSystem, boost::shared_ptr<Textverbosemessage> textverbosemessage, const bool lock_axis=true );

		virtual			~WorldB();
		virtual void		init();
// 		void		initShared();
		virtual void		process(const BeTimer& timer);
		virtual btVector3	findPosition();
		
		void setGraphics( boost::shared_ptr<ScGraphics> graphics )
		{
			m_graphics = graphics;
		};
		
		virtual			void makeFloor();
		void			activateFood() const;
		bool 			pause;
		bool			mouselook;

		float			insertHeight;
		void			getGeneralStats();
		void			killHalforDouble();
		void			expireFood();
		void			expireCritters();
		void			autoinsertFood();
		void			autosaveCritters(const BeTimer& timer);
		void			autoexchangeCritters(const BeTimer& timer);
		void			autoExchangeSaveCritter();
		void			autoinsertCritters();
		void			eat( CritterB* c );
		void			procreate( CritterB* c );
		void			makeDefaultFloor();
		
		Settings*		settings;
		Critterselection	*critterselection;
		Statsbuffer*		statsBuffer;
		BeRand*			randgen;
		BeMousePicker*		mousepicker;
		Dirlayout*		dirlayout;
// 		BeCamera		camera;
		BeCameraPerspective	m_camera;
		
// 		BeSceneNode&	m_currentSceneNode;
		BeSceneNode		m_sceneNodeCamera;
		BeSceneNode		m_sceneNodeCamera_follow1;
		BeSceneNode		m_sceneNodeCamera_follow2;
		BeSceneNode		m_sceneNodeCamera_follow3;
		
		
		virtual void resetCamera();
// 		unsigned int		findFoodIndex( const unsigned int number ) const;

		btCollisionDispatcher*	m_dispatcher;
		btBroadphaseInterface*	m_broadphase;
		btGhostPairCallback*	m_ghostpaircallback;
		btDefaultCollisionConfiguration* m_collisionConfiguration;
// 		boost::shared_ptr<btDynamicsWorld>	m_dynamicsWorldSPtr;
		btDynamicsWorld*	m_dynamicsWorld;
		btConstraintSolver*	m_solver;
		btAlignedObjectArray<btCollisionShape*>	m_collisionShapes;

		std::vector<CritterB*>	critters;
// 		vector<Entity*>		entities;
		std::vector<Food*>		food_units;
		std::vector<Wall*>		wall_units;

		unsigned long		currentCritterID;

		virtual void		drawWithGrid();
// 		virtual void		drawWithoutFaces();
 		void			drawShadow(btScalar* m,const btVector3& extrusion,const btCollisionShape* shape, Bodypart* bp,const btVector3& worldBoundsMin,const btVector3& worldBoundsMax);
		
		void			drawWithinCritterSight(CritterB *c);
// 		void			drawWithinCritterSight(unsigned int cid);
		
		virtual void	setLights();
		

		virtual void	drawfloor();
		virtual void	childPositionOffset(btTransform* v);
		void			drawFood(const btVector3* cposi=0);
		void			drawCritters(const btVector3* cposi=0, CritterB* within_sight_of=0);

		void			startfoodamount(unsigned int amount);

		void			insertRandomFood(int amount, float energy);

		virtual void		insertCritter();
		void			saveAllCritters();
		virtual void		loadAllCritters();
		virtual void		loadAllLastSavedCritters();

		void			removeSelectedCritter();
		void			removeAllSelectedCritters();
		void			duplicateCritter(int cid, bool brainmutant, bool bodymutant);

		void			duplicateSelectedCritter();
		void			spawnBrainMutantSelectedCritter();
		void			spawnBodyMutantSelectedCritter();
		void			spawnBrainBodyMutantSelectedCritter();
		void			feedSelectedCritter();
		void			resetageSelectedCritter();
		void			followCritter();
		void			followCritterRaycast();
		
		void			duplicateAllSelectedCritters();
		void			spawnBrainMutantAllSelectedCritters();
		void			spawnBodyMutantAllSelectedCritters();
		void			spawnBrainBodyMutantAllSelectedCritters();

		void			killHalfOfCritters();
		void			togglePause();
// 		void			toggleSleeper();
		void			toggleMouselook();

		void			pickBody();
		void			unpickBody();
		int				selectBody();
		void			deselectBody();
		void			castMouseRay();
		void			movePickedBodyTo();
		void			movePickedBodyFrom();
		float			autosaveCounter;
		float			autoexchangeCounter;

		// vision
		unsigned char		*retina;
		unsigned int		items;

		// FIXME > PROTECTED
		const int	retinasperrow;
// 		const int*	retinasperrow;
// 		const int*	critter_raycastvision;
		const int*	critter_retinasize;
		const int*	critter_maxenergy;
		const int*	worldsizeX;
		const int*	worldsizeY;
		const int*	worldsizeZ;
		const int*	skybox_rotationX;
		const int*	skybox_rotationY;
		const int*	skybox_rotationZ;
		const int*	m_skybox_scale;
// 		const int*	headless;
		const int*	threads;
		const int*	m_camera_mode;
		const int*	m_camera_smoothfactor;
		int			m_threads_last;
		
		void			checkCollisions( CritterB* c );

		void grabVision();

		void renderVision();

		void			calcMouseDirection();
// 		void			moveInMouseDirection(bool towards);

		void			calcFreeEnergy();
		float			m_freeEnergy;

		Dir			dirH;
		BeParser		parseH;

		bool			mouseRayHit;
		Entity*			mouseRayHitEntity;
		
		void setLogbuffer( boost::shared_ptr<Logbuffer> logBuffer ) { m_logBuffer = logBuffer; }

		// pointers to parents (evolution) mousepos
		int mousex;
		int mousey;
// 		int relx;
// 		int rely;
		
		// threading
// 		omp_lock_t my_lock1;

		boost::shared_ptr<BeGraphicsSystem> m_graphicsSystem;
		boost::shared_ptr<ScGraphics> m_graphics;
		
// 		GLint m_v_inv; // HACK
		const int* m_glsl; // HACK
		const int* m_hdr;
// 		boost::shared_ptr<BeGraphicsEffect> m_effect; // HACK

		const int m_initial_worldsizeX;
		const int m_initial_worldsizeY;
		const int m_initial_worldsizeZ;
		
		CritterB* m_follow_critterP;
// 		btTransform m_follow_critter_transform;
		btTransform m_follow_critter_transform_prev;
		const btTransform& getCameraTransform() const;
		virtual void updateCameraTransform( const float timeDelta );
		
	protected:
		void			makeSkybox();
		boost::shared_ptr<Textverbosemessage> m_panel_textverbosemessage;
		BeFilesystem& m_fileSystem;
		boost::shared_ptr<BeGraphicsModel> m_model_food;
		boost::shared_ptr<BeGraphicsModel> m_model_critter;
		void load(BeFilesystem& filesystem, const std::string& path);
		boost::shared_ptr<BeGraphicsModelSystem> m_modelSystem;
		std::string m_path;
		boost::shared_ptr<ScClientMapResource>	m_map;
		boost::shared_ptr<ServerMap>	m_physics_map;
		boost::shared_ptr<BeGraphicsModelResource>	m_skyBoxModel;
		boost::shared_ptr<BeGraphicsModelResource>	m_monolithModel;
		btTransform m_skyBoxTransform;
// 		btTransform m_transform;
		btVector3 m_skyBoxAngles;
		boost::shared_ptr<BeGeometrySystem> m_geometryModelSystem;
		void updateSkyboxAngles();

		btTransform		m_skyTransform;
		btVector3		m_skybox_offset;

// 		long long sunCounter;
		btTransform sun_position;
		btTransform static_sun_pos;
// 		btTransform sun_offset;
		
		btTransform m_follow_critter_view3;
		btTransform m_follow_critter_view3kink;
		void reset_follow_location();
		btTransform m_null_transform;
		btScalar m_follow_critter_view3_roty;
		btScalar m_follow_critter_view3_rotz;
		btTransform m_startpos_t_follow_critter;
	private:

// 		BeAudioSystem m_snd_sys;
		
		const int* drawscene;
		const int* drawdebug;
		
		Raycast*		raycast;

// 		boost::shared_ptr<BeWorkerPool> m_workerPool;
		
		btVector3 m_scale;
		btTransform m_transform;
		
		
		btTransform m_render_offset2;
		float m_sightrange;

		
#ifdef HAVE_OPENCL
// 		CLNBody nbody;
#endif

		static void CollisionNearOverride(btBroadphasePair& collisionPair, btCollisionDispatcher& dispatcher, const btDispatcherInfo& dispatchInfo);
		castResult		mouseRay;
		btVector3		mouseRayTo;
		
		
		unsigned int		insertCritterCounter;
		unsigned int		foodIntervalCounter;
		unsigned int		critterIntervalCounter;


		// methods
		inline void		removeCritter(unsigned int cid);
// 		inline void		createDirs();

		// Settings pointers.. performance
		const int*	critter_autosaveinterval;
		const int*	critter_autoexchangeinterval;
		const int*	critter_sightrange;
		const int*	critter_enableomnivores;

		const int*	population_limit_energy;
		const int*	population_limit_energy_percent;

		const int*	population_eliminate_portion;
		const int*	population_eliminate_portion_decrenergy;
		const int*	population_eliminate_portion_incrworldsizeX;
		const int*	population_eliminate_portion_incrworldsizeY;
		const int*	population_eliminate_portion_incrworldsizeZ;
		const int*	population_eliminate_portion_decrmaxlifetimepct;

		const int*	population_eliminate_portion_brainmutationratechange;
		const int*	population_eliminate_portion_brainmutationratemin;
		const int*	population_eliminate_portion_brainmutationratemax;
		const int*	population_eliminate_portion_bodymutationratechange;
		const int*	population_eliminate_portion_bodymutationratemin;
		const int*	population_eliminate_portion_bodymutationratemax;

		const int*	population_double;
		
		const int*	food_maxlifetime;
		const int*	energy;
		const int*	insertcritterevery;

		const int*	critter_startenergy;
		const int*	setting_critter_mutate_maxlifetime;
		
// 		bool			m_extinctionmode;
// 		unsigned int	m_extinctionmode_until;
		
// 		const unsigned int m_starttime;
		std::string	m_starttime_s;
// 		std::string	m_hostname;
		


		// vision opts
		unsigned int picwidth;
		btScalar drawposition[16];
		
		btManifoldArray   manifoldArray;

		int			findSelectedCritterID();
		int			findCritterID(unsigned int cid);

		GLuint color_tex;
		GLuint fb;
		GLuint depth_rb;
		boost::shared_ptr<Logbuffer> m_logBuffer;
		
// 		Videocap* videocap;

	protected:
		const int*	population_eliminate_portion_percent;
		const int*	critter_maxlifetime;
		const int*	food_maxenergy;
		const int*	mincritters;
		const int*	brain_mutationrate;
		const int*	body_mutationrate;


// 		const BeColor m_color_mouth;
// 		const BeColor m_color_wall;
// 		const BeColor m_color_floor;
// 		const BeColor m_color_default;
		
		btVector3 m_map_scale;
		void slerp( const btTransform& source, btTransform& target, const double timeDelta );
};

#endif
