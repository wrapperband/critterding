#include "displaylists.h"

// PFNGLBINDBUFFERARBPROC glBindBufferARB = NULL;
// PFNGLGENBUFFERSARBPROC glGenBuffersARB = NULL;
// PFNGLBUFFERDATAARBPROC glBufferDataARB = NULL;

// 	GLfloat vertices[] = 
// 	{
// 		1.0f, 1.0f, 1.0f,   //V2
// 		1.0f,-1.0f, 1.0f,   //V1
// 		1.0f,-1.0f,-1.0f,   //V3
// 		1.0f, 1.0f,-1.0f,   //V4
// 		//Quad 2
// 		1.0f, 1.0f,-1.0f,   //V4
// 		1.0f,-1.0f,-1.0f,   //V3
// 		-1.0f,-1.0f,-1.0f,   //V5
// 		-1.0f, 1.0f,-1.0f,   //V6
// 		//Quad 3
// 		-1.0f, 1.0f,-1.0f,   //V6
// 		-1.0f,-1.0f,-1.0f,   //V5
// 		-1.0f,-1.0f, 1.0f,   //V7
// 		-1.0f, 1.0f, 1.0f,   //V8
// 		//Quad 4
// 		-1.0f, 1.0f, 1.0f,   //V8
// 		-1.0f,-1.0f, 1.0f,   //V7
// 		 1.0f,-1.0f, 1.0f,   //V1
// 		 1.0f, 1.0f, 1.0f,   //V2
// 		//Quad 5
// 		-1.0f, 1.0f,-1.0f,   //V6
// 		-1.0f, 1.0f, 1.0f,   //V8
// 		 1.0f, 1.0f, 1.0f,   //V2
// 		 1.0f, 1.0f,-1.0f,   //V4
// 		//Quad 6
// 		 1.0f,-1.0f,-1.0f,   //V4
// 		 1.0f,-1.0f, 1.0f,   //V2
// 		-1.0f,-1.0f, 1.0f,   //V8
// 		-1.0f,-1.0f,-1.0f   //V6
// 	};
// 
// 	GLfloat normals[] = 
// 	{
// 		 1.0f, 0.0f, 0.0f,
// 		 0.0f, 0.0f,-1.0f,
// 		-1.0f, 0.0f, 0.0f,
// 		 0.0f, 0.0f, 1.0f,
// 		 0.0f, 1.0f, 0.0f,
// 		0.0f, -1.0f, 0.0f
// 	};
// 
// 	GLfloat colors[] = 
// 	{
// 		 1.0f, 0.0f, 0.0f,
// 		 0.0f, 0.0f, 1.0f,
// 		 1.0f, 0.0f, 0.0f,
// 		 0.0f, 0.0f, 1.0f,
// 		 0.0f, 1.0f, 0.0f,
// 		0.0f,  1.0f, 0.0f
// 	};
	
Displaylists* Displaylists::Instance() 
{
	static Displaylists t;
	return &t;
}

// void Displaylists::drawShadow(btScalar* m,const btVector3& extrusion,const btCollisionShape* shape, ShapeCache* sc,const btVector3& worldBoundsMin,const btVector3& worldBoundsMax)
// {
// 	glPushMatrix(); 
// // 	glMultMatrixf(m);
// // 	glMultMatrixd(m); // FIXME WATCHIT OK WENT TO DOUBLE HERE 
// 	glMultiMatrix(m);
// 	if(shape->getShapeType() == UNIFORM_SCALING_SHAPE_PROXYTYPE)
// 	{
// 		const btUniformScalingShape* scalingShape = static_cast<const btUniformScalingShape*>(shape);
// 		const btConvexShape* convexShape = scalingShape->getChildShape();
// 		float scalingFactor = (float)scalingShape->getUniformScalingFactor();
// 		btScalar tmpScaling[4][4]={ {scalingFactor,0,0,0}, {0,scalingFactor,0,0}, {0,0,scalingFactor,0}, {0,0,0,1} };
// 		drawShadow((btScalar*)tmpScaling,extrusion,convexShape,sc,worldBoundsMin,worldBoundsMax);
// 		glPopMatrix();
// 		return;
// 	}
// 	else if(shape->getShapeType()==COMPOUND_SHAPE_PROXYTYPE)
// 	{
// 		const btCompoundShape* compoundShape = static_cast<const btCompoundShape*>(shape);
// 		for (int i=compoundShape->getNumChildShapes()-1;i>=0;i--)
// 		{
// 			btTransform childTrans = compoundShape->getChildTransform(i);
// 			const btCollisionShape* colShape = compoundShape->getChildShape(i);
// 			btScalar childMat[16];
// 			childTrans.getOpenGLMatrix(childMat);
// 			drawShadow(childMat,extrusion*childTrans.getBasis(),colShape,sc,worldBoundsMin,worldBoundsMax);
// 		}
// 	}
// 	else
// 	{
// 		if (shape->isConvex())
// 		{
// 			btShapeHull* hull =&sc->m_shapehull;
// 			glBegin(GL_QUADS);
// 			for(int i=0;i<sc->m_edges.size();++i)
// 			{			
// 				const btScalar d=btDot(sc->m_edges[i].n[0],extrusion);
// 				if(( d * btDot(sc->m_edges[i].n[1],extrusion)) < 0 )
// 				{
// 					const int		q=	d<0?1:0;
// 					const btVector3&	a=	hull->getVertexPointer()[sc->m_edges[i].v[q]];
// 					const btVector3&	b=	hull->getVertexPointer()[sc->m_edges[i].v[1-q]];
// 					glVertex3f(a[0],a[1],a[2]);
// 					glVertex3f(b[0],b[1],b[2]);
// 					glVertex3f(b[0]+extrusion[0],b[1]+extrusion[1],b[2]+extrusion[2]);
// 					glVertex3f(a[0]+extrusion[0],a[1]+extrusion[1],a[2]+extrusion[2]);
// 				}
// 			}
// 			glEnd();
// 		}
// 
// 	}
// 	glPopMatrix();
// 
// }

