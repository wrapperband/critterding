#include "bodypart_rigid_box.h"

BodypartRigidBox::BodypartRigidBox(const btVector3& dimensions, btDynamicsWorld* ownerWorld, const btTransform& transform ,const float weight, const float linearDamping, const float angularDamping ) : 
BeRigidBody( ownerWorld )
{
	m_shape.reset(new btBoxShape( dimensions ));
	generalSetup( weight, transform, linearDamping, angularDamping );
}

BodypartRigidBox::~BodypartRigidBox()
{
}
