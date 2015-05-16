#ifndef HGENOTYPES_H
#define HGENOTYPES_H

// #include "../utils/color.h"
#include "../utils/settings.h"
#include "hgenotype.h"

#include <vector>
#include <iostream>

class HGenotypes
{
	public:
		static HGenotypes* Instance();
		HGenotype* newg(Settings* settings);
		void add(HGenotype* gt);
		HGenotype* copy(HGenotype* gt, bool brainmutant, unsigned int brruns);
		HGenotype* loadHGenotype(string& content);
		string saveHGenotype();

		void remove(HGenotype* gt);
// 		Color			colorH;

		vector<HGenotype*>	list;

	protected:
		HGenotypes();
		~HGenotypes();
	private:
		static HGenotypes* _instance;
};

#endif
