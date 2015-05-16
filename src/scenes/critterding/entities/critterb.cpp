// #include <gl\glext.h>
#include "critterb.h"

#include "genotypes.h"

using namespace std;

void CritterB::initConst()
{
	// settings and pointers
	settings = Settings::Instance();
	genotypes = Genotypes::Instance();
// 	retinasperrow = settings->getCVarPtr("retinasperrow");
	colormode = settings->getCVarPtr("colormode");
// 	critter_maxlifetime = settings->getCVarPtr("critter_maxlifetime");
	critter_maxenergy = settings->getCVarPtr("critter_maxenergy");
// 	critter_sightrange = settings->getCVarPtr("critter_sightrange");
	critter_procinterval = settings->getCVarPtr("critter_procinterval");
	critter_minenergyproc = settings->getCVarPtr("critter_minenergyproc");
// 	critter_raycastvision = settings->getCVarPtr("critter_raycastvision");

	brain_costhavingneuron = settings->getCVarPtr("brain_costhavingneuron");
	brain_costfiringneuron = settings->getCVarPtr("brain_costfiringneuron");
	brain_costfiringmotorneuron = settings->getCVarPtr("brain_costfiringmotorneuron");
	brain_costhavingsynapse = settings->getCVarPtr("brain_costhavingsynapse");
	body_costhavingbodypart = settings->getCVarPtr("body_costhavingbodypart");
	body_costtotalweight = settings->getCVarPtr("body_costtotalweight");

	setting_critter_mutate_minenergyproc = settings->getCVarPtr("critter_mutate_minenergyproc");

	type = CRITTER;
	isPicked		= false;

	totalFrames		= 0;
	procreateTimeCount	= 0;

// 	sightrange = settings->critter_sightrange;
// 	colorTrim		= 0.15f;

	eat			= false;
	procreate		= false;

	// raycast
// 	raycast = new Raycast(btDynWorld);
	
	
	canProcreate = false;
	touchingFood = false;
	touchingCritter = false;
	beingTouched = false;
	beingEaten = false;
	touchedEntity = 0;

	// renderConsts

	m_render_offset.setIdentity();
	m_render_offset.getBasis().setEulerZYX( 0.0f, SIMD_HALF_PI, 0.0f );

}

CritterB::CritterB(btDynamicsWorld* btWorld, long unsigned int id, const btTransform& startPos, unsigned char* retinap)
 : retinasperrow(119)
 , m_zNear(0.1f)
 , m_fovy(SIMD_HALF_PI/2)
 , m_frustumHalf(tan( m_fovy * 180.0f / M_PI * M_PI / 360.0f ) * m_zNear)
 , fitness_index(0.0f)
{
	loadError = false;

	// first things first
	btDynWorld						= btWorld;
	retina							= retinap;
	critterID						= id;

	initConst();

	energyLevel						= 1.0f*settings->getCVar("critter_startenergy");

// 	lifetime						= settings->critter_maxlifetime;

	genotype = genotypes->newg( settings );

	// BODY
// 	genotype->bodyArch->buildArch();
	body.wireArch( genotype->bodyArch, (void*)this, btDynWorld, startPos );
	for( unsigned int j=0; j < body.bodyparts.size(); j++)
		body.bodyparts[j]->body->setActivationState(DISABLE_DEACTIVATION);

	// LINK
	registerBrainInputOutputs();
	
	// BRAIN
// 	genotype->brainzArch->buildArch();
	brain.wireArch( genotype->brainzArch );
}

