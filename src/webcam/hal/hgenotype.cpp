#include "hgenotype.h"

HGenotype::HGenotype()
{
// 	bodyArch = new BodyArch();
	brainzArch = new BrainzArch();
	answers = Answers::Instance();

	count = 0;
	adamdist = 0;

	vinputs = 320 * 240 * 3;
}

void HGenotype::registerBrainInputOutputs()
{

// BRAIN INPUTS

	// vision retina number of items
		for ( unsigned int i=0; i < vinputs; i++ )
			brainzArch->registerInput( 50000+i );

// BRAIN OUTPUTS

	// load all quesion ids as motor neurons
// 	pthread_mutex_lock( &answers->answers_m );
	for ( unsigned int i=0; i < answers->questions.size(); i++ )
	{
// 	  cerr << "registering id: " << i << endl;
		brainzArch->registerOutput( answers->questions[i].id );
	}

// 	brainzArch->registerOutput( 100000 );
// 	pthread_mutex_unlock( &answers->answers_m );
}

string HGenotype::saveHGenotype()
{
	stringstream buf;
// 	buf << "color=" << color[0] << "," << color[1] << "," << color[2] << ";\n";
	buf << "adamdist=" << adamdist << ";\n";

	string* brainarch = brainzArch->getArch();
	buf << *brainarch;
	//cout << *arch << endl;

	return buf.str();
}

HGenotype::~HGenotype()
{
	delete brainzArch;
}


