#include "panel.h"
#include "textinput.h"
#include "canvas.h"
#include "textprinter.h"

Canvas::Canvas(BeFilesystem& filesystem) :
 BeWidgetContainer(true),
 m_x(0),
 m_y(0)
{
	m_active = true;

	Textprinter::Instance()->setUpFonts(filesystem);

	// mouse focus
	mouseFocus = false;
// 	m_focussedwidget = 0;

	// movable widget picking
// 	m_pickedwidget = 0;

	// point the widget width/height pointers to the window's width/height
	m_widthP = settings->winWidth;
	m_heightP = settings->winHeight;

	// set parent pointer to 0
// 	parent = 0;
}

void Canvas::setDefaultZAxis()
{
	// set default z axises to panels
	unsigned int panelc = 0;
	for( childit = children.begin(); childit != children.end(); childit++ )
	{
		boost::static_pointer_cast<Panel>(childit->second)->zaxis = panelc;
		// update sorted drawing/clicking vector
		m_sortedindices.push_back(childit->first);
		panelc++;
	}
}

void Canvas::moveMouse(unsigned int x, unsigned int y)
{
	int diffx = (int)x - m_x;
	int diffy = (int)y - m_y;

	m_x = x;
	m_y = y;

	// resize
	if ( m_pickedwidget && m_pickedwidget->parent->isPanel && m_pickedwidget->parent->isResizable() && m_pickedwidget->parent->children["resizebutton"] == m_pickedwidget ) //
	{
		// limits
		if ( m_pickedwidget->parent->getWidth() + diffx < m_pickedwidget->parent->getMinWidth() )
		{
			m_pickedwidget->parent->setWidth( m_pickedwidget->parent->getMinWidth() );
			diffx = 0;
		}
		if ( m_pickedwidget->parent->getHeight() + diffy < m_pickedwidget->parent->getMinHeight() )
		{
			m_pickedwidget->parent->setHeight( m_pickedwidget->parent->getMinHeight() );
			diffy = 0;
		}
		m_pickedwidget->parent->resize( diffx, diffy );	}


	// move
	else if ( m_pickedwidget && m_pickedwidget->isMovable )
	{
		m_pickedwidget->translate( Vector2i(diffx, diffy) );
	}
	else
	{
		mouseFocus = false;
		if ( mouseOverChild( &m_focussedwidget, m_x, m_y ) )
			mouseFocus = true;
	}
}

bool Canvas::buttonPress(const unsigned int& button)
{
	if ( mouseFocus )
	{
		m_pickedwidget = m_focussedwidget;
		m_pickedwidget->click(button);

		// raise panel of widget?
			BeWidget* w = m_pickedwidget.get();
			raisePanel(w);
			while ( w->parent != 0 )
			{
				w = w->parent;
				if ( w->isPanel )
					raisePanel(w);
			}
		return true;
	}
	return false;
}

void Canvas::buttonRelease(const unsigned int& button)
{
	if ( m_pickedwidget && !m_pickedwidget->isKeyboardGrabbing )
	{
		m_pickedwidget->release(button);
		m_pickedwidget.reset();
	}
}

bool Canvas::keyPress(const SDL_KeyboardEvent& event)
{
	if ( m_pickedwidget && m_pickedwidget->isKeyboardGrabbing )
	{
		boost::static_pointer_cast<BeWidgetTextInput>(m_pickedwidget)->acceptKey(event);
		return true;
	}
	return false;
}

bool Canvas::keyRelease(const SDL_KeyboardEvent& event)
{
	if ( m_pickedwidget && m_pickedwidget->isKeyboardGrabbing )
		return true;
	return false;
}

void Canvas::raisePanel(BeWidget* w)
{
	Panel* rp = static_cast<Panel*>(w);

	if ( rp->m_autofocuswidget )
	{
		m_pickedwidget = rp->m_autofocuswidget;
	}

	if ( rp->zaxis > 0 )
	{
		// the other lower zaxis values get ++
		for( childit = children.begin(); childit != children.end(); childit++ )
		{
			BeWidgetPanelPtr p = boost::static_pointer_cast<Panel>(childit->second);
			if ( p->zaxis < rp->zaxis )
				p->zaxis++;
/*			std::cout << "yeh" << std::endl;*/
		}
		// this zaxis goes to 0
		rp->zaxis = 0;

		// resort m_sortedindices
		for ( int i = m_sortedindices.size(); i>0; i--  )
			for ( int j = 0; j < i-1; j++  )
				if ( boost::static_pointer_cast<Panel>(children[m_sortedindices[j]])->zaxis < boost::static_pointer_cast<Panel>(children[m_sortedindices[j+1]])->zaxis )
				{
					std::string keepS	= m_sortedindices[j];
					m_sortedindices[j]	= m_sortedindices[j+1];
					m_sortedindices[j+1]	= keepS;
				}
	}
}

void Canvas::drawChildren()
{
	// draw the ordered panels
	for( unsigned int i=0; i < m_sortedindices.size(); i++ )
	{
		boost::shared_ptr<Panel> p = boost::static_pointer_cast<Panel>(children[m_sortedindices[i]]);
		if ( !p->isAlwaysOnTop )
			p->draw();
	}
	// draw the ordered panels (on top)
	for( unsigned int i=0; i < m_sortedindices.size(); i++ )
	{
		boost::shared_ptr<Panel> p = boost::static_pointer_cast<Panel>(children[m_sortedindices[i]]);
		if ( p->isAlwaysOnTop )
			p->draw();
	}
}

bool Canvas::mouseOverChild(BeWidgetPtr* fWidget, int x, int y)
{
	for ( int i = m_sortedindices.size()-1; i >= 0; i--  )
 	{
		BeWidgetPtr w = children[m_sortedindices[i]];
		if ( (w->isTouchable && w->isActive() && w->mouseOver(x, y)) || !w->isTouchable )
		{
			// RECURSIVE INTO CONTAINERS
			if ( w->isContainer )
			{
				boost::shared_ptr<BeWidgetContainer> c = boost::static_pointer_cast<BeWidgetContainer>(w);
				if ( c->mouseOverChild( fWidget, x, y ) )
				{
					return true;
				}
				else if ( w->isTouchable )
				{
					*fWidget = w;
					return true;
				}
			}
			else if ( w->isTouchable )
			{
				*fWidget = w;
				return true;
			}
		}
	}
	return false;
}

void Canvas::swapChild(const std::string& child)
{
// 	cerr << "swapping child " << child << endl;
	
	BeWidgetPtr t = get( child );
	if ( t )
	{
		children[child]->swap();
		if ( children[child]->isActive() )
			raisePanel(children[child].get());

		if ( children[child]->isMovable )
		{
			if ( children[child] == m_pickedwidget )
				m_pickedwidget.reset();
		}
		moveMouse(m_x, m_y);
	}
	else
	{
		std::cout << " '" << child << "' is undefined" << std::endl;
	}
}

Canvas::~Canvas() {
	if ( textprinter->m_font )
	{
		/* Destroy the font object. */
// 				FTGL::ftglDestroyFont(fonts[0]);
		delete textprinter->m_font;
// 				delete *fonts;
	}
};
