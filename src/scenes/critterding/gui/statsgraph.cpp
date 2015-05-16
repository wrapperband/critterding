#include "statsgraph.h"
#include "scenes/critterding/entities/statsbuffer.h"
#include <GL/glew.h>

Statsgraph::Statsgraph() : Panel(true)
{
	m_active = false;
	isMovable = true;
	statsBuffer = Statsbuffer::Instance();
	m_dimensions.set(700, 100);
	m_localposition.set(2, 24);
	BeWidgetPtr w = addWidgetButton( "statsgraph_close", Vector2i(2, 2), Vector2i(12, 12), "x", BeCommand("gui_togglepanel", "statsgraph"), EVENT_NOREPEAT, 0 );
		w->isAlignedRight = true;
}

void Statsgraph::draw()
{
	if (m_active)
	{
		drawBackground();
		drawBorders();

		const unsigned int number( statsBuffer->snapshots.size() );
		
		if ( number > 0 )
		{
			const unsigned int number_minus_one( number-1 );
			
			// inform statsbuffer from where to start considering the highest value for the graph
				int start = 0;
				if ( number > (unsigned int)getWidth() )
					start = number - getWidth();
				statsBuffer->m_graph_consider_highest_start = start;
			
			if ( statsBuffer->m_graph_highest > 0 )
			{
				const float heightratio = (float)getHeight() / statsBuffer->m_graph_highest;
			
				// set constants
				const float position_offset_x( getPosition().m_x - start );
				const float position_offset_x_plus( getPosition().m_x + 1 - start );
				const float position_offset_y( getPosition().m_y + getHeight() );

				// draw the number of critters graph
				glColor3f(0.0f, 1.0f, 0.0f);
				glBegin(GL_LINES);
					for ( unsigned int i=start; i < number_minus_one; ++i )
					{
						glVertex2f( position_offset_x + i, position_offset_y - (heightratio*statsBuffer->snapshots[i].food) );
						glVertex2f( position_offset_x_plus + i, position_offset_y - (heightratio*statsBuffer->snapshots[i+1].food) );
					}

				glColor3f(1.0f, 0.0f, 0.0f);
					for ( unsigned int i=start; i < number_minus_one; ++i )
					{
						glVertex2f( position_offset_x + i, position_offset_y - (heightratio*statsBuffer->snapshots[i].critters) );
						glVertex2f( position_offset_x_plus + i, position_offset_y - (heightratio*statsBuffer->snapshots[i+1].critters) );
					}
				glEnd();
			}
		}

		drawChildren();
	}
}

Statsgraph::~Statsgraph()
{
}



























// #include "statsgraph.h"
// 
// Statsgraph::Statsgraph() : Panel(true)
// {
// 	m_active = false;
// 	isMovable = true;
// 
// 	statsBuffer = Statsbuffer::Instance();
// 
// 	m_dimensions.set(780, 100);
// 	
// 	m_localposition.set(2, 24);
// 
// }
// 
// void Statsgraph::draw()
// {
// 	if (m_active)
// 	{
// 		// hack
// // 		v_width = *parent->v_widthP - 20;
// 
// // 		updateAbsPosition();
// 		drawBackground();
// 		drawBorders();
// 
// 		int number = statsBuffer->snapshots.size();
// 		
// 		if ( number > 0 )
// 		{
// 			int start = 0;
// 			if ( number > (int)getWidth() )
// 				start = number - getWidth();
// 			
// 		// find the highest value in the stats vector
// 			unsigned int highest = 0;
// 			for ( int i=start; i < number; i++ )
// 			{
// /*				unsigned int sum = statsBuffer->snapshots[i].food + statsBuffer->snapshots[i].critters;
// 				if ( sum > highest )
// 					highest = sum;*/
// // 				unsigned int sum = statsBuffer->snapshots[i].food + statsBuffer->snapshots[i].critters;
// 				if ( statsBuffer->snapshots[i].food > highest )
// 					highest = statsBuffer->snapshots[i].food;
// 				if ( statsBuffer->snapshots[i].critters > highest )
// 					highest = statsBuffer->snapshots[i].critters;
// 			}
// // 			cerr << highest << endl;
// 
// 			if ( highest > 0 )
// 			{
// 				// so,  highest ~ getHeight()
// 				// and, number ~ boxwidth
// 				float heightratio = ((float)getHeight()/highest);
// 			
// 			// draw the number of critters graph
// 				glColor3f(0.0f, 1.0f, 0.0f);
// 				glBegin(GL_LINES);
// 					for ( int i=start; i < number-1; i++ )
// 					{
// 						glVertex2f( getPosition().m_x+i-start, getPosition().m_y+getHeight()-(heightratio*statsBuffer->snapshots[i].food) );
// 						glVertex2f( getPosition().m_x+i+1-start, getPosition().m_y+getHeight()-(heightratio*statsBuffer->snapshots[i+1].food) );
// 					}
// // 				glEnd();
// 
// 				glColor3f(1.0f, 0.0f, 0.0f);
// // 				glBegin(GL_LINES);
// 					for ( int i=start; i < number-1; i++ )
// 					{
// 						glVertex2f( getPosition().m_x+i-start, getPosition().m_y+getHeight()-(heightratio*statsBuffer->snapshots[i].critters) );
// 						glVertex2f( getPosition().m_x+i+1-start, getPosition().m_y+getHeight()-(heightratio*statsBuffer->snapshots[i+1].critters) );
// 					}
// 				glEnd();
// 			}
// 		}
// 
// 		drawChildren();
// 	}
// }
// 
// Statsgraph::~Statsgraph()
// {
// }
