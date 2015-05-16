#ifndef DISPLAYLISTS_H
#define DISPLAYLISTS_H

#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionShapes/btTriangleInfoMap.h"
#include <SDL/SDL.h>

#include <string>
#include <iostream>
#include <sstream>
// #include "file.h"
#include "src/common/be_parser.h"
// #include "shapecache.h"
// #include <GL/gl.h>
//#include <SDL/SDL_opengl.h>
// #include <GL/glext.h>
// #include <GL/glu.h>
#include <GL/glew.h>

class GlDrawcallback : public btTriangleCallback
{
public:

	bool	m_wireframe;

	GlDrawcallback()
		:m_wireframe(false)
	{
// 		cerr << "hello0" << endl;
	}

	virtual void processTriangle(btVector3* triangle,int partId, int triangleIndex)
	{
// 		cerr << "hello" << endl;

		(void)triangleIndex;
		(void)partId;

		if (m_wireframe)
		{
			glBegin(GL_LINES);
			glColor3f(1, 0, 0);
			glVertex3d(triangle[0].getX(), triangle[0].getY(), triangle[0].getZ());
			glVertex3d(triangle[1].getX(), triangle[1].getY(), triangle[1].getZ());
			glColor3f(0, 1, 0);
			glVertex3d(triangle[2].getX(), triangle[2].getY(), triangle[2].getZ());
			glVertex3d(triangle[1].getX(), triangle[1].getY(), triangle[1].getZ());
			glColor3f(0, 0, 1);
			glVertex3d(triangle[2].getX(), triangle[2].getY(), triangle[2].getZ());
			glVertex3d(triangle[0].getX(), triangle[0].getY(), triangle[0].getZ());
			glEnd();
		} else
		{
			glBegin(GL_TRIANGLES);
			//glColor3f(1, 1, 1);
			
			
			glVertex3d(triangle[0].getX(), triangle[0].getY(), triangle[0].getZ());
			glVertex3d(triangle[1].getX(), triangle[1].getY(), triangle[1].getZ());
			glVertex3d(triangle[2].getX(), triangle[2].getY(), triangle[2].getZ());

			glVertex3d(triangle[2].getX(), triangle[2].getY(), triangle[2].getZ());
			glVertex3d(triangle[1].getX(), triangle[1].getY(), triangle[1].getZ());
			glVertex3d(triangle[0].getX(), triangle[0].getY(), triangle[0].getZ());
			glEnd();
		}
// 		cerr << "hello" << endl;
	}
};

class Displaylists
{
	public:
		static Displaylists*	Instance();
		int			displayLists;
		void			generateList();
		void			call();
// 		void			drawShadow(btScalar* m,const btVector3& extrusion,const btCollisionShape* shape, ShapeCache* sc,const btVector3& worldBoundsMin,const btVector3& worldBoundsMax);
		void			drawSphere(btScalar radius, int lats, int longs);
		void			drawCylinder(float radius, float height, unsigned int planes);

		void			glMultiMatrix( btScalar* pos );
		
// 		ShapeCache*		cache(btConvexShape*);
		
	protected:
		Displaylists();

	private:
		static Displaylists*	_instance;
		GLuint			vboId;
		GLuint createVBO(const void* data, int dataSize, GLenum target, GLenum usage);

/*PFNGLBINDBUFFERARBPROC glBindBufferARB;
PFNGLGENBUFFERSARBPROC glGenBuffersARB;
PFNGLBUFFERDATAARBPROC glBufferDataARB;
PFNGLBUFFERSUBDATAARBPROC glBufferSubDataARB;*/
};

#endif
