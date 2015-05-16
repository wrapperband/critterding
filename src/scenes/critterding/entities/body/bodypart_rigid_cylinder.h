#ifndef BODYPARTRIGIDCYLINDER_H
#define BODYPARTRIGIDCYLINDER_H

#include "bodypart_rigid.h"

class BodypartRigidCylinder : public BeRigidBody
{
	public:
		BodypartRigidCylinder(const btVector3& dimensions, btDynamicsWorld* ownerWorld, const btTransform& transform ,const float weight, const float linearDamping, const float angularDamping);
		virtual ~BodypartRigidCylinder();
};

#endif
