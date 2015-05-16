#include "widget.h"
#include "container.h"
#include "textprinter.h"

BeWidget::BeWidget() :
 parent(0),
 textprinter(Textprinter::Instance()),
 settings(Settings::Instance()),
 m_dimensions(0,0),
//  m_widthP(&m_dimensions.m_x),
//  m_heightP(&m_dimensions.m_y),
 m_min_width(16),
 m_min_height(16)
{
	m_widthP = &m_dimensions.m_x;
	m_heightP = &m_dimensions.m_y;
	isMovable = false;

	isVCentered = false;
	isHCentered = false;
	isAlignedRight = false;
	isAlignedBottom = false;

	isTouchable = false;
	isContainer = false;
	isPanel = false;

	isKeyboardGrabbing = false;

	isTransparant = false;
	m_active = true;
}

bool BeWidget::mouseOver(int x, int y) const
{
	if ( m_active && x > m_absposition.m_x && x < m_absposition.m_x+(int)getWidth() && y > m_absposition.m_y && y < m_absposition.m_y+(int)getHeight() )
		return true;
	return false;
}

void BeWidget::translate(const Vector2i& pos)
{
	m_localposition = m_localposition + pos;
	updateAbsPosition();
}

void BeWidget::updateAbsPosition()
{
	// FIXME isAlignedRight & bottom exceptions
	m_absposition = m_localposition;
	if ( parent )
	{
		m_absposition.m_x += parent->m_absposition.m_x + static_cast<BeWidgetContainer*>(parent)->getMarginLeft();
		m_absposition.m_y += parent->m_absposition.m_y + static_cast<BeWidgetContainer*>(parent)->getMarginTop();
	}
}

void BeWidget::swap()
{
	m_active = !m_active;
	if ( m_active )
		updateAbsPosition();
}

void BeWidget::resize( const Vector2i& pos ){};
void BeWidget::click(const int& button) {};
void BeWidget::release(const int& button) {};
