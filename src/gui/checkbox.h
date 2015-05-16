#ifndef CHECKBOX_H_INCLUDED
#define CHECKBOX_H_INCLUDED

#include "button.h"

class BeWidgetCheckbox : public BeWidgetButton
{
	public:
		BeWidgetCheckbox();
		virtual ~BeWidgetCheckbox() {};

		virtual void draw();
// 		void click(const int& button);
// 		void release(const int& button);
		void genEvent( const int& button, const std::string& name, const BeCommand& cmds, EventProperty_Repeat repeat, unsigned int delay );
		void genEvent(const int& button, const std::string& name, const BeCommand& cmds, const std::string& settingname, EventProperty_Repeat repeat, unsigned int delay );
	private:
		const int* settingPointer;
};

#endif
