#include "hud.h"
#include "gui/button.h"
#include "graphics/sc_graphics.h"
#include "scenes/critterding/entities/critterb.h"
#include "scenes/critterding/entities/worldb.h"
#include "scenes/critterding/entities/critterselection.h"


Hud::Hud(boost::shared_ptr<BeGraphicsSystem> graphicsSystem)
 : Panel(false)
 , m_graphicsSystem(graphicsSystem)
 , m_bwidth(27)
 , m_bheight(27)
{
	m_active = true;
	isHCentered = true;
	isAlignedBottom = true;

	m_dimensions.set(826, 62);
	m_localposition.m_y = 0;

	critterselection = Critterselection::Instance();

	unsigned int bwidth = 60;
	unsigned int bheight = 18;
	unsigned int bspacing = 2;
	
	unsigned int c_width = bspacing;
	unsigned int c_height = bspacing;

	addWidgetButton( "hud_exit", Vector2i(c_width, c_height), Vector2i(bwidth, bheight), "exit", BeCommand("gui_togglepanel", "panel_exit"), EVENT_NOREPEAT, 0 );

	c_height += bspacing + bheight;
	addWidgetButton( "hud_graph", Vector2i(c_width, c_height), Vector2i(bwidth, bheight), "graph", BeCommand("gui_togglepanel", "statsgraph"), EVENT_NOREPEAT, 0 );

	c_height += bspacing + bheight;
	addWidgetButton( "hud_stats", Vector2i(c_width, c_height), Vector2i(bwidth, bheight), "stats", BeCommand("gui_togglepanel", "infostats"), EVENT_NOREPEAT, 0 );


	c_width += bspacing + bwidth;
	c_height = bspacing;
	addWidgetButton( "hud_events", Vector2i(c_width, c_height), Vector2i(bwidth, bheight), "events", BeCommand("gui_togglepanel", "textverbosemessage"), EVENT_NOREPEAT, 0 );

	c_height += bspacing + bheight;
	addWidgetButton( "hud_settings", Vector2i(c_width, c_height), Vector2i(bwidth, bheight), "settings", BeCommand("gui_togglepanel", "globalsettingspanel"), EVENT_NOREPEAT, 0 );

	c_height += bspacing + bheight;
	addWidgetButton( "hud_mutationpanel", Vector2i(c_width, c_height), Vector2i(bwidth, bheight), "mutation", BeCommand("gui_togglepanel", "mutationpanel"), EVENT_NOREPEAT, 0 );


	c_width += bspacing + bwidth;
	c_height = bspacing;
	addWidgetButton( "hud_brainpanel", Vector2i(c_width, c_height), Vector2i(bwidth, bheight), "brain", BeCommand("gui_togglepanel", "settingsbrainpanel"), EVENT_NOREPEAT, 0 );

	c_height += bspacing + bheight;
	addWidgetButton( "hud_bodypanel", Vector2i(c_width, c_height), Vector2i(bwidth, bheight), "body", BeCommand("gui_togglepanel", "settingsbodypanel"), EVENT_NOREPEAT, 0 );

	c_height += bspacing + bheight;
	addWidgetButton( "hud_speciesview", Vector2i(c_width, c_height), Vector2i(bwidth, bheight), "species", BeCommand("gui_togglepanel", "speciesview"), EVENT_NOREPEAT, 0 );

	c_width += bspacing + bwidth;
	c_height = bspacing;
	addWidgetButton( "hud_lightpanel", Vector2i(c_width, c_height), Vector2i(bwidth, bheight), "lighting", BeCommand("gui_togglepanel", "settingslightpanel"), EVENT_NOREPEAT, 0 );

	c_height += bspacing + bheight;
	addWidgetButton( "hud_populationcontrol", Vector2i(c_width, c_height), Vector2i(bwidth, bheight), "population", BeCommand("gui_togglepanel", "populationcontrolpanel"), EVENT_NOREPEAT, 0 );
	
	// init critter selection buttons
	c_width += bspacing + bwidth;
	bheight = 28;
	bwidth = 28;
	c_height = bspacing;
	unsigned int tok = 0;
	std::stringstream s;
	s <<  "hud_cv1";
	for ( int i=0; i < 20; i++ )
	{
		s << i;
		cbuttons.push_back( addWidgetButton( s.str(), Vector2i(c_width, c_height), Vector2i(bwidth, bheight), "", Vector2i(16, 13), BeCommand("cs_select", i), EVENT_NOREPEAT, 0 ) );
		s << "2";
		static_cast<BeWidgetButton*>(cbuttons[i].get())->genEvent(3, s.str(), BeCommand("cs_unregister", i), EVENT_NOREPEAT, 0);
		cbuttons[i]->swap();

		if ( ++tok == 2 )
		{
			tok = 0;
			c_width += bspacing + bwidth;
			c_height = bspacing;
		}
		else
			c_height += bspacing + bheight;
	}

	bwidth = 90;
	bheight = 18;

	c_height = bspacing;
	addWidgetButton( "hud__cs_selectall", Vector2i(c_width, c_height), Vector2i(bwidth, bheight), "select all", BeCommand("cs_selectall"), EVENT_NOREPEAT, 0 );

	c_height += bspacing + bheight;
	addWidgetButton( "hud__cs_clear", Vector2i(c_width, c_height), Vector2i(bwidth, bheight), "clear", BeCommand("cs_clear"), EVENT_NOREPEAT, 0 );

	c_height += bspacing + bheight;
	addWidgetButton( "hud__cs_kill", Vector2i(c_width, c_height), Vector2i(bwidth, bheight), "kill", BeCommand("cs_killall"), EVENT_NOREPEAT, 0 );

	
	c_width += bspacing + bwidth;
	c_height = bspacing;
	addWidgetButton( "hud__cs_duplicateall", Vector2i(c_width, c_height), Vector2i(bwidth, bheight), "duplicate", BeCommand("cs_duplicateall"), EVENT_NOREPEAT, 0 );

	c_height += bspacing + bheight;
	addWidgetButton( "hud_cs_spawnbrainmutantall", Vector2i(c_width, c_height), Vector2i(bwidth, bheight), "brain mutants", BeCommand("cs_spawnbrainmutantall"), EVENT_NOREPEAT, 0 );

	c_height += bspacing + bheight;
	addWidgetButton( "cs_spawnbodymutantall", Vector2i(c_width, c_height), Vector2i(bwidth, bheight), "body mutants", BeCommand("cs_spawnbodymutantall"), EVENT_NOREPEAT, 0 );

	c_width += bspacing + bwidth;
	c_height = bspacing;
	addWidgetButton( "cs_spawnbrainbodymutantall", Vector2i(c_width, c_height), Vector2i(bwidth, bheight), "brain+body m", BeCommand("cs_spawnbrainbodymutantall"), EVENT_NOREPEAT, 0 );

	c_height += bspacing + bheight;
// 	addWidgetButton( "cs_feed", Vector2f(c_width, c_height), Vector2f(bwidth, bheight), "feed", BeCommand("cs_spawnbodymutantall"), EVENT_NOREPEAT, 0 );

}

