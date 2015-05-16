#ifndef SPECIESVIEW_H
#define SPECIESVIEW_H

#include "gui/panel.h"
#include "math/vector2f.h"

# include <vector>

class Genotypes;

class Speciesview : public Panel
{
	public:
		Speciesview();
		~Speciesview();

		void draw();
		void recalculate();
		
	private:
		Genotypes* genotypes;

		int titlebar;
		int v_space;
		int rowspacer;
		int qwidth;
		int qheight;
		
		std::string titlePos;
		std::string titleColor;
		std::string titleNum;
		std::string titleAd;
		std::string titleNeurons;
		std::string titleSynapses;
		std::string titleBodyparts;
		
		int titlePosW;
		int titleColorW;
		int titleNumW;
		int titleAdW;
		int titleNeuronsW;
		int titleSynapsesW;
		int titleBodypartsW;
		
		const int*	colormode;
		
		std::vector<int> indices;
		
		unsigned int highestCount;
		unsigned int highestAD;
		unsigned int highestNeurons;
		unsigned int highestSynapses;
		unsigned int highestBodyparts;
		
		const unsigned int m_recalc_interval;
		unsigned int m_recalc_count;
		
		
		unsigned int m_num_species_buttons;
		
		
		int titleNumWH;
		int titleColorWH;
		int titlePosWH;
		int titleAdWH;
		int titleNeuronsWH;
		int titleSynapsesWH;
		int titleBodypartsWH;
		
		int colw1;
		int colw2;
		int colw3;
		int colw4;
		int colw5;
		int colw6;
		int colw7;

		const int m_colspacer;
		int col1;
		int col2;
		int col3;
		int col4;
		int col5;
		int col6;
		int col7;
		int col8;
		
};

#endif













// #ifndef SPECIESVIEW_H
// #define SPECIESVIEW_H
// 
// #include "src/gui/panel.h"
// #include "../entities/genotypes.h"
// #include "src/math/vector2f.h"
// 
// # include <vector>
// 
// using namespace std;
// 
// class Speciesview : public Panel
// {
// 	public:
// 		Speciesview();
// 		~Speciesview();
// 
// 		void draw();
// 	private:
// 		Genotypes* genotypes;
// 
// 		int titlebar;
// 		int v_space;
// 		int rowspacer;
// 		int qwidth;
// 		int qheight;
// 		
// 		string titlePos;
// 		string titleColor;
// 		string titleNum;
// 		string titleAd;
// 		string titleNeurons;
// 		string titleSynapses;
// 		string titleBodyparts;
// 		
// 		int titlePosW;
// 		int titleColorW;
// 		int titleNumW;
// 		int titleAdW;
// 		int titleNeuronsW;
// 		int titleSynapsesW;
// 		int titleBodypartsW;
// 		
// 		const int*	colormode;
// };
// 
// #endif
