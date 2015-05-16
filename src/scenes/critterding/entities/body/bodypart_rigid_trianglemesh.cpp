#include "bodypart_rigid_trianglemesh.h"

#include "kernel/be_profiler.h"

#include <BulletCollision/CollisionShapes/btShapeHull.h>

BodypartRigidTrianglemesh::BodypartRigidTrianglemesh(const btVector3* const gVertices, const unsigned int numVertices, int* const gVIndices, const unsigned int numVIndices, btDynamicsWorld* ownerWorld, const btTransform& transform ,const float weight, const float linearDamping, const float angularDamping  )
  : BeRigidBody( ownerWorld )
{
	BE_FUNCTION_PROFILER;
	m_triangleIndexVertexArray.reset( new btTriangleIndexVertexArray(numVIndices/3, gVIndices, 3*sizeof(int), numVertices/3,(btScalar*) &gVertices[0].x(),3*sizeof(btScalar)) );
	if ( weight > 0.0f )
	{
		btGImpactMeshShape * tshape = new btGImpactMeshShape(m_triangleIndexVertexArray.get());
		tshape->setLocalScaling(btVector3(1.f,1.f,1.f));
		tshape->updateBound();
		m_shape.reset(tshape);
	}
	else
		m_shape.reset(new btBvhTriangleMeshShape(m_triangleIndexVertexArray.get(),true));
	generalSetup( weight, transform, linearDamping, angularDamping );
}

BodypartRigidTrianglemesh::BodypartRigidTrianglemesh(boost::shared_ptr<BeGeometry> model, btDynamicsWorld* ownerWorld, const btTransform& transform ,const float weight, const float linearDamping, const float angularDamping  )
  : BeRigidBody( ownerWorld )
{
	BE_FUNCTION_PROFILER;
	m_triangleIndexVertexArray.reset( new btTriangleIndexVertexArray( model->getNumberVIndices()/3, model->indices.get(), 3*sizeof(int), model->getNumberVertices()/3, (btScalar*) model->vertices.get(), 3*sizeof(btScalar) ) );
    if ( weight > 0.0f )
    {
        btGImpactMeshShape * tshape = new btGImpactMeshShape(m_triangleIndexVertexArray.get());
        tshape->setLocalScaling(btVector3(1.f,1.f,1.f));
        tshape->updateBound();
        m_shape.reset(tshape);
    }
    else
	    m_shape.reset(new btBvhTriangleMeshShape(m_triangleIndexVertexArray.get(),true));
	generalSetup( weight, transform, linearDamping, angularDamping );
}

BodypartRigidTrianglemesh::~BodypartRigidTrianglemesh()
{
	
}

BodypartRigidConvexmesh::BodypartRigidConvexmesh(const btVector3* const gVertices, const unsigned int numVertices, int* const gVIndices, const unsigned int numVIndices, btDynamicsWorld* ownerWorld, const btTransform& transform ,const float weight, const float linearDamping, const float angularDamping  )
    : BeRigidBody( ownerWorld )
{
    BE_FUNCTION_PROFILER;
    m_triangleIndexVertexArray.reset( new btTriangleIndexVertexArray(numVIndices/3, gVIndices, 3*sizeof(int), numVertices/3,(btScalar*) &gVertices[0].x(),3*sizeof(btScalar)) );
    m_convexTriangleMeshShape.reset(new btConvexTriangleMeshShape(m_triangleIndexVertexArray.get(),true));
    m_shapeHull.reset(new btShapeHull(m_convexTriangleMeshShape.get()));
    btScalar margin = m_convexTriangleMeshShape->getMargin();
    m_shapeHull->buildHull(margin);
    m_shape.reset(new btConvexHullShape((btScalar*)m_shapeHull->getVertexPointer(),m_shapeHull->numVertices()));
    generalSetup( weight, transform, linearDamping, angularDamping );
}

BodypartRigidConvexmesh::BodypartRigidConvexmesh(boost::shared_ptr<BeGeometry> model, btDynamicsWorld* ownerWorld, const btTransform& transform ,const float weight, const float linearDamping, const float angularDamping  )
    : BeRigidBody( ownerWorld )
{
    BE_FUNCTION_PROFILER;
    m_triangleIndexVertexArray.reset( new btTriangleIndexVertexArray( model->getNumberVIndices()/3, model->indices.get(), 3*sizeof(int), model->getNumberVertices()/3, (btScalar*) model->vertices.get(), 3*sizeof(btScalar) ) );
    m_convexTriangleMeshShape.reset(new btConvexTriangleMeshShape(m_triangleIndexVertexArray.get(),true));
    m_shapeHull.reset(new btShapeHull(m_convexTriangleMeshShape.get()));
    btScalar margin = m_convexTriangleMeshShape->getMargin();
    m_shapeHull->buildHull(margin);
    m_shape.reset(new btConvexHullShape((btScalar*)m_shapeHull->getVertexPointer(),m_shapeHull->numVertices()));
    generalSetup( weight, transform, linearDamping, angularDamping );
}

BodypartRigidConvexmesh::~BodypartRigidConvexmesh()
{

}
