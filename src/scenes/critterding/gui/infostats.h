#ifndef INFOSTATS_H
#define INFOSTATS_H

#include "gui/panel.h"
#include "../entities/statsbuffer.h"

class Infostats : public Panel
{
	public:
		Infostats();
		~Infostats();

		void draw();
	private:
		Statsbuffer*		statsBuffer;

		// horizontal/vertical spacers (padding)
		unsigned int hsp;
		unsigned int vsp;
};

#endif