CritterB::CritterB(CritterB& other, long unsigned int id, const btTransform& startPos, bool brainmutant, bool bodymutant)
 : retinasperrow(119)
 , 	m_zNear(0.1f)
 , 	m_fovy(SIMD_HALF_PI/2)
 , 	m_frustumHalf(tan( m_fovy * 180.0f / M_PI * M_PI / 360.0f ) * m_zNear)
 , fitness_index(0.0f)
{
	loadError = false;

	// first things first
	btDynWorld					= other.btDynWorld;
	retina						= other.retina;
	critterID					= id;

	initConst();

// 	lifetime					= other.lifetime;

	genotype = genotypes->copy(other.genotype, brainmutant, BeRand::Instance()->get(1, other.genotype->brainzArch->m_maxmutations), bodymutant, BeRand::Instance()->get(1, other.genotype->bodyArch->m_maxmutations));

	body.wireArch( genotype->bodyArch, (void*)this, btDynWorld, startPos );
	for( unsigned int j=0; j < body.bodyparts.size(); j++)
		body.bodyparts[j]->body->setActivationState(DISABLE_DEACTIVATION);

	// LINK
	registerBrainInputOutputs();

	brain.wireArch( genotype->brainzArch );
}

CritterB::CritterB(string &critterstring, btDynamicsWorld* btWorld, const btTransform& startPos, unsigned char* retinap)
 : retinasperrow(119)
 , 	m_zNear(0.1f)
 , 	m_fovy(SIMD_HALF_PI/2)
 , 	m_frustumHalf(tan( m_fovy * 180.0f / M_PI * M_PI / 360.0f ) * m_zNear)
 , fitness_index(0.0f)
{
	loadError = false;
	// critterID is arranged in world, definite critter insertion is not determined yet

	// first things first
	btDynWorld		= btWorld;
	retina			= retinap;

	initConst();

	energyLevel		= 1.0f*settings->getCVar("critter_startenergy");

	genotype = genotypes->loadGenotype(critterstring);

	if ( genotype->bodyArch->retinasize != (unsigned int)settings->getCVar("critter_retinasize"))
	{
		stringstream buf;
		buf << "ERROR: critter retinasize (" << genotype->bodyArch->retinasize << ") doesn't fit world retinasize (" << settings->getCVar("critter_retinasize") << ")";
// 		Logbuffer::Instance()->add(buf);

		cerr << "!ERROR: critter retinasize (" << genotype->bodyArch->retinasize << ") doesn't fit world retinasize (" << settings->getCVar("critter_retinasize") << ")" << endl;
		loadError = true;
	}
	else
	{
		// BODY
		body.wireArch( genotype->bodyArch, (void*)this, btDynWorld, startPos );
		for( unsigned int j=0; j < body.bodyparts.size(); j++)
			body.bodyparts[j]->body->setActivationState(DISABLE_DEACTIVATION);

		// LINK
		registerBrainInputOutputs();

		// BRAIN
		brain.wireArch( genotype->brainzArch );
	}
}

void CritterB::registerBrainInputOutputs()
{
// 	cerr << "attaching INPUTS" << endl;
	for ( unsigned int i=0; i < genotype->brainzArch->InputIDs.size(); ++i )
	{
		brain.registerInput( genotype->brainzArch->InputIDs[i] );
	}

// 	cerr << "attaching OUTPUTS" << endl;

	for ( unsigned int i=0; i < body.constraints.size(); ++i )
	{
		for ( unsigned int j=0; j < body.constraints[i]->Inputs.size(); j++ )
		{
			brain.registerOutput( body.constraints[i]->Inputs[j], genotype->brainzArch->OutputIDs[ (i*2)+j ] );
		}
	}

	// eat
		brain.registerOutput( &eat, 100000 );

	// procreate
		brain.registerOutput( &procreate, 100001 );
		
	// debug check
	if ( brain.Outputs.size() != genotype->brainzArch->OutputIDs.size() )
		cerr << "WARNING: brain.Outputs.size() != genotype->brainzArch->OutputIDs.size()" << endl;
}

