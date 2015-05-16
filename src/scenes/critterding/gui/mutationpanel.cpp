#include "mutationpanel.h"

Mutationpanel::Mutationpanel()
{
	isMovable = true;

	m_localposition.set(2, 24);

	col2 = 140;
	col3 = 180;
	
	addSettingmutator("body_maxmutations", 0, 0);
	addSettingmutator("body_mutationrate", 0, 0);

	row += rowspacer;
	addSettingmutator("brain_maxmutations", 0, 0);
	addSettingmutator("brain_mutationrate", 0, 0);

	m_dimensions.set(col3 + 50, row-rowspacer+10+10);  // 10 = height of 1 line, 10 = horizontal margin
	
	BeWidgetPtr w = addWidgetButton( "mutationpanel_close", Vector2i(2, 2), Vector2i(12, 12), "x", BeCommand("gui_togglepanel", "mutationpanel"), EVENT_NOREPEAT, 0 );
		w->isAlignedRight = true;
}

Mutationpanel::~Mutationpanel()
{
}
