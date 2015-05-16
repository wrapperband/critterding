#ifndef HBRAIN_H
#define HBRAIN_H

#include "src/brainz/brainz.h"
#include "src/halutils/hgenotypes.h"

using namespace std;

class brain
{
	public:
		brain();
		~brain();
		Brainz			b;
		HGenotype*		genotype;

		char			presence;
		char			cocacola;

		void			resetMotors();
		void			updateMotorCount();
		vector<char>		motors;
		vector<unsigned int>	motorCount;
		
		double			weight;

		float			processed;
};

#endif
