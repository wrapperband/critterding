#include "race.h"
#ifdef HAVE_OPENMP
	#include <omp.h>
#endif

#include <iomanip>
#include "common/be_mouse_picker.h"
#include "scenes/critterding/gui/textverbosemessage.h"
#include "scenes/critterding/entities/dirlayout.h"
#include "scenes/critterding/entities/critterselection.h"
#include "scenes/critterding/entities/food.h"
#include "scenes/critterding/entities/wall.h"
#include "scenes/critterding/entities/genotype.h"
#include "scenes/critterding/entities/critterb.h"

using namespace std;

WorldRace::WorldRace(  boost::shared_ptr<BeGraphicsSystem> system, BeFilesystem& fileSystem, boost::shared_ptr<Textverbosemessage> textverbosemessage )
 : WorldB( system, fileSystem, textverbosemessage )
{
}

void WorldRace::init()
{
	testcounter = 1;

	cerr << endl << "Initializing run " << testcounter << " ... " << endl;

	// insert Floor
		makeFloor();

	// autoload critters
	if ( settings->getCVar("autoload") )
		loadAllCritters();
	if ( settings->getCVar("autoloadlastsaved") )
		loadAllLastSavedCritters();

	// insert first batch of critters
		for ( unsigned int i=critters.size(); i < *mincritters; ++i  )
			insRandomCritter( i );

	// insert food
		for ( unsigned int i=0; i < *mincritters; ++i  )
			insFood( i );

		framecounter = 0;
		haveWinner = false;
		m_finished_counter = 0;

	// reset cam
		resetCamera();

	m_skyTransform.setIdentity();
	m_skyTransform.setOrigin( btVector3( 0.5f * *worldsizeX, -500, 0.5f * *worldsizeZ ) );

	cerr<< "Running" << " ... " << endl;
}

