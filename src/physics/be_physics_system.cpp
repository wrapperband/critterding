#include "be_physics_system.h"
#include "BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h"

BePhysicsSystem::BePhysicsSystem() :
m_collisionConfiguration(new btSoftBodyRigidBodyCollisionConfiguration),
m_dispatcher(new btCollisionDispatcher(m_collisionConfiguration.get())),
m_broadphase( new btAxisSweep3(btVector3(-100000,-100000,-100000),btVector3(100000,100000,100000))),
m_solver(new btSequentialImpulseConstraintSolver),
m_ghostPairCallback(new btGhostPairCallback),
m_logDebug("PHYSTICS")
{
	m_broadphase->getOverlappingPairCache()->setInternalGhostPairCallback( 0 );
	// debug drawer setup
	physicsdebugrenderer.setDebugMode( btIDebugDraw::DBG_DrawWireframe + btIDebugDraw::DBG_DrawConstraints + btIDebugDraw::DBG_DrawConstraintLimits );
}

boost::shared_ptr<btDynamicsWorld> BePhysicsSystem::createWorld()
{
	m_logDebug << "::PHYSTICS setting up bullet\n";
	boost::shared_ptr<btDynamicsWorld> dynamicsWorld(new btSoftRigidDynamicsWorld(m_dispatcher.get(),m_broadphase.get(),m_solver.get(),m_collisionConfiguration.get()));
	if(dynamicsWorld)
	{
		dynamicsWorld->getSolverInfo().m_numIterations = 10;
		dynamicsWorld->setDebugDrawer(0);
		btGImpactCollisionAlgorithm::registerAlgorithm(m_dispatcher.get());
		m_broadphase->getOverlappingPairCache()->setInternalGhostPairCallback( m_ghostPairCallback.get() );
	}
	return dynamicsWorld;
}