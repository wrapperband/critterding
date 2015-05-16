#ifndef BODYPARTRIGID_H
#define BODYPARTRIGID_H

#include "physics/be_physics_material.h"

#include "btBulletDynamicsCommon.h"
#include "src/graphics/be_color.h"
#include <boost/shared_ptr.hpp>

class BeShape
{
protected:
    BeShape(boost::shared_ptr<btCollisionShape> shape) : m_shape(shape)
    {
    }
    virtual ~BeShape()
    {
    }
private:
    boost::shared_ptr<btCollisionShape> m_shape;
};

class BeRigidBody
{
	public:
		BeRigidBody(btDynamicsWorld* ownerWorld);
		virtual ~BeRigidBody();
		const btTransform& transform() const { return m_motionState->m_graphicsWorldTrans; }
        void setMaterial(const BePhysicsMaterial& material);
		boost::shared_ptr<btRigidBody> getBody() { return m_body; }
	protected:
		void generalSetup( const float weight, const btTransform& transform, const float linearDamping, const float angularDamping);
        boost::shared_ptr<btCollisionShape> 	m_shape;
        boost::shared_ptr<btRigidBody> m_body;
        btDynamicsWorld* 	m_ownerWorld;
    private:
		boost::shared_ptr<btDefaultMotionState> m_motionState;
};
#endif
