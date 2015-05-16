#include "button.h"
#include "text.h"
// #include "common/be_event_system.h"

BeWidgetButton::BeWidgetButton() : Panel(false)
{
	isTransparant = false;
	m_active = true;
}

// void BeWidgetButton::setText( unsigned int posx, unsigned int posy, const std::string& buttontext )
// {
// 	addWidget("butontext", posx, posy, BeWidgetTextPtr(new BeWidgetText(buttontext)));
// }

void BeWidgetButton::genEvent(const int& button, const std::string& name, const BeCommand& cmds, EventProperty_Repeat repeat, unsigned int delay )
{
	eventname = name;
	buttonlist[button] = BeEventSystem::Instance()->registerEvent( DOWN, name, boost::shared_ptr<Event>(new Event( cmds, repeat, delay) ) );
}

BeCommand* BeWidgetButton::getCommand(const int& button)
{
	return BeEventSystem::Instance()->getEvent(buttonlist[button])->getCommand();
}

void BeWidgetButton::click(const int& button)
{
	BeEventSystem::Instance()->activateKeystate( buttonlist[button] );
}

void BeWidgetButton::release(const int& button)
{
	BeEventSystem::Instance()->deactivateKeystate( buttonlist[button] );
}
