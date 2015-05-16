#ifndef BRAINVIEW_H
#define BRAINVIEW_H

#include "src/gui/settingspanel.h"
// #include "../entities/worldb.h"
// #include "src/math/vector2f.h"
// #include "../entities/critterb.h"
// #include "../entities/critterselection.h"

class NeuronInterz;
class sensorNeuron;
class Critterselection;
class CritterB;

class Brainview : public Settingspanel
{
	struct neuron
	{
		neuron()
		{
			m_position_x = 0.0;
			m_position_y = 0.0;
			m_newposition_x = 0.0;
			m_newposition_y = 0.0;
			m_num_motor = 0;
		}
		NeuronInterz* nPointer;
		
		double m_position_x;
		double m_position_y;
		double m_newposition_x;
		double m_newposition_y;
		unsigned int m_num_motor;
// 		Vector2f position;
// 		Vector2f newposition;
		
		std::vector<unsigned int> m_links_to_sensor_sums;
		std::vector<unsigned int> m_links_to_neurons_sums;
	};
	struct sensor
	{
		sensor()
		{
			m_position_x = 0.0;
			m_position_y = 0.0;
		}
		sensorNeuron* sPointer;
// 		Vector2f position;
		double m_position_x;
		double m_position_y;
	};

	public:
		Brainview();
		~Brainview();

		void draw();
// 		WorldB*		world;
	private:
		Critterselection* critterselection;
		BeWidgetPtr viewbutton;
		BeWidgetPtr brainview;
// 		btScalar viewposition[16];
		CritterB* currentCritter;
		
		unsigned int m_num_motors;
		float m_motor_spacing;
		float m_half_motor_spacing;
		
		// drawing helpers
		int v_spacer;
		int v_space;
		float v_radius;
		float v_diam;
		int spacing;
		int column;
		int row;
		int rowlength;

		unsigned int start_of_vision_inputs;
		unsigned int stop_of_vision_inputs;
		
		// shortcuts to settings
		const int*	attractor1;
		const int*	attractor2;
		const int*	attractor3;
		const int*	attractor4;
		const int*	brain_maxfiringthreshold;

		std::vector<neuron> neurons;
		std::vector<sensor> sensors;

		const int*	threads;
		int m_threads_last;
};

#endif
