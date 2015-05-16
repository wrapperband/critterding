#include "archsynapse.h"

ArchSynapse::ArchSynapse()
{
	isSensorNeuron		= false;
	neuronID		= 0;
	neurontargetlayer	= 0;
	realneuronID = -1;
// 	dendriteBranch		= 1;
	weight			= 0.0f;
}


ArchSynapse::~ArchSynapse()
{
}