GLuint Displaylists::createVBO(const void* data, int dataSize, GLenum target, GLenum usage)
{
        GLuint id = 0;
        
//         glGenBuffersARB(1, &id);
//         glBindBufferARB(target, id);
//         glBufferDataARB(target, dataSize, data, usage);
        
        return id;
}

Displaylists::Displaylists()
{
	generateList();

// 	// vbo stuff
// 	glGenBuffersARB = (PFNGLGENBUFFERSARBPROC)SDL_GL_GetProcAddress("glGenBuffersARB");
// 	glBindBufferARB = (PFNGLBINDBUFFERARBPROC)SDL_GL_GetProcAddress("glBindBufferARB");
// 	glBufferDataARB = (PFNGLBUFFERDATAARBPROC)SDL_GL_GetProcAddress("glBufferDataARB");
// 	glBufferSubDataARB = (PFNGLBUFFERSUBDATAARBPROC)SDL_GL_GetProcAddress("glBufferSubDataARB");
// 
// 	vboId = createVBO(NULL, sizeof(vertices) + sizeof(normals), GL_ARRAY_BUFFER_ARB, GL_STATIC_DRAW_ARB); //  + sizeof(colors)
// 		glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, sizeof(vertices), vertices);
// 		glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, sizeof(vertices), sizeof(normals), normals);
// // 		glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, sizeof(vertices) + sizeof(normals), sizeof(colors), colors);
// 
// 	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboId);
//         glEnableClientState(GL_VERTEX_ARRAY);
}

void Displaylists::drawSphere(btScalar radius, int lats, int longs)
{
	int i, j;
	for(i = 0; i <= lats; i++) {
		btScalar lat0 = SIMD_PI * (-btScalar(0.5) + (btScalar) (i - 1) / lats);
		btScalar z0  = radius*sin(lat0);
		btScalar zradius =  radius*cos(lat0);

		btScalar lat1 = SIMD_PI * (-btScalar(0.5) + (btScalar) i / lats);
		btScalar z1 = radius*sin(lat1);
		btScalar zr1 = radius*cos(lat1);

		glBegin(GL_QUAD_STRIP);
		for(j = 0; j <= longs; j++) {
			btScalar lng = 2 * SIMD_PI * (btScalar) (j - 1) / longs;
			btScalar x = cos(lng);
			btScalar y = sin(lng);
			glNormal3f(x * zr1, y * zr1, z1);
			glVertex3f(x * zr1, y * zr1, z1);
			glNormal3f(x * zradius, y * zradius, z0);
			glVertex3f(x * zradius, y * zradius, z0);
		}
		glEnd();
	}
}

