#include "brainzarch.h"
#include <cstdio>

BrainzArch::BrainzArch()
{
		neuronsFired					= 0;

	// build time defaults

		maxNeurons					= 150;
		minSynapses					= 1;
		maxSynapses					= 150;

		totalNeurons					= 0;
		totalSynapses					= 0;

		numberOfInputs					= 0;
		numberOfOutputs					= 0;

		minNeuronsAtBuildtime				= 10;
		maxNeuronsAtBuildtime				= 150;

		// mutatables

		minSynapsesAtBuildtime				= 1;
		maxSynapsesAtBuildtime				= 50;

		percentChanceInhibitoryNeuron			= 50;
			mutate_percentChanceInhibitoryNeuron	= false;

		percentChanceConsistentSynapses			= 50;
			mutate_percentChanceConsistentSynapses	= false;

		percentChanceInhibitorySynapses			= 50;
			mutate_percentChanceInhibitorySynapses	= false;

		percentChanceMotorNeuron			= 50;
			mutate_percentChanceMotorNeuron		= false;

		percentChancePlasticNeuron			= 50;
			mutate_percentChancePlasticNeuron	= false;

		minPlasticityStrengthen				= 100;
		maxPlasticityStrengthen				= 1000;
		minPlasticityWeaken				= 1000;
		maxPlasticityWeaken				= 10000;
			mutate_PlasticityFactors		= false;

		percentChanceSensorySynapse			= 50;
			mutate_percentChanceSensorySynapse	= false;

		minFiringThreshold				= 10;
			mutate_minFiringThreshold		= false;

		maxFiringThreshold				= 100;
			mutate_maxFiringThreshold		= false;

// 		maxDendridicBranches				= 20;
// 			mutate_maxDendridicBranches		= false;

		percentMutation					= 1;
			mutate_percentMutation			= false;

		// 90 % > 10 for mutatables
		percentMutateEffectAddNeuron			= 5;
		percentMutateEffectRemoveNeuron			= 5;
		percentMutateEffectAlterNeuron			= 5;
		percentMutateEffectAddSynapse			= 40;
		percentMutateEffectRemoveSynapse		= 40;
		percentMutateEffectAlterMutable			= 5;
			mutate_MutateEffects			= false;

		m_maxmutations					= Settings::Instance()->getCVar("brain_maxmutations");
		m_mutationrate					= Settings::Instance()->getCVar("brain_mutationrate");

}

