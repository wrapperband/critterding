#include "checkbox.h"

BeWidgetCheckbox::BeWidgetCheckbox() :
 settingPointer(0)
{
	m_active = true;
}

void BeWidgetCheckbox::draw()
{
	if (m_active)
	{
		drawBackground();
		drawBorders();
		if ( *settingPointer )
			drawChildren();
	}
}

void BeWidgetCheckbox::genEvent(const int& button, const std::string& name, const BeCommand& cmds, EventProperty_Repeat repeat, unsigned int delay )
{
	eventname = name;
	buttonlist[button] = BeEventSystem::Instance()->registerEvent( DOWN, eventname, boost::shared_ptr<Event>(new Event( cmds, repeat, delay) ) );
	settingPointer = settings->getCVarPtr( cmds.m_argString );
}

void BeWidgetCheckbox::genEvent(const int& button, const std::string& name, const BeCommand& cmds, const std::string& settingname, EventProperty_Repeat repeat, unsigned int delay )
{
	eventname = name;
	buttonlist[button] = BeEventSystem::Instance()->registerEvent( DOWN, eventname, boost::shared_ptr<Event>(new Event( cmds, repeat, delay) ) );
	settingPointer = settings->getCVarPtr( settingname );
}