// void CritterB::draw(bool drawFaces)
// {
// 				std::cout << "0" << std::endl;
// 	for( unsigned int j=0; j < body.bodyparts.size(); j++)
// 	{
// 		body.bodyparts[j]->myMotionState->m_graphicsWorldTrans.getOpenGLMatrix(position);
// 
// 		glPushMatrix(); 
// // 		glMultMatrixf(position);
// // 		glMultMatrixd(position); // FIXME WATCHIT OK WENT TO DOUBLE HERE 
// 		Displaylists::Instance()->glMultiMatrix(position);
// 
// 			if ( *colormode == 1 )
// 				glColor4f( genotype->speciescolor.r(), genotype->speciescolor.g(), genotype->speciescolor.b(), genotype->speciescolor.a() );
// 			else
// 				glColor4f( genotype->bodyArch->color.r(), genotype->bodyArch->color.g(), genotype->bodyArch->color.b(), genotype->bodyArch->color.a() );
// 
// 				const btBoxShape* boxShape = static_cast<const btBoxShape*>(body.bodyparts[j]->shape);
// 				const btVector3& halfExtent = boxShape->getHalfExtentsWithMargin();
// 				glScalef(halfExtent[0], halfExtent[1], halfExtent[2]);
// 
// 				Displaylists::Instance()->call();
// 
// 		glPopMatrix();
// 	}
// 	if ( drawFaces )
// 	{
// 		for( unsigned int j=0; j < body.mouths.size(); j++)
// 		{
// 			body.mouths[j]->ghostObject->getWorldTransform().getOpenGLMatrix(position);
// 			glPushMatrix(); 
// // 			glMultMatrixf(position);
// // 			glMultMatrixd(position); // FIXME WATCHIT OK WENT TO DOUBLE HERE 
// 			Displaylists::Instance()->glMultiMatrix(position);
// 
// 					glColor4f( 1.0f, 0.0f, 0.0f, 0.0f );
// 
// 					const btBoxShape* boxShape = static_cast<const btBoxShape*>(body.mouths[j]->shape);
// 					const btVector3& halfExtent = boxShape->getHalfExtentsWithMargin();
// 					glScalef(halfExtent[0], halfExtent[1], halfExtent[2]);
// 
// 					Displaylists::Instance()->call();
// 
// 			glPopMatrix();
// 		}
// 	}
// }

// void CritterB::draw( const bool do_color, const bool do_scale )
// {
// 	if ( m_model )
// 	{
// 				std::cout << "1" << std::endl;
// 		for( unsigned int j=0; j < body.bodyparts.size(); j++)
// 		{
// // 			body.bodyparts[j]->myMotionState->m_graphicsWorldTrans.getOpenGLMatrix(position);
// 			glPushMatrix(); 
// // 				Displaylists::Instance()->glMultiMatrix(position);
// 
// 				if ( do_color )
// 				{
// 					if ( *colormode == 1 )
// 						glColor4f( genotype->speciescolor.r(), genotype->speciescolor.g(), genotype->speciescolor.b(), genotype->speciescolor.a() );
// 					else
// 						glColor4f( genotype->bodyArch->color.r(), genotype->bodyArch->color.g(), genotype->bodyArch->color.b(), genotype->bodyArch->color.a() );
// 	// 				glColor4f( color.r(), color.g(), color.b(), color.a() );
// 					m_model->get()->setContext( 0 );
// 				}
// 				
// 				if ( do_scale )
// 				{
// 					const btBoxShape* boxShape = static_cast<const btBoxShape*>(body.bodyparts[j]->shape);
// 					const btVector3& halfExtent = boxShape->getHalfExtentsWithMargin();
// 					glScalef(halfExtent[0], halfExtent[1], halfExtent[2]);
// 				}
// 				
// 				m_model->get()->draw( 0, body.bodyparts[j]->myMotionState->m_graphicsWorldTrans );
// 			glPopMatrix(); 
// 		}
// 	}
// 	else
// 	{
// 				std::cout << "2" << std::endl;
// 		for( unsigned int j=0; j < body.bodyparts.size(); j++)
// 		{
// 			body.bodyparts[j]->myMotionState->m_graphicsWorldTrans.getOpenGLMatrix(position);
// 			glPushMatrix(); 
// 				Displaylists::Instance()->glMultiMatrix(position);
// 
// 				if ( do_color )
// 				{
// 					if ( *colormode == 1 )
// 						glColor4f( genotype->speciescolor.r(), genotype->speciescolor.g(), genotype->speciescolor.b(), genotype->speciescolor.a() );
// 					else
// 						glColor4f( genotype->bodyArch->color.r(), genotype->bodyArch->color.g(), genotype->bodyArch->color.b(), genotype->bodyArch->color.a() );
// 	// 				glColor4f( color.r(), color.g(), color.b(), color.a() );
// 				}
// 				
// 				if ( do_scale )
// 				{
// 					const btBoxShape* boxShape = static_cast<const btBoxShape*>(body.bodyparts[j]->shape);
// 					const btVector3& halfExtent = boxShape->getHalfExtentsWithMargin();
// 					glScalef(halfExtent[0], halfExtent[1], halfExtent[2]);
// 				}
// 				
// 				Displaylists::Instance()->call();
// 			glPopMatrix(); 
// 		}
// 	}
// }

