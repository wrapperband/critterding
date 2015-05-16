#ifndef TEXT_H_INCLUDED
#define TEXT_H_INCLUDED

#include "widget.h"
#include <iostream>

class BeWidgetText : public BeWidget
{
	public:
		BeWidgetText() {};
		BeWidgetText( const std::string& newstring )
		{
			set(newstring);
		};
		virtual ~BeWidgetText() {};
		virtual void set(const std::string& newstring);
		const std::string& getString() const { return m_string; }
		virtual void draw();
		const Vector2i& getDimensions() const { return m_dimensions; }
	protected:
	private:
		std::string	m_string;
		Vector2i	m_dimensions;
};

#endif
