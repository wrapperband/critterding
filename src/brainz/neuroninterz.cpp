#include "neuroninterz.h"

NeuronInterz::NeuronInterz( const bool isInhibitory, const float firingThreshold, const float potentialDecay )
 : m_firingThreshold(firingThreshold)
 , m_potential(0.0f)
 , m_potentialDecay(potentialDecay)

//  , m_syn(0)
//  , m_syn_beginP(0)
//  , m_syn_endP(0)
 , m_synapses_begin(m_synapses.begin())
 , m_synapses_end(m_synapses.end())

 , m_waitoutput(0.0f)
 , m_fired(false)
 , m_isMotor(false)
 , m_motorFunc(0)
 , m_motorOoutputPointer(0)
 , m_isInhibitory(isInhibitory)
 , m_output(0.0f)
{
	if ( m_isInhibitory )
		m_inhibitorySignSwitch = -1.0f;
	else
		m_inhibitorySignSwitch = 1.0f;
}

NeuronInterz::NeuronInterz( const bool isInhibitory, const float firingThreshold, const float potentialDecay, const unsigned int motorFunc, bool* motorOoutputPointer )
 : m_firingThreshold(firingThreshold)
 , m_potential(0.0f)
 , m_potentialDecay(potentialDecay)

//  , m_syn(0)
//  , m_syn_beginP(0)
//  , m_syn_endP(0)
 , m_synapses_begin(m_synapses.begin())
 , m_synapses_end(m_synapses.end())

 , m_waitoutput(0.0f)
 , m_fired(false)
 , m_isMotor(true)
 , m_motorFunc(motorFunc)
 , m_motorOoutputPointer(motorOoutputPointer)
 , m_isInhibitory(isInhibitory)
 , m_output(0.0f)
{
	if ( m_isInhibitory )
		m_inhibitorySignSwitch = -1.0f;
	else
		m_inhibitorySignSwitch = 1.0f;
}

void NeuronInterz::connec( const float& output, const float synapticWeight ) // unsigned int dendrm_iteBranch, 
{
	m_synapses.push_back( Synapse(output, synapticWeight) );
// 	Synapse& s( (*m_synapses.rbegin()) );

// 	s.ref = output;
// 	s.weight = synapticWeight;

	m_synapses_begin = m_synapses.begin();
	m_synapses_end = m_synapses.end();
	
// 	m_syn_beginP = &(*m_synapses.begin());
// 	m_syn_endP = &(*m_synapses.end());
}

void NeuronInterz::reset()
{
	m_output = 0.0f;
	m_potential = 0.0f;
}

void NeuronInterz::commitOutput()
{
	m_output = m_waitoutput;
}

void NeuronInterz::not_firing( const float signSwitchedPotential )
{
	// set waitoutput
		m_waitoutput = 0.0f;
		m_fired = false;

	// reset potential if < 0
		if ( signSwitchedPotential < 0.0f )
			m_potential *= 0.5f;
}

void NeuronInterz::process()
{
	// potential decay
		m_potential *= m_potentialDecay;

	// add connected neurons output (synapse) to m_potential
// 		for ( m_syn = m_syn_beginP; m_syn != m_syn_endP; ++m_syn )
// 			m_potential += m_syn->m_weight * m_syn->m_ref; // m_it->dendrm_iteBranch * 

		for ( auto syn_it(m_synapses_begin); syn_it != m_synapses_end; ++syn_it )
		{
			m_potential += syn_it->getWeightedInfluence(); // m_it->dendrm_iteBranch * 
		}
		

	// calc "signed" potential
		const float signSwitchedPotential( m_inhibitorySignSwitch * m_potential );

	// neuron is firing
		if ( signSwitchedPotential >= m_firingThreshold )
		{
			// reset neural potential
				m_potential = 0.0f;

			// fire the neuron
				m_waitoutput = m_inhibitorySignSwitch;
				m_fired = true;
		}

	// neuron is not firing
		else
		{
			not_firing( signSwitchedPotential );
		}
}

NeuronInterz_Plastic::NeuronInterz_Plastic( const bool isInhibitory, const float firingThreshold, const float potentialDecay, const float plasticityStrengthen, const float plasticityWeaken )
 : NeuronInterz( isInhibitory, firingThreshold, potentialDecay )
 , m_plasticityStrengthen(plasticityStrengthen)
 , m_plasticityWeaken(plasticityWeaken)
{
}

NeuronInterz_Plastic::NeuronInterz_Plastic( const bool isInhibitory, const float firingThreshold, const float potentialDecay, const unsigned int motorFunc, bool* motorOoutputPointer, const float plasticityStrengthen, const float plasticityWeaken )
 : NeuronInterz( isInhibitory, firingThreshold, potentialDecay, motorFunc, motorOoutputPointer)
 , m_plasticityStrengthen(plasticityStrengthen)
 , m_plasticityWeaken(plasticityWeaken)
{
}

void NeuronInterz_Plastic::process()
{
	// potential decay
		m_potential *= m_potentialDecay;

	// add connected neurons output (synapse) to potential (but with plasticity)
		for ( auto syn_it(m_synapses_begin); syn_it != m_synapses_end; ++syn_it )
// 		for ( m_syn = m_syn_beginP; m_syn != m_syn_endP; ++m_syn )
		{
			auto& w(syn_it->m_weight);

			// lower synaptic weights
				w = w * m_plasticityWeaken;

			// add connected neurons output (synapse) to potential
// 				m_potential += w * *m_syn->ref; // m_it->dendrm_iteBranch * 
				m_potential += w * syn_it->m_ref; // m_it->dendrm_iteBranch * 
		}

	// calc "signed" potential
		const float signSwitchedPotential( m_inhibitorySignSwitch * m_potential );
	
	// neuron is firing
		if ( signSwitchedPotential >= m_firingThreshold )
		{
			// reset neural potential
				m_potential = 0.0f;

			// fire the neuron
				m_waitoutput = m_inhibitorySignSwitch;
				m_fired = true;

			// plasticity: if neuron & synapse fire together, the synapse strenghtens
// 				for ( m_syn = m_syn_beginP; m_syn != m_syn_endP; ++m_syn )
				for ( auto syn_it(m_synapses_begin); syn_it != m_synapses_end; ++syn_it )
				{
// 					const float r( m_inhibitorySignSwitch * *m_syn->ref );
					const float r( m_inhibitorySignSwitch * syn_it->m_ref );
					auto& w(syn_it->m_weight);
					// if synapse fired, strenghten the weight
					if ( (r > 0.0f && w > 0.0f) || (r < 0.0f && w < 0.0f) )
					{
						w *= m_plasticityStrengthen;

						// if weight > max back to max
						if	( w > 5.0f )
							w = 5.0f;
						else if ( w <-5.0f )
							w =-5.0f;
					}
				}
		}
	
	// neuron is not firing
		else
		{
			not_firing( signSwitchedPotential );
		}
}
