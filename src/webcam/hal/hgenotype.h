#ifndef HGENOTYPE_H
#define HGENOTYPE_H

// #include "../bodyarch.h"
#include "src/brainz/brainzarch.h"
// #include "../../utils/color.h"
#include "src/halutils/answers.h"
#include <pthread.h>

using namespace std;

class HGenotype
{
	public:
		HGenotype();
		~HGenotype();

		unsigned int adamdist;
// 		Color speciescolor;

// 		BodyArch* bodyArch;
		BrainzArch* brainzArch;

		unsigned int count;
		void registerBrainInputOutputs();

		string saveHGenotype();
		
		unsigned int vinputs;
	private:
		Answers*	answers;
		
};

#endif
