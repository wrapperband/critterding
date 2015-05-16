#ifndef SETTINGSPANEL_H_INCLUDED
#define SETTINGSPANEL_H_INCLUDED

#include "panel.h"

class Settingspanel : public Panel
{
	public:
		Settingspanel(bool resizable=false);
		virtual ~Settingspanel() {};
	protected:
		unsigned int col1;
		unsigned int col2;
		unsigned int col3;
		unsigned int row;
		unsigned int rowspacer;

		virtual void addSection( const std::string& name, unsigned int posx, unsigned int posy );
		virtual void addMutator( const std::string& name, const BeCommand& cmd1, const BeCommand& cmd2, unsigned int posx, unsigned int posy );
		virtual void addSettingmutator( const std::string& name, unsigned int posx, unsigned int posy );
		virtual void addSettingcheckbox( const std::string& name, unsigned int posx, unsigned int posy );
		virtual void addCheckbox( const std::string& name, const BeCommand& cmd1, unsigned int posx, unsigned int posy );
	private:
		
		const unsigned int m_margin;
		const unsigned int m_button_margin;
		const unsigned int m_time_interval;

};

#endif
