#ifndef WIDGET_H_INCLUDED
#define WIDGET_H_INCLUDED

#include <vector>
#include <map>

// #include "textprinter.h"
#include "math/vector2i.h"
// #include "kernel/be.h"

class BeWidgetContainer;
class Textprinter;
class Settings;

class BeWidget
{
	public:
		BeWidget();
		virtual ~BeWidget() {};

		virtual void		draw() {};
		virtual void		click(const int& button);
		virtual void		release(const int& button);
		virtual void		swap();
		bool			isActive() const { return m_active; }
		void			activate() { m_active = true; }
		void			deactivate() { m_active = false; }

		// placement
		bool			isMovable  ;// movable (pickable) object
		bool			isVCentered;
		bool			isHCentered;
		bool			isAlignedRight;
		bool			isAlignedBottom;

		// touchable object (allows ignoring containers as clickable)
		bool			isTouchable;
		// types
		bool			isContainer;
		bool			isPanel;

		// Does this widget grab text input
		bool			isKeyboardGrabbing;

		// drawing
		bool			isTransparant;
		
		// width/height accessors
		void			setDimensions(const Vector2i& dimensions) { m_dimensions = dimensions; }
		void			setWidth(const int width) { m_dimensions.m_x = width; }
		void			setHeight(const int height) { m_dimensions.m_y = height; }
		int			getWidth() const
		{
			if ( m_active )
				return *m_widthP;
			return 0;
		}
		int			getHeight() const
		{
			if ( m_active )
				return *m_heightP;
			return 0;
		}

		// pointer to parent
		BeWidgetContainer*	parent;

		// is mouse over widget
		bool			mouseOver( int x, int y ) const;

		// widget ops
		virtual void		translate( const Vector2i& pos );
		virtual void		resize( const Vector2i& pos );
		virtual void		updateAbsPosition();
		
		int			getMinWidth() const { return m_min_width; }
		int			getMinHeight() const { return m_min_height; }

		const Vector2i&		getPosition() const { return m_absposition; }
		const Vector2i&		getLocalPosition() const { return m_localposition; }
		Textprinter*		textprinter;
		Vector2i		m_localposition;
	protected:
		Vector2i		m_absposition;
		bool			m_active;
		
		Settings*		settings;

		// width and height
		Vector2i		m_dimensions;
		int*			m_widthP;
		int*			m_heightP;

		int			m_min_width;
		int			m_min_height;
	private:
};

#endif