void Displaylists::drawCylinder(float radius, float height, unsigned int planes)
{
// 	float height = 1.0f;
// 	float anglehelp = ;
	float angle;
// 	float radius = 1.0f;

	glBegin(GL_QUAD_STRIP);
	for ( unsigned int i = 0; i <= planes; i++ )
	{
		angle = i * 2.0 * M_PI / planes;

		glNormal3f(cos(angle), 0.0, sin(angle));
		glVertex3f(radius * cos(angle), -height, radius * sin(angle));
		glVertex3f(radius * cos(angle), height, radius * sin(angle));
	}
	glEnd();

	// same as above, but with triangles
// 	glBegin(GL_TRIANGLE_STRIP);
// 	for (unsigned int i = 0; i <= planes; i++)
// 	{
// 		angle = i * 2.0 * M_PI / planes;
// 
// 		glNormal3f(cos(angle), 0.0, sin(angle));
// 		glVertex3f(radius * cos(angle), -height, radius * sin(angle));
// 		glVertex3f(radius * cos(angle), height, radius * sin(angle));
// 	}
// 	glEnd();

	// top circle
	glBegin(GL_TRIANGLE_STRIP);
	for ( unsigned int i = 0; i <= planes; i++ )
	{
		angle = i * 2.0 * M_PI / planes;
		glNormal3f(0.0f, 1.0f, 0.0f);
		glVertex3f(radius * cos(angle), height, radius * sin(angle));
		glVertex3f(0.0f, height, 0.0f);
	}
	glEnd();

	// bottom circle
	glBegin(GL_TRIANGLE_STRIP);
	for ( unsigned int i = 0; i <= planes; i++ )
	{
		angle = i * 2.0 * M_PI / planes;
		glNormal3f(0.0f, -1.0f, 0.0f);
		glVertex3f(0.0f, -height, 0.0f);
		glVertex3f(radius * cos(angle), -height, radius * sin(angle));
	}
	glEnd();
}

void Displaylists::glMultiMatrix( btScalar* pos )
{
	glMultMatrixf(pos); // FIXME WATCHIT OK WENT TO DOUBLE HERE 
// 	glMultMatrixd(pos); // FIXME WATCHIT OK WENT TO DOUBLE HERE 
}

void Displaylists::call()
{
	glCallList(displayLists);

// 	if ( index == 0 )
// 	{
//                 glVertexPointer(3, GL_FLOAT, 0, 0);
//                 glDrawArrays(GL_QUADS, 0, 24);
// 	}
// 	else
// 	{
//                 glEnableClientState(GL_NORMAL_ARRAY);
// 
//                 glNormalPointer(GL_FLOAT, 0, (float*)NULL + (24)); // 
// //                 glNormalPointer(GL_FLOAT, 0, (void*)sizeof(vertices)); // ((float*)NULL + (24))
// 
//                 glVertexPointer(3, GL_FLOAT, 0, 0);
//                 glDrawArrays(GL_QUADS, 0, 24);
//                 glDisableClientState(GL_NORMAL_ARRAY);
// 	}


}

// ShapeCache* Displaylists::cache(btConvexShape* shape)
// {
// 	ShapeCache* sc = new(btAlignedAlloc(sizeof(ShapeCache),16)) ShapeCache(shape);
// 	sc->m_shapehull.buildHull(shape->getMargin());
// 
// 	/* Build edges	*/ 
// 	const int			ni=sc->m_shapehull.numIndices();
// 	const int			nv=sc->m_shapehull.numVertices();
// 	const unsigned int*	pi=sc->m_shapehull.getIndexPointer();
// 	const btVector3*	pv=sc->m_shapehull.getVertexPointer();
// 	btAlignedObjectArray<ShapeCache::Edge*>	edges;
// 	sc->m_edges.reserve(ni);
// 	edges.resize(nv*nv,0);
// 	for(int i=0;i<ni;i+=3)
// 	{
// 		const unsigned int* ti=pi+i;
// 		const btVector3		nrm=btCross(pv[ti[1]]-pv[ti[0]],pv[ti[2]]-pv[ti[0]]).normalized();
// 		for(int j=2,k=0;k<3;j=k++)
// 		{
// 			const unsigned int	a=ti[j];
// 			const unsigned int	b=ti[k];
// 			ShapeCache::Edge*&	e=edges[btMin(a,b)*nv+btMax(a,b)];
// 			if(!e)
// 			{
// 				sc->m_edges.push_back(ShapeCache::Edge());
// 				e=&sc->m_edges[sc->m_edges.size()-1];
// 				e->n[0]=nrm;e->n[1]=-nrm;
// 				e->v[0]=a;e->v[1]=b;
// 			}
// 			else
// 			{
// 				e->n[1]=nrm;
// 			}
// 		}
// 	}
// 	return(sc);
// }