// BUILD TIME

	void BrainzArch::registerInput(unsigned int id)
	{
/*		Inputs.push_back( sensorNeuron() );
		Inputs[numberOfInputs].id = id;*/
		InputIDs.push_back(id);
		numberOfInputs++;
	}

	void BrainzArch::registerOutput(unsigned int id)
	{
// 		Outputs.push_back( motorNeuron() );
// 		Outputs[numberOfOutputs].output = var;
// 		Outputs[numberOfOutputs].id = id;
		OutputIDs.push_back(id);
		numberOfOutputs++;
	}

	void BrainzArch::removeObsoleteMotorsAndSensors()
	{
// 		std::cerr << std::endl << "removing obsolete brainstuff" << std::endl;
		for ( int i = 0; i < (int)ArchNeurons.size(); ++i )
		{
			ArchNeuronz& an = ArchNeurons[i];
			// disable motor neurons
			if ( an.isMotor )
			{
				if ( findMotorNeuron( an.motorID ) == -1 )
				{
					an.isMotor = false;
				}
			}

			// disable sensor inputs
			for ( int j = 0; j < (int)an.ArchSynapses.size(); ++j )
			{
				ArchSynapse& as = an.ArchSynapses[j];
				if ( as.isSensorNeuron )
				{
// 					if ( findSensorNeuron( as.neuronID ) == -1 )
// 					{
// 						an.ArchSynapses.erase( an.ArchSynapses.begin()+j );
// 						j--;
// 					}
					if ( !sensorNeuronExists( as.neuronID ) )
					{
						an.ArchSynapses.erase( an.ArchSynapses.begin()+j );
						j--;
					}
				}
			}
		}
// 		std::cerr << "done removing obsolete brainstuff" << std::endl << std::endl;
	}


	void BrainzArch::buildArch()
	{
		// clear architecture by removing all architectural neurons
			ArchNeurons.clear();
	
		// determine number of neurons this brain will start with
			const unsigned int NeuronAmount(randgen->Instance()->get( minNeuronsAtBuildtime, maxNeuronsAtBuildtime ));
	
		// create the architectural neurons
			for ( unsigned i = 0; i < NeuronAmount; ++i )
				addRandomArchNeuron();
	
		// create architectural synapses
			for ( unsigned neuronID = 0; neuronID < NeuronAmount; ++neuronID )
			{
				// determine amount of synapses this neuron will start with
					const unsigned int SynapseAmount(randgen->Instance()->get( minSynapsesAtBuildtime, maxSynapsesAtBuildtime ));

				// create the architectural neurons
					for ( unsigned j = 0; j < SynapseAmount; ++j )
						addRandomArchSynapse(neuronID);
			}
	}

	void BrainzArch::addRandomArchNeuron()
	{
		// new architectural neuron
			ArchNeuronz an;

		// is it inhibitory ?
			if ( (unsigned int)randgen->Instance()->get(1,100) <= percentChanceInhibitoryNeuron )
				an.isInhibitory = true;

		// if not, is it motor ?
		// if 1 or 2nd archneuron, pic eat & procreate specifically
			else if ( (unsigned int)randgen->Instance()->get(1,100) <= percentChanceMotorNeuron || ArchNeurons.size() < 3 )
			{
				unsigned int motorID;
				if ( ArchNeurons.size() == 0 )
					motorID = OutputIDs[ numberOfOutputs-2 ];
				else if ( ArchNeurons.size() == 1 )
					motorID = OutputIDs[ numberOfOutputs-1 ];
				else
					motorID = OutputIDs[ randgen->Instance()->get( 0, numberOfOutputs-1 ) ];

				// check if motor already used
				bool proceed = true;
				for ( unsigned int i(0); i < ArchNeurons.size() && proceed; ++i )
					if ( ArchNeurons[i].isMotor && ArchNeurons[i].motorID == motorID )
						proceed = false;

				if ( proceed )
				{
					an.isMotor = true;
					an.motorID = motorID;
				}
			}

		// does it have synaptic plasticity ?
			if ( (unsigned int)randgen->Instance()->get(1,100) <= percentChancePlasticNeuron )
			{
				an.isPlastic = true;
				an.plasticityStrengthen = randgen->Instance()->get( minPlasticityStrengthen, maxPlasticityStrengthen );
				an.plasticityWeaken = randgen->Instance()->get( minPlasticityWeaken, maxPlasticityWeaken );
			}

		// does it have consistent synapses ?
			if ( (unsigned int)randgen->Instance()->get(1,100) <= percentChanceConsistentSynapses )
			{
				an.hasConsistentSynapses = true;

				// if so, does it have inhibitory synapses ?
					if ( (unsigned int)randgen->Instance()->get(1,100) <= percentChanceInhibitorySynapses )
						an.hasInhibitorySynapses = true;
			}

		// determine firing threshold
			/*if ( an.isMotor ) an.firingThreshold = maxFiringThreshold;
			else*/ an.firingThreshold = randgen->Instance()->get( minFiringThreshold, maxFiringThreshold );

		// determine dendritic branches
// 			if ( an.isMotor ) an.dendridicBranches = maxDendridicBranches;
// 			else an.dendridicBranches = randgen->Instance()->get( 1, maxDendridicBranches );
			
			an.potentialDecay = randgen->Instance()->get( 666, 1000 );

		// push it on the vector
			ArchNeurons.push_back( an );
	}

	unsigned int BrainzArch::addRandomArchSynapse(unsigned int parentneuron)
	{
		// new architectural synapse
			ArchSynapse as;
	
		// is it connected to a sensor neuron ?
										// < 2 because if only 1 archneuron, it can't connect to other one
			if ( (unsigned int)randgen->Instance()->get( 1, 100 ) <= percentChanceSensorySynapse || ArchNeurons.size() < 2 )
			{
				as.isSensorNeuron = true;

				// sensor neuron id synapse is connected to
				as.neuronID = InputIDs[ randgen->Instance()->get( 0, numberOfInputs-1 ) ];

			}
		// if not determine inter neuron id
	 		else
			{
				// as in real life, neurons can connect to themselves
				as.neuronID = randgen->Instance()->get( 0, ArchNeurons.size()-1 );
	 		}
	
		// dendrite branch number
// 			as.dendriteBranch = randgen->Instance()->get( 1, ArchNeurons[parentneuron].dendridicBranches );

		// synaptic weight
			if ( ArchNeurons[parentneuron].hasConsistentSynapses )
			{
				if ( ArchNeurons[parentneuron].hasInhibitorySynapses )
					as.weight	=-1.0f;
				else
					as.weight	=1.0f;
			}
			else
			{
				if ( (unsigned int)randgen->Instance()->get(1,100) <= percentChanceInhibitorySynapses )
					as.weight	=-1.0f;
				else
					as.weight	=1.0f;
			}

		// push it on the vector
			ArchNeurons[parentneuron].ArchSynapses.push_back( as );

		// return it's id
			return (ArchNeurons[parentneuron].ArchSynapses.size()-1);
	}

	int BrainzArch::findSensorNeuron( const unsigned int id ) const
	{
// 		int count(0);
// 		const auto& end(InputIDs.end());
// 		for ( auto it(InputIDs.begin()); it != end; ++it )
// 		{
// 			if ( *it == id )
// 				return count;
// 			++count;
// 		}
		
		const auto& result = std::find ( InputIDs.begin(), InputIDs.end(), id);
		if ( result != InputIDs.end()  )
		{
			return result - InputIDs.begin();
		}

// 		for ( unsigned int i(0); i < numberOfInputs; ++i )
// 		{
// 			if ( InputIDs[i] == id )
// 				return i;
// 		}
		
// 		std::cerr << "brain findSensorNeuron error for id: " << id << std::endl;
		return -1;
	}

	bool BrainzArch::sensorNeuronExists( const unsigned int id ) const
	{
// 		const auto& end(InputIDs.end());
// 		for ( auto it(InputIDs.begin()); it != end; ++it )
// 		{
// 			if ( *it == id )
// 				return true;
// 		}

		const auto& result = std::find ( InputIDs.begin(), InputIDs.end(), id);
		if ( result != InputIDs.end()  )
		{
			return true;
		}
		
		
// 		for ( unsigned int i(0); i < numberOfInputs; ++i )
// 		{
// 			if ( InputIDs[i] == id )
// 				return i;
// 		}
		
// 		std::cerr << "brain findSensorNeuron error for id: " << id << std::endl;
		return false;
	}

	int BrainzArch::findMotorNeuron( const unsigned int id ) const
	{
		for ( unsigned int i(0); i < numberOfOutputs; ++i )
			if ( OutputIDs[i] == id )
				return i;

// 		std::cerr << "brain findMotorNeuron error for id: " << id << std::endl;
		return -1;
	}

	void BrainzArch::mutate(unsigned int runs)
	{
		// have to do count cuz wireArch not done yet
		totalNeurons		= ArchNeurons.size();
		totalSynapses		= 0;
		for ( unsigned int i = 0; i < totalNeurons; ++i ) totalSynapses += ArchNeurons[i].ArchSynapses.size();

		if ( runs == 0 )
		{
			runs = randgen->Instance()->get(1, (int)(totalSynapses/(100/percentMutation)));
		}

		for ( unsigned int i(0); i < runs; ++i )
		{
			unsigned int tsum = 	percentMutateEffectAddNeuron
						+ percentMutateEffectRemoveNeuron
						+ percentMutateEffectAlterNeuron
						+ percentMutateEffectAddSynapse
						+ percentMutateEffectRemoveSynapse
						+ percentMutateEffectAlterMutable
						+ percentMutateEffectAlterMutable
// 						+ Settings::Instance()->getCVar("brain_percentmutateeffectchangemutationrate")
// 						+ Settings::Instance()->getCVar("brain_percentmutateeffectchangemaxmutations")
					;

			unsigned int mode = randgen->Instance()->get(1,tsum);

		// add a new neuron
			unsigned int modesum = percentMutateEffectAddNeuron;
			if ( mode <= modesum )
			{
				if ( ArchNeurons.size() < maxNeurons )
				{
					addRandomArchNeuron();
					unsigned int nid = ArchNeurons.size()-1;
					//cerr << "\t+N " << nid << std::endl;

					// random amount of connections, at mutation time we take the average syns / neuron *2 as a maximum
					unsigned int tNeurons = totalNeurons;
					if ( tNeurons == 0 )
						tNeurons = 1;

					int maxSynapsesAtMutation = (2*(totalSynapses/tNeurons)) + 1;

					unsigned int cAmount = randgen->Instance()->get( minSynapsesAtBuildtime, maxSynapsesAtMutation );
					for ( unsigned j = 0; j < cAmount; ++j )
						addRandomArchSynapse(nid);
				}
				else
					runs++;
				continue;
			}

		// remove a neuron
			modesum += percentMutateEffectRemoveNeuron;
			if ( mode <= modesum )
			{
				if ( ArchNeurons.size() > 0 )
				{
					// pick a random neuron
					unsigned int nid = randgen->Instance()->get( 0, ArchNeurons.size()-1 );
					//cerr << "\t-N " << nid << std::endl;

					// first remove all connections to this neuron
					for ( unsigned int i(0); i < ArchNeurons.size(); ++i )
					{
						for ( unsigned int j=0; j < ArchNeurons[i].ArchSynapses.size(); ++j )
						{
							if ( !ArchNeurons[i].ArchSynapses[j].isSensorNeuron )
							{
								if ( ArchNeurons[i].ArchSynapses[j].neuronID == nid )
								{
									//delete ArchNeurons[i].ArchSynapses[j];
									ArchNeurons[i].ArchSynapses.erase(ArchNeurons[i].ArchSynapses.begin()+j);
									j--;
								}
								// if higher id drop one
								else if ( ArchNeurons[i].ArchSynapses[j].neuronID > nid )
								{
									ArchNeurons[i].ArchSynapses[j].neuronID--;
								}
							}
						}
					}
					ArchNeurons.erase(ArchNeurons.begin()+nid);
				}
				// make sure we mutate
				else
					runs++;
				continue;
			}

		// alter a neuron
			modesum += percentMutateEffectAlterNeuron;
			if ( mode <= modesum )
			{
				if ( ArchNeurons.size() > 0 )
				{

					// pick a random neuron
						unsigned int nid = randgen->Instance()->get( 0, ArchNeurons.size()-1 );

					// decide what to alter
						unsigned int jmode = randgen->Instance()->get(1,6);

					// inhibitory function
						if ( jmode == 1 )
						{
							// backup old
							bool old = ArchNeurons[nid].isInhibitory;

							// reset
							ArchNeurons[nid].isInhibitory = false;

							// redetermine
							if ( (unsigned int)randgen->Instance()->get(1,100) <= percentChanceInhibitoryNeuron )
								ArchNeurons[nid].isInhibitory = true;

							// make sure we mutate
							if ( old == ArchNeurons[nid].isInhibitory ) runs++;
						}
					// motor neuron
						else if ( jmode == 2 )
						{
							// backup old
							bool old = ArchNeurons[nid].isMotor;
							unsigned int oldfunc = ArchNeurons[nid].motorID;

							// reset
							ArchNeurons[nid].isMotor = false;
							ArchNeurons[nid].motorID = 0;

							// redetermine
							if ( !ArchNeurons[nid].isInhibitory && (unsigned int)randgen->Instance()->get(1,100) <= percentChanceMotorNeuron )
							{
								unsigned int motorID = OutputIDs[ randgen->Instance()->get( 0, numberOfOutputs-1 ) ];

								bool proceed = true;
								for ( unsigned int i(0); i < ArchNeurons.size() && proceed; ++i )
									if ( ArchNeurons[i].isMotor && ArchNeurons[i].motorID == motorID )
										proceed = false;
				
								if ( proceed )
								{
									ArchNeurons[nid].isMotor = true;
									ArchNeurons[nid].motorID = motorID;
								}
							}

							// make sure we mutate
							if ( old == ArchNeurons[nid].isMotor && oldfunc == ArchNeurons[nid].motorID )
								runs++;
						}
					// synaptic plasticity
						else if ( jmode == 3 )
						{
							// backup old
							bool old = ArchNeurons[nid].isPlastic;
							unsigned int olds = ArchNeurons[nid].plasticityStrengthen;
							unsigned int oldw = ArchNeurons[nid].plasticityWeaken;

							// reset
							ArchNeurons[nid].isPlastic = false;
							ArchNeurons[nid].plasticityStrengthen = 1;
							ArchNeurons[nid].plasticityWeaken = 1;

							// redetermine
							if ( (unsigned int)randgen->Instance()->get(1,100) <= percentChancePlasticNeuron )
							{
								ArchNeurons[nid].isPlastic = true;
								ArchNeurons[nid].plasticityStrengthen = randgen->Instance()->get( minPlasticityStrengthen, maxPlasticityStrengthen );
								ArchNeurons[nid].plasticityWeaken = randgen->Instance()->get( minPlasticityWeaken, maxPlasticityWeaken );
							}

							// make sure we mutate
							if ( old == ArchNeurons[nid].isPlastic && olds == ArchNeurons[nid].plasticityStrengthen && oldw == ArchNeurons[nid].plasticityWeaken ) runs++;
						}

					// consistent synapses
						else if ( jmode == 4 )
						{
							// backup old
							bool old = ArchNeurons[nid].hasConsistentSynapses;
							bool oldi = ArchNeurons[nid].hasInhibitorySynapses;

							// reset
							ArchNeurons[nid].hasConsistentSynapses = false;

							// redetermine
							if ( (unsigned int)randgen->Instance()->get(1,100) <= percentChanceConsistentSynapses )
							{
								ArchNeurons[nid].hasConsistentSynapses = true;
				
								// if so, does it have inhibitory synapses ?
									if ( (unsigned int)randgen->Instance()->get(1,100) <= percentChanceInhibitorySynapses )
										ArchNeurons[nid].hasInhibitorySynapses = true;
							}

							// make sure we mutate
							if ( old == ArchNeurons[nid].hasConsistentSynapses && oldi == ArchNeurons[nid].hasInhibitorySynapses ) runs++;
						}

					// firing threshold
						else if ( jmode == 5 )
						{
							// backup old
							unsigned int old = ArchNeurons[nid].firingThreshold;

							ArchNeurons[nid].firingThreshold = randgen->Instance()->get( minFiringThreshold, maxFiringThreshold );

							// make sure we mutate
							if ( old == ArchNeurons[nid].firingThreshold ) runs++;
						}

					// potential decay
						else if ( jmode == 6 )
						{
							ArchNeurons[nid].potentialDecay = randgen->Instance()->get( 666, 1000 );
						}
// 					// dendritic branches
// 						else if ( jmode == 6 )
// 						{
// 							// backup old
// 							unsigned int old = ArchNeurons[nid].dendridicBranches;
// 
// 							ArchNeurons[nid].dendridicBranches = randgen->Instance()->get( 1, maxDendridicBranches );
// 
// 							// make sure we mutate
// 							if ( old == ArchNeurons[nid].dendridicBranches ) runs++;
// 						}

				}
				else
					runs++;
				continue;
			}

		// add a new synapse
			modesum += percentMutateEffectAddSynapse;
			if ( mode <= modesum )
			{
				if ( ArchNeurons.size() > 0 )
				{
					// pick a random neuron
					unsigned int nid = randgen->Instance()->get( 0, ArchNeurons.size()-1 );

					// don't go over maximum connections
					if ( ArchNeurons[nid].ArchSynapses.size() < maxSynapses )
					{
						//cerr << "\t+C " << nid << std::endl;
						addRandomArchSynapse(nid);
					}
					else runs++;
				}
				else
					runs++;
				continue;
			}

		// remove a synapse
			modesum += percentMutateEffectRemoveSynapse;
			if ( mode <= modesum )
			{
				if ( ArchNeurons.size() > 0 )
				{
					// pick a random neuron
					unsigned int nid = randgen->Instance()->get( 0, ArchNeurons.size()-1 );

					// don't go under minimum connections
					if ( ArchNeurons[nid].ArchSynapses.size() > minSynapses )
					{
						//cerr << "\t-C " << nid << std::endl;
						unsigned int sID = randgen->Instance()->get(0, ArchNeurons[nid].ArchSynapses.size()-1);
						//delete ArchNeurons[nid].ArchSynapses[connid];
						ArchNeurons[nid].ArchSynapses.erase(ArchNeurons[nid].ArchSynapses.begin()+sID);
					}
					else
						runs++;
				}
				else
					runs++;
				continue;
			}
// 		// ALTER MUTATIONRATE
// 			modesum += Settings::Instance()->getCVar("brain_percentmutateeffectchangemutationrate");
// 			if ( mode <= modesum )
// 			{
// 				const float pct = 0.01f * randgen->Instance()->get( 1, 90 );
// 				int amount( m_mutationrate * pct );
// 				amount = std::max( amount, 1 );
// 				
// 				if ( randgen->Instance()->get( 0, 1000 ) < 500 || m_mutationrate == Settings::Instance()->getCVar("brain_mutationrate") )
// 					m_mutationrate -= amount;
// 				else
// 				{
// 					m_mutationrate += amount;
// 					if ( m_mutationrate > Settings::Instance()->getCVar("brain_mutationrate") )
// 						m_mutationrate = Settings::Instance()->getCVar("brain_mutationrate");
// 				}
// 				
// 				
// 				continue;
// 			}
// 			
// 		// ALTER MAXMUTATIONS
// 			modesum += Settings::Instance()->getCVar("brain_percentmutateeffectchangemaxmutations");
// 			if ( mode <= modesum )
// 			{
// 				const float pct = 0.01f * randgen->Instance()->get( 1, 90 );
// 				int amount( m_maxmutations * pct );
// 				amount = std::max( amount, 1 );
// 				
// 				if ( randgen->Instance()->get( 0, 1000 ) < 500 || m_maxmutations == Settings::Instance()->getCVar("brain_maxmutations") )
// 					m_maxmutations -= amount;
// 				else
// 				{
// 					m_maxmutations += amount;
// 					if ( m_maxmutations > Settings::Instance()->getCVar("brain_maxmutations") )
// 						m_maxmutations = Settings::Instance()->getCVar("brain_maxmutations");
// 				}
// 				
// 				
// 				continue;
// 			}

		// change a mutatable
			modesum += percentMutateEffectAlterMutable;
			if ( mode <= modesum )
			{
				unsigned int imode = randgen->Instance()->get(1,11);

				if ( imode == 1 && mutate_percentChanceInhibitoryNeuron )
				{
					unsigned int jmode = randgen->Instance()->get(1,2);
					unsigned int factor = randgen->Instance()->get(1,5);

					if ( jmode == 1 && percentChanceInhibitoryNeuron < 100-factor )	percentChanceInhibitoryNeuron+=factor;
					else if ( percentChanceInhibitoryNeuron > 0+factor )		percentChanceInhibitoryNeuron-=factor;
				}

				else if ( imode == 2 && mutate_percentChanceConsistentSynapses )
				{
					unsigned int jmode = randgen->Instance()->get(1,2);
					unsigned int factor = randgen->Instance()->get(1,5);

					if ( jmode == 1 && percentChanceConsistentSynapses < 100-factor )	percentChanceConsistentSynapses+=factor;
					else if ( percentChanceConsistentSynapses > 0+factor )			percentChanceConsistentSynapses-=factor;
				}

				else if ( imode == 3 && mutate_percentChanceInhibitorySynapses )
				{
					unsigned int jmode = randgen->Instance()->get(1,2);
					unsigned int factor = randgen->Instance()->get(1,5);

					if ( jmode == 1 && percentChanceInhibitorySynapses < 100-factor )	percentChanceInhibitorySynapses+=factor;
					else if ( percentChanceInhibitorySynapses > 0+factor )			percentChanceInhibitorySynapses-=factor;
				}

				else if ( imode == 4 && mutate_percentChanceMotorNeuron )
				{
					unsigned int jmode = randgen->Instance()->get(1,2);
					unsigned int factor = randgen->Instance()->get(1,5);

					if ( jmode == 1 && percentChanceMotorNeuron < 100-factor )	percentChanceMotorNeuron+=factor;
					else if ( percentChanceMotorNeuron > 0+factor )			percentChanceMotorNeuron-=factor; // !!! > 1
				}

				else if ( imode == 5 && mutate_percentChancePlasticNeuron )
				{
					unsigned int jmode = randgen->Instance()->get(1,2);
					unsigned int factor = randgen->Instance()->get(1,5);

					if ( jmode == 1 && percentChancePlasticNeuron < 100-factor )	percentChancePlasticNeuron+=factor;
					else if ( percentChancePlasticNeuron > 0+factor )		percentChancePlasticNeuron-=factor;
				}

				else if ( imode == 6 && mutate_percentChanceSensorySynapse )
				{
					unsigned int jmode = randgen->Instance()->get(1,2);
					unsigned int factor = randgen->Instance()->get(1,5);

					if ( jmode == 1 && percentChanceSensorySynapse < 100-factor )	percentChanceSensorySynapse+=factor;
					else if ( percentChanceSensorySynapse > minSynapses+factor )	percentChanceSensorySynapse-=factor;
				}

				else if ( imode == 7 && mutate_minFiringThreshold )
				{
					unsigned int jmode = randgen->Instance()->get(1,2);
					unsigned int factor = randgen->Instance()->get(1,5);

					if ( jmode == 1 && minFiringThreshold < maxFiringThreshold-factor )	minFiringThreshold+=factor; // watch out idd
					else if ( minFiringThreshold > 1+factor )				minFiringThreshold-=factor; // !!! > 1
				}

				else if ( imode == 8 && mutate_maxFiringThreshold )
				{
					unsigned int jmode = randgen->Instance()->get(1,2);
					unsigned int factor = randgen->Instance()->get(1,5);

					if ( jmode == 1 && maxFiringThreshold < 1000-factor )		maxFiringThreshold+=factor;
					else if ( maxFiringThreshold > minFiringThreshold+factor )	maxFiringThreshold-=factor; // !!! > minFiringThreshold
				}

// 				else if ( imode == 9 && mutate_maxDendridicBranches )
// 				{
// 					unsigned int jmode = randgen->Instance()->get(1,2);
// 					unsigned int factor = randgen->Instance()->get(1,5);
// 
// 					if ( jmode == 1 && maxDendridicBranches < 100-factor )	maxDendridicBranches+=factor;
// 					else if ( maxDendridicBranches > 1+factor )		maxDendridicBranches-=factor; // !!! > 1
// 				}

				else if ( imode == 9 && mutate_percentMutation )
				{
					unsigned int jmode = randgen->Instance()->get(1,2);
					unsigned int factor = randgen->Instance()->get(1,5);

					if ( jmode == 1 && percentMutation < 100-factor )	percentMutation+=factor;
					else if ( percentMutation > 1+factor )			percentMutation-=factor; // !!! > 1 or no more mutation at all
				}

				else if ( imode == 10 && mutate_MutateEffects )
				{
					// up or down
					unsigned int jmode = randgen->Instance()->get(1,2);

					// which of 5
					unsigned int kmode = randgen->Instance()->get(1,6);

					if ( jmode == 1 && percentMutateEffectAddNeuron + percentMutateEffectRemoveNeuron + percentMutateEffectAlterNeuron + percentMutateEffectAddSynapse + percentMutateEffectRemoveSynapse + percentMutateEffectAlterMutable <= 100 )
					{
						if ( kmode == 1 )	percentMutateEffectAddNeuron++;
						else if ( kmode == 2 )	percentMutateEffectRemoveNeuron++;
						else if ( kmode == 3 )	percentMutateEffectAlterNeuron++;
						else if ( kmode == 4 )	percentMutateEffectAddSynapse++;
						else if ( kmode == 5 )	percentMutateEffectRemoveSynapse++;
						else if ( kmode == 6 )	percentMutateEffectAlterMutable++;
					}
					else if ( jmode == 2 )
					{
						if ( kmode == 1 && percentMutateEffectAddNeuron > 0 )		percentMutateEffectAddNeuron--;
						else if ( kmode == 2 && percentMutateEffectRemoveNeuron > 0 )	percentMutateEffectRemoveNeuron--;
						else if ( kmode == 3 && percentMutateEffectAlterNeuron > 0 )	percentMutateEffectAlterNeuron--;
						else if ( kmode == 4 && percentMutateEffectAddSynapse > 0 )	percentMutateEffectAddSynapse--;
						else if ( kmode == 5 && percentMutateEffectRemoveSynapse > 0 )	percentMutateEffectRemoveSynapse--;
						else if ( kmode == 6 && percentMutateEffectAlterMutable > 0 )	percentMutateEffectAlterMutable--;
					}
				}

				else if ( imode == 11 && mutate_PlasticityFactors )
				{
					// up or down
					unsigned int jmode = randgen->Instance()->get(1,2);

					// which of 4
					unsigned int kmode = randgen->Instance()->get(1,4);

					// factor
					unsigned int factor = randgen->Instance()->get(10,1000);

					if ( jmode == 1 )
					{
						if ( kmode == 1 && minPlasticityStrengthen+factor < maxPlasticityStrengthen )	minPlasticityStrengthen+=factor;
						else if ( kmode == 2 && maxPlasticityStrengthen+factor < 1000000 )		maxPlasticityStrengthen+=factor;
						else if ( kmode == 3 && minPlasticityWeaken+factor < maxPlasticityWeaken )	minPlasticityWeaken+=factor;
						else if ( kmode == 4 && maxPlasticityWeaken+factor < 1000000 )			maxPlasticityWeaken+=factor;
					}
					else
					{
						if ( kmode == 1 && minPlasticityStrengthen > 10+factor  )				minPlasticityStrengthen-=factor;
						else if ( kmode == 2 && maxPlasticityStrengthen > minPlasticityStrengthen+factor )	maxPlasticityStrengthen-=factor;
						else if ( kmode == 3 && minPlasticityWeaken > 100+factor )				minPlasticityWeaken-=factor;
						else if ( kmode == 4 && maxPlasticityWeaken > minPlasticityWeaken+factor )		maxPlasticityWeaken-=factor;
					}
				}

				else runs++;
			}
			else runs++;
			continue;
		}
	}

