#include "brainz.h"
#include <cstdio>
// #include <omp.h>

Brainz::Brainz()
{
		neuronsFired					= 0;

	// build time defaults

		numberOfInputs					= 0;
		numberOfOutputs					= 0;
}

// BUILD TIME

	void Brainz::registerInput(unsigned int id)
	{
		Inputs.push_back( sensorNeuron() );
		Inputs[numberOfInputs].id = id;
		++numberOfInputs;
	}

	void Brainz::registerOutput(bool* var, unsigned int id)
	{
		Outputs.push_back( motorNeuron() );
		Outputs[numberOfOutputs].output = var;
		Outputs[numberOfOutputs].id = id;
		++numberOfOutputs;
	}

	void Brainz::wireArch(BrainzArch* brainzArch)
	{

		// clear everything
			Neurons.clear();
// 			Inputs.clear();
// 			Outputs.clear();

			// we know the amount of neurons already, reset totalsynapses for the count later
			totalNeurons = brainzArch->ArchNeurons.size();
			totalSynapses = 0;

		// create all runtime neurons
			for ( unsigned int i=0; i < totalNeurons; ++i )
			{
				auto& t_archneuron( brainzArch->ArchNeurons[i] );

// 				Neurons.push_back( NeuronInterz( t_archneuron.isInhibitory, t_archneuron.isPlastic, t_archneuron.firingThreshold, 0.001f * t_archneuron.potentialDecay ) );
				

// 				ni.isMotor		= t_archneuron.isMotor;
				if (t_archneuron.isMotor)
				{
					const auto mneuron		= brainzArch->findMotorNeuron(t_archneuron.motorID);
					
					if ( t_archneuron.isPlastic )
						Neurons.push_back( new NeuronInterz_Plastic( t_archneuron.isInhibitory, t_archneuron.firingThreshold, 0.001f * t_archneuron.potentialDecay, mneuron, Outputs[mneuron].output, 1.0f+(1.0f/t_archneuron.plasticityStrengthen), 1.0f-(1.0f/t_archneuron.plasticityWeaken) ) );
					else
						Neurons.push_back( new NeuronInterz( t_archneuron.isInhibitory, /*t_archneuron.isPlastic, */t_archneuron.firingThreshold, 0.001f * t_archneuron.potentialDecay, mneuron, Outputs[mneuron].output ) );
				}
				else
				{
					if ( t_archneuron.isPlastic )
						Neurons.push_back( new NeuronInterz_Plastic( t_archneuron.isInhibitory, t_archneuron.firingThreshold, 0.001f * t_archneuron.potentialDecay, 1.0f+(1.0f/t_archneuron.plasticityStrengthen), 1.0f-(1.0f/t_archneuron.plasticityWeaken) ) );
					else
						Neurons.push_back( new NeuronInterz( t_archneuron.isInhibitory, /*t_archneuron.isPlastic, */t_archneuron.firingThreshold, 0.001f * t_archneuron.potentialDecay ) );
				}

// 				NeuronInterz& ni(*Neurons.rbegin());

// 				ni.isPlastic		= t_archneuron.isPlastic;
// 				if ( t_archneuron.isPlastic )
// 				{
// 					ni.plasticityStrengthen	= 1.0f+(1.0f/t_archneuron.plasticityStrengthen);
// 					ni.plasticityWeaken	= 1.0f-(1.0f/t_archneuron.plasticityWeaken);
// 				}

// 				Neurons.push_back( ni );
			}
		// create their synapses & link them to their inputneurons
			for ( unsigned int i=0; i < totalNeurons; ++i )
			{
				auto& slist( brainzArch->ArchNeurons[i].ArchSynapses );
				auto& n( Neurons[i] );
				
				// count connections
				totalSynapses += slist.size();

				for ( unsigned int j=0; j < slist.size(); ++j )
				{
					auto& t_synapse( slist[j] );
					if ( t_synapse.isSensorNeuron )
					{
						t_synapse.realneuronID = brainzArch->findSensorNeuron(t_synapse.neuronID);
// 						n->connec( &Inputs[ t_synapse.realneuronID ].output, 
						n->connec( Inputs[ t_synapse.realneuronID ].output, 
// 							t_synapse.dendriteBranch,
							t_synapse.weight );
					}
					else
// 						n->connec( Neurons[ t_synapse.neuronID ]->outputPointer(), 
// // 							t_synapse.dendriteBranch,
// 							t_synapse.weight );
						n->connec( Neurons[ t_synapse.neuronID ]->getOutput(), 
// 							t_synapse.dendriteBranch,
							t_synapse.weight );
				}
			}

			brainzArch->totalSynapses = totalSynapses;
			
			m_neurons_end = Neurons.end();

			//cerr << "total neurons: " << totalNeurons << "total synapses: " << totalSynapses << endl;
	}


