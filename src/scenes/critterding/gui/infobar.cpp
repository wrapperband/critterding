#include "infobar.h"
// #include "src/scenes/critterding/entities/critterb.h"
// #include "src/scenes/critterding/entities/food.h"
#include "common/be_timer.h"
#include "../entities/statsbuffer.h"
#include "gui/textprinter.h"
#include <GL/glew.h>
#include <iomanip>

Infobar::Infobar(BeTimer* timer) : Panel(false), m_timer(timer)
{
	statsBuffer = Statsbuffer::Instance();
	energy = settings->getCVarPtr("energy");

	m_active = true;
	isMovable = true;
	
	m_localposition.set(2, 2);

	m_dimensions.set(520, 20);

	hsp = 10;
	vsp = 13;
}

void Infobar::mark()
{
	fps.mark( m_timer->getSeconds() );
}

void Infobar::draw()
{
	if (m_active)
	{

		drawBackground();

		int col1 = (getWidth()/4);
		int col2 = col1 * 2;
		int col3 = col1 * 3;
		int col4 = col1 * 4;
// 		float col4 = col1 * 4.0f;

		int linespacer = 0;
		
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
					
					
 			glVertex2i(getPosition().m_x+col1, getPosition().m_y+linespacer);
 			glVertex2i(getPosition().m_x+col1, getPosition().m_y+getHeight() - linespacer);
 			glVertex2i(getPosition().m_x+col2, getPosition().m_y+linespacer);
 			glVertex2i(getPosition().m_x+col2, getPosition().m_y+getHeight() - linespacer);
 			glVertex2i(getPosition().m_x+col3, getPosition().m_y+linespacer);
 			glVertex2i(getPosition().m_x+col3, getPosition().m_y+getHeight() - linespacer);
					
				glEnd();
				glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			glPopMatrix();
		}


// 		glColor3f(1.0f, 1.0f, 1.0f);
// 		glEnable(GL_TEXTURE_2D);

		// Row 1
		Textprinter::Instance()->print(getPosition().m_x+hsp,		getPosition().m_y+vsp,	"fps:");
		
		
		Textprinter::Instance()->printR(getPosition().m_x+col1-hsp,	getPosition().m_y+vsp,	"%0.2f",	fps.getCurrentFramesPerSecondf());
		
		Textprinter::Instance()->printR(getPosition().m_x+col2-hsp,	getPosition().m_y+vsp,	"%0.2f",	fps.getAverageFramesPerSecondf());

// 		Textprinter::Instance()->print(getPosition().m_x+col1+hsp,		getPosition().m_y+vsp,	"avg fps:");
// 			Textprinter::Instance()->printR(getPosition().m_x+col2-hsp,	getPosition().m_y+vsp+20,	"%0.2f", (1000.0f * statsBuffer->frameCounter) / m_timer->getTotalMilliSeconds());
		
		Textprinter::Instance()->print(getPosition().m_x+col2+hsp,	getPosition().m_y+vsp,	"critters:");
// 		Textprinter::Instance()->printR(getPosition().m_x+col2-hsp,	getPosition().m_y+vsp,	"%1u",		settings->info_critters);
		Textprinter::Instance()->printR(getPosition().m_x+col3-hsp,	getPosition().m_y+vsp,	"%1u",		statsBuffer->current.critters);

		Textprinter::Instance()->print(getPosition().m_x+col3+hsp,	getPosition().m_y+vsp,	"food:");
// 		Textprinter::Instance()->printR(getPosition().m_x+col3-hsp,	getPosition().m_y+vsp,	"%1u/%1u",	settings->info_food, settings->getCVar("energy"));
// 		Textprinter::Instance()->printR(getPosition().m_x+col4-hsp,	getPosition().m_y+vsp,	"%1u/%1u",	(statsBuffer->current.food * settings->getCVar("food_maxenergy"))/1000, (*energy * settings->getCVar("food_maxenergy"))/1000);
		Textprinter::Instance()->printR(getPosition().m_x+col4-hsp,	getPosition().m_y+vsp,	"%1u/%1u",	statsBuffer->current.food, *energy);

// 		Textprinter::Instance()->print(col3+hsp,	vsp,	"corpses:");
// 		Textprinter::Instance()->printR(col4-hsp,	vsp,	"%1u",		Settings::Instance()->info_corpses);

	// Row 2
// 		Textprinter::Instance()->print(hsp,		vsp*2,	"neu/cri:");
// 		Textprinter::Instance()->printR(col1-hsp,	vsp*2,	"%1.2f",	(float)totalneurons / Settings::Instance()->info_critters);
// 
// 		Textprinter::Instance()->print(col1+hsp,	vsp*2,	"syn/cri:");
// 		Textprinter::Instance()->printR(col2-hsp,	vsp*2,	"%1.2f",	(float)totalsynapses / Settings::Instance()->info_critters);
// 
//  		Textprinter::Instance()->print(col2+hsp,	vsp*2,	"syn/neu:");
//  		Textprinter::Instance()->printR(col3-hsp,	vsp*2,	"%1.2f",	(float)totalsynapses / totalneurons);
// 
// 		Textprinter::Instance()->print(col3+hsp,	vsp*2,	"bullets:");
// 		Textprinter::Instance()->printR(col4-hsp,	vsp*2,	"%1u",		Settings::Instance()->info_bullets);


// 		glDisable(GL_TEXTURE_2D);
	}
}

Infobar::~Infobar()
{
}