void CritterB::process()
{
	// increase counters
		totalFrames++;
		procreateTimeCount++;

		eaten = false;

	// reset motor bools
		movementsmade	= 0;
// 		eat		= false;
// 		procreate	= false;
// 		carrydrop	= false;

	// SENSORS
		procInputNeurons();

	// INTERS
		brain.process();

	// MOTORS
// 		procOutputNeurons();

	// calc used energy, energyUsed is used in world aswell, don't remove

// 		energyUsed = ( (float)brain.totalNeurons + (float)brain.neuronsFired + (2.0f*(float)motorneuronsfired) + ((float)brain.totalSynapses/10.0f) ) / 200.0f;

 		energyUsed =  brain.totalNeurons		* *brain_costhavingneuron;
 		energyUsed += brain.neuronsFired		* *brain_costfiringneuron;
 		energyUsed += brain.motorneuronsFired	* *brain_costfiringmotorneuron;
 		energyUsed += brain.totalSynapses		* *brain_costhavingsynapse;
 		energyUsed += body.totalBodyparts()		* *body_costhavingbodypart;
 		energyUsed += body.totalWeight			* *body_costtotalweight;
		
// 		std::cout << "weight : " << body.totalWeight << std::endl;
		
		energyUsed = energyUsed / 1000000;
// 		cerr << energyUsed << endl;

	// apply energy usage
	energyLevel -= energyUsed;

	// move
		// motorate all constraints
		for ( auto it(body.constraints.begin()); it != body.constraints.end(); ++it )
			(*it)->motorate();

	// move ghostobject to mouth object position
		for ( auto it(body.mouths.begin()); it != body.mouths.end(); ++it )
			(*it)->updateGhostObjectPosition();
}

