#include "critterview.h"
#include "gui/text.h"
#include "gui/text_uintp.h"
#include "graphics/sc_graphics.h"
#include "gui/textprinter.h"
#include "scenes/critterding/entities/genotype.h"
#include "scenes/critterding/entities/worldb.h"
#include "scenes/critterding/entities/critterselection.h"
#include "scenes/critterding/entities/critterb.h"

#include <iomanip>

Critterview::Critterview(boost::shared_ptr<BeGraphicsSystem> graphicsSystem)
 : Panel(false)
 , m_graphicsSystem(graphicsSystem)
 , m_view_size(162)
{
	critterselection = Critterselection::Instance();
	
	m_active = false;
	isMovable = true;

	// action button settings
	unsigned int bwidth = 102;
	unsigned int bheight = 20;
	unsigned int bspacing = 4;
	unsigned int totalbuttonheight = (3*bheight) + bspacing;

// 	m_dimensions.set(346+m_view_size+10, 168);
	m_dimensions.set(520, 178);
	
	int buttons_starty = m_dimensions.m_y - totalbuttonheight-10;

	m_localposition.set(2, 24);

	currentCritter = 0;

	// text widgets
	v_spacer = 14;
	v_space = 3;
	v_space += v_spacer; addWidget( "cv_cid", Vector2i(10, v_space), BeWidgetTextPtr(new BeWidgetText("Critter") ) );
	v_space += v_spacer; addWidget( "cv_ad", Vector2i(10, v_space), BeWidgetTextPtr(new BeWidgetText("Adam Distance") ) );
	v_space += v_spacer; addWidget( "cv_age", Vector2i(10, v_space), BeWidgetTextPtr(new BeWidgetText("Age") ) );
		addWidget( "cv_age_/", Vector2i(200, v_space), BeWidgetTextPtr(new BeWidgetText("/") ) );
// 		addWidget( "cv_age_max", Vector2i(210, v_space), BeWidgetTextCVARuintPtr(new BeWidgetTextCVARuint(settings->getCVarPtr("critter_maxlifetime"))));

	v_space += v_spacer; addWidget( "cv_energy", Vector2i(10, v_space), BeWidgetTextPtr(new BeWidgetText("Energy") ) );
		addWidget( "cv_energy_/", Vector2i(200, v_space), BeWidgetTextPtr(new BeWidgetText("/") ) );
		addWidget( "cv_energy_max", Vector2i(210, v_space), BeWidgetTextCVARuintPtr(new BeWidgetTextCVARuint(settings->getCVarPtr("critter_maxenergy"))));

	v_space += v_spacer; addWidget( "cv_neurons", Vector2i(10, v_space), BeWidgetTextPtr(new BeWidgetText("Neurons") ) );
	v_space += v_spacer; addWidget( "cv_synapses", Vector2i(10, v_space), BeWidgetTextPtr(new BeWidgetText("Synapses") ) );
	
	// view widgets
// 	viewbutton = addWidgetButton( "cv_view", Vector2i(0+m_dimensions.m_x-m_view_size-10, 35), Vector2i(m_view_size, m_view_size), "", Vector2i(0, 0), BeCommand(""), EVENT_NOREPEAT, 0 );
	viewbutton = addWidgetButton( "cv_view", Vector2i(0+m_dimensions.m_x-m_view_size-25, 10), Vector2i(m_view_size, m_view_size), "", Vector2i(0, 0), BeCommand(""), EVENT_NOREPEAT, 0 );

	// close button
	addWidgetButton( "critterview_close", Vector2i(m_dimensions.m_x-14, 2), Vector2i(12, 12), "x", BeCommand("gui_togglepanel", "critterview"), EVENT_NOREPEAT, 0 );
// 	BeWidgetPtr w = addWidgetButton( "cv_close", Vector2i(2, 2), Vector2i(12, 12), "x", BeCommand("gui_togglepanel", "critterview"), EVENT_NOREPEAT, 0 );
// 		w->isAlignedRight = true;

// 	// action buttons
// 	unsigned int c_width = 10;
// 	unsigned int c_height = buttons_starty;
// 	addWidgetButton( "cv_action_kill", Vector2i(c_width, c_height), Vector2i(bwidth, bheight), "kill", BeCommand("cs_kill"), EVENT_NOREPEAT, 0 );
// 
// 	c_height += bspacing + bheight;
// 	addWidgetButton( "cv_action_brainview", Vector2i(c_width, c_height), Vector2i(bwidth, bheight), "brainview", BeCommand("gui_togglepanel", "brainview"), EVENT_NOREPEAT, 0 );
// 	
// 	c_width += bwidth + bspacing;
// 	c_height = buttons_starty;
// 	addWidgetButton( "cv_action_duplicate", Vector2i(c_width, c_height), Vector2i(bwidth, bheight), "duplicate", BeCommand("cs_duplicate"), EVENT_NOREPEAT, 0 );
// 	
// 	c_height += bspacing + bheight;
// 	addWidgetButton( "cv_action_spawnbrainmutant", Vector2i(c_width, c_height), Vector2i(bwidth, bheight), "brain mutant", BeCommand("cs_spawnbrainmutant"), EVENT_NOREPEAT, 0 );
// 
// 	c_width += bwidth + bspacing;
// 	c_height = buttons_starty;
// 	addWidgetButton( "cv_action_spawnbodymutant", Vector2i(c_width, c_height), Vector2i(bwidth, bheight), "body mutant", BeCommand("cs_spawnbodymutant"), EVENT_NOREPEAT, 0 );
// 
// 	c_height += bspacing + bheight;
// 	addWidgetButton( "cv_action_spawnbrainbodymutant", Vector2i(c_width, c_height), Vector2i(bwidth, bheight), "brain+body m", BeCommand("cs_spawnbrainbodymutant"), EVENT_NOREPEAT, 0 );
// 
// 	c_width += bwidth + bspacing;
// 	c_height = buttons_starty;
// 	addWidgetButton( "cv_action_feed", Vector2i(c_width, c_height), Vector2i(bwidth, bheight), "feed", BeCommand("cs_feed"), EVENT_NOREPEAT, 0 );
// 
// 	c_height += bspacing + bheight;
// 	addWidgetButton( "cv_action_resetage", Vector2i(c_width, c_height), Vector2i(bwidth, bheight), "reset age", BeCommand("cs_resetage"), EVENT_NOREPEAT, 0 );

	
	// action buttons
	unsigned int c_width = 10;
	unsigned int c_height = buttons_starty;
	addWidgetButton( "cv_action_kill", Vector2i(c_width, c_height), Vector2i(bwidth, bheight), "kill", BeCommand("cs_kill"), EVENT_NOREPEAT, 0 );

	c_height += bspacing + bheight;
	addWidgetButton( "cv_action_brainview", Vector2i(c_width, c_height), Vector2i(bwidth, bheight), "brainview", BeCommand("gui_togglepanel", "brainview"), EVENT_NOREPEAT, 0 );
	
	c_height += bspacing + bheight;
	addWidgetButton( "cv_action_duplicate", Vector2i(c_width, c_height), Vector2i(bwidth, bheight), "duplicate", BeCommand("cs_duplicate"), EVENT_NOREPEAT, 0 );
	
	c_width += bwidth + bspacing;
	c_height = buttons_starty;
	addWidgetButton( "cv_action_spawnbrainmutant", Vector2i(c_width, c_height), Vector2i(bwidth, bheight), "brain mutant", BeCommand("cs_spawnbrainmutant"), EVENT_NOREPEAT, 0 );

	c_height += bspacing + bheight;
	addWidgetButton( "cv_action_spawnbodymutant", Vector2i(c_width, c_height), Vector2i(bwidth, bheight), "body mutant", BeCommand("cs_spawnbodymutant"), EVENT_NOREPEAT, 0 );

	c_height += bspacing + bheight;
	addWidgetButton( "cv_action_spawnbrainbodymutant", Vector2i(c_width, c_height), Vector2i(bwidth, bheight), "brain+body mtnt", BeCommand("cs_spawnbrainbodymutant"), EVENT_NOREPEAT, 0 );

	c_width += bwidth + bspacing;
	c_height = buttons_starty;
	addWidgetButton( "cv_action_feed", Vector2i(c_width, c_height), Vector2i(bwidth, bheight), "feed", BeCommand("cs_feed"), EVENT_NOREPEAT, 0 );

	c_height += bspacing + bheight;
	addWidgetButton( "cv_action_resetage", Vector2i(c_width, c_height), Vector2i(bwidth, bheight), "reset age", BeCommand("cs_resetage"), EVENT_NOREPEAT, 0 );
	
	c_height += bspacing + bheight;
	addWidgetButton( "cv_action_follow", Vector2i(c_width, c_height), Vector2i(bwidth, bheight), "follow", BeCommand("cs_follow"), EVENT_NOREPEAT, 0 );
}

