#ifndef BECAMERA_H_INCLUDED
#define BECAMERA_H_INCLUDED

#include "src/utils/settings.h"
// #include "src/utils/displaylists.h"
#include "src/math/vector3f.h"
#include <GL/glew.h>
#include "btBulletDynamicsCommon.h"
#include "src/common/be_scene_node.h"

class BeCamera
{
		friend class BeGraphicsSystem;
protected:
		BeCamera();
		virtual ~BeCamera(){};
public:
	//TODO: move to BeRay and add width, height and transform arguments
	void setSceneNode(BeSceneNode* sceneNode) { m_sceneNode=sceneNode; }
	BeSceneNode* getSceneNode() { return m_sceneNode; }
	const BeSceneNode* getSceneNode() const { return m_sceneNode; }
protected:
	//TODO: remove settings once getScreenDirection has been removed
	BeSceneNode* m_sceneNode;
	Settings* settings;
};

class BeCameraPerspective : public BeCamera
{
	friend class BeGraphicsSystem;
public:
	BeCameraPerspective(const float fovy, const float aspect, const float zNear, const float zFar) : m_fovy(fovy), m_aspect(aspect), m_zNear(zNear), m_zFar(zFar)
	{
	}
	virtual ~BeCameraPerspective(){}
	
	btVector3 getScreenDirection(const float x, const float y);
	btVector3 getScreenDirection(const float x, const float y, const btTransform& tr);

	void setFovy ( const float fovy ) { m_fovy = fovy; }
	void setAspect( const float aspect ) { m_aspect = aspect; }
// private:
	float m_fovy;
	float m_aspect;
	float m_zNear;
	float m_zFar;
};

#endif