void WorldRace::process(const BeTimer& timer)
{
	if ( !pause )
	{
		++framecounter;

// 		m_skyBoxAngle += 0.15f * timer.getSeconds();
// 		if ( m_skyBoxAngle > 360.0f )
// 			m_skyBoxAngle -= 360.0f;
// 		m_skyTransform.getBasis().setEulerZYX(0,(1.0/180) * SIMD_PI * m_skyBoxAngle,0);
		updateSkyboxAngles();
		
		autosaveCritters(timer);

		// do a bullet step
			m_dynamicsWorld->stepSimulation(0.016667f, 3, 0.00833334f);
	// 		m_dynamicsWorld->stepSimulation(0.016667f);
	// 		m_dynamicsWorld->stepSimulation(Timer::Instance()->bullet_ms / 1000.f);

		// render critter vision, optimized for this sim
			renderVision();
		// Read pixels into retina
			grabVision();

		unsigned int lmax = critters.size();
		unsigned int i;

		for( i=0; i < lmax; ++i)
			checkCollisions( critters[i] );

	// process
#ifdef HAVE_OPENMP
		if ( m_threads_last != *threads )
		{
			m_threads_last = *threads;
			omp_set_num_threads(m_threads_last);
		}

		if ( m_threads_last > 1 )
		{
// 			unsigned int t_lmax = lmax;
			#pragma omp parallel for private(i)
			for( i=0; i < lmax; ++i)
			{
				critters[i]->process();
// 				freeEnergyc += c->energyUsed;
			}
		}
		else
#endif
		{
			for( i=0; i < lmax; ++i)
				critters[i]->process();
		}
		
		for( i=0; i < lmax; ++i)
		{
			CritterB *c = critters[i];
			
			if ( c->fitness_index == 0.0f )
			{
// 			// process
// 				c->process();

			// process Output Neurons
				if ( c->eat && c->touchingFood )
				{
					Food* f = static_cast<Food*>(c->touchedEntity);
					float eaten = 0.02f * *critter_maxenergy;
					if ( c->energyLevel + eaten > *critter_maxenergy )
						eaten -= (c->energyLevel + eaten) - *critter_maxenergy;
					if ( f->energyLevel - eaten < 0 )
						eaten = f->energyLevel;

					c->energyLevel += eaten;
					f->energyLevel -= eaten;
					
					// if a food unit has no more energy left, we have a winner, the race is over
// 					if ( f->energyLevel  == 0.0f )
// 						haveWinner = true;
					
					if ( f->energyLevel <= 0.0f )
					{
// 						c->fitness_index += 100.0f / m_finished_counter;
						c->fitness_index += *mincritters - m_finished_counter;
						++m_finished_counter;

						std::cout << "winner " << m_finished_counter << " at frame " << framecounter << std::endl;
					}
				}
			}
		}

		if ( (haveWinner || framecounter >= (unsigned int)*critter_maxlifetime) )
		{
			if ( haveWinner )
				cerr << "we have a WINNER after " << framecounter << " frames" << endl;

			cerr << "Evaluating..." << endl;

				// measure their distances from their respective food targets
					for ( unsigned int i=0; i < critters.size(); ++i  )
					{
						// fitness function 1: distance to food cube
							btDefaultMotionState* cmyMotionState = (btDefaultMotionState*)critters[i]->body.mouths[0]->body->getMotionState();
							const btVector3& cposi = cmyMotionState->m_graphicsWorldTrans.getOrigin();

							Food* f = food_units[i];
							
							btDefaultMotionState* fmyMotionState = (btDefaultMotionState*)f->body.bodyparts[0]->body->getMotionState();
							btVector3 fposi = fmyMotionState->m_graphicsWorldTrans.getOrigin();

							critters[i]->fitness_index +=  0.5 * (1.0 / (0.0000001 + cposi.distance(fposi)));
						
						// fitness function 2: energy of food consumed
// 							critters[i]->fitness_index += ( (float)*food_maxenergy /(f->energyLevel + 0.0000001));
							critters[i]->fitness_index += 0.5 * (1.0 / (0.0000001 + *food_maxenergy));

					}

				// initialize sort indices
					vector<int> indices ( critters.size(), 0 );
					for ( unsigned int i = 0; i < critters.size(); ++i )
						indices[i] = i;
		
				// sort results
					for ( int i = critters.size(); i>0; i--  )
						for ( int j = 0; j < i-1; j++  )
							if ( critters[indices[j]]->fitness_index < critters[indices[j+1]]->fitness_index )
							{
								unsigned keepI	= indices[j];
								indices[j]	= indices[j+1];
								indices[j+1]	= keepI;
							}

				// calc number to keep
					unsigned int bestNum;
					if ( critters.size() == 1 )
						bestNum = 1;
					else
					{
						const unsigned int eliminate_amount = 1 + (0.01 * critters.size()) * *population_eliminate_portion_percent;
						bestNum = critters.size() - eliminate_amount;
					}
// 					std::cout << "number to keep: " << bestNum << "  " << (float(bestNum) / critters.size()) * 100 << std::endl;

							
				// display results
					for ( unsigned int i=0; i < critters.size() && i < bestNum; ++i  )
					{
// 						if ( i < bestNum )
// 							cout << "k";
// 						else
// 							cout << " ";
						
						if ( food_units[indices[i]]->energyLevel <= 0.0f )
							cout << "f";
						else
							cout << " ";
	
						cout << " " << indices[i] << " : " << critters[indices[i]]->fitness_index << endl;
					}

					cerr << endl << "Initializing run " << ++testcounter << " ... " << endl;

				// backup the 50% best critters
					vector<CritterB*> best;
					
					if ( !critters.empty() )
					{
						btTransform t;
						t.setIdentity();

						for ( unsigned int i=0; i < bestNum; ++i  )
							best.push_back( new CritterB(*critters[indices[i]], critters[indices[i]]->critterID, t, false, false) );
					}
					
				// remove critters and food
					for ( unsigned int i=0; i < critters.size(); ++i )
					{
						stringstream buf;
						buf << setw(4) << critters[i]->critterID << " old";
						m_panel_textverbosemessage->addDeath(buf);

						if ( critters[i]->isPicked )
							mousepicker->detach();
						
						if ( m_follow_critterP == critters[i] )
						{
							m_follow_critterP = 0;
					// 		m_follow_critterP = *critters.rbegin();
							m_camera.setSceneNode(&m_sceneNodeCamera);
						}
						
						
						
	// FIXME on windows, we segfault here 1/10 after the first run
						critterselection->unregisterCritterID(critters[i]->critterID);
						critterselection->deselectCritter(critters[i]->critterID);
						delete critters[i];
	// FIXME
					}
					critters.clear();

					for ( unsigned int i=0; i < food_units.size(); ++i )
					{
						if ( food_units[i]->isPicked )
							mousepicker->detach();
						delete food_units[i];
					}
					food_units.clear();

// 					const int fsize( (int)food_units.size() );
// 					for ( int i=0; i < fsize; ++i )
// 					{
// // 						if ( entities[i]->type == FOOD )
// 						{
// 							Food* f( food_units[i] );
// 							if ( f->isPicked )
// 								mousepicker->detach();
// 							delete f;
// 							food_units.erase(food_units.begin()+i);
// 							i--;
// 						}
// 					}
// // 					food.clear();

				// clear floor and remake it
					makeFloor();

				// reinsert the best critters
					for ( unsigned int i=0; i < best.size() && i < *mincritters; ++i  )
						insMutatedCritter( *best[i], critters.size(), best[i]->critterID, false, false );

				// insert the mutants
					unsigned int count = 0;
					while ( critters.size() < *mincritters )
					{
						if ( best.size() > 0 )
						{
							bool brainmutant = false;
							bool bodymutant = false;
							CritterB* c = best[count];
							if ( randgen->Instance()->get(1,1000) <= c->genotype->brainzArch->m_mutationrate )
								brainmutant = true;

							if ( randgen->Instance()->get(1,1000) <= c->genotype->bodyArch->m_mutationrate )
								bodymutant = true;

							insMutatedCritter( *best[count], critters.size(), currentCritterID++, brainmutant, bodymutant );

							CritterB* nc = critters[critters.size()-1];
							stringstream buf;
							buf << setw(4) << c->critterID << " : " << setw(4) << nc->critterID;
							buf << " ad: " << setw(4) << nc->genotype->adamdist;
							buf << " n: " << setw(4) << nc->brain.totalNeurons << " s: " << setw(5) << nc->brain.totalSynapses;

							count++;
							if ( count == best.size() && count > 0 )
								count = 0;

							if ( brainmutant || bodymutant )
							{
								buf << " ";
								if ( brainmutant ) buf << "brain";
								if ( brainmutant && bodymutant ) buf << "+";
								if ( bodymutant ) buf << "body";
								buf << " mutant";
							}

							m_panel_textverbosemessage->addBirth(buf);
						}
						else
							insRandomCritter( critters.size() );
					}

				// remove best again
					for ( unsigned int i=0; i < best.size(); ++i  )
						delete best[i];

				// reinsert respective food units
					for ( unsigned int i=0; i < *mincritters; ++i  )
						insFood( i );

				framecounter = 0;
				m_finished_counter = 0;
				haveWinner = false;

				cerr << "Running... " << endl;
		}
		getGeneralStats();
	}
}

