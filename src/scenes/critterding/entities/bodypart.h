#ifndef BODYPART_H
#define BODYPART_H

#include "btBulletDynamicsCommon.h"

class Bodypart
{
	public:
		Bodypart(btDynamicsWorld* ownerWorld, void* owner, const btVector3& dimensions, float weight, btTransform& offset, btTransform& transform, unsigned int form=0);
		~Bodypart();

		btRigidBody* 		body;
		btCollisionShape* 	shape;

		btDefaultMotionState*	myMotionState;

		btVector3	m_dimensions;
		btVector3	m_dimensions_scaled;
	private:
		btDynamicsWorld* 	m_ownerWorld;
};
#endif
