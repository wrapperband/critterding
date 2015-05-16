#include "be_raycast.h"

Raycast::Raycast(btDynamicsWorld* btWorld)
{
	btDynWorld = btWorld;
}

castResult Raycast::cast(const btVector3& rayFrom, const btVector3& rayTo)
{
	result.m_hit = false;
	btCollisionWorld::ClosestRayResultCallback resultCallback(rayFrom,rayTo);
	btDynWorld->rayTest(rayFrom,rayTo,resultCallback);

	if (resultCallback.hasHit())
	{
// 		cerr << "1 true" << endl;
		result.m_hitBody = resultCallback.m_collisionObject;
		result.m_hit = true;
		result.m_hitPosition = resultCallback.m_hitPointWorld;

/*		if ( result.hitBody )
		{
			cerr << "2 true" << endl;
			result.hit = true;
			result.hitPosition = resultCallback.m_hitPointWorld;
		}*/
	}

	return result;
}

Raycast::~Raycast()
{
}


