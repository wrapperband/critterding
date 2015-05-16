#include "food.h"
#include "btBulletDynamicsCommon.h"
// #include "resource/be_graphics_model_resource.h"

Food::Food()
//  : halfExtent(1.76f,1.76f,1.76f)
{
// 	settings	= Settings::Instance();
// 	food_size 	= Settings::Instance()->getCVarPtr("food_size");
	totalFrames	= 0;
	lifetime	= 0;
	energyLevel	= 0;

	// FIXME REMOVE ALL OF THESE
// 	color.r 	= 0.0f;
	color.setG(0.5f);
// 	color.b		= 0.0f;
	color.setA(1.0f);

	type = FOOD;
	isPicked = false;
	
	
}

// void Food::draw()
// {
// 	myMotionState->m_graphicsWorldTrans.getOpenGLMatrix(position);
// 	glPushMatrix(); 
// 		Displaylists::Instance()->glMultiMatrix(position);
// 
// 		glColor4f( color.r(), color.g(), color.b(), color.a() );
// // 		glScalef(halfExtent[0], halfExtent[1], halfExtent[2]);
// 
// 		Displaylists::Instance()->call();
// 
// 	glPopMatrix();
// }
// 
// void Food::draw( const bool do_color, const bool do_scale )
// {
// // 	std::cout << "test" << std::endl;
// 	if ( m_model )
// 	{
// // 		glPushMatrix(); 
// 
// 			if ( do_color )
// 				glColor4f( color.r(), color.g(), color.b(), color.a() );
// 
// // 			if ( do_scale )
// // 				glScalef(halfExtent[0]*5, halfExtent[1]*5, halfExtent[2]*5);
// 
// // 			m_model->get()->draw( 0, myMotionState->m_graphicsWorldTrans, halfExtent );
// 			m_model->get()->draw( 0, myMotionState->m_graphicsWorldTrans );
// 
// // 		glPopMatrix(); 
// 	}
// 	else
// 	{
// 		myMotionState->m_graphicsWorldTrans.getOpenGLMatrix(position);
// 		glPushMatrix(); 
// 			Displaylists::Instance()->glMultiMatrix(position);
// 
// 			if ( do_color )
// 				glColor4f( color.r(), color.g(), color.b(), color.a() );
// 
// // 			if ( do_scale )
// // 				glScalef(halfExtent[0], halfExtent[1], halfExtent[2]);
// 			
// 			Displaylists::Instance()->call();
// 		glPopMatrix(); 
// 	}
// }


void Food::createBody(btDynamicsWorld* m_dynamicsWorld, const btVector3& startOffset)
{
	body.m_ownerWorld = m_dynamicsWorld;

	
	
// 	btTransform offset;
// 	offset.setIdentity();
// 	offset.setOrigin(startOffset);
// 
// 
// 	groundShape = new btSphereShape(btScalar( *worldsizeX ));
// 	
// 	fixedGround = new btCollisionObject();
// 	fixedGround->setUserPointer(this);
// 	fixedGround->setCollisionShape(groundShape);
// 	fixedGround->setWorldTransform(groundTransform);
// 	m_dynamicsWorld->addCollisionObject(fixedGround);
// 







	
	
	btTransform offset;
	offset.setIdentity();
	offset.setOrigin(startOffset);

	btTransform transform;
	transform.setIdentity();
	transform.setOrigin( btVector3(0.0f, 0.10f, 0.0f) );

// 	body.addBodyPart_Box((void*)this, 0.01f * *food_size, 0.01f * *food_size, 0.01f * *food_size, 1.0f, offset, transform);
	body.addBodyPart_Sphere((void*)this, 1.0f, 1.0f, 1.0f, 1.0f, offset, transform);
	

	myMotionState = (btDefaultMotionState*)body.bodyparts[0]->body->getMotionState();
// 	boxShape = static_cast<btBoxShape*>(body.bodyparts[0]->shape);
// 	halfExtent = boxShape->getHalfExtentsWithMargin();
	
// 	body.bodyparts[0]->body->setDeactivationTime(0.001f);
// 	body.bodyparts[0]->body->setSleepingThresholds(1.6f, 2.5f);
// 	body.bodyparts[0]->body->setActivationState(WANTS_DEACTIVATION);
}

Food::~Food()
{
}