// RUN TIME

// 	void Brainz::clearInputs()
// 	{
// 		const auto& end(Inputs.end());
// 		for ( auto it(Inputs.begin()); it != end; ++it )
// 		{
// 			it->output = 0;
// 		}
// 	}

	void Brainz::process()
	{
		// reset fired neurons counter
		neuronsFired = 0;
		motorneuronsFired = 0;
		
		const auto& end(Outputs.size());
		for ( unsigned int i(0); i < end; ++i )
			*Outputs[i].output = false;

		
		
		
		
		
// // 		const std::vector<NeuronInterz*> t_Neurons(Neurons);
// 		const unsigned int numNeurons = Neurons.size();
// 		unsigned int i;
// 		unsigned int t_neuronsFired(0);
// 		unsigned int t_motorneuronsFired(0);
// 		
// // 		omp_set_num_threads(2);
// 		#pragma omp parallel for
// 		for (i = 0; i < numNeurons; ++i)
// 		{
// 			Neurons[i]->process();
// 		}
// 	
// 		for (i = 0; i < numNeurons; ++i)
// 		{
// 			NeuronInterz* n(Neurons[i]);
// 			// if neuron fires
// 			if ( n->fired() )
// 			{
// 				++t_neuronsFired;
// 
// 				// motor neuron check & exec
// 				if ( n->motor() )
// 				{
// 					++t_motorneuronsFired;
// 					n->enableMotorOoutputPointer();
// 				}
// 			}
// 		}
// 		
// 		neuronsFired = t_neuronsFired;
// 		motorneuronsFired = t_motorneuronsFired;
		
		
		
		for ( m_neurons_it = Neurons.begin(); m_neurons_it != m_neurons_end; ++m_neurons_it )
		{
			NeuronInterz* n(*m_neurons_it);
	
			n->process();
	
			// if neuron fires
			if ( n->fired() )
			{
				++neuronsFired;

				// motor neuron check & exec
				if ( n->motor() )
				{
					++motorneuronsFired;
					n->enableMotorOoutputPointer();
				}
			}
		}

// 		const unsigned int numNeurons = Neurons.size();
// 		unsigned int i;
//  
// 		#pragma omp parallel for
// 		for (i = 0; i < numNeurons; ++i)
// 		{
// 			Neurons[i]->commitOutput();
// 		}

	
// 		// commit outputs at the end
// 		auto t_neurons_it;
// 		std::vector<NeuronInterz*>::iterator t_end = m_neurons_end;
// 		#pragma omp parallel for private(t_neurons_it,t_end) 
// 		for ( t_neurons_it = Neurons.begin(); t_neurons_it != t_end; ++t_neurons_it )
// 		{
// 			(*t_neurons_it)->commitOutput();
// 		}
// 		for ( m_neurons_it = Neurons.begin(); m_neurons_it != t_end; ++m_neurons_it )

		
		
		
		
		
		for ( m_neurons_it = Neurons.begin(); m_neurons_it != m_neurons_end; ++m_neurons_it )
			(*m_neurons_it)->commitOutput();
		
	}

	void Brainz::processTillAnswer()
	{
//		neuronsFired = 0;
	
		// clear Motor Outputs
		for ( unsigned int i=0; i < numberOfOutputs; ++i )
			Outputs[i].output = 0;
	
		// clear Neurons
		for ( unsigned int i=0; i < totalNeurons; ++i )
		{
			Neurons[i]->reset();
		}
	
		unsigned int counter = 0;
		bool motorFired = false;
	
		while ( counter < 1000 && !motorFired )
		{
			for ( unsigned int i=0; i < totalNeurons; ++i )
			{
				NeuronInterz* n = Neurons[i];
		
				n->process();
		
				// if neuron fires
				if ( n->fired() )
				{
					++neuronsFired;
		
					// motor neuron check & exec
					if ( n->motor() )
					{
						motorFired = true;
						n->enableMotorOoutputPointer();
// 						*Outputs[n->motorFunc].output = true;
						//cerr << "neuron " << i << " fired, motor is " << Neurons[i]->MotorFunc << " total now " << Outputs[Neurons[i]->MotorFunc]->output << endl;
					}
				}
			}
			// commit outputs at the end
			for ( unsigned int i=0; i < totalNeurons; ++i )
			{
				Neurons[i]->commitOutput();
// 				NeuronInterz& n(Neurons[i]);
// 	// 			n = &Neurons[i];
// 				n.output = n.waitoutput;
			}
	
			++counter;
		}
	}

Brainz::~Brainz()
{
	for ( unsigned int i=0; i < totalNeurons; ++i )
		delete Neurons[i];
}