void CritterB::procInputNeurons()
{
	// clear all inputs

	brain.clearInputs();

	unsigned int overstep = 0;

	// touching food
		if ( touchingFood )	brain.Inputs[overstep++].output = 1;
		else			brain.Inputs[overstep++].output = 0;

	// touching critter
		if ( touchingCritter )	brain.Inputs[overstep++].output = 1;
		else			brain.Inputs[overstep++].output = 0;

	// can procreate sensor neuron
		canProcreate	= false;
		
		unsigned int actual( *critter_minenergyproc );
		if ( *setting_critter_mutate_minenergyproc > 0 )
			actual = genotype->bodyArch->m_minenergyproc;
		
		if ( procreateTimeCount > (unsigned int)*critter_procinterval && energyLevel > actual )
		{
			brain.Inputs[overstep++].output = 1;
			canProcreate = true;
		}
		else brain.Inputs[overstep++].output = 0;

	// energy neurons
	{
		const unsigned int NeuronToFire((int)((energyLevel / (*critter_maxenergy+1)) * 10) + overstep);
		const unsigned int count(10 + overstep);
		while ( overstep < count )
		{
			if ( overstep <= NeuronToFire )	brain.Inputs[overstep++].output = 1; // !!! <=
			else 				brain.Inputs[overstep++].output = 0;
		}
	}
	// age neurons
	{
		const unsigned int NeuronToFire((int)(((float)totalFrames / (genotype->bodyArch->m_maxlifetime+1)) * 10) + overstep);
		const unsigned int count(10 + overstep);
		while ( overstep < count )
		{
			if ( overstep <= NeuronToFire )	brain.Inputs[overstep++].output = 1; // !!! <=
			else 				brain.Inputs[overstep++].output = 0;
		}
	}

	// Vision
// 		unsigned int vstart = overstep;

// 		if ( *critter_raycastvision )
// 		{
// 			const btVector3& head_origin(body.mouths[0]->ghostObject->getWorldTransform().getOrigin());
// 			
// 			for ( int y = genotype->bodyArch->retinasize-1; y >= 0; --y )
// 				for ( unsigned int x = 0; x < genotype->bodyArch->retinasize; ++x )
// 				{
// // 					cerr << "starting" << endl;
// 					mouseRay = raycast->cast( head_origin, getScreenDirection(x+1, y) );
// // 					cerr << "e1" << endl;
// 					if ( mouseRay.m_hit )
// 					{
// 						Entity* e = static_cast<Entity*>(mouseRay.m_hitBody->getUserPointer());
// 						if ( e->type == 0 )
// 						{
// 							CritterB* c = static_cast<CritterB*>(e);
// 							brain.Inputs[overstep++].output = c->genotype->bodyArch->color.r();
// 							brain.Inputs[overstep++].output = c->genotype->bodyArch->color.g();
// 							brain.Inputs[overstep++].output = c->genotype->bodyArch->color.b();
// 							brain.Inputs[overstep++].output = c->genotype->bodyArch->color.a();
// 						}
// 						else
// 						{
// 							brain.Inputs[overstep++].output = e->color.r();
// 							brain.Inputs[overstep++].output = e->color.g();
// 							brain.Inputs[overstep++].output = e->color.b();
// 							brain.Inputs[overstep++].output = e->color.a();
// 						}
// 					}
// 					else
// 					{
// 						brain.Inputs[overstep++].output = 0.0f;
// 						brain.Inputs[overstep++].output = 0.0f;
// 						brain.Inputs[overstep++].output = 0.0f;
// 						brain.Inputs[overstep++].output = 0.0f;
// 					}
// // 					cerr << "e2" << endl;
// 				}
// 		}
// 		else
		{
			const unsigned int val1(retinaColumnStart+((genotype->bodyArch->retinasize)*4));
			const unsigned int val3(retinaRowStart+(genotype->bodyArch->retinasize*retinaRowLength));
			
			for ( unsigned int h(retinaRowStart); h < val3; h += retinaRowLength )
			{
				const unsigned int val2(h+val1);
				for ( unsigned int w(h+retinaColumnStart); w < val2; ++w )
				{
					brain.Inputs[overstep++].output = 0.00390625f * retina[w];
				}
			}
		}

// 		for ( unsigned int x = 0; x < retinasize; ++x )
// 		{
// 			for ( unsigned int y = vstart+(retinasize*4*x); y < vstart+(retinasize*4*x)+(retinasize*4); y+=4 )
// 			{
// // 				cerr << " now = " << y << endl;
// 				if ( brain.Inputs[y].output ) cerr << "\033[1;34mB\033[0m";
// 				else cerr << ".";
// 				if ( brain.Inputs[y+1].output ) cerr << "\033[1;32mG\033[0m";
// 				else cerr << ".";
// 				if ( brain.Inputs[y+2].output ) cerr << "\033[1;31mR\033[0m";
// 				else cerr << ".";
// 				if ( brain.Inputs[y+3].output ) cerr << "\033[1;35mA\033[0m";
// 				else cerr << ".";
// 			}
// 			cerr << endl;
// 		}
// 		cerr << endl;
// 
// 		for ( unsigned int h=retinaRowStart; h < retinaRowStart+(retinasize*retinaRowLength); h += retinaRowLength )
// 		{
// 			for ( unsigned int w=h+retinaColumnStart; w < h+retinaColumnStart+((retinasize)*4); w+=4 )
// 			{
// 				if ( (int)retina[w] ) cerr << "\033[1;34mB\033[0m";
// 				else cerr << ".";
// 				if ( (int)retina[w+1] ) cerr << "\033[1;32mG\033[0m";
// 				else cerr << ".";
// 				if ( (int)retina[w+2] ) cerr << "\033[1;31mR\033[0m";
// 				else cerr << ".";
// 				if ( (int)retina[w+3] ) cerr << "\033[1;35mA\033[0m";
// 				else cerr << ".";
// 			}
// 			cerr << endl;
// 		}
// 		cerr << endl;

	// constraint angle neurons
		const auto& size(body.constraints.size());
		for ( unsigned int i(0); i < size; ++i )
			brain.Inputs[overstep++].output = body.constraints[i]->getAngle();

	// being touched
		if ( beingTouched )
			brain.Inputs[overstep++].output = 1;
		else
			brain.Inputs[overstep++].output = 0;

	// being eaten
		if ( beingEaten )
			brain.Inputs[overstep++].output = 1;
		else
			brain.Inputs[overstep++].output = 0;




	// debugging check
		if ( overstep-1 != brain.numberOfInputs-1 )
		{
			cerr << overstep << " does not equal " << brain.numberOfInputs-1 << endl;
			exit(0);
		}
}

