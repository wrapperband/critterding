#ifndef BRAINZ_H
#define BRAINZ_H

// internal includes
	#include "brainzarch.h"
	#include "neuroninterz.h"

// external includes
	#include <vector>
// 	#include <sstream>
	#include <iostream>

struct motorNeuron
{
	bool*		output;
	unsigned int	id;
};

struct sensorNeuron
{
	float		output;
	unsigned int	id;
};

class Brainz
{
	public:
		Brainz();
		virtual ~Brainz();
		
		// input / output accessor/mutators
			std::vector<sensorNeuron>	Inputs;
			std::vector<motorNeuron>	Outputs;
		// vectors
// 			std::vector<NeuronInterz>	Neurons;
			std::vector<NeuronInterz*>	Neurons;

	// BUILD TIME

 		// fixed numbers of inputs/outputs
			unsigned int		numberOfInputs;
			unsigned int		numberOfOutputs;

	// INFO
		// total neuron & connection keepers
			unsigned int		totalNeurons;
			unsigned int		totalSynapses;

		// after every time instance, this will contain how many neurons where fired in that instant (energy usage help)
			unsigned int		neuronsFired;
			unsigned int 		motorneuronsFired;

	// build
			void			registerOutput(bool* var, unsigned int id);
			void			registerInput(unsigned int id);
			void			wireArch(BrainzArch* brainzArch);

	// RUN TIME

		// functions
// 			void			clearInputs();
			inline void clearInputs()
			{
				const auto& end(Inputs.size());
				for ( unsigned int i(0); i < end; ++i )
				{
					Inputs[i].output = 0;
				}
			}
			void			process();
			void			processTillAnswer();
	private:

		std::vector<NeuronInterz*>::iterator m_neurons_end;
		std::vector<NeuronInterz*>::iterator m_neurons_it;
};

#endif
