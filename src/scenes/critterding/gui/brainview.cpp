#include "brainview.h"
#ifdef HAVE_OPENMP
	#include <omp.h>
#endif
#include "scenes/critterding/entities/genotype.h"
#include "scenes/critterding/entities/critterb.h"
#include "scenes/critterding/entities/critterselection.h"


Brainview::Brainview()
 : Settingspanel(true)
 , start_of_vision_inputs(0)
 , stop_of_vision_inputs(0)
 , threads(settings->getCVarPtr("threads"))
 , m_threads_last(0)

{
	critterselection = Critterselection::Instance();

	brain_maxfiringthreshold = settings->getCVarPtr("brain_maxfiringthreshold");

	m_active = false;
	isMovable = true;

	rowlength = 37;
	v_radius = 2;
	v_diam = 2*v_radius;
	spacing = 3;

	int brainview_width = 20 + ((v_diam+spacing) * settings->getCVar("critter_retinasize") * 4);
	if ( brainview_width < 600 )
		brainview_width = 600;

	int brainview_height = 20 + ((v_diam+spacing) * 40);
	if ( brainview_height < 550 )
		brainview_height = 550;

	m_dimensions.set(20 + brainview_width, 80 + brainview_height);

	int buttons_starty = getHeight() - brainview_height - 10;
	int brainview_starty = buttons_starty;

	m_localposition.set(2, 24);

	currentCritter = 0;

	// view widgets
	brainview = addWidgetButton( "bv_bview", Vector2i(10, m_dimensions.m_y+brainview_starty), Vector2i(brainview_width, brainview_height), "", Vector2i(0, 0), BeCommand(""), EVENT_NOREPEAT, 0 );
	children["bv_bview"]->isTransparant = false;

// 	addWidgetButton( "button_close_panel_cars", Vector2f(m_dimensions.m_x-button_height-bspacing, bspacing), Vector2f(button_height, button_height), "x", BeCommand("gui_togglepanel", "panel_cars"), EVENT_NOREPEAT, 0 );

// 	BeWidgetPtr w = addWidgetButton( "bv_close", Vector2i(m_dimensions.m_x-25, 10), Vector2i(15, 15), "x", BeCommand("gui_togglepanel", "brainview"), EVENT_NOREPEAT, 0 );
	BeWidgetPtr w = addWidgetButton( "brainview_close", Vector2i(2, 2), Vector2i(12, 12), "x", BeCommand("gui_togglepanel", "brainview"), EVENT_NOREPEAT, 0 );
		w->isAlignedRight = true;


	settings->registerLocalCVar("attract neurons", new CVar(0, 0, 1000000, false, "attract neurons"));
	addSettingmutator("attract neurons", 0, 0);

	settings->registerLocalCVar("repel neurons", new CVar(0, 0, 1000000, false, "repel neurons"));
	addSettingmutator("repel neurons", 0, 0);

	settings->registerLocalCVar("attract neuron inputs", new CVar(0, 0, 1000000, false, "attract neuron<>inputs"));
	addSettingmutator("attract neuron inputs", 0, 0);
	settings->registerLocalCVar("repel neuron inputs", new CVar(0, 0, 1000000, false, "repel neuron<>inputs"));
	addSettingmutator("repel neuron inputs", 0, 0);
// 	settings->registerLocalCVar("attract neuron inputs", new CVar(50, 0, 1000000, false, "attract neuron<>inputs"));
// 	addSettingmutator("attract neuron inputs", 0, 0);
// 	settings->registerLocalCVar("repel neuron inputs", new CVar(50, 0, 1000000, false, "repel neuron<>inputs"));
// 	addSettingmutator("repel neuron inputs", 0, 0);

	attractor1 = settings->getCVarPtr("attract neurons");
	attractor2 = settings->getCVarPtr("attract neuron inputs");
	attractor3 = settings->getCVarPtr("repel neurons");
	attractor4 = settings->getCVarPtr("repel neuron inputs");
}

