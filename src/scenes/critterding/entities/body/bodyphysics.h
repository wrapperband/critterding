#ifndef BODYPHYSICS_H
#define BODYPHYSICS_H

#include <vector>

#include "bodypart_rigid.h"
#include "bodypart_rigid_box.h"
#include "bodypart_rigid_sphere.h"
#include "bodypart_rigid_cylinder.h"
#include "bodypart_rigid_trianglemesh.h"

#include "filesystem/be_filesystem.h"

class BePhysicsModel
{
	public:
		BePhysicsModel( btDynamicsWorld* ownerWorld );
		virtual ~BePhysicsModel();

		boost::shared_ptr<BodypartRigidBox>	addBodyPart_Rigid_Box(const btVector3& dimensions, const btTransform& transform, const float weight, const float linearDamping, const float angularDamping );
		boost::shared_ptr<BodypartRigidSphere> addBodyPart_Rigid_Sphere(const float radius, const btTransform& transform, const float weight, const float linearDamping, const float angularDamping  );
        boost::shared_ptr<BodypartRigidCylinder>	addBodyPart_Rigid_Cylinder(const btVector3& dimensions, const btTransform& transform, const float weight, const float linearDamping, const float angularDamping  );
		boost::shared_ptr<BodypartRigidTrianglemesh>	addBodyPart_Rigid_Trianglemesh(const btVector3* const gVertices, const unsigned int numVertices, int* const gVIndices, const unsigned int numVIndices, const btTransform& transform, const float weight, const float linearDamping, const float angularDamping  );
		boost::shared_ptr<BodypartRigidTrianglemesh>	addBodyPart_Rigid_Trianglemesh(boost::shared_ptr<BeGeometry> model, const btTransform& transform , const float weight, const float linearDamping, const float angularDamping  );

        boost::shared_ptr<BodypartRigidConvexmesh>	addBodyPart_Rigid_Convexmesh(const btVector3* const gVertices, const unsigned int numVertices, int* const gVIndices, const unsigned int numVIndices, const btTransform& transform , const float weight, const float linearDamping, const float angularDamping  );
        boost::shared_ptr<BodypartRigidConvexmesh>	addBodyPart_Rigid_Convexmesh(boost::shared_ptr<BeGeometry> model, const btTransform& transform , const float weight, const float linearDamping, const float angularDamping  );

        std::vector<boost::shared_ptr<BeRigidBody> >	m_bodyparts;
public:
        btDynamicsWorld* 	m_ownerWorld;
};
#endif
