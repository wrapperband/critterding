#ifndef CONTAINER_H_INCLUDED
#define CONTAINER_H_INCLUDED

#include "kernel/be.h"
#include "common/be_event_system.h"
#include "common/be_command.h"
#include "widget.h"

class BeWidgetContainer : public BeWidget
{
	public:
		BeWidgetContainer(bool isresizable);
		virtual ~BeWidgetContainer() {};
		virtual void draw();

		// children map, publically accessible
		bool mouseOverChild( BeWidgetPtr* fBeWidget, int x, int y );

		BeWidgetPtr addWidget( const std::string& name, BeWidgetPtr nwidget );
		BeWidgetPtr addWidget( const std::string& name, const Vector2i& pos, BeWidgetPtr nwidget );

		virtual void resize( int x, int y );
		virtual void updateAbsPosition();
		
		bool isResizable() const { return m_isresizable; }

		int getMarginLeft() const { return m_margin_left; }
		int getMarginRight() const { return m_margin_right; }
		int getMarginTop() const { return m_margin_top; }
		int getMarginBottom() const { return m_margin_bottom; }

		std::map<std::string, BeWidgetPtr> children;
		
		BeWidgetPtr get( const std::string& name );
		
	protected:
		virtual void drawChildren();

		BeWidgetPtr addWidgetButton( const std::string& name, const Vector2i& pos, const Vector2i& dimensions, const std::string& textstring, const Vector2i& textpos, const BeCommand& cmds, EventProperty_Repeat repeat, unsigned int delay );
		BeWidgetPtr addWidgetButton( const std::string& name, const Vector2i& pos, const Vector2i& dimensions, const std::string& textstring, const BeCommand& cmds, EventProperty_Repeat repeat, unsigned int delay );

		BeWidgetPtr addWidgetCheckbox( const std::string& name, const Vector2i& pos, const Vector2i& dimensions, const std::string& textstring, const Vector2i& textpos, const BeCommand& cmds, EventProperty_Repeat repeat, unsigned int delay );
		BeWidgetPtr addWidgetCheckbox( const std::string& name, const Vector2i& pos, const Vector2i& dimensions, const std::string& textstring, const Vector2i& textpos, const BeCommand& cmds, const std::string& settingname, EventProperty_Repeat repeat, unsigned int delay );

		// children map iterator
		std::map <std::string, BeWidgetPtr>::const_iterator childit;
		
		// flags
		bool			m_isresizable;

		// margins
		int			m_margin_left;
		int			m_margin_right;
		int			m_margin_top;
		int			m_margin_bottom;
	private:


};

#endif
