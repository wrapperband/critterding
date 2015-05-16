#ifndef BE_MOUSE_PICKER_H_INCLUDED
#define BE_MOUSE_PICKER_H_INCLUDED

#include "btBulletDynamicsCommon.h"
#include <iostream>

class BeMousePicker
{
	public:
		BeMousePicker(btDynamicsWorld* btWorld);
		~BeMousePicker();

// 		void		attach( btRigidBody* pickBody, const btVector3& attachPosition, const btVector3& rayFrom, const btVector3& rayTo );
		void		attach( btRigidBody* pickBody, const btVector3& attachPosition, const btVector3& rayFrom );
		void		detach();
		void		moveTo( const btVector3& origin, const btVector3& newdirection );
		void		moveFrom( const btVector3& origin );

		bool*						m_pickedBoolP;
		bool						m_active;
	private:
		btDynamicsWorld*			m_btDynWorld;

		btVector3					m_direction;
		btPoint2PointConstraint*	m_constraint;
		btRigidBody*				m_pickedBody;
		float						m_oldPickingDist;
		int							m_wants_reactivation;
};

#endif