// RUN TIME

	void BrainzArch::copyFrom(const BrainzArch* otherBrain)
	{
		maxNeurons				= otherBrain->maxNeurons;
		minSynapses				= otherBrain->minSynapses;
		maxSynapses				= otherBrain->maxSynapses;
		percentChanceInhibitoryNeuron		= otherBrain->percentChanceInhibitoryNeuron;
		mutate_percentChanceInhibitoryNeuron	= otherBrain->mutate_percentChanceInhibitoryNeuron;

		percentChanceMotorNeuron		= otherBrain->percentChanceMotorNeuron;
		mutate_percentChanceMotorNeuron		= otherBrain->mutate_percentChanceMotorNeuron;
		percentChancePlasticNeuron		= otherBrain->percentChancePlasticNeuron;
		mutate_percentChancePlasticNeuron	= otherBrain->mutate_percentChancePlasticNeuron;
		minPlasticityStrengthen			= otherBrain->minPlasticityStrengthen;
		maxPlasticityStrengthen			= otherBrain->maxPlasticityStrengthen;
		minPlasticityWeaken			= otherBrain->minPlasticityWeaken;
		maxPlasticityWeaken			= otherBrain->maxPlasticityWeaken;

		mutate_PlasticityFactors		= otherBrain->mutate_PlasticityFactors;
		minFiringThreshold			= otherBrain->minFiringThreshold;
		mutate_minFiringThreshold		= otherBrain->mutate_minFiringThreshold;
		maxFiringThreshold			= otherBrain->maxFiringThreshold;
		mutate_maxFiringThreshold		= otherBrain->mutate_maxFiringThreshold;
// 		maxDendridicBranches			= otherBrain->maxDendridicBranches;
// 		mutate_maxDendridicBranches		= otherBrain->mutate_maxDendridicBranches;
		percentChanceConsistentSynapses		= otherBrain->percentChanceConsistentSynapses;
		mutate_percentChanceConsistentSynapses	= otherBrain->mutate_percentChanceConsistentSynapses;
		percentChanceInhibitorySynapses		= otherBrain->percentChanceInhibitorySynapses;
		mutate_percentChanceInhibitorySynapses	= otherBrain->mutate_percentChanceInhibitorySynapses;
		percentChanceSensorySynapse		= otherBrain->percentChanceSensorySynapse;
		mutate_percentChanceSensorySynapse	= otherBrain->mutate_percentChanceSensorySynapse;
		percentMutateEffectAddNeuron		= otherBrain->percentMutateEffectAddNeuron;
		percentMutateEffectRemoveNeuron		= otherBrain->percentMutateEffectRemoveNeuron;
		percentMutateEffectAlterNeuron		= otherBrain->percentMutateEffectAlterNeuron;
		percentMutateEffectAddSynapse		= otherBrain->percentMutateEffectAddSynapse;
		percentMutateEffectRemoveSynapse	= otherBrain->percentMutateEffectRemoveSynapse;
		percentMutateEffectAlterMutable		= otherBrain->percentMutateEffectAlterMutable;
		mutate_MutateEffects			= otherBrain->mutate_MutateEffects;

// 		m_maxmutations			= otherBrain->m_maxmutations;
// 		m_mutationrate			= otherBrain->m_mutationrate;
		m_maxmutations					= Settings::Instance()->getCVar("brain_maxmutations");
		m_mutationrate					= Settings::Instance()->getCVar("brain_mutationrate");
		
		for ( unsigned int i = 0; i < otherBrain->ArchNeurons.size(); ++i )
		{
			ArchNeuronz an;
			const ArchNeuronz *oan = &otherBrain->ArchNeurons[i];

			an.isInhibitory = oan->isInhibitory;

			an.hasConsistentSynapses = oan->hasConsistentSynapses;
			an.hasInhibitorySynapses = oan->hasInhibitorySynapses;
			an.firingThreshold = oan->firingThreshold;
			an.potentialDecay = oan->potentialDecay;
// 			an.dendridicBranches = oan->dendridicBranches;

			an.isMotor = oan->isMotor;
			an.motorID = oan->motorID;
			an.isPlastic = oan->isPlastic;
			an.isPlastic = oan->isPlastic;
			an.plasticityStrengthen = oan->plasticityStrengthen;
			an.plasticityWeaken = oan->plasticityWeaken;

			// inputs
			for ( unsigned int j = 0; j < oan->ArchSynapses.size(); ++j )
			{
				ArchSynapse as;
				const ArchSynapse *oas = &oan->ArchSynapses[j];

				as.isSensorNeuron	= oas->isSensorNeuron;
				as.neuronID		= oas->neuronID;
// 				as.dendriteBranch	= oas->dendriteBranch;
				as.weight		= oas->weight;
				an.ArchSynapses.push_back( as );
			}
			ArchNeurons.push_back( an );
		}
	}

	void BrainzArch::mergeFrom(const BrainzArch* otherBrain1, const BrainzArch* otherBrain2)
	{
		maxNeurons				= otherBrain1->maxNeurons;
		minSynapses				= otherBrain1->minSynapses;
		maxSynapses				= otherBrain1->maxSynapses;
		percentChanceInhibitoryNeuron		= otherBrain1->percentChanceInhibitoryNeuron;
		mutate_percentChanceInhibitoryNeuron	= otherBrain1->mutate_percentChanceInhibitoryNeuron;

		percentChanceMotorNeuron		= otherBrain1->percentChanceMotorNeuron;
		mutate_percentChanceMotorNeuron		= otherBrain1->mutate_percentChanceMotorNeuron;
		percentChancePlasticNeuron		= otherBrain1->percentChancePlasticNeuron;
		mutate_percentChancePlasticNeuron	= otherBrain1->mutate_percentChancePlasticNeuron;
		minPlasticityStrengthen			= otherBrain1->minPlasticityStrengthen;
		maxPlasticityStrengthen			= otherBrain1->maxPlasticityStrengthen;
		minPlasticityWeaken			= otherBrain1->minPlasticityWeaken;
		maxPlasticityWeaken			= otherBrain1->maxPlasticityWeaken;

		mutate_PlasticityFactors		= otherBrain1->mutate_PlasticityFactors;
		minFiringThreshold			= otherBrain1->minFiringThreshold;
		mutate_minFiringThreshold		= otherBrain1->mutate_minFiringThreshold;
		maxFiringThreshold			= otherBrain1->maxFiringThreshold;
		mutate_maxFiringThreshold		= otherBrain1->mutate_maxFiringThreshold;
// 		maxDendridicBranches			= otherBrain1->maxDendridicBranches;
// 		mutate_maxDendridicBranches		= otherBrain1->mutate_maxDendridicBranches;
		percentChanceConsistentSynapses		= otherBrain1->percentChanceConsistentSynapses;
		mutate_percentChanceConsistentSynapses	= otherBrain1->mutate_percentChanceConsistentSynapses;
		percentChanceInhibitorySynapses		= otherBrain1->percentChanceInhibitorySynapses;
		mutate_percentChanceInhibitorySynapses	= otherBrain1->mutate_percentChanceInhibitorySynapses;
		percentChanceSensorySynapse		= otherBrain1->percentChanceSensorySynapse;
		mutate_percentChanceSensorySynapse	= otherBrain1->mutate_percentChanceSensorySynapse;
		percentMutateEffectAddNeuron		= otherBrain1->percentMutateEffectAddNeuron;
		percentMutateEffectRemoveNeuron		= otherBrain1->percentMutateEffectRemoveNeuron;
		percentMutateEffectAlterNeuron		= otherBrain1->percentMutateEffectAlterNeuron;
		percentMutateEffectAddSynapse		= otherBrain1->percentMutateEffectAddSynapse;
		percentMutateEffectRemoveSynapse	= otherBrain1->percentMutateEffectRemoveSynapse;
		percentMutateEffectAlterMutable		= otherBrain1->percentMutateEffectAlterMutable;
		mutate_MutateEffects			= otherBrain1->mutate_MutateEffects;

// 		m_maxmutations			= otherBrain1->m_maxmutations;
// 		m_mutationrate			= otherBrain1->m_mutationrate;
		m_maxmutations					= Settings::Instance()->getCVar("brain_maxmutations");
		m_mutationrate					= Settings::Instance()->getCVar("brain_mutationrate");

		// take biggest
		unsigned int loops = otherBrain1->ArchNeurons.size();
		if ( otherBrain2->ArchNeurons.size() < loops ) loops = otherBrain2->ArchNeurons.size();

		bool even = false;
		for ( unsigned int i = 0; i < loops; ++i )
		{
			even = !even;
			ArchNeuronz an;

			const ArchNeuronz *oan;

			if ( i < otherBrain1->ArchNeurons.size() && even )
				oan = &otherBrain1->ArchNeurons[i];

			else //if ( i < otherBrain2.ArchNeurons.size() && !even )
				oan = &otherBrain2->ArchNeurons[i];

			an.isInhibitory = oan->isInhibitory;

			an.hasConsistentSynapses = oan->hasConsistentSynapses;
			an.hasInhibitorySynapses = oan->hasInhibitorySynapses;
			an.firingThreshold = oan->firingThreshold;
			an.potentialDecay = oan->potentialDecay;
// 			an.dendridicBranches = oan->dendridicBranches;

			an.isMotor = oan->isMotor;
			an.motorID = oan->motorID;
			an.isPlastic = oan->isPlastic;
			an.isPlastic = oan->isPlastic;
			an.plasticityStrengthen = oan->plasticityStrengthen;
			an.plasticityWeaken = oan->plasticityWeaken;

			// inputs
			for ( unsigned int j = 0; j < oan->ArchSynapses.size(); ++j )
			{
				ArchSynapse as;
				const ArchSynapse *oas = &oan->ArchSynapses[j];

				as.isSensorNeuron	= oas->isSensorNeuron;
				as.neuronID		= oas->neuronID;
// 				as.dendriteBranch	= oas->dendriteBranch;
				as.weight		= oas->weight;
				an.ArchSynapses.push_back( as );
			}
			ArchNeurons.push_back( an );
		}
	}

