#ifndef STATSBUFFER_H_INCLUDED
#define STATSBUFFER_H_INCLUDED

#include <vector>

struct statsSnapshot
{
	unsigned int		critters;
	unsigned int		food;
	
	unsigned int		neurons;
	unsigned int		synapses;
	float				adamdistance;
	unsigned int		bodyparts;
	float				weight;
};

class CritterB;
class Food;

class Statsbuffer
{
	public:
		static Statsbuffer*	Instance();
// 		Statsbuffer();
// 		~Statsbuffer();

// 		void add( const vector<CritterB*>& critters, const vector<Food*>& food );
// 		void add( const std::vector<CritterB*>& critters, const std::vector<Entity*>& entities );
		void add( const std::vector<CritterB*>& critters, const std::vector<Food*>& food );

		std::vector<statsSnapshot> snapshots;
		statsSnapshot current;
		float info_totalWeight;


		unsigned int m_graph_highest;
		unsigned int m_graph_consider_highest_start;
		long long frameCounter;
		
	protected:
		Statsbuffer();
	private:
		static Statsbuffer*	_instance;

		inline void findHighestGraphValue();
		
		const unsigned int		recordInterval;
		unsigned int			framecounter;
		const unsigned int		maxSnapshots;
};

#endif
