#ifndef GENOTYPE_H
#define GENOTYPE_H

#include "bodyarch.h"
#include "src/brainz/brainzarch.h"
#include "src/graphics/be_color.h"

class Genotype
{
	public:
		Genotype();
		~Genotype();

		unsigned int adamdist;
		BeColor speciescolor;
		
		BodyArch* bodyArch;
		BrainzArch* brainzArch;
		
		unsigned int count;
		void registerBrainInputOutputs();
		
		std::string saveGenotype();
		
	private:
};

#endif
