#include "wall.h"
#include "resource/be_graphics_model_resource.h"

Wall::Wall(const float X, const float Y, const float Z, const btTransform& transform, btDynamicsWorld* m_dynamicsWorld)
 : halfX(X/2)
 , halfY(Y/2)
 , halfZ(Z/2)
 , groundTransform(transform)
 , m_ownerWorld(m_dynamicsWorld)
 , groundShape(0)
 , fixedGround(0)

// Wall::Wall(float X, float Y, float Z, const btVector3& position, btDynamicsWorld* m_dynamicsWorld)
{


// 	color.r		= 0.0f;
// 	color.g		= 0.0f;
	color.setB(0.5f);
// 	color.a		= 0.0f;

	groundShape = new btBoxShape( btVector3( halfX ,halfY, halfZ ) );
	
// 	groundTransform.setIdentity();
// 	groundTransform.setOrigin( position );

	fixedGround = new btCollisionObject();
	fixedGround->setUserPointer(this);
	fixedGround->setCollisionShape(groundShape);
	fixedGround->setWorldTransform(groundTransform);
	
	fixedGround->setFriction(0.8f);                                  // added by Ethical
  
	m_ownerWorld->addCollisionObject(fixedGround);

	groundTransform.getOpenGLMatrix(nposition);
	
	type = WALL;
	isPicked = false;
}

void Wall::draw()
{
	glPushMatrix(); 
		Displaylists::Instance()->glMultiMatrix(nposition);

		glColor4f( color.r(), color.g(), color.b(), color.a() );
		glScalef(halfX, halfY, halfZ);

		Displaylists::Instance()->call();

	glPopMatrix();
}

void Wall::draw( const bool do_color, const bool do_scale )
{
// 	std::cout << "test" << std::endl;
	if ( m_model )
	{
// 		std::cout << "a" << std::endl;
		glPushMatrix(); 
			Displaylists::Instance()->glMultiMatrix(nposition);

			if ( do_color )
				glColor4f( color.r(), color.g(), color.b(), color.a() );

			if ( do_scale )
				glScalef(halfX, halfY, halfZ);

			m_model->get()->setContext( 0 );
			m_model->get()->draw( 0 );

		glPopMatrix(); 
	}
	else
	{
// 		std::cout << "b" << std::endl;
// 		myMotionState->m_graphicsWorldTrans.getOpenGLMatrix(position);
		glPushMatrix(); 
			Displaylists::Instance()->glMultiMatrix(nposition);

			if ( do_color )
				glColor4f( color.r(), color.g(), color.b(), color.a() );

			if ( do_scale )
				glScalef(halfX, halfY, halfZ);
			
			Displaylists::Instance()->call();
		glPopMatrix(); 
	}
}

// void Wall::draw()
// {
// 	glPushMatrix(); 
// 		Displaylists::Instance()->glMultiMatrix(nposition);
// 		glColor4f( color.r(), color.g(), color.b(), color.a() );
// 		glScalef(halfX, halfY, halfZ);
// 		Displaylists::Instance()->call();
// 	glPopMatrix(); 
// }
// 
// void Wall::draw( const bool do_color, const bool do_scale )
// {
// 	glPushMatrix(); 
// 		Displaylists::Instance()->glMultiMatrix(nposition);
// 
// 		if ( do_color )
// 			glColor4f( color.r(), color.g(), color.b(), color.a() );
// 		
// 		if ( do_scale )
// 			glScalef(halfX, halfY, halfZ);
// 		
// 		Displaylists::Instance()->call();
// 	glPopMatrix(); 
// }

Wall::~Wall()
{
	if ( fixedGround )
		m_ownerWorld->removeCollisionObject(fixedGround);
	
	if ( groundShape )
		delete groundShape;
	
	if ( fixedGround )
		delete fixedGround;
}


