#include "hgenotypes.h"

HGenotypes* HGenotypes::Instance () 
{
	static HGenotypes t;
	return &t;
}

HGenotypes::HGenotypes()
{
}

HGenotype* HGenotypes::newg( Settings* settings )
{
// cerr << " gentotype start" << endl;
	HGenotype* gt = new HGenotype();

// 	gt->bodyArch->retinasize = settings->getCVar("critter_retinasize");
// 	gt->bodyArch->color = colorH.randomColorRGB();

// 	gt->bodyArch->buildArch();
	gt->registerBrainInputOutputs();

	gt->brainzArch->maxNeurons					= settings->getCVar("brain_maxneurons");
	gt->brainzArch->minSynapses					= settings->getCVar("brain_minsynapses");
	gt->brainzArch->maxSynapses					= settings->getCVar("brain_maxsynapses");
	gt->brainzArch->minNeuronsAtBuildtime				= settings->getCVar("brain_minneuronsatbuildtime");
	gt->brainzArch->maxNeuronsAtBuildtime				= settings->getCVar("brain_maxneuronsatbuildtime");

	gt->brainzArch->minSynapsesAtBuildtime				= settings->getCVar("brain_minsynapsesatbuildtime");
// 		brain.mutate_minSynapsesAtBuildtime		= settings->getCVar("brain_mutate_minsynapsesatbuildtime");

	gt->brainzArch->maxSynapsesAtBuildtime				= settings->getCVar("brain_maxsynapsesatbuildtime");
// 		brain.mutate_maxSynapsesAtBuildtime		= settings->getCVar("brain_mutate_maxsynapsesatbuildtime");

	gt->brainzArch->percentChanceInhibitoryNeuron			= settings->getCVar("brain_percentchanceinhibitoryneuron");
		gt->brainzArch->mutate_percentChanceInhibitoryNeuron	= settings->getCVar("brain_mutate_percentchanceinhibitoryneuron");

	gt->brainzArch->percentChanceMotorNeuron				= settings->getCVar("brain_percentchancemotorneuron");
		gt->brainzArch->mutate_percentChanceMotorNeuron		= settings->getCVar("brain_mutate_percentchancemotorneuron");

	gt->brainzArch->percentChancePlasticNeuron			= settings->getCVar("brain_percentchanceplasticneuron");
		gt->brainzArch->mutate_percentChancePlasticNeuron			= settings->getCVar("brain_mutate_percentchanceplasticneuron");

	gt->brainzArch->minPlasticityStrengthen				= settings->getCVar("brain_minplasticitystrengthen");
	gt->brainzArch->maxPlasticityStrengthen				= settings->getCVar("brain_maxplasticitystrengthen");
	gt->brainzArch->minPlasticityWeaken				= settings->getCVar("brain_minplasticityweaken");
	gt->brainzArch->maxPlasticityWeaken				= settings->getCVar("brain_maxplasticityweaken");
		gt->brainzArch->mutate_PlasticityFactors				= settings->getCVar("brain_mutate_plasticityfactors");

	gt->brainzArch->minFiringThreshold				= settings->getCVar("brain_minfiringthreshold");
		gt->brainzArch->mutate_minFiringThreshold				= settings->getCVar("brain_mutate_minfiringthreshold");

	gt->brainzArch->maxFiringThreshold				= settings->getCVar("brain_maxfiringthreshold");
		gt->brainzArch->mutate_maxFiringThreshold				= settings->getCVar("brain_mutate_maxfiringthreshold");

	gt->brainzArch->maxDendridicBranches				= settings->getCVar("brain_maxdendridicbranches");
		gt->brainzArch->mutate_maxDendridicBranches		= settings->getCVar("brain_mutate_maxdendridicbranches");

	gt->brainzArch->percentChanceConsistentSynapses			= settings->getCVar("brain_percentchanceconsistentsynapses");
		gt->brainzArch->mutate_percentChanceConsistentSynapses		= settings->getCVar("brain_mutate_percentchanceconsistentsynapses");

	gt->brainzArch->percentChanceInhibitorySynapses			= settings->getCVar("brain_percentchanceinhibitorysynapses");
		gt->brainzArch->mutate_percentChanceInhibitorySynapses		= settings->getCVar("brain_mutate_percentchanceinhibitorysynapses");

	gt->brainzArch->percentChanceSensorySynapse			= settings->getCVar("brain_percentchancesensorysynapse");
		gt->brainzArch->mutate_percentChanceSensorySynapse	= settings->getCVar("brain_mutate_percentchancesensorysynapse");

	gt->brainzArch->percentMutateEffectAddNeuron			= settings->getCVar("brain_percentmutateeffectaddneuron");
	gt->brainzArch->percentMutateEffectRemoveNeuron			= settings->getCVar("brain_percentmutateeffectremoveneuron");
	gt->brainzArch->percentMutateEffectAlterNeuron			= settings->getCVar("brain_percentmutateeffectalterneuron");
	gt->brainzArch->percentMutateEffectAddSynapse			= settings->getCVar("brain_percentmutateeffectaddsynapse");
	gt->brainzArch->percentMutateEffectRemoveSynapse			= settings->getCVar("brain_percentmutateeffectremovesynapse");
		gt->brainzArch->mutate_MutateEffects				= settings->getCVar("brain_mutate_mutateeffects");

	gt->brainzArch->buildArch();

	gt->count=1;
	gt->adamdist=0;
// 	gt->speciescolor = colorH.randomColorRGB().getNormalized();

	list.push_back(gt);
// 	cerr << " new genotype " << list.size() << endl;
	return gt;
}

