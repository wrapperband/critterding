#include "bodypart.h"

Bodypart::Bodypart(btDynamicsWorld* ownerWorld, void* owner, const btVector3& dimensions, float weight, btTransform& offset, btTransform& transform, unsigned int form)
 : m_dimensions(dimensions)
//  , m_dimensions_scaled(dimensions*2.26)
 , m_dimensions_scaled(dimensions*2.16)
{
	m_ownerWorld = ownerWorld;

	if ( form == 0 )
		shape = new btBoxShape( dimensions );
	else if ( form == 1 )
		shape = new btSphereShape( 1.0f );

	btVector3 localInertia(0,0,0);
	if (weight != 0.f) // weight of non zero = dynamic
		shape->calculateLocalInertia(weight,localInertia);

	myMotionState = new btDefaultMotionState(offset*transform);
	
	btRigidBody::btRigidBodyConstructionInfo rbInfo(weight,myMotionState,shape,localInertia);
	body = new btRigidBody(rbInfo);

	body->setUserPointer(owner);
// 	body->setDamping(0.05, 0.85);
	body->setDamping(0.05f, 0.185f);
// 	body->setDeactivationTime(0.001f);
// 	body->setSleepingThresholds(1.6f, 2.5f);
	body->setDeactivationTime(0.00001f);
	body->setSleepingThresholds(0.4f, 0.4f);
	
// 	body->setDeactivationTime(0.f);
// 	body->setSleepingThresholds(0.f, 0.f);
// 	body->setActivationState(DISABLE_DEACTIVATION);

    body->setFriction(0.8f);                                     // added by Ethical
    body->setRestitution(0.6f);
	
	m_ownerWorld->addRigidBody(body);
}

Bodypart::~Bodypart()
{
	// Remove all bodies and shapes
	m_ownerWorld->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(body->getBroadphaseHandle(),m_ownerWorld->getDispatcher());
	m_ownerWorld->removeRigidBody(body);
	delete myMotionState;
	delete shape;
// 	delete body->getMotionState();
	delete body;
}


