#ifndef CRITTERVIEW_H
#define CRITTERVIEW_H

#include "src/gui/panel.h"

class WorldB;
class Critterselection;
class BeGraphicsSystem;
class CritterB;

class Critterview : public Panel
{
	public:
		Critterview(boost::shared_ptr<BeGraphicsSystem> graphicsSystem);
		~Critterview();

		void draw();
		WorldB*		world;
	private:
		Critterselection* critterselection;
		BeWidgetPtr viewbutton;
		btScalar viewposition[16];
		CritterB* currentCritter;
		boost::shared_ptr<BeGraphicsSystem> m_graphicsSystem;

		// drawing helpers
		int v_spacer;
		int v_space;
		const unsigned int m_view_size;
// 		int v_radius;
// 		int v_diam;
// 		int spacing;
// 		int column;
// 		int row;
// 		int rowlength;
};

#endif