// SERIALIZE FUNC

	void BrainzArch::setArch(std::string& content)
	{
		contentParser.reset();
		std::string line = contentParser.returnUntillStrip( '\n', content );
		while ( !line.empty() )
		{
			parseH.reset();
			// neuron(ft=24|iwr=20|mtr=4|inputs(|s,78,6|s,186,-12|s,123,10|n,19,5|n,3,3|n,11,-19));

			if ( parseH.beginMatchesStrip( "n(", line ) )
			{
				ArchNeuronz an;
//				unsigned int nid = (ArchNeurons.size()-1);
	
				if ( parseH.beginMatchesStrip( "i=", line ) )
				{
					std::string II = parseH.returnUntillStrip( '|', line );
					//cerr << "II: " << II  << std::endl;
					int isIt;
					if(EOF == sscanf(II.c_str(), "%d", &isIt))				std::cerr << "ERROR INSERTING CRITTER" << std::endl;
					if ( isIt == 1 ) an.isInhibitory = true;
				}

				if ( parseH.beginMatchesStrip( "cs=", line ) )
				{
					std::string CS = parseH.returnUntillStrip( '|', line );
					//cerr << "II: " << II  << std::endl;
					int isItC;
					if(EOF == sscanf(CS.c_str(), "%d", &isItC))				std::cerr << "ERROR INSERTING CRITTER" << std::endl;
					if ( isItC == 1 ) an.hasConsistentSynapses = true;

					if ( parseH.beginMatchesStrip( "is=", line ) )
					{
						std::string IS = parseH.returnUntillStrip( '|', line );
						//cerr << "II: " << II  << std::endl;
						int isIt;
						if(EOF == sscanf(IS.c_str(), "%d", &isIt))				std::cerr << "ERROR INSERTING CRITTER" << std::endl;
						if ( isIt == 1 ) an.hasInhibitorySynapses = true;
					}
				}

				if ( parseH.beginMatchesStrip( "f=", line ) )
				{
					std::string FT = parseH.returnUntillStrip( '|', line );
					//cerr << "FT: " << FT  << std::endl;
					if(EOF == sscanf(FT.c_str(), "%d", &an.firingThreshold))		std::cerr << "ERROR INSERTING CRITTER" << std::endl;
				}
	
				if ( parseH.beginMatchesStrip( "pd=", line ) )
				{
					std::string PD = parseH.returnUntillStrip( '|', line );
					//cerr << "FT: " << FT  << std::endl;
					if(EOF == sscanf(PD.c_str(), "%d", &an.potentialDecay))		std::cerr << "ERROR INSERTING CRITTER" << std::endl;
				}
	
// 				if ( parseH.beginMatchesStrip( "d=", line ) )
// 				{
// 					string DB = parseH.returnUntillStrip( "|", line );
// 					//cerr << "IWR: " << IWR  << std::endl;
// 					if(EOF == sscanf(DB.c_str(), "%d", &an.dendridicBranches))		std::cerr << "ERROR INSERTING CRITTER" << std::endl;
// 				}
	
				if ( parseH.beginMatchesStrip( "m=", line ) )
				{
					std::string MTR = parseH.returnUntillStrip( '|', line );
					//cerr << "MTR: " << MTR  << std::endl;
					if(EOF == sscanf(MTR.c_str(), "%d", &an.motorID))			std::cerr << "ERROR INSERTING CRITTER" << std::endl;
					an.isMotor = true;
				}

				if ( parseH.beginMatchesStrip( "p=", line ) )
				{
					std::string PLAS = parseH.returnUntillStrip( ',', line );
					std::string PLAW = parseH.returnUntillStrip( '|', line );
					if(EOF == sscanf(PLAS.c_str(), "%d", &an.plasticityStrengthen))			std::cerr << "ERROR INSERTING CRITTER" << std::endl;
					if(EOF == sscanf(PLAW.c_str(), "%d", &an.plasticityWeaken))			std::cerr << "ERROR INSERTING CRITTER" << std::endl;
					an.isPlastic = true;
				}

				while ( parseH.beginMatchesStrip( "s=", line ) )
				{
						ArchSynapse as;

						std::string isSensorNeuron	= parseH.returnUntillStrip( ',', line );
						std::string neuronID		= parseH.returnUntillStrip( ',', line );
						std::string weight		= parseH.returnUntillStrip( '|', line );

						int isIt;
						if(EOF == sscanf(isSensorNeuron.c_str(), "%d", &isIt))			std::cerr << "ERROR INSERTING CRITTER" << std::endl;
						if ( isIt == 1 ) as.isSensorNeuron = true;
	
						if(EOF == sscanf(neuronID.c_str(), "%d", &as.neuronID))			std::cerr << "ERROR INSERTING CRITTER" << std::endl;
// 						if(EOF == sscanf(dendriteBranch.c_str(), "%d", &as.dendriteBranch))	std::cerr << "ERROR INSERTING CRITTER" << std::endl;
						if(EOF == sscanf(weight.c_str(), "%f", &as.weight))			std::cerr << "ERROR INSERTING CRITTER" << std::endl;
						an.ArchSynapses.push_back( as );
				}

				ArchNeurons.push_back( an );
			}

// 			else if ( parseH.beginMatchesStrip( "numberOfInputs=", line ) )
// 			{
// 				std::string Holder = parseH.returnUntillStrip( ";", line );
// 				if(EOF == sscanf(Holder.c_str(), "%d", &numberOfInputs))				std::cerr << "ERROR INSERTING CRITTER" << std::endl;
// 			}

// 			else if ( parseH.beginMatchesStrip( "numberOfOutputs=", line ) )
// 			{
// 				std::string Holder = parseH.returnUntillStrip( ";", line );
// 				if(EOF == sscanf(Holder.c_str(), "%d", &numberOfOutputs))				std::cerr << "ERROR INSERTING CRITTER" << std::endl;
// 			}

			else if ( parseH.beginMatchesStrip( "maxNeurons=", line ) )
			{
				std::string Holder = parseH.returnUntillStrip( ';', line );
				if(EOF == sscanf(Holder.c_str(), "%d", &maxNeurons))					std::cerr << "ERROR INSERTING CRITTER" << std::endl;
			}

			else if ( parseH.beginMatchesStrip( "minSynapses=", line ) )
			{
				std::string Holder = parseH.returnUntillStrip( ';', line );
				if(EOF == sscanf(Holder.c_str(), "%d", &minSynapses))					std::cerr << "ERROR INSERTING CRITTER" << std::endl;
			}

			else if ( parseH.beginMatchesStrip( "maxSynapses=", line ) )
			{
				std::string Holder = parseH.returnUntillStrip( ';', line );
				if(EOF == sscanf(Holder.c_str(), "%d", &maxSynapses))					std::cerr << "ERROR INSERTING CRITTER" << std::endl;
			}

			else if ( parseH.beginMatchesStrip( "percentChanceInhibitoryNeuron=", line ) )
			{
				std::string Holder = parseH.returnUntillStrip( ';', line );
				if(EOF == sscanf(Holder.c_str(), "%d", &percentChanceInhibitoryNeuron))			std::cerr << "ERROR INSERTING CRITTER" << std::endl;
			}
				else if ( parseH.beginMatchesStrip( "mutate_percentChanceInhibitoryNeuron=", line ) )
				{
					std::string Holder = parseH.returnUntillStrip( ';', line );
					int result;
					if(EOF == sscanf(Holder.c_str(), "%d", &result))			std::cerr << "ERROR INSERTING CRITTER" << std::endl;
					if ( result == 1 )	mutate_percentChanceInhibitoryNeuron = true;
					else			mutate_percentChanceInhibitoryNeuron = false;
				}

			else if ( parseH.beginMatchesStrip( "percentChanceConsistentSynapses=", line ) )
			{
				std::string Holder = parseH.returnUntillStrip( ';', line );
				if(EOF == sscanf(Holder.c_str(), "%d", &percentChanceConsistentSynapses))		std::cerr << "ERROR INSERTING CRITTER" << std::endl;
			}
				else if ( parseH.beginMatchesStrip( "mutate_percentChanceConsistentSynapses=", line ) )
				{
					std::string Holder = parseH.returnUntillStrip( ';', line );
					int result;
					if(EOF == sscanf(Holder.c_str(), "%d", &result))			std::cerr << "ERROR INSERTING CRITTER" << std::endl;
					if ( result == 1 )	mutate_percentChanceConsistentSynapses = true;
					else			mutate_percentChanceConsistentSynapses = false;
				}

			else if ( parseH.beginMatchesStrip( "percentChanceInhibitorySynapses=", line ) )
			{
				std::string Holder = parseH.returnUntillStrip( ';', line );
				if(EOF == sscanf(Holder.c_str(), "%d", &percentChanceInhibitorySynapses))		std::cerr << "ERROR INSERTING CRITTER" << std::endl;
			}
				else if ( parseH.beginMatchesStrip( "mutate_percentChanceInhibitorySynapses=", line ) )
				{
					std::string Holder = parseH.returnUntillStrip( ';', line );
					int result;
					if(EOF == sscanf(Holder.c_str(), "%d", &result))			std::cerr << "ERROR INSERTING CRITTER" << std::endl;
					if ( result == 1 )	mutate_percentChanceInhibitorySynapses = true;
					else			mutate_percentChanceInhibitorySynapses = false;
				}

			else if ( parseH.beginMatchesStrip( "percentChanceMotorNeuron=", line ) )
			{
				std::string Holder = parseH.returnUntillStrip( ';', line );
				if(EOF == sscanf(Holder.c_str(), "%d", &percentChanceMotorNeuron))			std::cerr << "ERROR INSERTING CRITTER" << std::endl;
			}
				else if ( parseH.beginMatchesStrip( "mutate_percentChanceMotorNeuron=", line ) )
				{
					std::string Holder = parseH.returnUntillStrip( ';', line );
					int result;
					if(EOF == sscanf(Holder.c_str(), "%d", &result))			std::cerr << "ERROR INSERTING CRITTER" << std::endl;
					if ( result == 1 )	mutate_percentChanceMotorNeuron = true;
					else			mutate_percentChanceMotorNeuron = false;
				}

			else if ( parseH.beginMatchesStrip( "percentChancePlasticNeuron=", line ) )
			{
				std::string Holder = parseH.returnUntillStrip( ';', line );
				if(EOF == sscanf(Holder.c_str(), "%d", &percentChancePlasticNeuron))			std::cerr << "ERROR INSERTING CRITTER" << std::endl;
			}
				else if ( parseH.beginMatchesStrip( "mutate_percentChancePlasticNeuron=", line ) )
				{
					std::string Holder = parseH.returnUntillStrip( ';', line );
					int result;
					if(EOF == sscanf(Holder.c_str(), "%d", &result))			std::cerr << "ERROR INSERTING CRITTER" << std::endl;
					if ( result == 1 )	mutate_percentChancePlasticNeuron = true;
					else			mutate_percentChancePlasticNeuron = false;
				}

			else if ( parseH.beginMatchesStrip( "percentChanceSensorySynapse=", line ) )
			{
				std::string Holder = parseH.returnUntillStrip( ';', line );
				if(EOF == sscanf(Holder.c_str(), "%d", &percentChanceSensorySynapse))			std::cerr << "ERROR INSERTING CRITTER" << std::endl;
			}
				else if ( parseH.beginMatchesStrip( "mutate_percentChanceSensorySynapse=", line ) )
				{
					std::string Holder = parseH.returnUntillStrip( ';', line );
					int result;
					if(EOF == sscanf(Holder.c_str(), "%d", &result))			std::cerr << "ERROR INSERTING CRITTER" << std::endl;
					if ( result == 1 )	mutate_percentChanceSensorySynapse = true;
					else			mutate_percentChanceSensorySynapse = false;
				}

			else if ( parseH.beginMatchesStrip( "minFiringThreshold=", line ) )
			{
				std::string Holder = parseH.returnUntillStrip( ';', line );
				if(EOF == sscanf(Holder.c_str(), "%d", &minFiringThreshold))				std::cerr << "ERROR INSERTING CRITTER" << std::endl;
			}
				else if ( parseH.beginMatchesStrip( "mutate_minFiringThreshold=", line ) )
				{
					std::string Holder = parseH.returnUntillStrip( ';', line );
					int result;
					if(EOF == sscanf(Holder.c_str(), "%d", &result))			std::cerr << "ERROR INSERTING CRITTER" << std::endl;
					if ( result == 1 )	mutate_minFiringThreshold = true;
					else			mutate_minFiringThreshold = false;
				}

			else if ( parseH.beginMatchesStrip( "maxFiringThreshold=", line ) )
			{
				std::string Holder = parseH.returnUntillStrip( ';', line );
				if(EOF == sscanf(Holder.c_str(), "%d", &maxFiringThreshold))				std::cerr << "ERROR INSERTING CRITTER" << std::endl;
			}
				else if ( parseH.beginMatchesStrip( "mutate_maxFiringThreshold=", line ) )
				{
					std::string Holder = parseH.returnUntillStrip( ';', line );
					int result;
					if(EOF == sscanf(Holder.c_str(), "%d", &result))			std::cerr << "ERROR INSERTING CRITTER" << std::endl;
					if ( result == 1 )	mutate_maxFiringThreshold = true;
					else			mutate_maxFiringThreshold = false;
				}
				
				
// 			else if ( parseH.beginMatchesStrip( "maxDendridicBranches=", line ) )
// 			{
// 				std::string Holder = parseH.returnUntillStrip( ";", line );
// 				if(EOF == sscanf(Holder.c_str(), "%d", &maxDendridicBranches))				std::cerr << "ERROR INSERTING CRITTER" << std::endl;
// 			}
// 				else if ( parseH.beginMatchesStrip( "mutate_maxDendridicBranches=", line ) )
// 				{
// 					std::string Holder = parseH.returnUntillStrip( ";", line );
// 					int result;
// 					if(EOF == sscanf(Holder.c_str(), "%d", &result))			std::cerr << "ERROR INSERTING CRITTER" << std::endl;
// 					if ( result == 1 )	mutate_maxDendridicBranches = true;
// 					else			mutate_maxDendridicBranches = false;
// 				}

// 			else if ( parseH.beginMatchesStrip( "percentMutation=", line ) )
// 			{
// 				std::string Holder = parseH.returnUntillStrip( ";", line );
// 				if(EOF == sscanf(Holder.c_str(), "%d", &percentMutation))				std::cerr << "ERROR INSERTING CRITTER" << std::endl;
// 			}
// 				else if ( parseH.beginMatchesStrip( "mutate_percentMutation=", line ) )
// 				{
// 					std::string Holder = parseH.returnUntillStrip( ";", line );
// 					int result;
// 					if(EOF == sscanf(Holder.c_str(), "%d", &result))			std::cerr << "ERROR INSERTING CRITTER" << std::endl;
// 					if ( result == 1 )	mutate_percentMutation = true;
// 					else			mutate_percentMutation = false;
// 				}

			else if ( parseH.beginMatchesStrip( "percentMutateEffectAddNeuron=", line ) )
			{
				std::string Holder = parseH.returnUntillStrip( ';', line );
				if(EOF == sscanf(Holder.c_str(), "%d", &percentMutateEffectAddNeuron))				std::cerr << "ERROR INSERTING CRITTER" << std::endl;
			}

			else if ( parseH.beginMatchesStrip( "percentMutateEffectRemoveNeuron=", line ) )
			{
				std::string Holder = parseH.returnUntillStrip( ';', line );
				if(EOF == sscanf(Holder.c_str(), "%d", &percentMutateEffectRemoveNeuron))				std::cerr << "ERROR INSERTING CRITTER" << std::endl;
			}

			else if ( parseH.beginMatchesStrip( "percentMutateEffectAlterNeuron=", line ) )
			{
				std::string Holder = parseH.returnUntillStrip( ';', line );
				if(EOF == sscanf(Holder.c_str(), "%d", &percentMutateEffectAlterNeuron))				std::cerr << "ERROR INSERTING CRITTER" << std::endl;
			}

			else if ( parseH.beginMatchesStrip( "percentMutateEffectAddSynapse=", line ) )
			{
				std::string Holder = parseH.returnUntillStrip( ';', line );
				if(EOF == sscanf(Holder.c_str(), "%d", &percentMutateEffectAddSynapse))				std::cerr << "ERROR INSERTING CRITTER" << std::endl;
			}

			else if ( parseH.beginMatchesStrip( "percentMutateEffectRemoveSynapse=", line ) )
			{
				std::string Holder = parseH.returnUntillStrip( ';', line );
				if(EOF == sscanf(Holder.c_str(), "%d", &percentMutateEffectRemoveSynapse))				std::cerr << "ERROR INSERTING CRITTER" << std::endl;
			}

			else if ( parseH.beginMatchesStrip( "percentMutateEffectAlterMutable=", line ) )
			{
				std::string Holder = parseH.returnUntillStrip( ';', line );
				if(EOF == sscanf(Holder.c_str(), "%d", &percentMutateEffectAlterMutable))				std::cerr << "ERROR INSERTING CRITTER" << std::endl;
			}


				else if ( parseH.beginMatchesStrip( "mutate_MutateEffects=", line ) )
				{
					std::string Holder = parseH.returnUntillStrip( ';', line );
					int result;
					if(EOF == sscanf(Holder.c_str(), "%d", &result))			std::cerr << "ERROR INSERTING CRITTER" << std::endl;
					if ( result == 1 )	mutate_MutateEffects = true;
					else			mutate_MutateEffects = false;
				}

			else if ( parseH.beginMatchesStrip( "minPlasticityStrengthen=", line ) )
			{
				std::string Holder = parseH.returnUntillStrip( ';', line );
				if(EOF == sscanf(Holder.c_str(), "%d", &minPlasticityStrengthen))				std::cerr << "ERROR INSERTING CRITTER" << std::endl;
			}
			else if ( parseH.beginMatchesStrip( "maxPlasticityStrengthen=", line ) )
			{
				std::string Holder = parseH.returnUntillStrip( ';', line );
				if(EOF == sscanf(Holder.c_str(), "%d", &maxPlasticityStrengthen))				std::cerr << "ERROR INSERTING CRITTER" << std::endl;
			}
			else if ( parseH.beginMatchesStrip( "minPlasticityWeaken=", line ) )
			{
				std::string Holder = parseH.returnUntillStrip( ';', line );
				if(EOF == sscanf(Holder.c_str(), "%d", &minPlasticityWeaken))				std::cerr << "ERROR INSERTING CRITTER" << std::endl;
			}
			else if ( parseH.beginMatchesStrip( "maxPlasticityWeaken=", line ) )
			{
				std::string Holder = parseH.returnUntillStrip( ';', line );
				if(EOF == sscanf(Holder.c_str(), "%d", &maxPlasticityWeaken))				std::cerr << "ERROR INSERTING CRITTER" << std::endl;
			}

				else if ( parseH.beginMatchesStrip( "mutate_PlasticityFactors=", line ) )
				{
					std::string Holder = parseH.returnUntillStrip( ';', line );
					int result;
					if(EOF == sscanf(Holder.c_str(), "%d", &result))			std::cerr << "ERROR INSERTING CRITTER" << std::endl;
					if ( result == 1 )	mutate_PlasticityFactors = true;
					else			mutate_PlasticityFactors = false;
				}


			else if ( parseH.beginMatchesStrip( "brain_maxmutations=", line ) )
			{
				std::string Holder = parseH.returnUntillStrip( ';', line );
				if(EOF == sscanf(Holder.c_str(), "%d", &m_maxmutations))				std::cerr << "ERROR INSERTING CRITTER" << std::endl;
			}
			else if ( parseH.beginMatchesStrip( "brain_mutationrate=", line ) )
			{
				std::string Holder = parseH.returnUntillStrip( ';', line );
				if(EOF == sscanf(Holder.c_str(), "%d", &m_mutationrate))				std::cerr << "ERROR INSERTING CRITTER" << std::endl;
			}
				
			line = contentParser.returnUntillStrip( '\n', content );
		}
	}
	
	std::string* BrainzArch::getArch()
	{
			std::stringstream buf;
	
			buf << "maxNeurons="				<< maxNeurons << ";" << std::endl;
			buf << "minSynapses="				<< minSynapses << ";" << std::endl;
			buf << "maxSynapses="				<< maxSynapses << ";" << std::endl;

			buf << "percentChanceInhibitoryNeuron="		<< percentChanceInhibitoryNeuron << ";" << std::endl;
				buf << "mutate_percentChanceInhibitoryNeuron="	<< mutate_percentChanceInhibitoryNeuron << ";" << std::endl;
			buf << "percentChanceMotorNeuron="		<< percentChanceMotorNeuron << ";" << std::endl;
				buf << "mutate_percentChanceMotorNeuron="	<< mutate_percentChanceMotorNeuron << ";" << std::endl;
			buf << "percentChancePlasticNeuron="		<< percentChancePlasticNeuron << ";" << std::endl;
				buf << "mutate_percentChancePlasticNeuron="	<< mutate_percentChancePlasticNeuron << ";" << std::endl;
			buf << "minPlasticityStrengthen="		<< minPlasticityStrengthen << ";" << std::endl;
			buf << "maxPlasticityStrengthen="		<< maxPlasticityStrengthen << ";" << std::endl;
			buf << "minPlasticityWeaken="			<< minPlasticityWeaken << ";" << std::endl;
			buf << "maxPlasticityWeaken="			<< maxPlasticityWeaken << ";" << std::endl;
				buf << "mutate_PlasticityFactors="		<< mutate_PlasticityFactors << ";" << std::endl;

			buf << "minFiringThreshold="			<< minFiringThreshold << ";" << std::endl;
				buf << "mutate_minFiringThreshold="		<< mutate_minFiringThreshold << ";" << std::endl;
			buf << "maxFiringThreshold="			<< maxFiringThreshold << ";" << std::endl;
				buf << "mutate_maxFiringThreshold="		<< mutate_maxFiringThreshold << ";" << std::endl;
// 			buf << "maxDendridicBranches="			<< maxDendridicBranches << ";" << std::endl;
// 				buf << "mutate_maxDendridicBranches="		<< mutate_maxDendridicBranches << ";" << std::endl;
	
			buf << "percentChanceConsistentSynapses="	<< percentChanceConsistentSynapses << ";" << std::endl;
				buf << "mutate_percentChanceConsistentSynapses="<< mutate_percentChanceConsistentSynapses << ";" << std::endl;
			buf << "percentChanceInhibitorySynapses="	<< percentChanceInhibitorySynapses << ";" << std::endl;
				buf << "mutate_percentChanceInhibitorySynapses="<< mutate_percentChanceInhibitorySynapses << ";" << std::endl;
			buf << "percentChanceSensorySynapse="		<< percentChanceSensorySynapse << ";" << std::endl;
				buf << "mutate_percentChanceSensorySynapse="	<< mutate_percentChanceSensorySynapse << ";" << std::endl;

			buf << "percentMutateEffectAddNeuron="		<< percentMutateEffectAddNeuron << ";" << std::endl;
			buf << "percentMutateEffectRemoveNeuron="	<< percentMutateEffectRemoveNeuron << ";" << std::endl;
			buf << "percentMutateEffectAlterNeuron="	<< percentMutateEffectAlterNeuron << ";" << std::endl;
			buf << "percentMutateEffectAddSynapse="		<< percentMutateEffectAddSynapse << ";" << std::endl;
			buf << "percentMutateEffectRemoveSynapse="	<< percentMutateEffectRemoveSynapse << ";" << std::endl;
			buf << "percentMutateEffectAlterMutable="	<< percentMutateEffectAlterMutable << ";" << std::endl;

				buf << "mutate_MutateEffects="			<< mutate_MutateEffects << ";" << std::endl;
	
			buf << "brain_maxmutations="	<< m_maxmutations << ";" << std::endl;
			buf << "brain_mutationrate="	<< m_mutationrate << ";" << std::endl;
	
			// neuronal arch & connections
			for ( unsigned int i = 0; i < ArchNeurons.size(); ++i )
			{
				// neuron info
				buf << "n(";
	
				if ( ArchNeurons[i].isInhibitory )
					buf << "i=1|";
				else
					buf << "i=0|";
	
				if ( ArchNeurons[i].hasConsistentSynapses )
				{
					buf << "cs=1|";
					if ( ArchNeurons[i].hasInhibitorySynapses )
						buf << "is=1|";
					else
						buf << "is=0|";
				}
				else
					buf << "cs=0|";
	
				buf << "f=" << ArchNeurons[i].firingThreshold << "|";
				buf << "pd=" << ArchNeurons[i].potentialDecay << "|";
// 				buf << "d=" << ArchNeurons[i].dendridicBranches << "|";
				if ( ArchNeurons[i].isMotor ) buf << "m=" << ArchNeurons[i].motorID << "|";
				if ( ArchNeurons[i].isPlastic ) buf << "p=" << ArchNeurons[i].plasticityStrengthen << "," << ArchNeurons[i].plasticityWeaken << "|";
		
				// inputs
				for ( unsigned int j = 0; j < ArchNeurons[i].ArchSynapses.size(); ++j )
				{
					buf << "s=";
						// sensor or inter neuron
						if ( ArchNeurons[i].ArchSynapses[j].isSensorNeuron )
							buf << "1";
						else
							buf << "0";
						buf << "," << ArchNeurons[i].ArchSynapses[j].neuronID;
// 						buf << "," << ArchNeurons[i].ArchSynapses[j].dendriteBranch;
						buf << "," << ArchNeurons[i].ArchSynapses[j].weight;
					buf << "|";
				}
				buf << ");\n";
			}

		archBuffer = buf.str();
		return &archBuffer;
	}

BrainzArch::~BrainzArch()
{
}
