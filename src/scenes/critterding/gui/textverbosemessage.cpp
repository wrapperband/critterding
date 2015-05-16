#include "textverbosemessage.h"
#include "gui/textprinter.h"
#include <GL/glew.h>

Textverbosemessage::Textverbosemessage() : Panel(false)
{
	m_localposition.set(2, 24);
	
	m_dimensions.set(520, 75);

	col2 = (unsigned int)(((float)m_dimensions.m_x/16)*11);
	
// 	active = false;
	isMovable = true;
	
	maxMessages = 5;
	msgLifetime = 0;
// 	longestLength = 0;

	BeWidgetPtr w = addWidgetButton( "textverbosemessage_close", Vector2i(2, 2), Vector2i(12, 12), "x", BeCommand("gui_togglepanel", "textverbosemessage"), EVENT_NOREPEAT, 0 );
		w->isAlignedRight = true;
	
}

void Textverbosemessage::addBirth(std::stringstream& streamptr)
{
	vmsg *Msg = new vmsg();
	Msg->str = streamptr.str();
// 	Msg->appeartime = Timer::Instance()->sdl_lasttime;
	births.push_back(Msg);

	// to prevent overfilling:
	deleteExpiredMsg();
}

void Textverbosemessage::addDeath(std::stringstream& streamptr)
{
	vmsg *Msg = new vmsg;
	Msg->str = streamptr.str();
// 	Msg->appeartime = Timer::Instance()->sdl_lasttime;
	deaths.push_back(Msg);

	// to prevent overfilling:
	deleteExpiredMsg();
}

void Textverbosemessage::deleteExpiredMsg()
{
	if ( !births.empty() )
	{
		if (
			births.size() > maxMessages
// 			|| ( msgLifetime > 0 && Timer::Instance()->sdl_lasttime - births[0]->appeartime > msgLifetime )
		)
		{
			delete births[0];
			births.erase(births.begin()+0);
		}
	}
	if ( !deaths.empty() )
	{
		if (
			deaths.size() > maxMessages
// 			|| ( msgLifetime > 0 && Timer::Instance()->sdl_lasttime - deaths[0]->appeartime > msgLifetime )
		)
		{
			delete deaths[0];
			deaths.erase(deaths.begin()+0);
		}
	}
}

void Textverbosemessage::draw()
{
	if ( m_active )
	{
// 		updateAbsPosition();

		deleteExpiredMsg();

		drawBackground();
		drawChildren();
		//	drawBorders();
		{
			glPushMatrix();
				glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
				glBegin(GL_LINES);
					glVertex2i(m_absposition.m_x,         m_absposition.m_y+m_dimensions.m_y);
					glVertex2i(m_absposition.m_x,         m_absposition.m_y);

					glVertex2i(m_absposition.m_x,         m_absposition.m_y);
					glVertex2i(m_absposition.m_x+m_dimensions.m_x, m_absposition.m_y);

					glVertex2i(m_absposition.m_x+m_dimensions.m_x, m_absposition.m_y);
					glVertex2i(m_absposition.m_x+m_dimensions.m_x, m_absposition.m_y+m_dimensions.m_y);

					glVertex2i(m_absposition.m_x+m_dimensions.m_x, m_absposition.m_y+m_dimensions.m_y);
					glVertex2i(m_absposition.m_x,         m_absposition.m_y+m_dimensions.m_y);
					
					
			glVertex2f(getPosition().m_x+col2, getPosition().m_y);
			glVertex2f(getPosition().m_x+col2, getPosition().m_y+getHeight());
					
				glEnd();
				glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			glPopMatrix();
		}

		if ( !births.empty() )
		{
		// render text
// 			glEnable(GL_TEXTURE_2D);

			glColor3f(1.0f, 1.0f, 1.0f);

			for ( unsigned int i = 0; i < births.size(); i++ )
				Textprinter::Instance()->print(getPosition().m_x+10, getPosition().m_y + 13 + 1 + (i*13), births[i]->str.c_str());

// 			glDisable(GL_TEXTURE_2D);
		}

		if ( !deaths.empty() )
		{
		// render text
// 			glEnable(GL_TEXTURE_2D);

			glColor3f(1.0f, 1.0f, 1.0f);

			for ( unsigned int i = 0; i < deaths.size(); i++ )
				Textprinter::Instance()->print(getPosition().m_x + col2 + 10, (getPosition().m_y + 13) + 1 + (i*13), deaths[i]->str.c_str());

// 			glDisable(GL_TEXTURE_2D);
		}
	}
}

Textverbosemessage::~Textverbosemessage()
{
	for ( unsigned int i=0; i < births.size(); i++ )
		delete births[i];
	for ( unsigned int i=0; i < deaths.size(); i++ )
		delete deaths[i];
}

