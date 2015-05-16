#include "bodypart_rigid_sphere.h"

BodypartRigidSphere::BodypartRigidSphere(const float rad, btDynamicsWorld* ownerWorld, const btTransform& transform ,const float weight, const float linearDamping, const float angularDamping  )
  : BeRigidBody( ownerWorld )
{
	m_shape.reset(new btSphereShape( rad ));
	generalSetup( weight, transform, linearDamping, angularDamping );
}

BodypartRigidSphere::~BodypartRigidSphere()
{
}
