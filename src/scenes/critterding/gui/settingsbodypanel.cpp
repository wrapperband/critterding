#include "settingsbodypanel.h"

Settingsbodypanel::Settingsbodypanel()
{
	isMovable = true;

	m_localposition.set(2, 24);

	col2 = 370;
	col3 = 450;

	addSettingmutator("body_maxbodyparts", 0, 0);
	addSettingmutator("body_maxbodypartsatbuildtime", 0, 0);
	addSettingmutator("body_minbodypartsize", 0, 0);
	addSettingmutator("body_maxbodypartsize", 0, 0);
	addSettingmutator("body_minheadsize", 0, 0);
	addSettingmutator("body_maxheadsize", 0, 0);

	row+=8;

	addSettingmutator("body_maxconstraintangle", 0, 0);
	addSettingmutator("body_maxconstraintlimit", 0, 0);
	addSettingmutator("body_constraintpositionrange", 0, 0);
	
	row+=8;

// 	addSettingmutator("body_percentmutateminenergyproc", 0, 0);
// 	addSettingmutator("body_percentmutatesightrange", 0, 0);
// 	addSettingmutator("body_percentmutatemaxlifetime", 0, 0);
	addSettingmutator("body_percentmutateeffectchangecolor", 0, 0);
	addSettingmutator("body_percentmutateeffectchangecolor_slightly", 0, 0);
	addSettingmutator("body_percentmutateeffectaddbodypart", 0, 0);
	addSettingmutator("body_percentmutateeffectremovebodypart", 0, 0);
	addSettingmutator("body_percentmutateeffectresizebodypart", 0, 0);
	addSettingmutator("body_percentmutateeffectresizebodypart_slightly", 0, 0);
	addSettingmutator("body_percentmutateeffectchangeconstraintlimits", 0, 0);
	addSettingmutator("body_percentmutateeffectchangeconstraintlimits_slightly", 0, 0);
	addSettingmutator("body_percentmutateeffectchangeconstraintangles", 0, 0);
	addSettingmutator("body_percentmutateeffectchangeconstraintangles_slightly", 0, 0);
	addSettingmutator("body_percentmutateeffectchangeconstraintposition", 0, 0);
	addSettingmutator("body_percentmutateeffectchangeconstraintposition_slightly", 0, 0);
	addSettingmutator("body_percentmutateeffectresizehead", 0, 0);
	addSettingmutator("body_percentmutateeffectresizehead_slightly", 0, 0);
	addSettingmutator("body_percentmutateeffectrepositionhead", 0, 0);
	
	row+=8;
	
	addSettingmutator("body_costhavingbodypart", 0, 0);
	addSettingmutator("body_costtotalweight", 0, 0);

	m_dimensions.set(col3 + 50, row-rowspacer+10+10);  // 10 = height of 1 line, 10 = horizontal margin
	BeWidgetPtr w = addWidgetButton( "settingsbodypanel_close", Vector2i(2, 2), Vector2i(12, 12), "x", BeCommand("gui_togglepanel", "settingsbodypanel"), EVENT_NOREPEAT, 0 );
		w->isAlignedRight = true;
}

Settingsbodypanel::~Settingsbodypanel()
{
}
