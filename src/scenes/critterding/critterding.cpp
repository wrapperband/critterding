#include "scenes/critterding/evolution.h"
#include "utils/settings.h"
// #include "utils/dirlayout.h"
#include "filesystem/be_filesystem.h"

using namespace std;

int main(int argc, char* argv[])
{
	// setup roothpaths
	BeFilesystem filesystem;
	Settings *settings = Settings::Instance();

	std::string home(filesystem.getHomedir("critterding"));
	filesystem.getRootPaths().push(home);
	
	#if !defined(WIN32)
		
		// parse argv[0] for binary path
		char buf3[1024];
		readlink("/proc/self/exe", buf3, 1024);
		
		string path(buf3);
		size_t pos = path.find_last_of("/", path.size());
		std::stringstream buff;
		if ( pos != string::npos )
			buff << path.substr( 0, pos+1 );
		
		buff << "../share/critterding/";
		filesystem.getRootPaths().push( buff.str() );
// 		std::cout << "ROOTPATHS: " << buff.str() << std::endl;

	#else
		filesystem.getRootPaths().push( "critterding/" );
	#endif		
	
	// settings belonging to Critterding
	settings->registerCVar("threads",							new CVar(1, 1, 2147483647, false, "threads to use"));
	settings->registerCVar("startseed",							new CVar(0, 0, 2147483647, true, "startseed for random number generation"));
	
// 	4294967296
// 	2147483648
// 	1112992850

	settings->registerCVar("map",								new CVar(	"easy",			"map to load (directory)") );
	settings->registerCVar("skybox",							new CVar(	"night-day/skydome3.obj",	"skybox to load (obj file)") );
	
	settings->registerCVar("benchmark",							new CVar(0, 0, 1, true, "run the critterding benchmark"));
	settings->registerCVar("benchmark_frames",						new CVar(1000, 1, 2147483647, true, "length of benchmark in frames"));

	settings->registerCVar("race",								new CVar(0, 0, 1, false, "enable race simulation"));
	settings->registerCVar("roundworld",							new CVar(0, 0, 1, false, "enable round planet"));
// 	settings->registerCVar("concavefloor",							new CVar(0, 0, 1, false, "enable concave floor"));
// 	settings->registerCVar("testworld",							new CVar(0, 0, 1, false, "a world for test purposes"));

	settings->registerCVar("worldsizeX",							new CVar(200, 1, 2147483647, false, "size of the world along axis X"));
	settings->registerCVar("worldsizeY",							new CVar(100, 1, 2147483647, false, "size of the world along axis Y"));
	settings->registerCVar("worldsizeZ",							new CVar(120, 0, 2147483647, false, "size of the world along axis Z"));
	settings->registerCVar("skybox_rotationX",						new CVar(0, 0, 2147483647, false, "rotational speed of skybox along the X axis"));
	settings->registerCVar("skybox_rotationY",						new CVar(0, 0, 2147483647, false, "rotational speed of skybox along the Y axis"));
	settings->registerCVar("skybox_rotationZ",						new CVar(0, 0, 2147483647, false, "rotational speed of skybox along the Z axis"));
	settings->registerCVar("skybox_scale",							new CVar(200, 0, 2147483647, false, "scale of the skybox (100=1)"));
// 	settings->registerCVar("worldwalls",							new CVar(1, 0, 1, false, "enable walls around the world"));

	settings->registerCVar("energy",							new CVar(1200, 0, 2147483647, false, "energy in the system by number of food cubes"));
	settings->registerCVar("mincritters",							new CVar(20, 0, 1000, false, "minimum number of critters"));

// 	settings->registerCVar("retinasperrow",							new CVar(119, 1, 1000, false, "number of vision retinas to stack per row onscreen"));
	settings->registerCVar("camerasensitivity_move",					new CVar(600, 1, 10000, false, "sensitivity of the camera when moving"));
	settings->registerCVar("camerasensitivity_look",					new CVar(950, 1, 10000, false, "sensitivity of the camera when looking around"));
	settings->registerCVar("camera_mode",							new CVar(0, 0, 3, true, "camera mode"));
	settings->registerCVar("camera_smoothfactor",				new CVar(860, 0, 1000, false, "camera smoothing factor (permille)"));

	settings->registerCVar("colormode",							new CVar(0, 0, 1, true, "colors genetically exact critters identically"));
	settings->registerCVar("exit_if_empty",							new CVar(0, 0, 1, true, "exit simulation if there are no critters"));
	settings->registerCVar("autoload",							new CVar(0, 0, 1, true, "autoload critters from ~/.critterding/load"));
	settings->registerCVar("autoloadlastsaved",						new CVar(0, 0, 1, true, "autoload critters from ~/.critterding/lastsaved"));

	settings->registerCVar("critter_insertevery",						new CVar(0, 0, 2147483647, false, "inserts a random critter every n frames"));
	settings->registerCVar("critter_maxlifetime",						new CVar(32000, 1, 2147483647, false, "maximum number of frames a critter lives"));
	settings->registerCVar("critter_maxenergy",						new CVar(5000, 1, 2147483647, false, "maximum amount of energy a critter has"));

	settings->registerCVar("critter_startenergy",						new CVar(3000, 1, 2147483647, false, "energy a new critter (adam) starts with"));
	settings->registerCVar("critter_procinterval",						new CVar(100, 0, 2147483647, false, "minimum frames between procreations"));
	settings->registerCVar("critter_minenergyproc",						new CVar(3000, 1, 2147483647, false, "energy a critters needs to procreate"));
	settings->registerCVar("critter_sightrange",						new CVar(500, 1, 2147483647, false, "distance a critter can see (10 = 1 worldsize)"));

	settings->registerCVar("critter_retinasize",						new CVar(16, 1, 50, false, "size of a critters eye retina"));
	settings->registerCVar("critter_autosaveinterval",					new CVar(0, 0, 2147483647, false, "save critters every n seconds"));
	settings->registerCVar("critter_autoexchangeinterval",					new CVar(0, 0, 2147483647, false, "save critters every n seconds"));
	settings->registerCVar("critter_enableomnivores",					new CVar(0, 0, 1, true, "enables critters to eat each other"));
// 	settings->registerCVar("critter_raycastvision",						new CVar(0, 0, 1, true, "use raycast vision instead of opengl"));
	
	// FIXME MOVE THESE TO BODY ARCH
	settings->registerCVar("critter_mutate_minenergyproc",				new CVar(0, 0, 2147483647, false, "chance of mutating the energy a critters needs to procreate"));
// 	settings->registerCVar("critter_mutate_sightrange",					new CVar(0, 0, 2147483647, false, "chance of mutating the distance a critter can see"));
	settings->registerCVar("critter_mutate_maxlifetime",				new CVar(0, 0, 2147483647, false, "chance of mutating the maximum number of frames a critter lives"));
	
	
	settings->registerCVar("food_maxlifetime",						new CVar(32000, 1, 2147483647, false, "maximum number of frames a food unit exists"));
	settings->registerCVar("food_maxenergy",						new CVar(1500, 1, 2147483647, false, "maximum amount of energy a food unit has"));
// 	settings->registerCVar("food_size",								new CVar(100, 1, 2147483647, false, "size of a food unit"));

	settings->registerCVar("body_maxmutations",						new CVar(4, 1, 2147483647, false, "maximum mutations on a body mutant"));
	settings->registerCVar("body_mutationrate",						new CVar(160, 0, 1000, false, "rate of body mutation (permille)"));

	settings->registerCVar("body_minbodyparts",						new CVar(2, 0, 2147483647, false, "minimum body parts per critter"));
	settings->registerCVar("body_maxbodyparts",						new CVar(30, 0, 2147483647, false, "maximum body parts per critter"));
	settings->registerCVar("body_minbodypartsatbuildtime",					new CVar(4, 1, 2147483647, false, "minimum body parts for a new critter"));
	settings->registerCVar("body_maxbodypartsatbuildtime",					new CVar(8, 1, 2147483647, false, "maximum body parts for a new critter"));
	
	settings->registerCVar("body_maxconstraintangle",					new CVar(31415, 0, 2147483647, false, "maximum amount of rotation on a contraint"));
	settings->registerCVar("body_maxconstraintlimit",					new CVar(14000, 0, 2147483647, false, "maximum limit of a contraint"));
// 	settings->registerCVar("body_maxconstraintlimit",					new CVar(7853, 0, 2147483647, false, "maximum limit of a contraint"));
// 	settings->registerCVar("body_maxconstraintlimit",					new CVar(15706, 0, 2147483647, false, "maximum limit of a contraint"));
	settings->registerCVar("body_constraintpositionrange",			new CVar(60, 0, 100, false, "amount muscles can deviate from the center of a plane (%)"));

	settings->registerCVar("body_minbodypartsize",						new CVar(20, 1, 2147483647, false, "minimum size of a critters body part"));
	settings->registerCVar("body_maxbodypartsize",						new CVar(100, 1, 2147483647, false, "maximum size of a critters body part"));
	settings->registerCVar("body_minheadsize",						new CVar(15, 1, 2147483647, false, "minimum size of a critters head"));
	settings->registerCVar("body_maxheadsize",						new CVar(50, 1, 2147483647, false, "maximum size of a critters head"));

	
	settings->registerCVar("body_percentmutateeffectchangecolor",						new CVar(2, 0, 100, false, "chance of changing body color"));
	settings->registerCVar("body_percentmutateeffectchangecolor_slightly",				new CVar(5, 0, 100, false, "chance of slightly changing body color"));
	settings->registerCVar("body_percentmutateeffectaddbodypart",						new CVar(2, 0, 100, false, "chance of adding a body part"));
	settings->registerCVar("body_percentmutateeffectremovebodypart",					new CVar(2, 0, 100, false, "chance of removing a body part"));
	settings->registerCVar("body_percentmutateeffectresizebodypart",					new CVar(2, 0, 100, false, "chance of resizing a body part"));
	settings->registerCVar("body_percentmutateeffectresizebodypart_slightly",			new CVar(5, 0, 100, false, "chance of slightly resizing a body part"));
	settings->registerCVar("body_percentmutateeffectchangeconstraintlimits",			new CVar(2, 0, 100, false, "chance of changing a joints motion limits"));
	settings->registerCVar("body_percentmutateeffectchangeconstraintlimits_slightly",	new CVar(5, 0, 100, false, "chance of slightly changing a joints motion limits"));
	settings->registerCVar("body_percentmutateeffectchangeconstraintangles",			new CVar(2, 0, 100, false, "chance of changing a joints position angles"));
	settings->registerCVar("body_percentmutateeffectchangeconstraintangles_slightly",	new CVar(5, 0, 100, false, "chance of changing a joints position angles"));
	settings->registerCVar("body_percentmutateeffectchangeconstraintposition",			new CVar(2, 0, 100, false, "chance of changing a joints position"));
	settings->registerCVar("body_percentmutateeffectchangeconstraintposition_slightly",	new CVar(0, 0, 100, false, "chance of slightly changing a joints position"));
	settings->registerCVar("body_percentmutateeffectresizehead",						new CVar(2, 0, 100, false, "chance of resizing a head"));
	settings->registerCVar("body_percentmutateeffectresizehead_slightly",				new CVar(5, 0, 100, false, "chance of slightly resizing a head"));
	settings->registerCVar("body_percentmutateeffectrepositionhead",					new CVar(5, 0, 100, false, "chance of repositioning head"));
// 	settings->registerCVar("body_percentmutateeffectchangemutationrate",				new CVar(0, 0, 100, false, "chance of changing body mutationrate"));
// 	settings->registerCVar("body_percentmutateeffectchangemaxmutations",				new CVar(0, 0, 100, false, "chance of changing max mutations on body"));

	
	settings->registerCVar("body_selfcollisions",						new CVar(0, 0, 1, true, "critters can exploit physics glitches"));

	settings->registerCVar("body_costhavingbodypart",					new CVar(1000, 0, 2147483647, false, "cost of having a bodypart (1/100000 energy)"));
	settings->registerCVar("body_costtotalweight",						new CVar(1000, 0, 2147483647, false, "cost multiplier of total body weight (1/100000 energy)"));
	
	
	settings->registerCVar("brain_maxmutations",						new CVar(10, 1, 2147483647, false, "maximum mutations on a brain mutant"));
	settings->registerCVar("brain_mutationrate",						new CVar(160, 0, 1000, false, "rate of brain mutation (permille)"));

	settings->registerCVar("brain_maxneurons",						new CVar(1000, 1, 2147483647, false, "maximum neurons per critter"));
	settings->registerCVar("brain_minsynapses",						new CVar(1, 1, 2147483647, false, "minimum synapses per neuron"));
	settings->registerCVar("brain_maxsynapses",						new CVar(1000, 1, 2147483647, false, "maximum synapses per neuron"));
	settings->registerCVar("brain_minneuronsatbuildtime",					new CVar(10, 1, 2147483647, false, "minimum neurons for a new critter"));
	settings->registerCVar("brain_maxneuronsatbuildtime",					new CVar(150, 1, 2147483647, false, "maximum neurons for a new critter"));

	settings->registerCVar("brain_minsynapsesatbuildtime",					new CVar(10, 1, 2147483647, false, "minimum synapses for a new neuron"));
	settings->registerCVar("brain_maxsynapsesatbuildtime",					new CVar(200, 1, 2147483647, false, "maximum synapses for a new neuron of a new critter"));

	settings->registerCVar("brain_percentchanceinhibitoryneuron",				new CVar(50, 0, 100, false, "percent chance a neuron is inhibotory"));
	settings->registerCVar("brain_mutate_percentchanceinhibitoryneuron",				new CVar(0, 0, 1, true, "mutate this value"));

	settings->registerCVar("brain_percentchancemotorneuron",				new CVar(100, 0, 100, false, "percent chance a neuron is a motor neuron"));
	settings->registerCVar("brain_mutate_percentchancemotorneuron",					new CVar(0, 0, 1, true, "mutate this value"));

	settings->registerCVar("brain_percentchanceplasticneuron",				new CVar(20, 0, 100, false, "percent chance a neuron has plastic synapses"));
	settings->registerCVar("brain_mutate_percentchanceplasticneuron",				new CVar(0, 0, 1, true, "mutate this value"));

	settings->registerCVar("brain_minplasticitystrengthen",					new CVar(100, 1, 2147483647, false, "minimum weight by which plastic synapses strengthen"));
	settings->registerCVar("brain_maxplasticitystrengthen",					new CVar(1000, 1, 2147483647, false, "maximum weight by which plastic synapses strengthen"));
	settings->registerCVar("brain_minplasticityweaken",					new CVar(1000, 1, 2147483647, false, "minimum weight by which plastic synapses weaken"));
	settings->registerCVar("brain_maxplasticityweaken",					new CVar(10000, 1, 2147483647, false, "maximum weight by which plastic synapses weaken"));
	settings->registerCVar("brain_mutate_plasticityfactors",					new CVar(0, 0, 1, true, "mutate min/max plasticity values"));

	settings->registerCVar("brain_minfiringthreshold",					new CVar(2, 1, 2147483647, false, "minimum firingthreshold of a neuron"));
	settings->registerCVar("brain_mutate_minfiringthreshold",					new CVar(0, 0, 1, true, "mutate this value"));

	settings->registerCVar("brain_maxfiringthreshold",					new CVar(12, 1, 2147483647, false, "maximum firingthreshold of a neuron"));
	settings->registerCVar("brain_mutate_maxfiringthreshold",					new CVar(0, 0, 1, true, "mutate this value"));

	settings->registerCVar("brain_percentchanceconsistentsynapses",				new CVar(0, 0, 100, false, "percent chance a neurons synapses are all inhibitory or excitatory"));
	settings->registerCVar("brain_mutate_percentchanceconsistentsynapses",				new CVar(0, 0, 1, true, "mutate this value"));

	settings->registerCVar("brain_percentchanceinhibitorysynapses",				new CVar(50, 0, 100, false, "percent chance a synapse is inhibitory"));
	settings->registerCVar("brain_mutate_percentchanceinhibitorysynapses",				new CVar(0, 0, 1, true, "mutate this value"));

	settings->registerCVar("brain_percentchancesensorysynapse",				new CVar(20, 0, 100, false, "percent change a synapse connects to an input"));
	settings->registerCVar("brain_mutate_percentchancesensorysynapse",				new CVar(0, 0, 1, true, "mutate this value"));

	settings->registerCVar("brain_percentmutateeffectaddneuron",				new CVar(2, 0, 100, false, "chance of adding a neuron"));
	settings->registerCVar("brain_percentmutateeffectremoveneuron",				new CVar(2, 0, 100, false, "chance of removing a neuron"));
	settings->registerCVar("brain_percentmutateeffectalterneuron",				new CVar(5, 0, 100, false, "chance of altering a neuron"));
	settings->registerCVar("brain_percentmutateeffectaddsynapse",				new CVar(10, 0, 100, false, "chance of adding a synapse"));
	settings->registerCVar("brain_percentmutateeffectremovesynapse",			new CVar(10, 0, 100, false, "chance of removing a synapse"));
// 	settings->registerCVar("brain_percentmutateeffectchangemutationrate",		new CVar(0, 0, 100, false, "chance of changing brain mutationrate"));
// 	settings->registerCVar("brain_percentmutateeffectchangemaxmutations",		new CVar(0, 0, 100, false, "chance of changing max mutations on brain"));
	settings->registerCVar("brain_mutate_mutateeffects",						new CVar(0, 0, 1, true, "mutate mutation effects"));

	settings->registerCVar("brain_percentmutateeffectaltermutable",				new CVar(1, 0, 100, false,"mutate value of a mutatable option"));

	settings->registerCVar("brain_costhavingneuron",					new CVar(100, 0, 2147483647, false, "cost of having a neuron (1/100000 energy)"));
	settings->registerCVar("brain_costfiringneuron",					new CVar(50, 0, 2147483647, false, "cost of firing a neuron"));
	settings->registerCVar("brain_costfiringmotorneuron",					new CVar(1000, 0, 2147483647, false, "cost of firing a motor neuron"));
	settings->registerCVar("brain_costhavingsynapse",					new CVar(20, 0, 2147483647, false, "cost of having a synapse"));

	
	settings->registerCVar("population_eliminate_portion",						new CVar(160, 2, 2147483647, false, "eliminate a percentage of critters if population reaches n"));
	settings->registerCVar("population_eliminate_portion_percent",					new CVar(35, 1, 100, false, "eliminate n% of critters if population reaches population_eliminate_portion"));
	settings->registerCVar("population_eliminate_portion_decrenergy",					new CVar(2, 0, 100, false, "decrease energy by n when killhalfat triggers"));
	settings->registerCVar("population_eliminate_portion_incrworldsizeX",					new CVar(0, 0, 100, false, "increase worldsizeX by n when killhalfat triggers"));
	settings->registerCVar("population_eliminate_portion_incrworldsizeY",					new CVar(0, 0, 100, false, "increase worldsizeY by n when killhalfat triggers"));
	settings->registerCVar("population_eliminate_portion_incrworldsizeZ",					new CVar(0, 0, 100, false, "increase worldsizeZ by n when killhalfat triggers"));
	settings->registerCVar("population_eliminate_portion_decrmaxlifetimepct",					new CVar(0, 0, 100, false, "decrease critter_maxlifetime by n when killhalfat triggers"));

	settings->registerCVar("population_eliminate_portion_brainmutationratechange",				new CVar(0, 0, 1, true, "change mutationrate of brain between min and max"));
	settings->registerCVar("population_eliminate_portion_brainmutationratemin",					new CVar(0, 0, 100, false, "minimum mutationrate of body"));
	settings->registerCVar("population_eliminate_portion_brainmutationratemax",					new CVar(0, 0, 100, false, "maximum mutationrate of body"));
	settings->registerCVar("population_eliminate_portion_bodymutationratechange",				new CVar(0, 0, 1, true, "change mutationrate of brain between min and max"));
	settings->registerCVar("population_eliminate_portion_bodymutationratemin",					new CVar(0, 0, 100, false, "minimum mutationrate of brain"));
	settings->registerCVar("population_eliminate_portion_bodymutationratemax",					new CVar(0, 0, 100, false, "maximum mutationrate of brain"));
	
	
	settings->registerCVar("population_limit_energy",							new CVar(150, 0, 2147483647, false, "population limit"));
	settings->registerCVar("population_limit_energy_percent",		new CVar(100, 0, 100, false, "if population limit is exceeded drop energy to percentage"));

	settings->registerCVar("population_double",						new CVar(0, 0, 2147483647, false, "duplicate all critters if population reaches n"));


	settings->registerCVar("light_ambient_modelR",					new CVar(8, 0, 255, false, "lighting setting"));
	settings->registerCVar("light_ambient_modelG",					new CVar(8, 0, 255, false, "lighting setting"));
	settings->registerCVar("light_ambient_modelB",					new CVar(8, 0, 255, false, "lighting setting"));
	
	settings->registerCVar("light_ambientR",						new CVar(8, 0, 255, false, "lighting setting"));
	settings->registerCVar("light_ambientG",						new CVar(8, 0, 255, false, "lighting setting"));
	settings->registerCVar("light_ambientB",						new CVar(8, 0, 255, false, "lighting setting"));
	
	settings->registerCVar("light_diffuseR",						new CVar(255, 0, 255, false, "lighting setting"));
	settings->registerCVar("light_diffuseG",						new CVar(255, 0, 255, false, "lighting setting"));
	settings->registerCVar("light_diffuseB",						new CVar(255, 0, 255, false, "lighting setting"));
	
	settings->registerCVar("light_specularR",						new CVar(255, 0, 255, false, "lighting setting"));
	settings->registerCVar("light_specularG",						new CVar(255, 0, 255, false, "lighting setting"));
	settings->registerCVar("light_specularB",						new CVar(255, 0, 255, false, "lighting setting"));

	settings->registerCVar("light_attenuation_constant",			new CVar(200, 0, 2147483647, false, "lighting setting"));
	settings->registerCVar("light_attenuation_linear",				new CVar(200, 0, 2147483647, false, "lighting setting"));
	settings->registerCVar("light_attenuation_quadratic",			new CVar(200, 0, 2147483647, false, "lighting setting"));
	
	settings->checkCommandLineOptions(argc,argv );
	
	// MAIN LOOP
	Evolution evolution(filesystem);
	evolution.run();
	return 0;

}
