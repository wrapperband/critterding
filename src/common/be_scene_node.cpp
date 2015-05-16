#include "be_scene_node.h"

BeSceneNode::BeSceneNode( const bool lock_axis )
 : m_lock_x(false)
 , m_lock_y(false)
 , m_lock_z(false)
 , m_follow_critter_view0_rotx(0)
 , m_follow_critter_view0_roty(0)
 , m_follow_critter_view0_rotz(0)
 , m_locked(lock_axis)
{

	m_transform.setIdentity();
	m_transform_actual.setIdentity();
	m_null_transform.setIdentity();
}

	void BeSceneNode::setTransform(const btTransform& transform)
	{
		m_follow_critter_view0_rotx = 0;
		m_follow_critter_view0_roty = 0;
		m_follow_critter_view0_rotz = 0;
		m_transform_actual=transform;
	}
	const btTransform& BeSceneNode::getTransform() const
	{
		return m_transform_actual;
// 		return m_transform;
	}

// 	btTransform getTransform()
// 	{
// 		return m_transform;
// 	}

	void BeSceneNode::setOrigin(const btVector3& origin)
	{
		m_transform_actual.setIdentity();
		m_transform_actual.setOrigin(origin);
	}

	void BeSceneNode::setRotation(const btQuaternion& rotation)
	{
		m_transform.setRotation(rotation);
		m_follow_critter_view0_rotx = 0;
		m_follow_critter_view0_roty = 0;
		m_follow_critter_view0_rotz = 0;
		
		recalcActualTransform();
	}

	const btVector3& BeSceneNode::getOrigin() const
	{
		return m_transform_actual.getOrigin();
	}

	void BeSceneNode::translate(const btVector3& vector)
	{
		m_transform_actual.setOrigin(m_transform.getOrigin()+vector);
	}

	
	void BeSceneNode::translateLocal(const btVector3& vector)
	{
		btTransform transform;
		transform.setIdentity();
		transform.setOrigin( vector );
		m_transform_actual *= transform;
	}

	
	const btVector3& BeSceneNode::getRightVector() const
	{
		return m_transform_actual.getBasis().getRow(0);
	}

	const btVector3& BeSceneNode::getUpVector() const
	{
		return m_transform_actual.getBasis().getRow(1);
	}

	const btVector3& BeSceneNode::getForwardVector() const
	{
		return m_transform_actual.getBasis().getRow(2);
	}

	void BeSceneNode::yaw(const float radians)
	{
		m_transform *= btTransform(btQuaternion(radians,0,0));
		recalcActualTransform();
	}

	void BeSceneNode::pitch(const float radians)
	{
		m_transform *= btTransform(btQuaternion(0,radians,0));
		recalcActualTransform();
	}

	void BeSceneNode::roll(const float radians)
	{
		m_transform *= btTransform(btQuaternion(0,0,radians));
		recalcActualTransform();
	}

	// FIXED AXIS SOLUTION
	void BeSceneNode::recalcActualTransform()
	{
		if ( m_locked )
		{
			m_transform_actual.getOrigin() += m_transform.getOrigin();
	// 		m_follow_critter_view0.setOrigin( m_transform_actual.getOrigin() );
	// 		m_follow_critter_view0.setRotation( m_transform.getRotation() );
	// 		m_transform = m_null_transform;

			btScalar rotation_x(0), rotation_y(0), rotation_z(0);
			m_transform.getBasis().getEulerYPR(rotation_x, rotation_y, rotation_z);

			// UPDATE ROTATIONS & CAP
				m_follow_critter_view0_rotx += rotation_x;
				m_follow_critter_view0_roty += rotation_y;
				m_follow_critter_view0_rotz += rotation_z;
				
				if ( m_follow_critter_view0_roty > SIMD_2_PI )
					m_follow_critter_view0_roty -= SIMD_2_PI;
				else if ( m_follow_critter_view0_roty < -SIMD_2_PI )
					m_follow_critter_view0_roty += SIMD_2_PI;
				
				if ( m_follow_critter_view0_rotz > SIMD_PI/2 )
					m_follow_critter_view0_rotz = SIMD_PI/2;
				else if ( m_follow_critter_view0_rotz < -SIMD_PI/2 )
					m_follow_critter_view0_rotz = -SIMD_PI/2;
				

			
			// TRANSFORM
				btTransform turn_y;
				turn_y.setIdentity();
				turn_y.setOrigin( m_transform_actual.getOrigin() );
				turn_y.getBasis().setEulerZYX( m_follow_critter_view0_rotz, m_follow_critter_view0_roty, m_follow_critter_view0_rotx );
				
				
			// RESET VIEW ROTATION
	// 			m_follow_critter_view0.setRotation( m_null_transform.getRotation() );
				m_transform = m_null_transform;

			m_transform_actual = turn_y;
		}
		else
		{
			m_transform_actual.setRotation( m_transform_actual.getRotation() * m_transform.getRotation() );
			m_transform = m_null_transform;
		}
	}