void Displaylists::generateList()
{
// 	cerr << "generating displaylists" << endl;
	glDeleteLists(displayLists, 2);
	displayLists = glGenLists(2);

// 	// 0 = cube with the bottom missing
// 	glNewList(displayLists,GL_COMPILE);
// 
// 		glBegin(GL_QUADS);
// 			//Quad 1
// 			glVertex3f( 1.0f, 1.0f, 1.0f);   //V2
// 			glVertex3f( 1.0f,-1.0f, 1.0f);   //V1
// 			glVertex3f( 1.0f,-1.0f,-1.0f);   //V3
// 			glVertex3f( 1.0f, 1.0f,-1.0f);   //V4
// 			//Quad 2
// 			glVertex3f( 1.0f, 1.0f,-1.0f);   //V4
// 			glVertex3f( 1.0f,-1.0f,-1.0f);   //V3
// 			glVertex3f(-1.0f,-1.0f,-1.0f);   //V5
// 			glVertex3f(-1.0f, 1.0f,-1.0f);   //V6
// 			//Quad 3
// 			glVertex3f(-1.0f, 1.0f,-1.0f);   //V6
// 			glVertex3f(-1.0f,-1.0f,-1.0f);   //V5
// 			glVertex3f(-1.0f,-1.0f, 1.0f);   //V7
// 			glVertex3f(-1.0f, 1.0f, 1.0f);   //V8
// 			//Quad 4
// 			glVertex3f(-1.0f, 1.0f, 1.0f);   //V8
// 			glVertex3f(-1.0f,-1.0f, 1.0f);   //V7
// 			glVertex3f( 1.0f,-1.0f, 1.0f);   //V1
// 			glVertex3f( 1.0f, 1.0f, 1.0f);   //V2
// 			//Quad 5
// 			glVertex3f(-1.0f, 1.0f,-1.0f);   //V6
// 			glVertex3f(-1.0f, 1.0f, 1.0f);   //V8
// 			glVertex3f( 1.0f, 1.0f, 1.0f);   //V2
// 			glVertex3f( 1.0f, 1.0f,-1.0f);   //V4
// 			//Quad 6
// 			glVertex3f( 1.0f,-1.0f,-1.0f);   //V4
// 			glVertex3f( 1.0f,-1.0f, 1.0f);   //V2
// 			glVertex3f(-1.0f,-1.0f, 1.0f);   //V8
// 			glVertex3f(-1.0f,-1.0f,-1.0f);   //V6
// 		glEnd();
// 
// 	glEndList();

	glNewList(displayLists,GL_COMPILE);

		glBegin(GL_QUADS);
			//Quad 1
			glNormal3f( 1.0f, 0.0f, 0.0f);
			glVertex3f( 1.0f, 1.0f, 1.0f);   //V2
			glVertex3f( 1.0f,-1.0f, 1.0f);   //V1
			glVertex3f( 1.0f,-1.0f,-1.0f);   //V3
			glVertex3f( 1.0f, 1.0f,-1.0f);   //V4
			//Quad 2
			glNormal3f( 0.0f, 0.0f,-1.0f);
			glVertex3f( 1.0f, 1.0f,-1.0f);   //V4
			glVertex3f( 1.0f,-1.0f,-1.0f);   //V3
			glVertex3f(-1.0f,-1.0f,-1.0f);   //V5
			glVertex3f(-1.0f, 1.0f,-1.0f);   //V6
			//Quad 3
			glNormal3f(-1.0f, 0.0f, 0.0f);
			glVertex3f(-1.0f, 1.0f,-1.0f);   //V6
			glVertex3f(-1.0f,-1.0f,-1.0f);   //V5
			glVertex3f(-1.0f,-1.0f, 1.0f);   //V7
			glVertex3f(-1.0f, 1.0f, 1.0f);   //V8
			//Quad 4
			glNormal3f( 0.0f, 0.0f, 1.0f);
			glVertex3f(-1.0f, 1.0f, 1.0f);   //V8
			glVertex3f(-1.0f,-1.0f, 1.0f);   //V7
			glVertex3f( 1.0f,-1.0f, 1.0f);   //V1
			glVertex3f( 1.0f, 1.0f, 1.0f);   //V2
			//Quad 5
			glNormal3f( 0.0f, 1.0f, 0.0f);
			glVertex3f(-1.0f, 1.0f,-1.0f);   //V6
			glVertex3f(-1.0f, 1.0f, 1.0f);   //V8
			glVertex3f( 1.0f, 1.0f, 1.0f);   //V2
			glVertex3f( 1.0f, 1.0f,-1.0f);   //V4
			//Quad 6
			glNormal3f(0.0f, -1.0f, 0.0f);
			glVertex3f( 1.0f,-1.0f,-1.0f);   //V4
			glVertex3f( 1.0f,-1.0f, 1.0f);   //V2
			glVertex3f(-1.0f,-1.0f, 1.0f);   //V8
			glVertex3f(-1.0f,-1.0f,-1.0f);   //V6
		glEnd();

	glEndList();

/*	// 1 = floor
	glNewList(displayLists+2,GL_COMPILE);

		glColor4f( 0.0f, 0.0f, 1.0f, 0.0f );
		glBegin(GL_QUADS);
			glVertex3f( 0.0f, 0.0f, 0.0f);
			glVertex3f( 0.0f, 0.0f, 1.0f);
			glVertex3f( 1.0f, 0.0f, 1.0f);
			glVertex3f( 1.0f, 0.0f, 0.0f);
		glEnd();

	glEndList();*/
}