void Critterview::draw()
{
	// deactive when selected critter goes missing
	if ( critterselection->selectedCritter == 0 )
	{
		m_active = false;
		currentCritter = 0;
	}

	if (m_active)
	{
		if ( critterselection->selectedCritter != currentCritter )
			currentCritter = critterselection->selectedCritter;

		drawBackground();
		drawBorders();
		// draw the rest
		drawChildren();
		
		// draw values of critter
		v_space = 3;
		v_space += v_spacer; textprinter->print( m_absposition.m_x+110, m_absposition.m_y+v_space, critterselection->selectedCritter->critterID );
		v_space += v_spacer; textprinter->print( m_absposition.m_x+110, m_absposition.m_y+v_space, critterselection->selectedCritter->genotype->adamdist );
 		v_space += v_spacer; textprinter->print( m_absposition.m_x+110, m_absposition.m_y+v_space, critterselection->selectedCritter->totalFrames );
		
 		textprinter->print( m_absposition.m_x+210, m_absposition.m_y+v_space, critterselection->selectedCritter->genotype->bodyArch->m_maxlifetime );
		
// 		std::stringstream s;
// 		s << std::setprecision(3) << critterselection->selectedCritter->energyLevel;
//  		v_space += v_spacer; textprinter->print( m_absposition.m_x+110, m_absposition.m_y+v_space, s.str() );
 		v_space += v_spacer; textprinter->print( m_absposition.m_x+110, m_absposition.m_y+v_space, "%1.1f", critterselection->selectedCritter->energyLevel );

 		v_space += v_spacer; textprinter->print( m_absposition.m_x+110, m_absposition.m_y+v_space, critterselection->selectedCritter->brain.totalNeurons );
 		v_space += v_spacer; textprinter->print( m_absposition.m_x+110, m_absposition.m_y+v_space, critterselection->selectedCritter->brain.totalSynapses );
		
// 		std::stringstream s2;
// 		s2 << std::setprecision(3) << (float)critterselection->selectedCritter->brain.totalSynapses / critterselection->selectedCritter->brain.totalNeurons;
// 		textprinter->print( m_absposition.m_x+200, m_absposition.m_y+v_space, s2.str() );
		textprinter->print( m_absposition.m_x+200, m_absposition.m_y+v_space, "%5.2f avg", (float)critterselection->selectedCritter->brain.totalSynapses / critterselection->selectedCritter->brain.totalNeurons );

		// draw gl view
		{
			glEnable (GL_DEPTH_TEST);
			glEnable (GL_LIGHTING);
			glEnable (GL_CULL_FACE);
			glEnable(GL_TEXTURE_2D);

			if(*world->m_glsl)
				world->m_graphicsSystem->useProgram(world->m_graphics->m_effect->m_program.get());

			const unsigned int t_view_size(m_view_size -1);
			critterselection->selectedCritter->place( viewbutton->getPosition().m_x+1, *settings->winHeight-t_view_size-viewbutton->getPosition().m_y, t_view_size, t_view_size, world->m_graphics->m_v_inv );

			world->drawWithinCritterSight(critterselection->selectedCritter);
		}

		// switch back to 2d 
		{
			glDisable (GL_DEPTH_TEST);
			glDisable (GL_LIGHTING);
			glDisable (GL_CULL_FACE);
			glDisable (GL_TEXTURE_2D);

			if(*world->m_glsl)
				world->m_graphicsSystem->useProgram(0);

			glViewport(0,0,*settings->winWidth,*settings->winHeight);
			m_graphicsSystem->matrixLoadIdentity(GL_PROJECTION);
			m_graphicsSystem->matrixOrtho(GL_PROJECTION, 0, *settings->winWidth, *settings->winHeight, 0,  0, 1);
			m_graphicsSystem->matrixLoadIdentity(GL_MODELVIEW);
			m_graphicsSystem->matrixTranslate(GL_MODELVIEW, 0.5f, -0.5f, 0.0f); // pixel precision offset			
// 			m_graphicsSystem->matrixTranslate(GL_MODELVIEW, 0.0f, 0.0f, 0.0f); // pixel precision offset			
		}
	}
	
}

Critterview::~Critterview()
{
}
