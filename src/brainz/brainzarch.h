#ifndef BRAINZARCH_H
#define BRAINZARCH_H

// internal includes
	#include "src/common/be_rand.h"
	#include "src/common/be_parser.h"

	#include "archneuronz.h"
	#include "neuroninterz.h"

// external includes
	#include <vector>
	#include <sstream>
	#include <iostream>

class BrainzArch
{
	public:
		BrainzArch();
		~BrainzArch();

		// input / output accessor/mutators
			std::vector<unsigned int>	InputIDs;
			std::vector<unsigned int>	OutputIDs;
		// vectors
			std::vector<ArchNeuronz>	ArchNeurons;

	// BUILD TIME

		// min/max neurons at build time
			unsigned int		minNeuronsAtBuildtime;
			unsigned int		maxNeuronsAtBuildtime;

		// fixed numbers of inputs/outputs
			unsigned int		numberOfInputs;
			unsigned int		numberOfOutputs;

		// absolute max neurons (mutation bounding)
			unsigned int		maxNeurons;

			unsigned int		percentMutateEffectAddNeuron;
			unsigned int		percentMutateEffectRemoveNeuron;
			unsigned int		percentMutateEffectAlterNeuron;
			unsigned int		percentMutateEffectAddSynapse;
			unsigned int		percentMutateEffectRemoveSynapse;
			unsigned int		percentMutateEffectAlterMutable;
				unsigned int			mutate_MutateEffects;

			unsigned int m_maxmutations;
			unsigned int m_mutationrate;
				
		// percent chance that when adding a new random neuron, it's inhibitory
			unsigned int		percentChanceInhibitoryNeuron;
				unsigned int			mutate_percentChanceInhibitoryNeuron;

		// synaptic consistancy, meaning all synapses of a neuron will be OR I OR E
		// if set to 0, neurons will have mixed I and E synapses
			unsigned int		percentChanceConsistentSynapses;
				unsigned int			mutate_percentChanceConsistentSynapses;

		// percent chance that when adding a new random neuron, it has inhibitory synapses
			unsigned int		percentChanceInhibitorySynapses;
				unsigned int			mutate_percentChanceInhibitorySynapses;

		// percent chance that when adding a new random neuron, it has a motor function
			unsigned int		percentChanceMotorNeuron;
				unsigned int			mutate_percentChanceMotorNeuron;

		// percent chance that when adding a new random neuron, it is has synaptic plasticity
			unsigned int		percentChancePlasticNeuron;
				unsigned int			mutate_percentChancePlasticNeuron;

			// min/max synaptic plasticity strengthening factor
				unsigned int		minPlasticityStrengthen;
				unsigned int		maxPlasticityStrengthen;

			// min/max synaptic plasticity weakening factor
				unsigned int		minPlasticityWeaken;
				unsigned int		maxPlasticityWeaken;

			// flag if we'll mutate these
				unsigned int			mutate_PlasticityFactors;

		// min/max firing threshold
			unsigned int		minFiringThreshold;
				unsigned int			mutate_minFiringThreshold;
			unsigned int		maxFiringThreshold;
				unsigned int			mutate_maxFiringThreshold;

		// max dendritic branches in a new neuron
// 			unsigned int		maxDendridicBranches;
// 				unsigned int			mutate_maxDendridicBranches;

		// percent chance that a new synapse is connected to a sensor neuron
			unsigned int		percentChanceSensorySynapse;
				unsigned int			mutate_percentChanceSensorySynapse;

		// min/max synapses at build time
			unsigned int		minSynapsesAtBuildtime;
			unsigned int		maxSynapsesAtBuildtime;

		// absolute min/max synapses (mutation/plastic bounding)
			unsigned int		minSynapses;
			unsigned int		maxSynapses;

		// brain architecture mutation factor @ mutation time (%)
			unsigned int		percentMutation;
				unsigned int			mutate_percentMutation;

	// INFO
		// total neuron & connection keepers
			unsigned int		totalNeurons;
			unsigned int		totalSynapses;

		// after every time instance, this will contain how many neurons where fired in that instant (energy usage help)
			unsigned int		neuronsFired;
			unsigned int 		motorneuronsFired;

	// build commands

		// functions
			void			copyFrom(const BrainzArch* otherBrain);
			void			mergeFrom(const BrainzArch* otherBrain1, const BrainzArch* otherBrain2);
			void			registerOutput(unsigned int id);
			void			registerInput(unsigned int id);
			void			removeObsoleteMotorsAndSensors();
			void			buildArch();
			void			mutate(unsigned int runs);
			void			wireArch();
		// load save architecture (serialize)
			void			setArch(std::string& content);
			std::string*		getArch();

	// RUN TIME

		// functions
			int	findSensorNeuron( const unsigned int id ) const;
			bool sensorNeuronExists( const unsigned int id ) const;
			int	findMotorNeuron( const unsigned int id ) const;
	private:
		// utilities
			BeParser		parseH;
			BeParser		contentParser;
			BeRand			*randgen;

		// a buffer for reading architecture
			std::string		archBuffer;

// 			int			findSensorNeuron( unsigned int id );

		// build time functions
			void			addRandomArchNeuron();
			unsigned int		addRandomArchSynapse(unsigned int parentneuron);

};

#endif