void WorldRace::makeFloor()
{
	
	if ( !m_model_cube )
	{
		std::string mapName = "cube/cube.obj";
		
		btTransform t;
		t.setIdentity();
		
		m_model_cube=m_modelSystem->load(mapName, m_graphicsSystem, m_modelSystem, btVector3(1.0f,1.0f,1.0f), t);
// 		m_skybox = boost::shared_ptr<ScClientMapResource>(new ScClientMapResource(m_fileSystem, mapName, m_graphicsSystem, m_modelSystem));
	}
	
	makeSkybox();
// 	if ( !m_skyBoxModel )
// 	{
// 		std::string mapName = "skys/default/skydome3.obj";
// 		
// 		btTransform t;
// 		t.setIdentity();
// 		
// 		m_skyBoxModel=m_modelSystem->load(mapName, m_graphicsSystem, m_modelSystem, btVector3(1.0f,1.0f,1.0f), t);
// 
// // 		m_skybox = boost::shared_ptr<ScClientMapResource>(new ScClientMapResource(m_fileSystem, mapName, m_graphicsSystem, m_modelSystem));
// 	}
// 	m_skyTransform.setOrigin( btVector3( 0.5f * *worldsizeX, -500, 0.5f * *worldsizeZ ) );



// 	for ( unsigned int i=0; i < walls.size(); ++i )	
// 		delete walls[i];
// 	walls.clear();
	for ( int i=0; i < (int)wall_units.size(); ++i )
	{
// 		if ( entities[i]->type == WALL )
		{
			delete wall_units[i];
			wall_units.erase(wall_units.begin()+i);
			i--;
		}
	}

	critterspacing = (float)*worldsizeX / *mincritters;

// 	makeDefaultFloor();

	// Wall Constants
		const float WallWidth = 2.0f;
		const float WallHalfWidth = WallWidth/2.0f;
		const float WallHeight = 5.0f;
		const float WallHalfHeight = WallHeight/2.0f;

		// Left Wall
		{
			btTransform t;
			t.setIdentity();
			t.setOrigin(btVector3( 0.0f-WallHalfWidth, WallHalfHeight-WallWidth, *worldsizeZ/2.0f ));

			Wall* w = new Wall( WallWidth, WallHeight, *worldsizeZ, t, m_dynamicsWorld );
			w->m_model = m_model_cube;
			wall_units.push_back(w);
		}
		// Right Wall
		{
			btTransform t;
			t.setIdentity();
			t.setOrigin(btVector3( *worldsizeX+WallHalfWidth, WallHalfHeight-WallWidth, *worldsizeZ/2.0f ));

			Wall* w = new Wall( WallWidth, WallHeight, *worldsizeZ, t, m_dynamicsWorld );
			w->m_model = m_model_cube;
			wall_units.push_back(w);
		}
		// Top Wall
		{
			btTransform t;
			t.setIdentity();
			t.setOrigin(btVector3( *worldsizeX/2.0f, WallHalfHeight-WallWidth, 0.0f-WallHalfWidth ));

			Wall* w = new Wall( *worldsizeX+(WallWidth*2), WallHeight, WallWidth, t, m_dynamicsWorld );
			w->m_model = m_model_cube;
			wall_units.push_back(w);
		}
		// Bottom Wall
		{
			btTransform t;
			t.setIdentity();
			t.setOrigin(btVector3( *worldsizeX/2.0f, WallHalfHeight-WallWidth, *worldsizeZ+WallHalfWidth ));

			Wall* w = new Wall( *worldsizeX+(WallWidth*2), WallHeight, WallWidth, t, m_dynamicsWorld );
			w->m_model = m_model_cube;
			wall_units.push_back(w);
		}
		
		// Ground Floor
		{
			btTransform t;
			t.setIdentity();
			t.setOrigin(btVector3( *worldsizeX/2.0f, -WallHalfWidth, *worldsizeZ/2.0f ));
			
			Wall* w = new Wall( *worldsizeX, WallWidth, *worldsizeZ, t, m_dynamicsWorld );
			w->m_model = m_model_cube;
			wall_units.push_back(w);
		}

	
		for ( unsigned int i=1; i < *mincritters; ++i  )
		{
			btVector3 position = btVector3 ( 0.0f-WallHalfWidth + (critterspacing*i), WallHalfHeight-WallWidth, *worldsizeZ/2.0f );
			
			btTransform t;
			t.setIdentity();
			t.setOrigin(position);

			Wall* w = new Wall( WallWidth, WallHeight, *worldsizeZ, t, m_dynamicsWorld );
			w->m_model = m_model_cube;
			wall_units.push_back(w);
		}
		
	activateFood();
}

