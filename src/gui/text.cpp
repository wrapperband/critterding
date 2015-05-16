#include "text.h"
#include "container.h"
#include "textprinter.h"

void BeWidgetText::draw()
{
// 	glColor3f(1.0f, 1.0f, 1.0f);

	if ( isHCentered )
	{
		m_absposition.m_x = parent->getPosition().m_x + (parent->getWidth()/2) - (m_dimensions.m_x/2);
		m_absposition.m_y = parent->getPosition().m_y + (parent->getHeight()/2) - (m_dimensions.m_y/2) + m_dimensions.m_y;
	}

	textprinter->print( m_absposition, m_string );
}

void BeWidgetText::set(const std::string& newstring)
{
	m_string = newstring;
	textprinter->getDimensions(m_string, m_dimensions);
}
