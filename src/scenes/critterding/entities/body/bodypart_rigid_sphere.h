#ifndef BODYPARTRIGIDSPHERE_H
#define BODYPARTRIGIDSPHERE_H

#include "bodypart_rigid.h"

class BodypartRigidSphere : public BeRigidBody
{
	public:
		BodypartRigidSphere(const float rad, btDynamicsWorld* ownerWorld, const btTransform& transform ,const float weight, const float linearDamping, const float angularDamping  );
		virtual ~BodypartRigidSphere();
};
#endif