void WorldRace::resetCamera()
{
	if ( &m_sceneNodeCamera == m_camera.getSceneNode() )
	{
		btTransform t;
		t.setIdentity();
		m_sceneNodeCamera.setTransform(t);

		int biggest = *worldsizeX;
		if ( *worldsizeZ > biggest )
			biggest = 1.4f * *worldsizeZ;

		m_sceneNodeCamera.setOrigin( btVector3( 0.5f * *worldsizeX, 1.f*biggest, 0.5f * *worldsizeZ) );
	// 	camera.position.setRotation(btQuaternion(btVector3(1, 0, 0), btScalar(90)));
		m_sceneNodeCamera.pitch( -SIMD_HALF_PI ); // 1.5707f  (float)*energy/10
	}
	else
	{
		reset_follow_location();
// 			m_camera.getSceneNode()->setTransform(m_null_transform);
	}
}

void WorldRace::insRandomCritter(int nr)
{
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3( (critterspacing/2)+(critterspacing*nr), 3.0f, *worldsizeZ-8.0f ));
				
	CritterB *c = new CritterB(m_dynamicsWorld, currentCritterID++, t, retina);
	c->energyLevel = *critter_maxenergy / 2;
	critters.push_back( c );
	c->calcFramePos(critters.size()-1);
}

void WorldRace::insMutatedCritter(CritterB& other, int nr, unsigned int id, bool mutateBrain, bool mutateBody)
{
	btTransform t;
	t.setIdentity();
	t.setOrigin(btVector3( (critterspacing/2)+(critterspacing*nr), 3.0f, *worldsizeZ-8.0f ));
	
	CritterB *nc = new CritterB(other, id, t, mutateBrain, mutateBody);
	nc->energyLevel = *critter_maxenergy / 2;
	critters.push_back( nc );
	nc->calcFramePos(critters.size()-1);
}

