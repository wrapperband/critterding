#ifndef HUD_H
#define HUD_H

#include <vector>
#include "gui/panel.h"

class WorldB;
class Critterselection;
class BeGraphicsSystem;

class Hud : public Panel
{
	public:
		Hud(boost::shared_ptr<BeGraphicsSystem> graphicsSystem);
		~Hud();
		void		draw();
		WorldB*		world;
		
	private:
		void drawBorders();
		Critterselection* critterselection;
		std::vector<BeWidgetPtr> cbuttons; // FIXME save as buttons
		float m_viewposition[16];
		boost::shared_ptr<BeGraphicsSystem> m_graphicsSystem;
		
		const unsigned int m_bwidth;
		const unsigned int m_bheight;
};

#endif
