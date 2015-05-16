#ifndef INFOBAR_H
#define INFOBAR_H

#include "gui/panel.h"
#include "common/be_frame_counter.h"

class BeTimer;
class Statsbuffer;

class Infobar : public Panel
{
	public:
		Infobar(BeTimer* timer);
		~Infobar();

		void draw();
		void mark();

	private:
		BeFrameCounter fps;
		Statsbuffer*		statsBuffer;

		// horizontal/vertical spacers (padding)
		unsigned int		hsp;
		unsigned int		vsp;
		const int*	energy;
		BeTimer*		m_timer;
};

#endif