void Brainview::draw()
{
	column = 0;
	row = 0;

	if ( critterselection->selectedCritter == 0 )
	{
		m_active = false;
		currentCritter = 0;
	}

	if (m_active)
	{
// 		brainview->getLocalPosition().m_x = m_dimensions.m_y+brainview_starty;
// 		brainview->getLocalPosition().m_y = m_dimensions.m_y+getHeight() - brainview_height - 10;
// 		Vector2i(10, m_dimensions.m_y+brainview_starty), Vector2i(brainview_width, brainview_height)
		
// 		m_dimensions.set(20 + brainview_width, 80 + brainview_height);
		
		brainview->setDimensions(Vector2i(m_dimensions.m_x-20, m_dimensions.m_y - 80));
		
		
		if ( critterselection->selectedCritter != currentCritter )
		{
			currentCritter = critterselection->selectedCritter;

			const auto& n_size(currentCritter->brain.totalNeurons);
// 			const auto& s_size(currentCritter->brain.Inputs.size());

			sensors.clear();
			int sverticalpos;
			int constraintcount = 0;

			const auto& b_inputs_it_end( currentCritter->brain.Inputs.end() );

			for ( auto b_inputs_it(currentCritter->brain.Inputs.begin()); b_inputs_it != b_inputs_it_end; ++b_inputs_it )
// 			for ( unsigned int i=0; i < currentCritter->brain.numberOfInputs; ++i )
			{
				if ( (*b_inputs_it).id == 10000 )
				{
					sverticalpos = 0;
					sensor s; s.sPointer = &(*b_inputs_it);
					s.m_position_x = brainview->getLocalPosition().m_x;
					s.m_position_y = 10.0f;
					sensors.push_back(s);
				}
				else if ( (*b_inputs_it).id == 10001 )
				{
					sverticalpos = 1;
					sensor s; s.sPointer = &(*b_inputs_it);
					s.m_position_x = brainview->getLocalPosition().m_x;
					s.m_position_y = 10.0f+(spacing+v_diam)*sverticalpos;
					sensors.push_back(s);
				}
				else if ( (*b_inputs_it).id == 10002 )
				{
					sverticalpos = 2;
					sensor s; s.sPointer = &(*b_inputs_it);
					s.m_position_x = brainview->getLocalPosition().m_x;
					s.m_position_y = 10.0f+(spacing+v_diam)*sverticalpos;
					sensors.push_back(s);
				}
				else if ( (*b_inputs_it).id == 10003 )
				{
					sverticalpos = 3;
					sensor s; s.sPointer = &(*b_inputs_it);
					s.m_position_x = brainview->getLocalPosition().m_x;
					s.m_position_y = 10.0f+(spacing+v_diam)*sverticalpos;
					sensors.push_back(s);
				}
				else if ( (*b_inputs_it).id == 20000 )
				{
					sverticalpos = 4;
					sensor s; s.sPointer = &(*b_inputs_it);
					s.m_position_x = brainview->getLocalPosition().m_x;
					s.m_position_y = 10.0f+(spacing+v_diam)*sverticalpos;
					sensors.push_back(s);
				}
				else if ( (*b_inputs_it).id >= 30000 && (*b_inputs_it).id < 40000 )
				{
					sverticalpos = 6;
					sensor s; s.sPointer = &(*b_inputs_it);
					s.m_position_x = brainview->getLocalPosition().m_x;
					s.m_position_y = 10.0f+(spacing+v_diam)*sverticalpos +((spacing+v_diam)*((*b_inputs_it).id-30000));
					sensors.push_back(s);
				}
				else if ( (*b_inputs_it).id >= 40000 && (*b_inputs_it).id < 50000 )
				{
					sverticalpos = 17;
					sensor s; s.sPointer = &(*b_inputs_it);
					s.m_position_x = brainview->getLocalPosition().m_x;
					s.m_position_y = 10.0f+(spacing+v_diam)*sverticalpos +((spacing+v_diam)*((*b_inputs_it).id-40000));
					sensors.push_back(s);
				}
				else if ( (*b_inputs_it).id < 10000 )
				{
					sverticalpos = 28;
					sensor s; s.sPointer = &(*b_inputs_it);
					s.m_position_x = brainview->getLocalPosition().m_x;
					s.m_position_y = 10.0f+(spacing+v_diam)*sverticalpos +((spacing+v_diam)*(constraintcount));
					sensors.push_back(s);
					constraintcount++;
				}
				else if ( (*b_inputs_it).id >= 50000 && (*b_inputs_it).id < 60000 )
				{
					if ( start_of_vision_inputs == 0 )
						start_of_vision_inputs = sensors.size();
					
					stop_of_vision_inputs = sensors.size();
					
					const int vinput = (*b_inputs_it).id-50000;
					const int vcolmax = currentCritter->genotype->bodyArch->retinasize * 4;
					const float vinput_ratio = vinput / vcolmax;
					const int col = vinput - (vinput_ratio * vcolmax);
					const int row = currentCritter->genotype->bodyArch->retinasize - vinput_ratio - 1;

					sensor s; s.sPointer = &(*b_inputs_it);
// 					s.position = Vector2f(brainview->getLocalPosition().m_x+((spacing+v_diam)*col)+10, 10.0f+((spacing+v_diam)*row));
					s.m_position_x = brainview->getLocalPosition().m_x+(((spacing/4)+v_diam)*col)+10;
					s.m_position_y = 10.0f+((spacing+v_diam)*row);
					sensors.push_back(s);
				}
				else
				{
					std::cerr << "critterview: inputs don't add up" << std::endl;
					exit(0);
				}
			}

// 			for ( unsigned int i=0; i < currentCritter->brain.numberOfInputs; ++i )
// 			{
// 				sensor s;
// 				s.sPointer = &(*b_inputs_it);
// 				int woffset, hoffset;
// 				if ( s.sPointer->id == 10000 )
// 					woffset = 100; hoffset = 50;
// 				else
// 				{
// // 					woffset = v_radius+((spacing+v_diam)*column);
// // 					hoffset = v_radius+((spacing+v_diam)*row);
// 					woffset = 20; hoffset = 20;
// 				}
// 				s.position = Vector2f(woffset, hoffset);
// 				sensors.push_back(s);
// 
// 				if ( ++column == rowlength ) { column = 0; row++; }
// 			}

			row = (currentCritter->brain.numberOfInputs/rowlength) + 1;
			neurons.clear();
			
			// ADD NON MOTOR NEURONS, meanwhile count them
			m_num_motors = 0;
			const auto& b_neurons_it_end( currentCritter->brain.Neurons.end() );
			for ( auto n_it(currentCritter->brain.Neurons.begin()); n_it != b_neurons_it_end; ++n_it )
// 			for ( unsigned int i=0; i < currentCritter->brain.totalNeurons; ++i )
			{
				neuron n;
				n.nPointer = (*n_it);
				n.m_position_x = 50+v_radius+((spacing+v_diam)*column);
				n.m_position_y = 30+v_radius+((spacing+v_diam)*row);

				if ( (*n_it)->motor() )
				{
					n.m_num_motor = m_num_motors;
					++m_num_motors;
				}
				
				neurons.push_back(n);

				if ( ++column == rowlength )
				{
					column = 0;
					++row;
				}
			}

			// calculate links_to_sensor_sums
			for ( unsigned int i=0; i < n_size; ++i )
			{
				auto& n(neurons[i]);
				const auto& syn(currentCritter->genotype->brainzArch->ArchNeurons[i].ArchSynapses);

				n.m_links_to_sensor_sums.clear();

				for ( unsigned int j=0; j < sensors.size(); ++j )
				{
					// how many connections do they have underling
					unsigned int nrlinks(0);
					for ( unsigned int k=0; k < syn.size(); ++k )
					{
						const auto& as = syn[k];
						if ( as.isSensorNeuron && as.realneuronID == (int)j )
							++nrlinks;
					}
					n.m_links_to_sensor_sums.push_back(nrlinks);
				}
			}
			
			// calculate links between each other
			
				// first clear & add all as 0
				for ( unsigned int i=0; i < n_size; ++i )
				{
					auto& n(neurons[i]);
					n.m_links_to_neurons_sums.clear();
					for ( unsigned int j=0; j < n_size; ++j )
					{
						n.m_links_to_neurons_sums.push_back(0);
					}
				}

				// fill in values
				const unsigned int n_size_minus_one(n_size-1);
				for ( unsigned int i=0; i < n_size_minus_one; ++i )
				{
					auto& n(neurons[i]);
					const auto& syn(currentCritter->genotype->brainzArch->ArchNeurons[i].ArchSynapses);
					
// 					n.m_links_to_neurons_sums.clear();
					
					for ( unsigned int j=i+1; j < n_size; ++j )
					{
						auto& n2(neurons[j]);
						const auto& syn2(currentCritter->genotype->brainzArch->ArchNeurons[j].ArchSynapses);

						// how many connections do they have underling
						unsigned int nrlinks(0);
						for ( unsigned int k=0; k < syn.size(); ++k )
						{
							const auto& as = syn[k];
							if ( !as.isSensorNeuron && as.neuronID == j )
								nrlinks++;
							
							const auto& as2 = syn2[k];
							if ( !as2.isSensorNeuron && as2.neuronID == i )
								nrlinks++;
						}
						n.m_links_to_neurons_sums[j] = nrlinks;
						n2.m_links_to_neurons_sums[i] = nrlinks;
					}
				}
			
			
		}

		drawBackground();
		drawBorders();
		// draw the rest
		drawChildren();

		// DRIFT

			const auto& n_size(neurons.size());
			const auto& s_size(sensors.size());
		
			unsigned int i;
			const unsigned int n_size_minus_one(n_size-1);
		
// 			#pragma omp parallel for private(i, j, k, nrlinks, xD, yD, dist, oneoverdistancesquared)
			
#ifdef HAVE_OPENMP
		// adjust threads
			if ( m_threads_last != *threads )
			{
				m_threads_last = *threads;
				omp_set_num_threads(m_threads_last);
			}

			#pragma omp parallel for private(i)
#endif
			for ( i=0; i < n_size_minus_one; ++i )
			{
				auto& n(neurons[i]);
// 				const auto& syn(currentCritter->genotype->brainzArch->ArchNeurons[i].ArchSynapses);
				for ( unsigned int j=i+1; j < n_size; ++j )
				{
					auto& n2(neurons[j]);
// 					const auto& syn2(currentCritter->genotype->brainzArch->ArchNeurons[j].ArchSynapses);

					const unsigned int nrlinks = n.m_links_to_neurons_sums[j];

					const double xD = n2.m_position_x - n.m_position_x;
					const double yD = n2.m_position_y - n.m_position_y;
					const double dist = sqrt((xD*xD)+(yD*yD));

					const double oneoverdistance = 1.0/dist;
					double oneoverdistancesquared = oneoverdistance/* * oneoverdistance*/;
					if ( oneoverdistancesquared > 0.01 )
						oneoverdistancesquared = 0.01;

					double n_x(0.0);
					double n_y(0.0);
					double n2_x(0.0);
					double n2_y(0.0);

					double one_over_links(1.0);
					if ( nrlinks > 0 )
					{
						const double t(oneoverdistancesquared * nrlinks * nrlinks * *attractor1 * 0.01);
						n_x += t * xD;
						n_y += t * yD;
						n2_x -= t * xD;
						n2_y -= t * yD;

						one_over_links = 1.0/nrlinks;
					}

					// general antigravity
// 					const double t(oneoverdistancesquared * oneoverdistancesquared * *attractor3 * 100);
// 					const double t(oneoverdistancesquared * *attractor3 / 1000);
					const double t(oneoverdistancesquared * one_over_links * one_over_links * *attractor3 * 0.01);
					n_x -= t * xD;
					n_y -= t * yD;
					n2_x += t * xD;
					n2_y += t * yD;
					
#ifdef HAVE_OPENMP
					#pragma omp critical
#endif
					{
						n.m_newposition_x += n_x;
						n.m_newposition_y += n_y;
						n2.m_newposition_x += n2_x;
						n2.m_newposition_y += n2_y;
					}
				}
			}

			// neuron vs sensor + APPLY && check borders
			
			const unsigned int t_spacing(20);
			const double minx = v_radius+t_spacing;
			const double maxx = brainview->getWidth()-v_radius - t_spacing;
			const double miny = sensors[start_of_vision_inputs].m_position_y + t_spacing;
			const double maxy = brainview->getHeight()-v_radius - t_spacing;

			m_motor_spacing = (float)brainview->getWidth( ) / m_num_motors;
			m_half_motor_spacing = m_motor_spacing / 2;
			
#ifdef HAVE_OPENMP
			#pragma omp parallel for private(i)
#endif
			for ( i=0; i < n_size; ++i )
			{
				auto& n(neurons[i]);
				for ( unsigned int j=0; j < s_size; ++j )
				{
					const auto& s(sensors[j]);
					// how many connections do they have underling
					const unsigned int nrlinks = n.m_links_to_sensor_sums[j];

					const double xD = s.m_position_x - n.m_position_x;
					const double yD = s.m_position_y - n.m_position_y;
					const double dist = sqrt((xD*xD)+(yD*yD));

					
					const double oneoverdistance = 1.0/dist;
					double oneoverdistancesquared = oneoverdistance/* * oneoverdistance*/;
					if ( oneoverdistancesquared > 0.01 )
						oneoverdistancesquared = 0.01;



					
// 					double oneoverdistancesquared = 1.0/(dist*dist*dist);
// 					if ( oneoverdistancesquared > 0.001 )
// 						oneoverdistancesquared = 0.001;

					double one_over_links(1.0);
					if ( nrlinks > 0 )
					{
						const double t(oneoverdistancesquared * nrlinks * nrlinks * *attractor2 * 0.01);
						n.m_newposition_x += t * xD;
						n.m_newposition_y += t * yD;
						
						one_over_links = 1.0/nrlinks;
						
// 						n.m_newposition_x += xD * oneoverdistancesquared * *attractor2 * nrlinks * 10;
// 						n.m_newposition_y += yD * oneoverdistancesquared * *attractor2 * nrlinks * 10;
					}
					// general antigravity
					const double t(oneoverdistancesquared * one_over_links * one_over_links * *attractor4 * 0.001);
					n.m_newposition_x -= t * xD;
					n.m_newposition_y -= t * yD;
// 					n.m_newposition_x -= xD * oneoverdistancesquared * oneoverdistancesquared * *attractor4 * 100000;
// 					n.m_newposition_y -= yD * oneoverdistancesquared * oneoverdistancesquared * *attractor4 * 100000;
				}

			// apply newpositions & check boundaries
// 				auto& n(neurons[i]);
				
				if ( !n.nPointer->motor() )
				{
					n.m_position_x += n.m_newposition_x;
					n.m_position_y += n.m_newposition_y;

					if ( n.m_position_x > maxx )
						n.m_position_x = maxx;
					
					if ( n.m_position_x < minx )
						n.m_position_x = minx;
					
					if ( n.m_position_y > maxy )
						n.m_position_y = maxy;
					
					if ( n.m_position_y < miny )
						n.m_position_y = miny;
				}
				else
				{
					// place motors in relation to window dimensions
					n.m_position_x = m_half_motor_spacing + (m_motor_spacing * n.m_num_motor);
					n.m_position_y = brainview->getHeight()-15;
// 					n.m_position_y = brainview->getHeight() / 2;
				}

				n.m_newposition_x = 0.0;
				n.m_newposition_y = 0.0;

			}

		// DRAW
		
			// connections
			glBegin(GL_LINES);
			float dimmed = 0.30f;

// 			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

			for ( unsigned int i=0; i < n_size; ++i )
			{
				auto& n(neurons[i]);
				const auto& syn(currentCritter->genotype->brainzArch->ArchNeurons[i].ArchSynapses);
				
				// sensors
// 				glColor4f(dimmed/2, 0.0f, dimmed/2, 1.0f);
				for ( unsigned int j=0; j < syn.size(); ++j )
				{
					const ArchSynapse& as = syn[j];
					const auto& braininput(currentCritter->brain.Inputs[as.realneuronID]);
					if ( as.isSensorNeuron && braininput.output )
					{
						const auto& s_as( sensors[as.realneuronID] );
						const auto color(dimmed*braininput.output);

						glColor4f(color, color, color, 1.0f);
						
						glVertex2f(n.m_position_x+brainview->getPosition().m_x,         n.m_position_y+brainview->getPosition().m_y);
						glVertex2f(brainview->getPosition().m_x+s_as.m_position_x,         brainview->getPosition().m_y+s_as.m_position_y);
					}
				}

				// inhibitory
				glColor4f(dimmed, 0.0f, 0.0f, 1.0f);
				for ( unsigned int j=0; j < syn.size(); ++j )
				{
					const ArchSynapse& as = syn[j];
					if ( !as.isSensorNeuron && currentCritter->brain.Neurons[as.neuronID]->fired() )
					{
						const auto& n_as( neurons[as.neuronID] );
						if ( n_as.nPointer->inhibitory() )
						{
							glVertex2f(n.m_position_x+brainview->getPosition().m_x,         n.m_position_y+brainview->getPosition().m_y);
							glVertex2f(brainview->getPosition().m_x+n_as.m_position_x,         brainview->getPosition().m_y+n_as.m_position_y);
						}
					}
				}
				
				// excitatory
				glColor4f(0.0f, dimmed, 0.0f, 1.0f);
				for ( unsigned int j=0; j < syn.size(); ++j )
				{
					const ArchSynapse& as = syn[j];
					if ( !as.isSensorNeuron && currentCritter->brain.Neurons[as.neuronID]->fired() )
					{
						const auto& n_as( neurons[as.neuronID] );
						if ( !n_as.nPointer->inhibitory() )
						{
							glVertex2f(n.m_position_x+brainview->getPosition().m_x,         n.m_position_y+brainview->getPosition().m_y);
							glVertex2f(brainview->getPosition().m_x+n_as.m_position_x,         brainview->getPosition().m_y+n_as.m_position_y);
						}
					}
				}
				
// 				for ( unsigned int j=0; j < syn.size(); ++j )
// 				{
// 					const ArchSynapse& as = syn[j];
// 					if ( 
// 							(!as.isSensorNeuron && currentCritter->brain.Neurons[as.neuronID]->fired())
// 						|| ( as.isSensorNeuron && currentCritter->brain.Inputs[as.realneuronID].output) )
// 					{
// 						if ( as.isSensorNeuron )
// 							glColor4f(dimmed/2, 0.0f, dimmed, 1.0f);
// 						else if ( !neurons[as.neuronID].nPointer->inhibitory() )
// 							glColor4f(0.0f, dimmed, 0.0f, 1.0f);
// 						else
// 							glColor4f(dimmed, 0.0f, 0.0f, 1.0f);
// 
// 						glVertex2f(n.m_position_x+brainview->getPosition().m_x,         n.m_position_y+brainview->getPosition().m_y);
// 						if ( as.isSensorNeuron )
// 						{
// 							glVertex2f(brainview->getPosition().m_x+sensors[as.realneuronID].m_position_x,         brainview->getPosition().m_y+sensors[as.realneuronID].m_position_y);
// 						}
// 						else
// 						{
// 							glVertex2f(brainview->getPosition().m_x+neurons[as.neuronID].m_position_x,         brainview->getPosition().m_y+neurons[as.neuronID].m_position_y);
// 						}
// 					}
// 				}
			}
			glEnd();

			glBegin(GL_QUADS);

			// inputs
			unsigned int color_rotation(0);
			for ( unsigned int i=0; i < s_size; ++i )
			{
				// non vision neurons
				auto& s(sensors[i]);
				const auto output(s.sPointer->output);
				if ( i < start_of_vision_inputs || i > stop_of_vision_inputs )
				{
					glColor4f(0.0f, output, 0.0f, 1.0f);
				}
				// vision neurons
				else
				{
					if ( color_rotation == 0 )
						glColor4f(output, 0.0f, 0.0f, 1.0f);
					else if ( color_rotation == 1 )
						glColor4f(0.0f, output, 0.0f, 1.0f);
					else if ( color_rotation == 2 )
						glColor4f(0.0f, 0.0f, output, 1.0f);
					else if ( color_rotation == 3 )
					{
						glColor4f(output, output, output, 1.0f);
// 						if ( output )
// 							std::cout << output << std::endl;
					}

					if ( ++color_rotation == 4 )
						color_rotation = 0;
				}
				float nv_radius = v_radius;
// 				float nv_radius = 0.5f * v_radius;
// 				float nv_radius_width = 0.25f * v_radius;

				glVertex2f(s.m_position_x+brainview->getPosition().m_x-nv_radius, s.m_position_y+brainview->getPosition().m_y+nv_radius);
				glVertex2f(s.m_position_x+brainview->getPosition().m_x-nv_radius, s.m_position_y+brainview->getPosition().m_y-nv_radius);
				glVertex2f(s.m_position_x+brainview->getPosition().m_x+nv_radius, s.m_position_y+brainview->getPosition().m_y-nv_radius);
				glVertex2f(s.m_position_x+brainview->getPosition().m_x+nv_radius, s.m_position_y+brainview->getPosition().m_y+nv_radius);
			}

			// motors neurons
			glColor4f(0.3f, 0.3f, 1.0f, 1.0f);
			for ( unsigned int i=0; i < n_size; ++i )
			{
				auto& n(neurons[i]);
				if ( n.nPointer->motor() )
				{
					float nv_radius = abs(n.nPointer->potential() * (v_diam / *brain_maxfiringthreshold));
					// show minimum
					if ( nv_radius < 0.5f )
						nv_radius = 0.5f;
					glVertex2f(n.m_position_x+brainview->getPosition().m_x-nv_radius, n.m_position_y+brainview->getPosition().m_y+nv_radius);
					glVertex2f(n.m_position_x+brainview->getPosition().m_x-nv_radius, n.m_position_y+brainview->getPosition().m_y-nv_radius);
					glVertex2f(n.m_position_x+brainview->getPosition().m_x+nv_radius, n.m_position_y+brainview->getPosition().m_y-nv_radius);
					glVertex2f(n.m_position_x+brainview->getPosition().m_x+nv_radius, n.m_position_y+brainview->getPosition().m_y+nv_radius);
				}
			}

			// inhibitory neurons
			glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
			for ( unsigned int i=0; i < n_size; ++i )
			{
				auto& n(neurons[i]);
				if ( !n.nPointer->motor() && n.nPointer->inhibitory() )
				{
					float nv_radius = abs(n.nPointer->potential() * (v_diam / *brain_maxfiringthreshold));
					// show minimum
					if ( nv_radius < 0.5f )
						nv_radius = 0.5f;
					glVertex2f(n.m_position_x+brainview->getPosition().m_x-nv_radius, n.m_position_y+brainview->getPosition().m_y+nv_radius);
					glVertex2f(n.m_position_x+brainview->getPosition().m_x-nv_radius, n.m_position_y+brainview->getPosition().m_y-nv_radius);
					glVertex2f(n.m_position_x+brainview->getPosition().m_x+nv_radius, n.m_position_y+brainview->getPosition().m_y-nv_radius);
					glVertex2f(n.m_position_x+brainview->getPosition().m_x+nv_radius, n.m_position_y+brainview->getPosition().m_y+nv_radius);
				}
			}
			
			// excitatory neurons
			glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
			for ( unsigned int i=0; i < n_size; ++i )
			{
				auto& n(neurons[i]);
				if ( !n.nPointer->motor() && !n.nPointer->inhibitory() )
				{
					float nv_radius = abs(n.nPointer->potential() * (v_diam / *brain_maxfiringthreshold));
					// show minimum
					if ( nv_radius < 0.5f )
						nv_radius = 0.5f;
					glVertex2f(n.m_position_x+brainview->getPosition().m_x-nv_radius, n.m_position_y+brainview->getPosition().m_y+nv_radius);
					glVertex2f(n.m_position_x+brainview->getPosition().m_x-nv_radius, n.m_position_y+brainview->getPosition().m_y-nv_radius);
					glVertex2f(n.m_position_x+brainview->getPosition().m_x+nv_radius, n.m_position_y+brainview->getPosition().m_y-nv_radius);
					glVertex2f(n.m_position_x+brainview->getPosition().m_x+nv_radius, n.m_position_y+brainview->getPosition().m_y+nv_radius);
				}
			}
			
// 			for ( unsigned int i=0; i < n_size; ++i )
// 			{
// 				auto& n(neurons[i]);
// 
// 				if ( n.nPointer->motor() )
// 					glColor4f(0.3f, 0.3f, 1.0f, 1.0f);
// 				else if ( n.nPointer->inhibitory() )
// 					glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
// 				else
// 					glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
// 
// 				float nv_radius = abs(n.nPointer->potential() * (v_diam / *brain_maxfiringthreshold));
// 				// show minimum
// 				if ( nv_radius < 0.5f )
// 					nv_radius = 0.5f;
// 				glVertex2f(n.m_position_x+brainview->getPosition().m_x-nv_radius, n.m_position_y+brainview->getPosition().m_y+nv_radius);
// 				glVertex2f(n.m_position_x+brainview->getPosition().m_x-nv_radius, n.m_position_y+brainview->getPosition().m_y-nv_radius);
// 				glVertex2f(n.m_position_x+brainview->getPosition().m_x+nv_radius, n.m_position_y+brainview->getPosition().m_y-nv_radius);
// 				glVertex2f(n.m_position_x+brainview->getPosition().m_x+nv_radius, n.m_position_y+brainview->getPosition().m_y+nv_radius);
// 			}

			glEnd();

	}
}

Brainview::~Brainview()
{
}
