#ifndef BE_PHYSICS_SYSTEM_H_INCLUDED
#define BE_PHYSICS_SYSTEM_H_INCLUDED

// #include "BulletSoftBody/btSoftBody.h"

#include "btBulletDynamicsCommon.h"
#include "BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "src/common/be_physics_debug_renderer.h"
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include "kernel/be_log.h"

class BePhysicsSystem
{
	public:
		BePhysicsSystem();
		virtual ~BePhysicsSystem()
		{
// 			std::cerr << "exiting physics" << std::endl;
		};

		boost::shared_ptr<btDynamicsWorld> createWorld();
		void debugDrawWorld(boost::shared_ptr<btDynamicsWorld> dynamicsWorld)
		{
			dynamicsWorld->setDebugDrawer(&physicsdebugrenderer);
			dynamicsWorld->debugDrawWorld();
			dynamicsWorld->setDebugDrawer(0);
		}

	private:
		boost::scoped_ptr<btCollisionConfiguration> m_collisionConfiguration;
		boost::scoped_ptr<btCollisionDispatcher> m_dispatcher;
		boost::scoped_ptr<btBroadphaseInterface> m_broadphase;
		boost::scoped_ptr<btConstraintSolver> m_solver;
		boost::scoped_ptr<btGhostPairCallback> m_ghostPairCallback;

	public:
		BePhysicsDebugRenderer physicsdebugrenderer;
		BeLogDebug m_logDebug;
};

#endif
