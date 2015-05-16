#include "bodypart_rigid_cylinder.h"

BodypartRigidCylinder::BodypartRigidCylinder(const btVector3& dimensions, btDynamicsWorld* ownerWorld, const btTransform& transform ,const float weight, const float linearDamping, const float angularDamping )
  : BeRigidBody( ownerWorld )
{
	m_shape.reset( new btCylinderShape( dimensions ) );
	generalSetup( weight, transform, linearDamping, angularDamping );
}

BodypartRigidCylinder::~BodypartRigidCylinder()
{
}
