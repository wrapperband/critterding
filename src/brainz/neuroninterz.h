#ifndef NEURONINTERZ_H
#define NEURONINTERZ_H

#include "synapse.h"
#include <vector>

class NeuronInterz
{
	public:
		NeuronInterz( const bool isInhibitory, const float firingThreshold, const float potentialDecay );
		NeuronInterz( const bool isInhibitory, const float firingThreshold, const float potentialDecay, const unsigned int motorFunc, bool* motorOoutputPointer );
		virtual ~NeuronInterz() {};

		virtual void	process();
		void			connec( const float& output, const float synapticWeight ); // unsigned int dendriteBranch, 
		void			reset();
		void			commitOutput();
		bool		inhibitory() const { return m_isInhibitory; }
		bool		motor() const { return m_isMotor; }
		bool		fired() const { return m_fired; }
		float		potential() const { return m_potential; }
// 		float*			outputPointer() { return &m_output; }
		const float&	getOutput() { return m_output; }
		void			enableMotorOoutputPointer() { *m_motorOoutputPointer=true; }

	protected:
		inline void		not_firing( const float signSwitchedPotential );
		
		float				m_firingThreshold;
		float				m_potential;
		float				m_potentialDecay;
// 		Synapse*			m_syn;
// 		Synapse*			m_syn_beginP;
// 		Synapse*			m_syn_endP;
		std::vector<Synapse>	m_synapses;
		std::vector<Synapse>::iterator m_synapses_begin;
		std::vector<Synapse>::iterator m_synapses_end;

 		float				m_waitoutput;
		bool				m_fired;
		float				m_inhibitorySignSwitch;

	private:
		const bool			m_isMotor;
		unsigned int		m_motorFunc;
		bool*				m_motorOoutputPointer;
		const bool			m_isInhibitory;
 		float				m_output;
};

class NeuronInterz_Plastic : public NeuronInterz
{
	public:
		NeuronInterz_Plastic( const bool isInhibitory, const float firingThreshold, const float potentialDecay, const float plasticityStrengthen, const float plasticityWeaken );
		NeuronInterz_Plastic( const bool isInhibitory, const float firingThreshold, const float potentialDecay, const unsigned int motorFunc, bool* motorOoutputPointer, const float plasticityStrengthen, const float plasticityWeaken );
		virtual ~NeuronInterz_Plastic() {};

		virtual void	process();

	private:
		float			m_plasticityStrengthen;
		float			m_plasticityWeaken;
};

#endif
