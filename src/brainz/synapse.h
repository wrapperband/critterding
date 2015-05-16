#ifndef SYNAPSE_H
#define SYNAPSE_H

class Synapse
{
	public:
		Synapse(const float& ref, const float weight);
		~Synapse();

		// InputNeuron's Output Pointer
		//int *ref;
// 		float *ref;
		const float& m_ref;

		// dendridic branch synapse is located on
// 		float dendriteBranch;

		// it's synaptic weight -1.0f <-> +1.0f
		float m_weight;

		bool getWeightedInfluence_ready() const
		{
			if ( m_weight != 0.0f && m_ref != 0.0f )
				return true;
			return false;
		}
		
		float getWeightedInfluence() const
		{
// 			if ( m_weight != 0.0f && m_ref != 0.0f )
				return m_weight * m_ref;
// 			return 0.0f;
		}

	private:
};

#endif
