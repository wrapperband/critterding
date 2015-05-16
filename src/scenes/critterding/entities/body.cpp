#include "body.h"
#include <iostream>

Body::Body()
 : m_density(0.001f)
{
	totalWeight = 0.0f;
}

// void Body::addBodyPart_Capsule(void* owner, float width, float height, float weight, btTransform& offset, btTransform& transform)
// {
// 	Bodypart *b = new Bodypart(m_ownerWorld);
// 	bodyparts.push_back( b );
// 
// 	b->shape = new btCapsuleShape(width, height);
// 
// 	btVector3 localInertia(0,0,0);
// 	if (weight != 0.f) // weight of non zero = dynamic
// 		b->shape->calculateLocalInertia(weight,localInertia);
// 
// 	btDefaultMotionState* myMotionState = new btDefaultMotionState(offset*transform);
// 	
// 	btRigidBody::btRigidBodyConstructionInfo rbInfo(weight,myMotionState,b->shape,localInertia);
// 	b->body = new btRigidBody(rbInfo);
// 
// 	m_ownerWorld->addRigidBody(b->body);
// 
//  	b->body->setUserPointer(owner);
// 	b->body->setDamping(0.05, 0.85);
// 	b->body->setDeactivationTime(0.001);
// 	b->body->setSleepingThresholds(1.6, 2.5);
// }

void Body::addBodyPart_Sphere(void* owner, float x, float y, float z, float weight, btTransform& offset, btTransform& transform)
{
	Bodypart *b = new Bodypart(
		m_ownerWorld,
		owner,
		btVector3( x, y, z ),
		weight,
		offset,
		transform,
		1
	);
	bodyparts.push_back( b );
}


void Body::addBodyPart_Box(void* owner, float x, float y, float z, float weight, btTransform& offset, btTransform& transform)
{
	Bodypart *b = new Bodypart(
		m_ownerWorld,
		owner,
		btVector3( x, y, z ),
		weight,
		offset,
		transform
	);
	bodyparts.push_back( b );
}

void Body::addMouth(void* owner, float x, float y, float z, float weight, btTransform& offset, btTransform& transform)
{
	Mouth *m = new Mouth(
		m_ownerWorld,
		owner,
		btVector3( x, y, z ),
		weight,
		offset,
		transform
	);
	mouths.push_back( m );

	// connect mouth and body, for some reason this segfaults in the constructor
	m->connectMouthAndBody();
}

void Body::addConstraint(unsigned int bodypartID1, unsigned int bodypartID2, btTransform& localA, btTransform& localB, double limitA, double limitB)
{
	Constraint *c = new Constraint(
		m_ownerWorld,
		*bodyparts[bodypartID1]->body,
		*bodyparts[bodypartID2]->body,
		localA, localB,
		limitA, limitB
	);
	constraints.push_back( c );
}

void Body::addMouthConstraint(unsigned int bodypartID, unsigned int mouthID, btTransform& localA, btTransform& localB, double limitA, double limitB)
{
	Constraint *c = new Constraint(m_ownerWorld,
		*bodyparts[bodypartID]->body,
		*mouths[mouthID]->body,
		localA, localB,
		limitA, limitB
	);
	constraints.push_back( c );
}

void Body::wireArch(BodyArch* bodyArch, void* owner, btDynamicsWorld* ownerWorld, const btTransform& startOffset)
{
	m_ownerWorld = ownerWorld;
	
	btTransform offset(startOffset);
	for ( unsigned int i=0; i < bodyArch->archBodyparts.size(); i++ )
	{
		btTransform transform;
		transform.setIdentity();
		transform.setOrigin( btVector3( 0.0f, 0.0f, 0.0f ) );

		archBodypart *bp = &bodyArch->archBodyparts[i];

		// BOX
		if ( bp->type ==0 )
		{
			// calculate weight
			const float weight = m_density * (0.01*(bp->x*bp->y*bp->z)); // FIXME 0.001 is density of material
			totalWeight += weight;
// 			std::cout << "add bodypart:: weight: " << weight << " << totalWeight: " << totalWeight << std::endl;
			addBodyPart_Box( owner, 0.01f*bp->x, 0.01f*bp->y, 0.01f*bp->z, weight, offset, transform);

// 			Bodypart *b = new Bodypart(m_ownerWorld, owner, btVector3( bp->x/100, bp->y/100, bp->z/100 ), weight, offset, transform);
// 			bodyparts.push_back( b );
		}
	}

	// 5 MOUTH-HEAD
	for ( unsigned int i=0; i < bodyArch->archMouths.size(); i++ )
	{
		btTransform transform;
		transform.setIdentity();
		transform.setOrigin( btVector3( 0.0f, 0.0f, 0.0f ) );

		archMouth *bp = &bodyArch->archMouths[i];

		// calculate weight
		const float weight = m_density * (0.01*(bp->x*bp->y*bp->z)); // FIXME 0.001 is density of material
		totalWeight += weight;
// 		std::cout << "add mouth:: weight: " << weight << " << totalWeight: " << totalWeight << std::endl << std::endl;
		addMouth( owner, 0.01f*bp->x, 0.01f*bp->y, 0.01f*bp->z, weight, offset, transform);

// 		// calculate weight
// 		float weight = ((mo->x*mo->y*mo->z)/100) * 0.001f; // FIXME 0.001 is density of material
// 		totalWeight += weight;
// 		addMouth(owner, mo->x/100, mo->y/100, mo->z/100, weight, offset, transform);
	}

	for ( unsigned int i=0; i < bodyArch->archConstraints.size(); i++ )
	{
		archConstraint *co = &bodyArch->archConstraints[i];

		btTransform localA;
		localA.setIdentity();
		localA.getBasis().setEulerZYX( co->rot_x_1, co->rot_y_1, co->rot_z_1 );
		localA.setOrigin( btVector3(co->pos_x_1, co->pos_y_1, co->pos_z_1) );

		btTransform localB;
		localB.setIdentity();
		localB.getBasis().setEulerZYX( co->rot_x_2, co->rot_y_2, co->rot_z_2 );
		localB.setOrigin( btVector3(co->pos_x_2, co->pos_y_2, co->pos_z_2) );

		// find the ID's to connect
		int bpID_1 = bodyArch->findBodypart( co->id_1 );
		
		if ( co->isMouthConstraint )
		{
			int bpID_2 = bodyArch->findMouth( co->id_2 );
			if ( bpID_1 > -1 && bpID_2 > -1 )
				addMouthConstraint( (unsigned int)bpID_1, (unsigned int)bpID_2, localA, localB, co->limit_1, co->limit_2 );
		}
		else
		{
			int bpID_2 = bodyArch->findBodypart( co->id_2 );
			if ( bpID_1 > -1 && bpID_2 > -1 )
				addConstraint( (unsigned int)bpID_1, (unsigned int)bpID_2, localA, localB, co->limit_1, co->limit_2 );
		}
	}
}

Body::~Body()
{
		for ( unsigned int i=0; i < constraints.size(); i++ )
			delete constraints[i];

		for ( unsigned int i=0; i < bodyparts.size(); i++ )
			delete bodyparts[i];

		for ( unsigned int i=0; i < mouths.size(); i++ )
			delete mouths[i];
}
