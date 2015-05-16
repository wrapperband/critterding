#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include <iostream>
#include <vector>

class btDynamicsWorld;
class btRigidBody;
class btTransform;
class btHingeConstraint;

class Constraint
{
public:
	// USING FLOATS ON LIMITS SO THAT HEADER CAN BE MOVED TO CPP
	Constraint(btDynamicsWorld* ownerWorld, btRigidBody& bodyA, btRigidBody& bodyB, btTransform& localA, btTransform& localB, float limitA, float limitB);
	~Constraint();

	void			motorate();
	float			getAngle();
	
	std::vector<bool*>		Inputs;
// 	btTypedConstraint*	hinge;
	btHingeConstraint*	hinge;
	float			fullRange;
	float			diffFromZero;
private:
	btDynamicsWorld* 	m_ownerWorld;

	bool			input1;
	bool			input2;
	
// 	float			limitA;
// 	float			limitB;
};
#endif
