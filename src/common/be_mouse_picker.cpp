#include "be_mouse_picker.h"

BeMousePicker::BeMousePicker(btDynamicsWorld* btWorld)
 : m_pickedBoolP(0)
 , m_active(false)
 , m_btDynWorld(btWorld)
 , m_direction(0.0f, 0.0f, 0.0f)
 , m_constraint(0)
 , m_pickedBody(0)
 , m_oldPickingDist(0.0f)
 , m_wants_reactivation(0)
{
}

// void BeMousePicker::attach( btRigidBody* pickBody, const btVector3& attachPosition, const btVector3& rayFrom, const btVector3& rayTo )
void BeMousePicker::attach( btRigidBody* pickBody, const btVector3& attachPosition, const btVector3& rayFrom )
{
// 	if ( pickBody )
	{
		m_active = true;

		m_pickedBody = pickBody;

		// record activation state
		m_wants_reactivation = m_pickedBody->getActivationState();

		m_pickedBody->setActivationState(DISABLE_DEACTIVATION);
		m_pickedBody->activate();

		btVector3 localPivot = m_pickedBody->getCenterOfMassTransform().inverse() * attachPosition;

		// create m_constraint and add it to bulletworld
		m_constraint = new btPoint2PointConstraint(*m_pickedBody,localPivot);
		m_btDynWorld->addConstraint(m_constraint);
		m_constraint->m_setting.m_impulseClamp = 30.f;
		m_constraint->m_setting.m_tau = 0.001f;

		m_oldPickingDist = (attachPosition - rayFrom).length();
	}
}

void BeMousePicker::detach()
{
	if ( m_active )
	{
		m_active = false;

		m_btDynWorld->removeConstraint(m_constraint);
		delete m_constraint;
// 		m_pickedBody->forceActivationState(m_active_TAG);
// 		m_pickedBody->setDeactivationTime( 0.001 );

		m_pickedBody->setActivationState(m_wants_reactivation);
		
		
		// set the object that was picked back to false
		*m_pickedBoolP = false;
	}
}

void BeMousePicker::moveTo( const btVector3& origin, const btVector3& newdirection )
{
	m_direction = newdirection - origin;
	m_direction.normalize();
	m_direction *= m_oldPickingDist;
	m_constraint->setPivotB(origin + m_direction);
}

void BeMousePicker::moveFrom( const btVector3& origin )
{
	m_constraint->setPivotB(origin + m_direction);
}

BeMousePicker::~BeMousePicker()
{
}
