#include "exitpanel.h"
#include "text.h"

Exitpanel::Exitpanel() : Panel(false)
{
	isHCentered = true;
	isVCentered = true;
	isAlwaysOnTop = true;
	
	m_dimensions.set(300, 86);

	addWidget("askexit", Vector2i(10, 20), BeWidgetTextPtr(new BeWidgetText("Are you sure you want to exit?")));
	addWidgetButton( "exit-yes", Vector2i( ((m_dimensions.m_x/5)*1)+((m_dimensions.m_x/5)/2)-25, 40 ), Vector2i(50, 30), "Yes", Vector2i(15, 19), BeCommand("quit"), EVENT_NOREPEAT, 0 );
	addWidgetButton( "exit-no",  Vector2i( ((m_dimensions.m_x/5)*3)+((m_dimensions.m_x/5)/2)-25, 40 ), Vector2i(50, 30), "No", Vector2i(18, 19), BeCommand("gui_togglepanel", "panel_exit"), EVENT_NOREPEAT, 0 );
}
