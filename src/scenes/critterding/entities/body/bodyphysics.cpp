#include "bodyphysics.h"
#include <boost/smart_ptr/make_shared.hpp>

BePhysicsModel::BePhysicsModel( btDynamicsWorld*ownerWorld ) : m_ownerWorld(ownerWorld)
{
}

boost::shared_ptr<BodypartRigidBox> BePhysicsModel::addBodyPart_Rigid_Box(const btVector3& dimensions, const btTransform& transform,  const float weight, const float linearDamping, const float angularDamping )
{
	boost::shared_ptr<BodypartRigidBox> b = boost::make_shared<BodypartRigidBox>(
		dimensions,
		m_ownerWorld,
		transform,
        weight,
        linearDamping,
        angularDamping
	);
	m_bodyparts.push_back( b );
	return b;
}

boost::shared_ptr<BodypartRigidSphere> BePhysicsModel::addBodyPart_Rigid_Sphere(const float radius, const btTransform& transform,  const float weight, const float linearDamping, const float angularDamping)
{
	boost::shared_ptr<BodypartRigidSphere> b = boost::make_shared<BodypartRigidSphere>(
        radius,
        m_ownerWorld,
        transform,
        weight,
        linearDamping,
        angularDamping
    );
    m_bodyparts.push_back( b );
    return b;
}

boost::shared_ptr<BodypartRigidCylinder> BePhysicsModel::addBodyPart_Rigid_Cylinder(const btVector3& dimensions, const btTransform& transform,  const float weight, const float linearDamping, const float angularDamping)
{
	boost::shared_ptr<BodypartRigidCylinder> b = boost::make_shared<BodypartRigidCylinder>(
		dimensions,
		m_ownerWorld,
		transform,
        weight,
        linearDamping,
        angularDamping
	);
	m_bodyparts.push_back( b );
	return b;
}

boost::shared_ptr<BodypartRigidTrianglemesh> BePhysicsModel::addBodyPart_Rigid_Trianglemesh(const btVector3*const  gVertices, const unsigned int numVertices, int* const gVIndices, const unsigned int numVIndices, const btTransform& transform, const float weight, const float linearDamping, const float angularDamping )
{
	boost::shared_ptr<BodypartRigidTrianglemesh> b = boost::make_shared<BodypartRigidTrianglemesh>(
		gVertices, numVertices, gVIndices, numVIndices, 
		m_ownerWorld,
		transform,
        weight,
        linearDamping,
        angularDamping
	);
	m_bodyparts.push_back( b );
	return b;
}

boost::shared_ptr<BodypartRigidTrianglemesh> BePhysicsModel::addBodyPart_Rigid_Trianglemesh(boost::shared_ptr<BeGeometry> model, const btTransform& transform, const float weight, const float linearDamping, const float angularDamping )
{
	boost::shared_ptr<BodypartRigidTrianglemesh> b = boost::make_shared<BodypartRigidTrianglemesh>(
		model,
		m_ownerWorld,
		transform,
        weight,
        linearDamping,
        angularDamping
	);
	m_bodyparts.push_back( b );
	return b;
}

boost::shared_ptr<BodypartRigidConvexmesh> BePhysicsModel::addBodyPart_Rigid_Convexmesh(const btVector3* const gVertices, const unsigned int numVertices, int* const gVIndices, const unsigned int numVIndices, const btTransform& transform, const float weight, const float linearDamping, const float angularDamping )
{
    boost::shared_ptr<BodypartRigidConvexmesh> b = boost::make_shared<BodypartRigidConvexmesh>(
        gVertices, numVertices, gVIndices, numVIndices, 
        m_ownerWorld,
        transform,
        weight,
        linearDamping,
        angularDamping
        );
    m_bodyparts.push_back( b );
    return b;
}

boost::shared_ptr<BodypartRigidConvexmesh> BePhysicsModel::addBodyPart_Rigid_Convexmesh(boost::shared_ptr<BeGeometry> model, const btTransform& transform,  const float weight, const float linearDamping, const float angularDamping)
{
    boost::shared_ptr<BodypartRigidConvexmesh> b = boost::make_shared<BodypartRigidConvexmesh>(
        model,
        m_ownerWorld,
        transform, 
        weight,
        linearDamping, 
        angularDamping
        );
    m_bodyparts.push_back( b );
    return b;
}

BePhysicsModel::~BePhysicsModel()
{
}
