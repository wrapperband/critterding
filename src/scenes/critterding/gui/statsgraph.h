#ifndef STATSGRAPH_H_INCLUDED
#define STATSGRAPH_H_INCLUDED

#include "gui/panel.h"

class Statsbuffer;

class Statsgraph : public Panel
{
	public:
		Statsgraph();
		~Statsgraph();

		virtual void draw();

	private:
		Statsbuffer* statsBuffer;
};

#endif
