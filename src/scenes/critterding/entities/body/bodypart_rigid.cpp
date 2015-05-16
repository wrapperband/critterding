#include "bodypart_rigid.h"

BeRigidBody::BeRigidBody(btDynamicsWorld* ownerWorld) : m_ownerWorld(ownerWorld)
{
}

void BeRigidBody::generalSetup( const float weight, const btTransform& transform, const float linearDamping, const float angularDamping )
{
    btVector3 localInertia(0,0,0);
	if (weight != 0.f) // weight of non zero = dynamic
		m_shape->calculateLocalInertia( weight, localInertia );
	m_motionState = boost::shared_ptr<btDefaultMotionState>(new btDefaultMotionState( transform ));
	btRigidBody::btRigidBodyConstructionInfo rbInfo( weight, m_motionState.get(), m_shape.get(), localInertia );
	m_body = boost::shared_ptr<btRigidBody>(new btRigidBody(rbInfo));
	m_body->setDamping(linearDamping, angularDamping);
	m_body->setDeactivationTime(0.0001f);
	m_body->setSleepingThresholds(0.36f, 0.05f);
	m_ownerWorld->addRigidBody(m_body.get());
}

BeRigidBody::~BeRigidBody()
{
	m_ownerWorld->removeRigidBody(m_body.get());
}

void BeRigidBody::setMaterial( const BePhysicsMaterial& material )
{
    if(m_body)
    {
        m_body->setFriction(material.friction());
        m_body->setRestitution(material.restitution());
    }
}



