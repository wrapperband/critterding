#include "concavefloor.h"
#ifdef HAVE_OPENMP
	#include <omp.h>
#endif

Concavefloor::Concavefloor(  boost::shared_ptr<BeGraphicsSystem> system, boost::shared_ptr<Textverbosemessage> textverbosemessage )
 : WorldB( system, textverbosemessage )
{
	gVertices = 0;
	gIndices = 0;
	trimeshShape = 0;
	m_indexVertexArrays = 0;
	fixedGround = 0;
	serializer = 0;

	aabbMin = btVector3(-1000,-1000,-1000);
	aabbMax = btVector3(1000,1000,1000);

	serializer = new btDefaultSerializer(1024*1024*5);
// 		serializer->setSerializationFlags(BT_SERIALIZE_NO_BVH);//	or BT_SERIALIZE_NO_TRIANGLEINFOMAP
}

void Concavefloor::init()
{
	worldsizeZ = settings->getCVarPtr("worldsizeZ");

	makeFloor();

	if ( settings->getCVar("autoload") )
		loadAllCritters();
	if ( settings->getCVar("autoloadlastsaved") )
		loadAllLastSavedCritters();
}

// void Concavefloor::process(const BeTimer& timer)
// {
// 	if ( !pause )
// 	{
// 		// recalc freeEnergy
// 			calcFreeEnergy();
// 		
// 		#ifdef HAVE_OPENCL
// // 			nbody.process();
// 		#endif
// 	
// 		// kill half?
// 			killHalforDouble();
// 
// 		// Remove food
// 			expireFood();
// 
// 		// Autoinsert Food
// 			autoinsertFood();
// 
// 		// remove all dead critters
// 			expireCritters();
// 
// 		// Autosave Critters?
// 			autosaveCritters(timer);
// 
// 		// Autoexchange Critters?
// 			autoexchangeCritters(timer);
// 
// 		// Autoinsert Critters?
// 			autoinsertCritters();
// 
// 		if ( *critter_raycastvision == 0 )
// 		{
// 			renderVision();
// 			grabVision();
// 		}
// 
// 
// 		m_dynamicsWorld->stepSimulation(0.016667f, 2, 0.00833334f);
// 			
// 			
// 		// process all critters
// 		unsigned int lmax = critters.size();
// 		unsigned int i;
// 		float freeEnergyc = 0.0f;
// 
// 		for( i=0; i < lmax; ++i)
// 			checkCollisions( critters[i] );
// 		
// #ifdef HAVE_OPENMP
// 		std::cout << "found" << std::endl;
// 	// process
// 		if ( m_threads_last != *threads )
// 		{
// 			m_threads_last = *threads;
// 			omp_set_num_threads(m_threads_last);
// 		}
// 
// 		if ( m_threads_last > 1 )
// 		{
// // 			unsigned int t_lmax = lmax;
// 			#pragma omp parallel for private(i)
// 			for( i=0; i < lmax; ++i)
// 			{
// 				critters[i]->process();
// // 				freeEnergyc += c->energyUsed;
// 			}
// 		}
// 		else
// #endif
// 		{
// 			for( i=0; i < lmax; ++i)
// 				critters[i]->process();
// 		}
// 
// 		for( i=0; i < lmax; ++i)
// 		{
// 			CritterB* c = critters[i];
// 
// 			// record critter used energy
// 				freeEnergyc += c->energyUsed;
// 
// 			// process Output Neurons
// 				eat(c);
// 
// 			// procreation if procreation energy trigger is hit
// 				procreate(c);
// 				
// 				c->beingTouched = false;
// 				c->beingEaten   = false;
// 		}
// 		
// 		m_freeEnergy += freeEnergyc;
// 
// 		getGeneralStats();
// 	}
// }

void Concavefloor::setVertexPositions(float waveheight, float offset)
{
	for ( int i=0;i<NUM_VERTS_X;++i )
		for ( int j=0;j<NUM_VERTS_Y;++j )
			gVertices[ i+j*NUM_VERTS_X ].setValue( (i-NUM_VERTS_X*0.5f) * TRIANGLE_SIZE, waveheight*sinf((float)i+offset)*cosf((float)j+offset), (j-NUM_VERTS_Y*0.5f)*TRIANGLE_SIZE );
}

