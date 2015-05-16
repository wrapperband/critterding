#include "globalsettingspanel.h"

Globalsettingspanel::Globalsettingspanel()
{
	m_dimensions.set(340, 388);

	isMovable = true;

	m_localposition.set(2, 24);

	// FIXME camera sensitivity is missing
	addSection("Global / Application", 0, 0);

	addMutator("worldsizeX", BeCommand("dec_worldsizex"), BeCommand("inc_worldsizex"), 0, 0);
	addMutator("worldsizeY", BeCommand("dec_worldsizey"), BeCommand("inc_worldsizey"), 0, 0);
	addMutator("worldsizeZ", BeCommand("dec_worldsizez"), BeCommand("inc_worldsizez"), 0, 0);
	
	addSettingmutator("skybox_rotationX", 0, 0);
	addSettingmutator("skybox_rotationY", 0, 0);
	addSettingmutator("skybox_rotationZ", 0, 0);
// 	addSettingmutator("skybox_scale", 0, 0);
	
	
	
// 	addSettingmutator("threads", 0, 0);
	addSettingcheckbox("colormode", 0, 0);
	addSettingcheckbox("exit_if_empty", 0, 0);
// 	addSettingmutator("fsX", 0, 0);
// 	addSettingmutator("fsY", 0, 0);
// 	addSettingmutator("fullscreen", 0, 0);
// 	addSettingmutator("fpslimit", 0, 0);
// 	addSettingmutator("drawscene", 0, 0);

	row+=8; addSection("Critter", 0, 0);
	addSettingmutator("mincritters", 0, 0);
	addSettingmutator("critter_insertevery", 0, 0);
	addSettingmutator("critter_maxlifetime", 0, 0);
	addSettingmutator("critter_maxenergy", 0, 0);
	addSettingmutator("critter_startenergy", 0, 0);
	addSettingmutator("critter_procinterval", 0, 0);
	addSettingmutator("critter_minenergyproc", 0, 0);
	addSettingmutator("critter_sightrange", 0, 0);
	addSettingmutator("critter_autosaveinterval", 0, 0);
	addSettingmutator("critter_autoexchangeinterval", 0, 0);
	addSettingcheckbox("critter_enableomnivores", 0, 0);
// 	addSettingcheckbox("critter_raycastvision", 0, 0);
	
	addSettingmutator("critter_mutate_minenergyproc", 0, 0);
// 	addSettingmutator("critter_mutate_sightrange", 0, 0);
	addSettingmutator("critter_mutate_maxlifetime", 0, 0);
	
	
// 	addSettingmutator("body_percentmutateeffectchangemaxmutations", 0, 0);
// 	addSettingmutator("body_percentmutateeffectchangemutationrate", 0, 0);
// 	addSettingmutator("brain_percentmutateeffectchangemaxmutations", 0, 0);
// 	addSettingmutator("brain_percentmutateeffectchangemutationrate", 0, 0);
	
	

	row+=8; addSection("Food", 0, 0);
	addMutator("energy", BeCommand("decreaseenergy"), BeCommand("increaseenergy"), 0, 0);
	addSettingmutator("food_maxlifetime", 0, 0);
	addMutator("food_maxenergy", BeCommand("dec_foodmaxenergy"), BeCommand("inc_foodmaxenergy"), 0, 0);
// 	addSettingmutator("food_size", 0, 0);
/*
	row+=8; addSection("Natural Disaster", 0, 0);
	addSettingmutator("critter_killhalfat", 0, 0);
	addSettingmutator("killhalf_decrenergy", 0, 0);
	addSettingmutator("killhalf_incrworldsizeX", 0, 0);
	addSettingmutator("killhalf_incrworldsizeY", 0, 0);
	addSettingmutator("killhalf_decrmaxlifetimepct", 0, 0);

	addSettingmutator("killhalf_brainmutationratechange", 0, 0);
	addSettingmutator("killhalf_brainmutationratemin", 0, 0);
	addSettingmutator("killhalf_brainmutationratemax", 0, 0);
	addSettingmutator("killhalf_bodymutationratechange", 0, 0);
	addSettingmutator("killhalf_bodymutationratemin", 0, 0);
	addSettingmutator("killhalf_bodymutationratemax", 0, 0);
	
	
	row+=8; addSection("Divine Doubling", 0, 0);
	addSettingmutator("critter_doubleat", 0, 0);*/

	m_dimensions.set(col3 + 40, row-rowspacer+10+10);  // 10 = height of 1 line, 10 = horizontal margin

	// close button
// 	addWidgetButton( "cv_close", Vector2i(m_dimensions.m_x-14, 2), Vector2i(12, 12), "x", BeCommand("gui_togglepanel", "globalsettingspanel"), EVENT_NOREPEAT, 0 );
	BeWidgetPtr w = addWidgetButton( "globalsettingspanel_close", Vector2i(2, 2), Vector2i(12, 12), "x", BeCommand("gui_togglepanel", "globalsettingspanel"), EVENT_NOREPEAT, 0 );
		w->isAlignedRight = true;
}

Globalsettingspanel::~Globalsettingspanel()
{
}