void HGenotypes::add(HGenotype* gt)
{
	gt->count++;
// 	cerr << " added genotype " << list.size() << endl;
}

HGenotype* HGenotypes::copy(HGenotype* gt, bool brainmutant, unsigned int brruns)
{
// cerr << " gentotype copy start" << endl;
	if ( brainmutant )
	{
		HGenotype* newgt = new HGenotype();

// 		newgt->bodyArch->copyFrom(gt->bodyArch);
// 		if ( bodymutant )
// 		{
// 			unsigned int runs = RandGen::Instance()->get(1, boruns);
// 			newgt->bodyArch->mutate( runs );
// 		}

		newgt->registerBrainInputOutputs();
		newgt->brainzArch->copyFrom(gt->brainzArch);

// 		if ( bodymutant )
// 			newgt->brainzArch->removeObsoleteMotorsAndSensors();

		if ( brainmutant )
		{
			unsigned int runs = RandGen::Instance()->get(1, brruns);
			newgt->brainzArch->mutate( runs );
		}

		newgt->count=1;
		newgt->adamdist = gt->adamdist+1;
// 		newgt->speciescolor = colorH.randomColorRGB().getNormalized();

		list.push_back(newgt);
// cerr << " gentotype copy end" << endl;
		return newgt;
	}
	else
	{
		add(gt);
// cerr << " gentotype copy end" << endl;
		return gt;
	}

	
// 	cerr << " copied genotype " << list.size() << endl;
}


void HGenotypes::remove(HGenotype* gt)
{
// 	cerr << gt->count << endl;
	if ( gt->count > 1 )
		gt->count--;
	else
	{
		for ( unsigned int i=0; i < list.size(); i++ )
			if ( list[i] == gt )
			{
				delete gt;
				list.erase(list.begin()+i);
// 				cerr << " removing genotype " << list.size() << endl;
				return;
			}
	}
}

HGenotype* HGenotypes::loadHGenotype(string& content)
{
// 	cerr << "loading genotype start" << endl;
	HGenotype* gt = new HGenotype();

	string passToBody;
	string passToBrain;
	string line = Parser::Instance()->returnUntillStrip( "\n", content );
	while ( !line.empty() )
	{
		// adamdist=690;
			if ( Parser::Instance()->beginMatchesStrip( "adamdist=", line ) )
			{
				string AD = Parser::Instance()->returnUntillStrip( ";", line );
				//cerr << "AD: " << AD << endl;
				if(EOF == sscanf(AD.c_str(), "%d", &gt->adamdist)) cerr << "ERROR INSERTING CRITTER" << endl;
			}

		// the rest goes to the body or brain
			else
			{
				if ( 
					Parser::Instance()->beginMatches( "color=", line ) ||
					Parser::Instance()->beginMatches( "retinasize=", line ) ||
					Parser::Instance()->beginMatches( "b ", line ) ||
					Parser::Instance()->beginMatches( "m ", line ) ||
					Parser::Instance()->beginMatches( "c ", line ) ||
					Parser::Instance()->beginMatches( "cm ", line )
				)
				{
					passToBody.append(line);
					passToBody.append("\n");
				}
				else
				{
					passToBrain.append(line);
					passToBrain.append("\n");
				}
			}

		line = Parser::Instance()->returnUntillStrip( "\n", content );
	}

// 	gt->bodyArch->setArch(&passToBody);
	
	gt->registerBrainInputOutputs();
	gt->brainzArch->setArch(&passToBrain);

	gt->count=1;
// 	gt->speciescolor = colorH.randomColorRGB().getNormalized();
	list.push_back(gt);
	
// 	genotype = genotypes->newg(passToBody, passToBrain, retinasize); // FIXME, with a speciesID

// 	genotype->bodyArch->setArch(&passToBody);
// 	genotype->brainzArch->setArch(&passToBrain);
// 	cerr << "loading genotype end" << endl;
	return gt;
}

HGenotypes::~HGenotypes()
{
	for (unsigned int i=0; i<list.size(); i++) delete list[i];
}


