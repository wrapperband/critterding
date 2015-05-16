#include "statsbuffer.h"
#include "src/scenes/critterding/entities/critterb.h"
#include "src/scenes/critterding/entities/food.h"
#include "genotype.h"

using namespace std;

Statsbuffer* Statsbuffer::Instance () 
{
	static Statsbuffer t;
	return &t;
}

Statsbuffer::Statsbuffer()
 : recordInterval(100)
 , framecounter(0)
 , maxSnapshots(7680)
 , frameCounter(0)

{
// 	settings = Settings::Instance();

}

// void Statsbuffer::add( const vector<CritterB*>& critters, const vector<Food*>& food )
// void Statsbuffer::add( const vector<CritterB*>& critters, const vector<Entity*>& entities )
void Statsbuffer::add( const vector<CritterB*>& critters, const vector<Food*>& food )
{
	// get stats of this frame
	current.neurons	= 0;
	current.synapses	= 0;
	current.adamdistance	= 0;
	current.bodyparts	= 0;
	current.weight		= 0;
	CritterB* c;

	current.critters = critters.size();

	for( unsigned int i=0; i < current.critters; ++i )
	{
		c = critters[i];
		current.neurons			+= c->brain.totalNeurons;
		current.synapses		+= c->brain.totalSynapses;
		current.adamdistance	+= c->genotype->adamdist;
		current.bodyparts		+= c->body.bodyparts.size() + c->body.mouths.size();
		current.weight			+= c->body.totalWeight;
	}

	// foodcount
		current.food			= food.size();
// 		unsigned int fcount(0);
// 		const auto& e_size(entities.size());
// 		for( unsigned int i(0); i < e_size; ++i)
// 		{
// 			Entity* e = entities[i];
// 			if ( e->type == FOOD )
// 			{
// 				++fcount;
// 			}
// 		}
// 		current.food			= fcount; // FIXME INTO FOODCOUNT loop

	if ( ++framecounter == recordInterval )
	{
		// remove expired snapshots
		if ( snapshots.size() == maxSnapshots )
			snapshots.erase(snapshots.begin());

		// insert the current snapshot
		snapshots.push_back(current);

		findHighestGraphValue();

		framecounter = 0;
	}
}

void Statsbuffer::findHighestGraphValue()
{
// find the highest value in the stats vector
	m_graph_highest = 0;
	for ( unsigned int i=m_graph_consider_highest_start; i < snapshots.size(); ++i )
	{
		auto const& sn( snapshots[i] );
		if ( sn.food > m_graph_highest )
			m_graph_highest = sn.food;

		if ( sn.critters > m_graph_highest )
			m_graph_highest = sn.critters;
	}
}