btVector3 CritterB::getScreenDirection(const int& x, const int& y)
{
	float directionlength = 1000000.f;

	btTransform tr = body.mouths[0]->ghostObject->getWorldTransform();
	btVector3 forwardRay( 
		-tr.getBasis()[0][2], 
		-tr.getBasis()[1][2], 
		-tr.getBasis()[2][2]); 
	forwardRay *=  directionlength;

	btVector3 upRay( 
		tr.getBasis()[0][1], 
		tr.getBasis()[1][1], 
		tr.getBasis()[2][1]); 

	btVector3 hor = forwardRay.cross(upRay);
	hor.normalize();
	hor *= directionlength;

	upRay = hor.cross(forwardRay);
	upRay.normalize();
	upRay *= directionlength;

	btVector3 rayTo = (tr.getOrigin() + forwardRay) - (0.5f * hor) + (0.5f * upRay);
	rayTo += x * (hor * (1.0f/(genotype->bodyArch->retinasize)));
	rayTo -= y * (upRay * (1.0f/(genotype->bodyArch->retinasize)));

	return rayTo;
}

// Vision Stuff
	void CritterB::place(const GLint v_inv)
	{
/*		// FBO render pass
		glViewport (0, 0, 8, 8);
		// set The Current Viewport to the fbo size
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb); 
// 		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depth_rb);

		glClearColor (0.0f, 0.0f, 0.0f, 0.0f);
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION); //
		glLoadIdentity ();

		glFrustum( -0.05f, 0.05f, -0.05, 0.05, 0.1f, (float)*critter_sightrange/10);

		body.mouths[0]->ghostObject->getWorldTransform().inverse().getOpenGLMatrix(position);
		glMultMatrixf(position);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();*/
// division
		place( framePosX, framePosY, genotype->bodyArch->retinasize, genotype->bodyArch->retinasize, v_inv );
	}

	void CritterB::place( const unsigned int pos_x, const unsigned int pos_y, const unsigned int size_x, const unsigned int size_y, const GLint v_inv )
	{
		glViewport(pos_x, pos_y, size_x, size_y);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();


// 		glFrustum(-m_frustumHalf, m_frustumHalf, -m_frustumHalf, m_frustumHalf, m_zNear, m_zFar);
// 		glFrustum(-m_frustumHalf, m_frustumHalf, -m_frustumHalf, m_frustumHalf, m_zNear, 0.1f * *critter_sightrange);
		glFrustum(-m_frustumHalf, m_frustumHalf, -m_frustumHalf, m_frustumHalf, m_zNear, 1800.0f);
		
		
		btTransform t2( m_render_offset * body.mouths[0]->ghostObject->getWorldTransform().inverse() );
	
		t2.getOpenGLMatrix(position);
		
// 		body.mouths[0]->ghostObject->getWorldTransform().inverse().getOpenGLMatrix(position);
		glMultMatrixf(position);
		
		if(v_inv != -1)
		{
			btScalar modelview2[16];
			body.mouths[0]->ghostObject->getWorldTransform().getOpenGLMatrix(modelview2);
			glUniformMatrix4fv(v_inv, 1, GL_FALSE, modelview2);
		}
		

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	
	
// 	void CritterB::releaseFBO()
// 	{
// /*		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); 
// 		glBindTexture(GL_TEXTURE_2D, 0);
// 		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
// 
// 		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb); 
// 		glBindTexture(GL_TEXTURE_2D, color_tex);
// 		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depth_rb);
// 		glEnable(GL_TEXTURE_2D);*/
// 
// // 		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); 
// // 		glBindTexture(GL_TEXTURE_2D, color_tex);
// // 		glDisable(GL_TEXTURE_2D);
// 
// // 		glBindTexture(GL_TEXTURE_2D, 0);
// // 		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb); 
// // 		glBindTexture(GL_TEXTURE_2D, color_tex);
// 
// 		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); 
// 		glBindTexture(GL_TEXTURE_2D, color_tex);
// 
// // 		glPixelStoref(GL_UNPACK_ALIGNMENT, 1);
// // 		unsigned char *temp = new unsigned char[8 * 8];
// // 		glGetTexImage(GL_TEXTURE_2D, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, temp);
// 
// // 		for ( unsigned int h=0; h < (genotype->bodyArch->retinasize*8); h += 8 )
// // 		{
// // 			for ( unsigned int w=h+0; w < h+((genotype->bodyArch->retinasize)*4); w+=4 )
// // 			{
// // 				if ( (int)temp[w+2] ) cerr << "\033[1;31mR\033[0m";
// // 				else cerr << ".";
// // 				if ( (int)temp[w+1] ) cerr << "\033[1;32mG\033[0m";
// // 				else cerr << ".";
// // 				if ( (int)temp[w] ) cerr << "\033[1;34mB\033[0m";
// // 				else cerr << ".";
// // 				if ( (int)temp[w+3] ) cerr << "\033[1;35mA\033[0m";
// // 				else cerr << ".";
// // 			}
// // 			cerr << "" << endl;
// // 		}
// // 		cerr << "" << endl;
// 
// 	}

	void CritterB::calcFramePos(unsigned int pos)
	{
		visionPosition = pos;

		// Calc 2D cartesian vectors X & Y for frame positioning of retina
		framePosY = 0;
		while ( pos >= retinasperrow )
		{
			pos -= retinasperrow;
			framePosY += genotype->bodyArch->retinasize;
		}
		framePosX = (pos * genotype->bodyArch->retinasize) + pos;

		// Calculate where in the Great Retina this critter shold start (column & row)
		unsigned int target = visionPosition;
		retinaRowStart = 0;

		// determine on which row of the retina to start for this critter
		retinaRowLength = retinasperrow * (genotype->bodyArch->retinasize+1) * 4;

		// determine on which column to start
		while ( target >= retinasperrow )
		{
			retinaRowStart += genotype->bodyArch->retinasize * retinaRowLength;
			target -= retinasperrow;
		}
		retinaColumnStart = target * (genotype->bodyArch->retinasize+1) * 4;

	// cerr << framePosX << " : " << framePosY << endl;
	// usleep (1000);

	}

	void CritterB::printVision()
	{
		for ( unsigned int h=retinaRowStart; h < retinaRowStart+(genotype->bodyArch->retinasize*retinaRowLength); h += retinaRowLength )
		{
			for ( unsigned int w=h+retinaColumnStart; w < h+retinaColumnStart+((genotype->bodyArch->retinasize)*4); w+=4 )
			{
				if ( (int)retina[w+2] ) cerr << "\033[1;31mR\033[0m";
				else cerr << ".";
				if ( (int)retina[w+1] ) cerr << "\033[1;32mG\033[0m";
				else cerr << ".";
				if ( (int)retina[w] ) cerr << "\033[1;34mB\033[0m";
				else cerr << ".";
				if ( (int)retina[w+3] ) cerr << "\033[1;35mA\033[0m";
				else cerr << ".";
			}
			cerr << "" << endl;
		}
		cerr << "" << endl;
	}

CritterB::~CritterB()
{
	genotypes->remove(genotype);
// 	delete raycast;
}


