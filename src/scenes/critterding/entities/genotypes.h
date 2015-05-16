#ifndef GENOTYPES_H
#define GENOTYPES_H

#include "genotype.h"
#include "src/utils/settings.h"

#include <vector>
#include <iostream>
using std::cerr;
using std::endl;

class Genotypes
{
	public:
		static Genotypes* Instance();
		Genotype* newg(Settings* settings);
		void add(Genotype* gt);
		Genotype* copy(Genotype* gt, bool brainmutant, unsigned int brruns, bool bodymutant, unsigned int boruns);
		Genotype* loadGenotype(std::string& content);
		std::string saveGenotype();

		void remove(Genotype* gt);
		BeColor			colorH;

		std::vector<Genotype*>	list;

	protected:
		Genotypes();
		~Genotypes();
	private:
		static Genotypes* _instance;
		BeParser parseH;
		BeParser contentParser;
};

#endif
