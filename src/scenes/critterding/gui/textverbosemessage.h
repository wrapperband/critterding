#ifndef TEXTVERBOSEMESSAGE_H
#define TEXTVERBOSEMESSAGE_H

// #include "src/utils/timer.h" // FIXME howso this is here?
#include "src/gui/panel.h"

class Textverbosemessage : public Panel
{
	public:
struct vmsg
{
	std::string str;
	unsigned int	appeartime;
};

		Textverbosemessage();
		virtual ~Textverbosemessage();

		virtual void		draw();

		void		addBirth(std::stringstream& streamptr);
		void		addDeath(std::stringstream& streamptr);
		unsigned int	maxMessages;
		unsigned int	msgLifetime;
	protected:
	private:
		std::vector<vmsg*>	births;
		std::vector<vmsg*>	deaths;

		void		getLongestMsg();

		void		deleteExpiredMsg();
		
		unsigned int	col2;
};

#endif
