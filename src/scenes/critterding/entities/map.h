#ifndef MAP_H
#define MAP_H

#include "scenes/critterding/entities/body/bodyphysics.h"
#include "common/be_model_system.h"
#include "physics/be_physics_material.h"
// #include "src/common/be_xml_helper.h"
// #include "physics/be_physics_system.h"

#include <boost/shared_ptr.hpp>


class ClientMap
{
public:

	ClientMap(BeFilesystem& filesystem, const std::string& path);
	~ClientMap();

	void draw(const btVector3& scale);

	void upload(boost::shared_ptr<BeGraphicsSystem> graphicsSystem, boost::shared_ptr<BeGraphicsModelSystem> modelSystem)
	{
		btTransform chassis_tranform;
		chassis_tranform.setIdentity(); 
		m_model=modelSystem->load(m_path, graphicsSystem, modelSystem, btVector3(1,1,1), chassis_tranform);
// 		m_skyBoxModel=modelSystem->load("skys/default/skydome3.obj", graphicsSystem, modelSystem, Vector3f(1,1,1), chassis_tranform);
	}

// 	float m_skyBoxAngle;
// 	void drawSkyBox();
// 	void setSkyBoxTransform(const btTransform& transform)
// 	{
// 		m_skyBoxTransform=transform;
// 	}
	
protected:

	void load(BeFilesystem& filesystem, const std::string& path);
	boost::shared_ptr<BeGraphicsModelResource> m_model;
    btVector3 m_scale;
	btTransform m_transform;
	std::string m_path;

};

class ServerMap
{
public:

	ServerMap(BeFilesystem& filesystem, const std::string& path, boost::shared_ptr<BeGeometrySystem> modelSystem, btDynamicsWorld* dynamicsWorld/*, const btVector3& tscale*/);
	~ServerMap();

// 	btTransform finish;
	btAlignedObjectArray<btTransform> m_checkpoints;

	float finish_width;
	float finish_height;
	bool hasModel() { return m_model!=0; }
	BePhysicsModel& body() { return m_body; }

protected:
	BePhysicsModel m_body;
	void load(BeFilesystem& filesystem, const std::string& path, boost::shared_ptr<BeGeometrySystem> modelSystem/*, const btVector3& tscale*/);
	bool loadPhysicsMaterials(const std::string& path);
	boost::shared_ptr<BeGeometry> m_model;
	btTransform m_transform;
	typedef std::map< std::string, BePhysicsMaterial > PhysicsMaterialMap;
	PhysicsMaterialMap m_physicsMaterialMap;
	BeLogDebug m_logDebug;
};

#endif
