#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include "panel.h"

class BeWidgetButton : public Panel
{
	public:
		BeWidgetButton();
		virtual ~BeWidgetButton() {};

// 		virtual void setText( unsigned int posx, unsigned int posy, const std::string& buttontext );
		virtual void click( const int& button );
		virtual void release( const int& button );
		virtual void genEvent( const int& button, const std::string& name, const BeCommand& cmds, EventProperty_Repeat repeat, unsigned int delay );
		virtual BeCommand* getCommand(const int& button);
	protected:
		std::map<unsigned int, long unsigned int> buttonlist;
		std::string eventname;
	private:
// 		vector<string> eventnames;

// 		typedef map <string, cmd*>::const_iterator cmdlist_iterator;
// 		cmdlist_iterator cmdit;

};

#endif
