#ifndef PANEL_H_INCLUDED
#define PANEL_H_INCLUDED

#include "container.h"

class Panel : public BeWidgetContainer
{
	public:
		Panel( bool resizable );
		virtual ~Panel() {};
		void		draw();
		unsigned int	zaxis;
		bool		isAlwaysOnTop;

		// autofocus widgetpointer
		BeWidgetPtr 		m_autofocuswidget;

	protected:
// 		inline void	drawBackground();
// 		inline void	drawBorders();

		void drawBackground();
// 		{
// 			if (isTransparant)
// 			{
// 				glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
// 				glEnable(GL_BLEND);
// 			}
// 
// 			glPushMatrix();
// 				glColor4f(0.05f, 0.05f, 0.05f, 0.75f);
// 			// 	glTranslatef(0.5f, 0.5f, 0.0f); // pixel precision offset
// 				glBegin(GL_QUADS);
// 					glVertex2i(m_absposition.m_x,         m_absposition.m_y+m_dimensions.m_y);
// 					glVertex2i(m_absposition.m_x,         m_absposition.m_y);
// 					glVertex2i(m_absposition.m_x+m_dimensions.m_x, m_absposition.m_y);
// 					glVertex2i(m_absposition.m_x+m_dimensions.m_x, m_absposition.m_y+m_dimensions.m_y);
// 				glEnd();
// 			glPopMatrix();
// 
// 			if (isTransparant)
// 				glDisable(GL_BLEND);
// 		}

		void drawBorders();
// 		{
// 			glPushMatrix();
// 				glColor3f(0.5f, 0.5f, 0.5f);
// 			// 	glTranslatef(0.5f, 0.5f, 0.0f); // pixel precision offset
// 				glBegin(GL_LINES);
// 					glVertex2i(m_absposition.m_x,         m_absposition.m_y+m_dimensions.m_y);
// 					glVertex2i(m_absposition.m_x,         m_absposition.m_y);
// 
// 					glVertex2i(m_absposition.m_x,         m_absposition.m_y);
// 					glVertex2i(m_absposition.m_x+m_dimensions.m_x, m_absposition.m_y);
// 
// 					glVertex2i(m_absposition.m_x+m_dimensions.m_x, m_absposition.m_y);
// 					glVertex2i(m_absposition.m_x+m_dimensions.m_x, m_absposition.m_y+m_dimensions.m_y);
// 
// 					glVertex2i(m_absposition.m_x+m_dimensions.m_x, m_absposition.m_y+m_dimensions.m_y);
// 					glVertex2i(m_absposition.m_x,         m_absposition.m_y+m_dimensions.m_y);
// 				glEnd();
// 			glPopMatrix();
// 		}
		
// 		inline void	switch3D();
// 		inline void	switch2D();

	private:

};

#endif