void WorldRace::insFood(int nr)
{
	Food *f = new Food;
	f->energyLevel = *food_maxenergy;
	f->createBody( m_dynamicsWorld, btVector3( (critterspacing/2)+(critterspacing*nr), 3.0f, 8.0f ) );
	food_units.push_back( f );
}

void WorldRace::insertCritter()
{
	cerr << "inserting critters is disabled during race" << endl;
}

void WorldRace::loadAllCritters()
{
	if ( critters.size() > 0 )
	{
		stringstream buf;
		buf << "use --autoload 1 at commandline to autoload critters into a race";
// 		Logbuffer::Instance()->add(buf);
		cerr << "use --autoload 1 at commandline to autoload critters into a race" << endl;
	}
	else
	{
		vector<string> files;
		dirH.listContentsFull(dirlayout->loaddir, files);

		unsigned int inserted = 0;
		for ( unsigned int i = 0; i < files.size() && inserted < *mincritters; ++i )
		{
			auto& f( files[i] );
			
			if ( parseH.endMatches( ".cr", f ) || parseH.endMatches(".cr.bz2", f) )
			{
				BeFile befileCritter;
				if ( m_fileSystem.load( befileCritter, f ) )
				{
					std::string content( befileCritter.getContent().str() );
					
					critterspacing = (float)*worldsizeX / *mincritters;
					
					btTransform t;
					t.setIdentity();
					t.setOrigin(btVector3( (critterspacing/2)+(critterspacing*critters.size()), 1.0f, *worldsizeZ-(*worldsizeZ/4) ));
					
					CritterB *c = new CritterB(content, m_dynamicsWorld, t, retina);

					unsigned int error = 0;
					if ( c->genotype->bodyArch->retinasize != (unsigned int)*critter_retinasize )
						error = 1;

					if ( !error)
					{
						critters.push_back( c );

						c->critterID = currentCritterID++;
						c->calcFramePos(critters.size()-1);
						c->energyLevel = *critter_maxenergy / 2;
						inserted++;
					}
					else
					{
						if ( error == 1 )
						{
							cerr << "ERROR: critter retinasize (" << c->genotype->bodyArch->retinasize << ") doesn't fit world retinasize (" << *critter_retinasize << ")" << endl;
						}
						delete c;
					}
				}
			}
		}
		cerr << endl << "Loaded critters from " << dirlayout->loaddir << endl << endl;
	}
}

// void WorldRace::setLights()
// {
// 	int biggest = *worldsizeX;
// 	const auto Z(1.4f * *worldsizeZ);
// 	if ( Z > biggest )
// 		biggest = Z;
// 
// 	GLfloat lightPos[] = { 0.5f * *worldsizeX, 6.f*biggest, 0.5f * *worldsizeZ, 1.0f };
// // 				GLfloat lightPos[] = { 0.47f * *worldsizeX, 2.5f * (*worldsizeX + *worldsizeY), 0.47f * *worldsizeY, 1.0f };
// 	
// // 				GLfloat lightPos[] = { 0.0f, 0.1f * *worldsizeX, 0.0f, 1.0f };
// // 			GLfloat lightPos[] = { 0.25f * *worldsizeX, 0.5f * *worldsizeX, 0.25f * *worldsizeY, 1.0f };
// 	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
// 
// 	GLfloat lightDir[] = { 0.0f, -1.0f, 0.0f };
// 	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightDir);
// 
// 	GLfloat lightCutoff[] = { 90.0f }; // angle is 0 to 180
// 	glLightfv(GL_LIGHT0, GL_SPOT_CUTOFF, lightCutoff);
// 
// 	GLfloat lightExponent[] = { 128.0f }; // exponent is 0 to 128
// 	glLightfv(GL_LIGHT0, GL_SPOT_EXPONENT, lightExponent);
// 	
// 	GLfloat lightAttenuation[] = { 0.5f };
// 	glLightfv(GL_LIGHT0, GL_CONSTANT_ATTENUATION, lightAttenuation);
// 
// 	GLfloat lightLAttenuation[] = { 0.000005f };
// 	glLightfv(GL_LIGHT0, GL_LINEAR_ATTENUATION, lightLAttenuation);
// 
// 	GLfloat lightQAttenuation[] = { 0.000000f };
// 	glLightfv(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, lightQAttenuation);
// }


WorldRace::~WorldRace()
{
}
