#ifndef RAYCAST_H
#define RAYCAST_H

#include "btBulletDynamicsCommon.h"

struct castResult
{
	bool		m_hit;
	const btCollisionObject*	m_hitBody;
	btVector3	m_hitPosition;
};

class Raycast
{
	public:
		Raycast(btDynamicsWorld* btWorld);
		~Raycast();

		castResult cast(const btVector3& rayFrom, const btVector3& rayTo);
	private:
		btDynamicsWorld*	btDynWorld;
		castResult result;
};

#endif