void Hud::draw()
{
	if (m_active)
	{
		// activate the proper buttons before drawing children
		const unsigned int numCButtons( cbuttons.size() );
		const unsigned int numCSSize( critterselection->clist.size() );
		unsigned int numActiveButtons(0);
		for (unsigned int i(0); i < numCButtons; ++i )
		{
			if ( i < numCSSize )
			{
				cbuttons[i]->activate();
				++numActiveButtons;
			}
			else
			{
				cbuttons[i]->deactivate();
			}
		}

		drawBackground();
		drawBorders();
		drawChildren();
		
// 		if ( !critterselection->clist.empty() )
		{
			glEnable (GL_DEPTH_TEST);
			glEnable (GL_LIGHTING);
			glEnable (GL_CULL_FACE);
			glEnable(GL_TEXTURE_2D);
// 			glDisable(GL_BLEND); 
// 			glDisable(GL_ALPHA_TEST);

			if(*world->m_glsl)
				world->m_graphicsSystem->useProgram(world->m_graphics->m_effect->m_program.get());
// 				world->m_graphicsSystem->useProgram(world->m_graphics->m_critter_effect->m_program.get());
			

			// 3D
			for (unsigned int i(0); i < numActiveButtons; ++i )
			{
				const Vector2i& cbuttonpos = cbuttons[i]->getPosition();
				CritterB* c = critterselection->clist[i];

				c->place( cbuttonpos.m_x+1, *settings->winHeight-m_bheight-(cbuttonpos.m_y), m_bwidth, m_bheight, world->m_graphics->m_v_inv );
				
// 				world->drawWithinCritterSight(i);
				world->drawWithinCritterSight(c);
			}

			// Back to 2D 
			{
				glDisable (GL_DEPTH_TEST);
				glDisable (GL_LIGHTING);
				glDisable (GL_CULL_FACE);
				glDisable(GL_TEXTURE_2D);

				if(*world->m_glsl)
					world->m_graphicsSystem->useProgram(0);
				
				
				glViewport(0,0,*settings->winWidth,*settings->winHeight);
				m_graphicsSystem->matrixLoadIdentity(GL_PROJECTION);
				m_graphicsSystem->matrixOrtho(GL_PROJECTION, 0, *settings->winWidth, *settings->winHeight, 0,  0, 1);
				m_graphicsSystem->matrixLoadIdentity(GL_MODELVIEW);
				m_graphicsSystem->matrixTranslate(GL_MODELVIEW, 0.5f, -0.5f, 0.0f); // pixel precision offset
// 				m_graphicsSystem->matrixTranslate(GL_MODELVIEW, 0.f, 0.0f, 0.0f); // pixel precision offset
			}
		}
	}
}

void Hud::drawBorders()
{
	glColor3f(0.5f, 0.5f, 0.5f);
	glBegin(GL_LINES);
		glVertex2i(getPosition().m_x,         getPosition().m_y+getHeight());
		glVertex2i(getPosition().m_x,         getPosition().m_y);

		glVertex2i(getPosition().m_x,         getPosition().m_y);
		glVertex2i(getPosition().m_x+getWidth(), getPosition().m_y);

		glVertex2i(getPosition().m_x+getWidth(), getPosition().m_y);
		glVertex2i(getPosition().m_x+getWidth(), getPosition().m_y+getHeight());

		glVertex2i(getPosition().m_x+getWidth(), getPosition().m_y+getHeight());
		glVertex2i(getPosition().m_x,         getPosition().m_y+getHeight());
	glEnd();
}

Hud::~Hud()
{
}
