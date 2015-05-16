#ifndef BODYPARTRIGIDTRIANGLEMESH_H
#define BODYPARTRIGIDTRIANGLEMESH_H

#include "bodypart_rigid.h"
#include "BulletCollision/Gimpact/btGImpactShape.h"
#include "src/common/be_obj_loader.h"
#include "src/utils/texturestore.h"
#include "src/common/be_model_system.h"

#include <BulletCollision/CollisionShapes/btShapeHull.h>

class BodypartRigidTrianglemesh : public BeRigidBody
{
	public:
		BodypartRigidTrianglemesh(const btVector3* const gVertices, const unsigned int numVertices, int* const gVIndices, const unsigned int numVIndices, btDynamicsWorld* ownerWorld, const btTransform& transform, const float weight, const float linearDamping, const float angularDamping );
		BodypartRigidTrianglemesh(boost::shared_ptr<BeGeometry> model, btDynamicsWorld* ownerWorld, const btTransform& transform, const float weight, const float linearDamping, const float angularDamping );
 
		virtual ~BodypartRigidTrianglemesh();

	private:
		boost::shared_ptr<btTriangleIndexVertexArray> m_triangleIndexVertexArray;
};

class BodypartRigidConvexmesh : public BeRigidBody
{
public:
    BodypartRigidConvexmesh(const btVector3* const gVertices, const unsigned int numVertices, int* const gVIndices, const unsigned int numVIndices, btDynamicsWorld* ownerWorld, const btTransform& transform, const float weight, const float linearDamping, const float angularDamping );
    BodypartRigidConvexmesh(boost::shared_ptr<BeGeometry> model, btDynamicsWorld* ownerWorld, const btTransform& transform, const float weight, const float linearDamping, const float angularDamping );

    virtual ~BodypartRigidConvexmesh();

private:
    boost::shared_ptr<btTriangleIndexVertexArray> m_triangleIndexVertexArray;
    boost::shared_ptr<btConvexTriangleMeshShape> m_convexTriangleMeshShape;
    boost::shared_ptr<btShapeHull> m_shapeHull;
};

#endif
