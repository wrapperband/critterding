#ifndef MUTATIONPANEL_H
#define MUTATIONPANEL_H

#include "src/gui/settingspanel.h"

class Mutationpanel : public Settingspanel
{
	public:
		Mutationpanel();
		~Mutationpanel();

	private:
		unsigned int vspace;
		unsigned int hspace;
};

#endif
