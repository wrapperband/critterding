#ifndef BE_SCENE_NODE_H_INCLUDED
#define BE_SCENE_NODE_H_INCLUDED

#include "LinearMath/btTransform.h"

class BeSceneNode
{
public:
	BeSceneNode(const bool lock_axis);
	void setTransform(const btTransform& transform);
	const btTransform& getTransform() const;

// 	btTransform getTransform()
// 	{
// 		return m_transform;
// 	}

	void setOrigin(const btVector3& origin);
	void setRotation(const btQuaternion& rotation);
	const btVector3& getOrigin() const;
	void translate(const btVector3& vector);
	void translateLocal(const btVector3& vector);
	const btVector3& getRightVector() const;
	const btVector3& getUpVector() const;
	const btVector3& getForwardVector() const;
	void yaw(const float radians);
	void pitch(const float radians);
	void roll(const float radians);


	void lockAxis_X(){ m_lock_x = true;}
	void lockAxis_Y(){ m_lock_y = true;}
	void lockAxis_Z(){ m_lock_z = true;}


private:
	inline void recalcActualTransform();
	
	btTransform m_transform;
	
	bool m_lock_x;
	bool m_lock_y;
	bool m_lock_z;
	
	btTransform m_null_transform;
// 	btTransform m_follow_critter_view0;
	btTransform m_transform_actual;
	btScalar m_follow_critter_view0_rotx;
	btScalar m_follow_critter_view0_roty;
	btScalar m_follow_critter_view0_rotz;
	bool m_locked;
};

#endif