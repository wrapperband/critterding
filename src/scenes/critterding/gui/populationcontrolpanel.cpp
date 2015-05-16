#include "populationcontrolpanel.h"

Populationcontrolpanel::Populationcontrolpanel()
{
	isMovable = true;

	m_localposition.set(2, 24);

	col2 = 370;
	col3 = 450;

// 	row+=8;
	addSection("Limit energy at population limit", 0, 0);
	
	addSettingmutator("population_limit_energy", 0, 0);
	addSettingmutator("population_limit_energy_percent", 0, 0);
	
	row+=8;
	addSection("Eliminate portion of population", 0, 0);
	addSettingmutator("population_eliminate_portion", 0, 0);
	addSettingmutator("population_eliminate_portion_percent", 0, 0);
	addSettingmutator("population_eliminate_portion_decrenergy", 0, 0);
	addSettingmutator("population_eliminate_portion_incrworldsizeX", 0, 0);
	addSettingmutator("population_eliminate_portion_incrworldsizeY", 0, 0);
	addSettingmutator("population_eliminate_portion_incrworldsizeZ", 0, 0);
	addSettingmutator("population_eliminate_portion_decrmaxlifetimepct", 0, 0);

	addSettingmutator("population_eliminate_portion_brainmutationratechange", 0, 0);
	addSettingmutator("population_eliminate_portion_brainmutationratemin", 0, 0);
	addSettingmutator("population_eliminate_portion_brainmutationratemax", 0, 0);
	addSettingmutator("population_eliminate_portion_bodymutationratechange", 0, 0);
	addSettingmutator("population_eliminate_portion_bodymutationratemin", 0, 0);
	addSettingmutator("population_eliminate_portion_bodymutationratemax", 0, 0);
	
	
	row+=8;
// 	addSection("Limit minima", 0, 0);
	addSection("Double a population", 0, 0);
	addSettingmutator("population_double", 0, 0);
	
// 	addSettingmutator("body_maxbodypartsatbuildtime", 0, 0);
// 	addSettingmutator("body_minbodypartsize", 0, 0);
// 	addSettingmutator("body_maxbodypartsize", 0, 0);
// 	addSettingmutator("body_minheadsize", 0, 0);
// 	addSettingmutator("body_maxheadsize", 0, 0);
// 	
// 	row+=8;
// 
// 	addSettingmutator("body_maxconstraintangle", 0, 0);
// 	addSettingmutator("body_maxconstraintlimit", 0, 0);
// 	
// 	row+=8;
// 
// 	addSettingmutator("body_percentmutateeffectchangecolor", 0, 0);
// 	addSettingmutator("body_percentmutateeffectchangecolor_slightly", 0, 0);
// 	addSettingmutator("body_percentmutateeffectaddbodypart", 0, 0);
// 	addSettingmutator("body_percentmutateeffectremovebodypart", 0, 0);
// 	addSettingmutator("body_percentmutateeffectresizebodypart", 0, 0);
// 	addSettingmutator("body_percentmutateeffectresizebodypart_slightly", 0, 0);
// 	addSettingmutator("body_percentmutateeffectchangeconstraintlimits", 0, 0);
// 	addSettingmutator("body_percentmutateeffectchangeconstraintlimits_slightly", 0, 0);
// 	addSettingmutator("body_percentmutateeffectchangeconstraintangles", 0, 0);
// 	addSettingmutator("body_percentmutateeffectchangeconstraintangles_slightly", 0, 0);
// 	addSettingmutator("body_percentmutateeffectchangeconstraintposition", 0, 0);
// 	addSettingmutator("body_percentmutateeffectchangeconstraintposition_slightly", 0, 0);
// 	addSettingmutator("body_percentmutateeffectresizehead", 0, 0);
// 	addSettingmutator("body_percentmutateeffectresizehead_slightly", 0, 0);
// 	addSettingmutator("body_percentmutateeffectrepositionhead", 0, 0);
// 	
// 	row+=8;
// 	
// 	addSettingmutator("body_costhavingbodypart", 0, 0);
// 	addSettingmutator("body_costtotalweight", 0, 0);

	m_dimensions.set(col3 + 50, row-rowspacer+10+10);  // 10 = height of 1 line, 10 = horizontal margin

	BeWidgetPtr w = addWidgetButton( "populationcontrolpanel_close", Vector2i(2, 2), Vector2i(12, 12), "x", BeCommand("gui_togglepanel", "populationcontrolpanel"), EVENT_NOREPEAT, 0 );
		w->isAlignedRight = true;
	
}

Populationcontrolpanel::~Populationcontrolpanel()
{
}
