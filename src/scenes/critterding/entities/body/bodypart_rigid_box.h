#ifndef BODYPARTRIGIDBOX_H
#define BODYPARTRIGIDBOX_H

#include "bodypart_rigid.h"

class BodypartRigidBox : public BeRigidBody
{
	public:
		BodypartRigidBox(const btVector3& dimensions, btDynamicsWorld* ownerWorld, const btTransform& transform , const float weight, const float linearDamping, const float angularDamping  );
		virtual ~BodypartRigidBox();
};
#endif
