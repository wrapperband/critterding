#ifndef BE_PHYSICS_SENSOR_H_INCLUDED
#define BE_PHYSICS_SENSOR_H_INCLUDED

#include "BulletCollision/CollisionDispatch/btGhostObject.h"

typedef boost::shared_ptr<std::vector<void*> > collisionObjects;

class BePhysicsSensor
{
public:
	BePhysicsSensor( boost::shared_ptr<btDynamicsWorld> ownerWorld, const btVector3& dimensions, btTransform& position ) :
	m_ownerWorld(ownerWorld)
	{
		ghostObject = new btPairCachingGhostObject();
		ghostObject->setCollisionShape( new btBoxShape( dimensions ) );
		
		ghostObject->setCollisionFlags( btCollisionObject::CF_KINEMATIC_OBJECT | btCollisionObject::CF_NO_CONTACT_RESPONSE );
		ghostObject->setWorldTransform( position );
		m_ownerWorld->addCollisionObject(ghostObject);
	}
	virtual ~BePhysicsSensor()
	{
		m_ownerWorld->removeCollisionObject(ghostObject);
		delete ghostObject->getCollisionShape();
		delete ghostObject;
		
	};

	collisionObjects check()
	{
		collisionObjects collisionobjects;
		collisionobjects.reset( new std::vector<void*> );

		btBroadphasePairArray& pairArray = ghostObject->getOverlappingPairCache()->getOverlappingPairArray();
		int numPairs = pairArray.size();
		for ( int i=0; i < numPairs; i++ )
		{
			btManifoldArray manifoldArray;

			const btBroadphasePair& pair = pairArray[i];

			//unless we manually perform collision detection on this pair, the contacts are in the dynamics world paircache:
			btBroadphasePair* collisionPair = m_ownerWorld->getPairCache()->findPair(pair.m_pProxy0,pair.m_pProxy1);
			if (!collisionPair)
				continue;

			if (collisionPair->m_algorithm)
				collisionPair->m_algorithm->getAllContactManifolds(manifoldArray);

			for ( int j = 0; j < manifoldArray.size(); j++ )
			{
				btPersistentManifold* manifold = manifoldArray[j];
				
				btCollisionObject* object1 = static_cast<btCollisionObject*>(manifold->getBody0());
				btCollisionObject* object2 = static_cast<btCollisionObject*>(manifold->getBody1());

// 				if ( object1->getUserPointer() == c && object2->getUserPointer() == c )
// 					continue;

				for ( int p = 0; p < manifold->getNumContacts(); p++ )
				{
					const btManifoldPoint &pt = manifold->getContactPoint(p);
					if ( pt.getDistance() < 0.0f )
					{
						void* Collidingobject;
						if ( object1->getUserPointer() != 0 )
							Collidingobject = object1->getUserPointer();
						else if ( object2->getUserPointer() != 0 )
							Collidingobject = object2->getUserPointer();
						else 
							continue;

						collisionobjects->push_back( Collidingobject );
					}
				}
			}
		}

		return collisionobjects;
	}

	btPairCachingGhostObject* ghostObject;
private:
	boost::shared_ptr<btDynamicsWorld> m_ownerWorld;

};

#endif
