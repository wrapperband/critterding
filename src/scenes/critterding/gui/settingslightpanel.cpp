#include "settingslightpanel.h"

Settingslightpanel::Settingslightpanel()
{
	isMovable = true;

	m_localposition.set(2, 24);

	col2 = 200;
	col3 = 250;

	addSettingmutator("light_ambient_modelR", 0, 0);
	addSettingmutator("light_ambient_modelG", 0, 0);
	addSettingmutator("light_ambient_modelB", 0, 0);
	
	row+=8;

	addSettingmutator("light_ambientR", 0, 0);
	addSettingmutator("light_ambientG", 0, 0);
	addSettingmutator("light_ambientB", 0, 0);

	row+=8;

	addSettingmutator("light_diffuseR", 0, 0);
	addSettingmutator("light_diffuseG", 0, 0);
	addSettingmutator("light_diffuseB", 0, 0);

	row+=8;

	addSettingmutator("light_specularR", 0, 0);
	addSettingmutator("light_specularG", 0, 0);
	addSettingmutator("light_specularB", 0, 0);
	
	row+=8;

	addSettingmutator("light_attenuation_constant", 0, 0);
	addSettingmutator("light_attenuation_linear", 0, 0);
	addSettingmutator("light_attenuation_quadratic", 0, 0);

	m_dimensions.set(col3 + 50, row-rowspacer+10+10);  // 10 = height of 1 line, 10 = horizontal margin
	BeWidgetPtr w = addWidgetButton( "settingslightpanel_close", Vector2i(2, 2), Vector2i(12, 12), "x", BeCommand("gui_togglepanel", "settingslightpanel"), EVENT_NOREPEAT, 0 );
		w->isAlignedRight = true;
}

Settingslightpanel::~Settingslightpanel()
{
}
