#include "text.h"
#include "text_uintp.h"
#include "textinput.h"
#include "button.h"
#include "checkbox.h"
#include "container.h"

BeWidgetContainer::BeWidgetContainer( bool isresizable ) : 
 m_isresizable(isresizable),
 m_margin_left(0),
 m_margin_right(0),
 m_margin_top(0),
 m_margin_bottom(0)
{
	isContainer = true;
}

void BeWidgetContainer::draw()
{
	if ( m_active )
		drawChildren();
}

void BeWidgetContainer::drawChildren()
{
	for( childit = children.begin(); childit != children.end(); ++childit )
		childit->second->draw();
}

BeWidgetPtr BeWidgetContainer::addWidget( const std::string& name, BeWidgetPtr nwidget )
{
	return addWidget(name, Vector2i(0, 0), nwidget);
}

BeWidgetPtr BeWidgetContainer::addWidget( const std::string& name, const Vector2i& pos, BeWidgetPtr nwidget )
{
	nwidget->parent = this;
	nwidget->translate(pos);
	children[name] = nwidget;
	return nwidget;
}

// BUTTON
	BeWidgetPtr BeWidgetContainer::addWidgetButton( const std::string& name, const Vector2i& pos, const Vector2i& dimensions, const std::string& textstring, const Vector2i& textpos, const BeCommand& cmds, EventProperty_Repeat repeat, unsigned int delay )
	{
		BeWidgetButtonPtr t = BeWidgetButtonPtr(new BeWidgetButton());
		t->setDimensions(dimensions);
		t->addWidget( "buttontext", textpos, BeWidgetTextPtr(new BeWidgetText(textstring)) );
		t->genEvent(1, name, cmds, repeat, delay);
		addWidget( name, pos, t );
		return t;
	}

	BeWidgetPtr BeWidgetContainer::addWidgetButton( const std::string& name, const Vector2i& pos, const Vector2i& dimensions, const std::string& textstring, const BeCommand& cmds, EventProperty_Repeat repeat, unsigned int delay )
	{
		BeWidgetButtonPtr t = BeWidgetButtonPtr(new BeWidgetButton());
		t->setDimensions(dimensions);
		t->genEvent(1, name, cmds, repeat, delay);

		// button text
		BeWidgetPtr subtext = t->addWidget( "buttontext", BeWidgetTextPtr(new BeWidgetText(textstring)) );
		subtext->isVCentered = true;
		subtext->isHCentered = true;

		addWidget( name, pos, t );
		return t;
	}

// CHECKBOX
	BeWidgetPtr BeWidgetContainer::addWidgetCheckbox( const std::string& name, const Vector2i& pos, const Vector2i& dimensions, const std::string& textstring, const Vector2i& textpos, const BeCommand& cmds, EventProperty_Repeat repeat, unsigned int delay )
	{
		BeWidgetCheckboxPtr t = BeWidgetCheckboxPtr(new BeWidgetCheckbox());
		t->setDimensions(dimensions);
		t->addWidget( "buttontext", textpos, BeWidgetTextPtr(new BeWidgetText(textstring)) );
		t->genEvent(1, name, cmds, repeat, delay);
		addWidget( name, pos, t );
		return t;
	}

	BeWidgetPtr BeWidgetContainer::addWidgetCheckbox( const std::string& name, const Vector2i& pos, const Vector2i& dimensions, const std::string& textstring, const Vector2i& textpos, const BeCommand& cmds, const std::string& settingname, EventProperty_Repeat repeat, unsigned int delay )
	{
		BeWidgetCheckboxPtr t = BeWidgetCheckboxPtr(new BeWidgetCheckbox());
		t->setDimensions(dimensions);
		t->addWidget( "buttontext", textpos, BeWidgetTextPtr(new BeWidgetText(textstring)) );
		t->genEvent(1, name, cmds, settingname, repeat, delay);
		addWidget( name, pos, t );
		return t;
	}

bool BeWidgetContainer::mouseOverChild(BeWidgetPtr* fBeWidget, int x, int y)
{
	for( childit = children.begin(); childit != children.end(); ++childit )
	{
		if ( (childit->second->isTouchable && childit->second->isActive() && childit->second->mouseOver(x, y)) || !childit->second->isTouchable )
		{
			// RECURSIVE INTO CONTAINERS
			if ( childit->second->isContainer )
			{
				boost::shared_ptr<BeWidgetContainer> c = boost::static_pointer_cast<BeWidgetContainer>(childit->second);
				if ( c->mouseOverChild( fBeWidget, x, y ) )
				{
					return true;
				}
				else if ( childit->second->isTouchable )
				{
					*fBeWidget = childit->second;
					return true;
				}
			}
			else if ( childit->second->isTouchable )
			{
				*fBeWidget = childit->second;
				return true;
			}
		}
	}
	return false;
}

BeWidgetPtr BeWidgetContainer::get( const std::string& name )
{
	childit = children.find(name);
	if ( childit != children.end() )
	{
		return childit->second;
	}
	return boost::shared_ptr<BeWidget>();
}


void BeWidgetContainer::resize(int x, int y)
{
	if ( isResizable() )
	{
		m_dimensions.m_x += x;
		m_dimensions.m_y += y;
		updateAbsPosition();
	}
}

void BeWidgetContainer::updateAbsPosition()
{
	m_absposition = m_localposition;
	if ( parent )
	{
		if ( isHCentered )
			m_absposition.m_x = parent->m_absposition.m_x + (parent->getWidth()/2) - getWidth()/2;
		else if ( isAlignedRight )
			m_absposition.m_x = parent->m_absposition.m_x + (parent->getWidth() - getWidth()) - m_localposition.m_x -  parent->getMarginRight();
		else
			m_absposition.m_x += parent->m_absposition.m_x + parent->getMarginLeft();

		if ( isVCentered )
			m_absposition.m_y = parent->m_absposition.m_y + (parent->getHeight()/2) - getHeight()/2;
		else if ( isAlignedBottom )
			m_absposition.m_y = parent->m_absposition.m_y + (parent->getHeight()) - getHeight() - m_localposition.m_y -  parent->getMarginBottom();
		else
			m_absposition.m_y += parent->m_absposition.m_y + parent->getMarginTop();
	}

	// update children
	for( childit = children.begin(); childit != children.end(); ++childit )
		childit->second->updateAbsPosition();
}