void Concavefloor::makeFloor()
{
// 	for ( unsigned int i=0; i < walls.size(); ++i )	
// 		delete walls[i];
// 	walls.clear();
	
	if ( trimeshShape > 0 )
		delete trimeshShape;

	if ( fixedGround > 0 )
	{
		m_dynamicsWorld->removeRigidBody(fixedGround);
		delete fixedGround;
	}

	if ( m_indexVertexArrays > 0 )
		delete m_indexVertexArrays;

	if ( gVertices > 0 )
		delete gVertices;

	if ( gIndices > 0 )
		delete gIndices;
	
	
	
	for ( int i=0; i < (int)entities.size(); ++i )
	{
		if ( entities[i]->type == WALL )
		{
			delete entities[i];
			entities.erase(entities.begin()+i);
			i--;
		}
	}

 
	// Wall Constants
		float WallWidth = 5.f;
		float WallHalfWidth = WallWidth/2.0f;
		float WallHeight = 5.0f;
		float WallHalfHeight = WallHeight/2.0f;

		border = 1;
		waveheight = (float)*worldsizeZ / 100; // 5.0f
		TRIANGLE_SIZE = 1.0f; // 8.f
// 		NUM_VERTS_X = *worldsizeX/3 + 1 + border*2;  // 30
// 		NUM_VERTS_Y = *worldsizeY/3 + 1 + border*2;  // 30
		NUM_VERTS_X = *worldsizeX + 1;  // 30
		NUM_VERTS_Y = *worldsizeY + 1;  // 30
		totalVerts = NUM_VERTS_X*NUM_VERTS_Y;

	// Ground Floor
// 		btVector3 position( *worldsizeX/2.0f, -WallHalfWidth - waveheight, *worldsizeY/2.0f );
// 		Wall* w = new Wall( *worldsizeX, WallWidth, *worldsizeY, position, m_dynamicsWorld );
// 		w->color.r = 0.30f; w->color.g = 0.20f; w->color.b = 0.10f;
// 		walls.push_back(w);
	
	if ( settings->getCVar("worldwalls") )
	{
		// Left Wall
		{
			const btVector3 position( 0.0f-WallHalfWidth, WallHalfHeight-WallWidth, *worldsizeY/2.0f );
			btTransform t;
			t.setIdentity();
			t.setOrigin(position);
			Wall* w = new Wall( WallWidth, WallHeight, *worldsizeY, t, m_dynamicsWorld );
			w->color.setR(0.34f);
			w->color.setG(0.25f);
			w->color.setB(0.11f);
			entities.push_back(w);
		}
		// Right Wall
		{
			const btVector3 position( *worldsizeX+WallHalfWidth, WallHalfHeight-WallWidth, *worldsizeY/2.0f );
			btTransform t;
			t.setIdentity();
			t.setOrigin(position);
			Wall* w = new Wall( WallWidth, WallHeight, *worldsizeY, t, m_dynamicsWorld );
			w->color.setR(0.34f);
			w->color.setG(0.25f);
			w->color.setB(0.11f);
			entities.push_back(w);
		}
		// Top Wall
		{
			const btVector3 position( *worldsizeX/2.0f, WallHalfHeight-WallWidth, 0.0f-WallHalfWidth );
			btTransform t;
			t.setIdentity();
			t.setOrigin(position);
			Wall* w = new Wall( *worldsizeX+(WallWidth*2), WallHeight, WallWidth, t, m_dynamicsWorld );
			w->color.setR(0.34f);
			w->color.setG(0.25f);
			w->color.setB(0.11f);
			entities.push_back(w);
		}
		// Bottom Wall
		{
			const btVector3 position( *worldsizeX/2.0f, WallHalfHeight-WallWidth, *worldsizeY+WallHalfWidth );
			btTransform t;
			t.setIdentity();
			t.setOrigin(position);
			Wall* w = new Wall( *worldsizeX+(WallWidth*2), WallHeight, WallWidth, t, m_dynamicsWorld );
			w->color.setR(0.34f);
			w->color.setG(0.25f);
			w->color.setB(0.11f);
			entities.push_back(w);
		}
	}

	int vertStride = sizeof(btVector3);
	int indexStride = 3*sizeof(int);
	const int totalTriangles = 2*(NUM_VERTS_X-1)*(NUM_VERTS_Y-1);
	gVertices = new btVector3[totalVerts];
	gIndices = new int[totalTriangles*3];
	setVertexPositions(waveheight,0.f);

	int index=0;
	for ( int i=0;i<NUM_VERTS_X-1;++i )
	{
		for ( int j=0;j<NUM_VERTS_Y-1;++j )
		{
			gIndices[index++] = j*NUM_VERTS_X+i;
			gIndices[index++] = j*NUM_VERTS_X+i+1;
			gIndices[index++] = (j+1)*NUM_VERTS_X+i+1;

			gIndices[index++] = j*NUM_VERTS_X+i;
			gIndices[index++] = (j+1)*NUM_VERTS_X+i+1;
			gIndices[index++] = (j+1)*NUM_VERTS_X+i;
		}
	}

	m_indexVertexArrays = new btTriangleIndexVertexArray(totalTriangles, gIndices, indexStride, totalVerts,(btScalar*) &gVertices[0].x(),vertStride);
	trimeshShape = new btBvhTriangleMeshShape(m_indexVertexArrays,true,aabbMin,aabbMax);

	serializer->startSerialization();
//		trimeshShape->serializeSingleShape(serializer);
	trimeshShape->serializeSingleBvh(serializer);
	serializer->finishSerialization();
	
	groundTransform.setIdentity();
	groundTransform.setOrigin( btVector3((float)*worldsizeX/2 + 0.5f, 0, (float)*worldsizeY/2 + 0.5f) );

	fixedGround = new btRigidBody(0,0,trimeshShape,btVector3(0,0,0));	
	fixedGround->setWorldTransform(groundTransform);
	m_dynamicsWorld->addRigidBody(fixedGround);
	
	activateFood();
}


