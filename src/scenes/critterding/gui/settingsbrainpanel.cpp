#include "settingsbrainpanel.h"

Settingsbrainpanel::Settingsbrainpanel()
{
	isMovable = true;

	m_localposition.set(2, 24);

	col2 = 320;
	col3 = 400;
	
	addSettingmutator("brain_maxneurons", 0, 0);
	addSettingmutator("brain_minsynapses", 0, 0);
	addSettingmutator("brain_maxsynapses", 0, 0);
	addSettingmutator("brain_minneuronsatbuildtime", 0, 0);
	addSettingmutator("brain_maxneuronsatbuildtime", 0, 0);
	addSettingmutator("brain_minsynapsesatbuildtime", 0, 0);
	addSettingmutator("brain_maxsynapsesatbuildtime", 0, 0);
	addSettingmutator("brain_percentchanceinhibitoryneuron", 0, 0);
	addSettingmutator("brain_mutate_percentchanceinhibitoryneuron", 0, 0);
	addSettingmutator("brain_percentchancemotorneuron", 0, 0);
	addSettingmutator("brain_mutate_percentchancemotorneuron", 0, 0);
	addSettingmutator("brain_percentchanceplasticneuron", 0, 0);
	addSettingmutator("brain_mutate_percentchanceplasticneuron", 0, 0);
	addSettingmutator("brain_minplasticitystrengthen", 0, 0);
	addSettingmutator("brain_maxplasticitystrengthen", 0, 0);
	addSettingmutator("brain_minplasticityweaken", 0, 0);
	addSettingmutator("brain_maxplasticityweaken", 0, 0);
	addSettingmutator("brain_mutate_plasticityfactors", 0, 0);
	addSettingmutator("brain_minfiringthreshold", 0, 0);
	addSettingmutator("brain_mutate_minfiringthreshold", 0, 0);
	addSettingmutator("brain_maxfiringthreshold", 0, 0);
	addSettingmutator("brain_mutate_maxfiringthreshold", 0, 0);
// 	addSettingmutator("brain_maxdendridicbranches", 0, 0);
// 	addSettingmutator("brain_mutate_maxdendridicbranches", 0, 0);
	addSettingmutator("brain_percentchanceconsistentsynapses", 0, 0);
	addSettingmutator("brain_mutate_percentchanceconsistentsynapses", 0, 0);
	addSettingmutator("brain_percentchanceinhibitorysynapses", 0, 0);
	addSettingmutator("brain_mutate_percentchanceinhibitorysynapses", 0, 0);
	addSettingmutator("brain_percentchancesensorysynapse", 0, 0);
	addSettingmutator("brain_mutate_percentchancesensorysynapse", 0, 0);
	addSettingmutator("brain_percentmutateeffectaddneuron", 0, 0);
	addSettingmutator("brain_percentmutateeffectremoveneuron", 0, 0);

	addSettingmutator("brain_percentmutateeffectalterneuron", 0, 0);
	addSettingmutator("brain_percentmutateeffectaddsynapse", 0, 0);
	addSettingmutator("brain_percentmutateeffectremovesynapse", 0, 0);
	addSettingmutator("brain_mutate_mutateeffects", 0, 0);
	
	addSettingmutator("brain_percentmutateeffectaltermutable", 0, 0);
	
	row+=8;
	
	addSettingmutator("brain_costhavingneuron", 0, 0);
	addSettingmutator("brain_costfiringneuron", 0, 0);
	addSettingmutator("brain_costfiringmotorneuron", 0, 0);
	addSettingmutator("brain_costhavingsynapse", 0, 0);
	
	m_dimensions.set(col3 + 50, row-rowspacer+10+10);  // 10 = height of 1 line, 10 = horizontal margin
	BeWidgetPtr w = addWidgetButton( "settingsbrainpanel_close", Vector2i(2, 2), Vector2i(12, 12), "x", BeCommand("gui_togglepanel", "settingsbrainpanel"), EVENT_NOREPEAT, 0 );
		w->isAlignedRight = true;
	
}

Settingsbrainpanel::~Settingsbrainpanel()
{
}