void Concavefloor::drawWithGrid()
{
	glClear(GL_STENCIL_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

 	drawfloor(1.0f);
	for( unsigned int i=0; i < critters.size(); ++i)
		critters[i]->draw(true);

	for( unsigned int i=0; i < entities.size(); ++i)
		entities[i]->draw();

	glDisable(GL_STENCIL_TEST);
	glDisable(GL_CULL_FACE);
}

// void Concavefloor::childPositionOffset(btVector3* v)
// {
// 	*v+= (v->normalized()*insertHight);
// }

// btVector3 Concavefloor::findPosition()
// {
// 	return btVector3(	((float)randgen->Instance()->get( 0, 200 )-100.0f) / 100,
// 				((float)randgen->Instance()->get( 0, 200 )-100.0f) / 100,
// 				((float)randgen->Instance()->get( 0, 200 )-100.0f) / 100
// 	).normalized() * ( *worldsizeX + insertHight);
// }

void Concavefloor::drawfloor(float dim)
{
	glPushMatrix(); 
		glTranslatef((float)*worldsizeX/2 + 0.5f, 0, (float)*worldsizeY/2 + 0.5f);
		glColor4f( 0.3f * dim, 0.2f * dim, 0.1f * dim, 0.0f );
		trimeshShape->processAllTriangles(&drawCallback,aabbMin,aabbMax);
	glPopMatrix(); 
// 	for( unsigned int i=0; i < walls.size(); ++i)
// 		walls[i]->draw();
}

void Concavefloor::drawfloor()
{
	glPushMatrix(); 
		glTranslatef((float)*worldsizeX/2 + 0.5f, 0, (float)*worldsizeY/2 + 0.5f);
		glColor4f( 0.3f, 0.2f, 0.1f, 0.0f );
		trimeshShape->processAllTriangles(&drawCallback,aabbMin,aabbMax);
	glPopMatrix(); 
	for( unsigned int i=0; i < entities.size(); ++i)
		if ( entities[i]->type == WALL )
			entities[i]->draw();
// 	for( unsigned int i=0; i < walls.size(); ++i)
// 		walls[i]->draw();
}

Concavefloor::~Concavefloor()
{
	m_dynamicsWorld->removeRigidBody(fixedGround);
	delete fixedGround;
	delete m_indexVertexArrays;
	delete gVertices;
	delete gIndices;
	delete trimeshShape;
	delete serializer;
}
