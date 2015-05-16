#include "common/be_xml_helper.h"
#include "common/be_command.h"
#include "be_settings_loader.h"
#include <SDL/SDL.h>

//Windows.h defines these macros
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

bool BeSettingsLoader::load( BeEventSystem* eventsystem, const BeFile* befile )
{
	TiXmlDocument document;
	const bool result = document.LoadFile( befile->getFullFilename().c_str() );
	if(result)
	{
		const TiXmlElement* const rootElementPtr=document.RootElement();
		if(rootElementPtr)
		{
			const TiXmlElement& rootElement=(*rootElementPtr);
			const TiXmlElement* elementPtr=rootElement.FirstChildElement();
			while(elementPtr)
			{
// KEYBOARD
				const TiXmlElement& element=(*elementPtr);
				if ( element.ValueTStr() == "input_keyboard" )
				{
					std::string key("");
					std::string keystate("down");
					std::string action("");
					std::string param("");
					float value(-2.0f);
					// timing
					int repeat(0);
					int delay(0);

					BeXmlHelper xmlHelper;
						xmlHelper.addString("key", key);
						xmlHelper.addString("keystate", keystate);
						xmlHelper.addString("action", action);
						xmlHelper.addString("param", param);
						xmlHelper.addFloat("value", value);
						xmlHelper.addInt("repeat", repeat);
						xmlHelper.addInt("delay", delay);
					xmlHelper.parse(element);

					if ( key.size() > 0 )
					{
						EventProperty_Repeat EP_Repeat = EVENT_NOREPEAT;
						if ( repeat == 1 )
							EP_Repeat = EVENT_REPEAT;

						// construct command settings
							BeCommand cmdDown;
							if ( param.size() > 0 )
								cmdDown = BeCommand(action, param);
							else if ( value >= -1.0f && value <= 1.0f )
								cmdDown = BeCommand(action, value);
							else
								cmdDown = BeCommand(action);

						if ( keystate == "up" )
						{
							eventsystem->registerEvent( UP, key, boost::shared_ptr<Event>(new Event(cmdDown, EP_Repeat, delay)) );
						}
						else if ( keystate == "down" )
						{
							eventsystem->registerEvent( DOWN, key, boost::shared_ptr<Event>(new Event(cmdDown, EP_Repeat, delay)) );
						}
					}
				}

// JOYSTICK
				else if ( element.ValueTStr() == "joystick" )
				{
					std::string name("");
					BeXmlHelper xmlHelper;
					xmlHelper.addString("name", name);
					xmlHelper.parse(element);
					if ( SDL_NumJoysticks() > 0 )
						if( name == SDL_JoystickName(0) )
						{
							parseJoystick(*eventsystem, element);
						}
				}

// MOUSE
				else if ( element.ValueTStr() == "input_mouse_axis" )
				{
					int axis(-1);
					std::string action("");

					// calibration settings
					int min(-1);
					int max(1);
					float map_min(-1.0f);
					float map_max(1.0f);

					BeXmlHelper xmlHelper;
						xmlHelper.addInt("axis", axis);
						xmlHelper.addString("action", action);
						xmlHelper.addInt("min", min);
						xmlHelper.addInt("max", max);
						xmlHelper.addFloat("map_min", map_min);
						xmlHelper.addFloat("map_max", map_max);
					xmlHelper.parse(element);

					if ( axis > -1 )
					{
						// never repeat for axis
						BeCommand cmd = BeCommand( action, 0.0f );
						boost::shared_ptr<Event> event(new Event(cmd, EVENT_NOREPEAT));
						boost::shared_ptr<VirtualAxis> virtualaxis(new VirtualAxis(event, min, max, map_min, map_max));
						
						eventsystem->registerEvent( axis+7168, virtualaxis );
					}
				}
				else if ( element.ValueTStr() == "input_mouse_button" )
				{
					int button(-1);
					std::string keystate("down");
					std::string action("");
					std::string param("");
					float value(-2.0f);
					
					// timing
					int repeat(0);
					int delay(0);

					BeXmlHelper xmlHelper;
						xmlHelper.addInt("button", button);
						xmlHelper.addString("keystate", keystate);
						xmlHelper.addString("action", action);
						xmlHelper.addString("param", param);
						xmlHelper.addFloat("value", value);
						xmlHelper.addInt("repeat", repeat);
						xmlHelper.addInt("delay", delay);
					xmlHelper.parse(element);

					if ( button > -1 )
					{
						EventProperty_Repeat EP_Repeat = EVENT_NOREPEAT;
						if ( repeat == 1 )
							EP_Repeat = EVENT_REPEAT;

						// construct command settings
							BeCommand cmdDown;
							if ( param.size() > 0 )
								cmdDown = BeCommand(action, param);
							else if ( value >= -1.0f && value <= 1.0f )
								cmdDown = BeCommand(action, value);
							else
								cmdDown = BeCommand(action);

						if ( keystate == "up" )
							eventsystem->registerEvent( UP,   button+2048, boost::shared_ptr<Event>(new Event(cmdDown, EP_Repeat, delay)) );
						else if ( keystate == "down" )
							eventsystem->registerEvent( DOWN, button+2048, boost::shared_ptr<Event>(new Event(cmdDown, EP_Repeat, delay)) );
						//TODO log
					}
				}


// 				else if ( element.ValueTStr()=="joystick_axis_control" )
// 				{
// 					int axis = 0;
// 					std::string action = "";
// 
// 					int min = 0;
// 					int max = 0;
// 					
// 					BeXmlHelper<4> xmlHelper;
// 					xmlHelper.addInt("axis", axis);
// 					xmlHelper.addString("action", action);
// 					xmlHelper.addInt("min", min);
// 					xmlHelper.addInt("max", max);
// 					xmlHelper.parse(element);
// 					
// // 					Events::Instance()->registerEvent(axis+4096, action, BeCommand(action, 0), 0, 0, 0 );
// 
// // 					axisrangemap[action] = new AxisRange(min, max);
// 					axisrangemap[action] = boost::shared_ptr<AxisRange>(new AxisRange(min, max));
// 					
// // 					boost::shared_ptr<Player>(new Player( m_idcounter++ ));
// 				}
				elementPtr=element.NextSiblingElement();
			}
		}
		return true;
	}
	return false;
}

void BeSettingsLoader::parseJoystick(BeEventSystem& eventsystem, const TiXmlElement& rootElement)
{
	const TiXmlElement* elementPtr=rootElement.FirstChildElement();
	while(elementPtr)
	{
		// KEYBOARD
		const TiXmlElement& element=(*elementPtr);
		if ( element.ValueTStr() == "input_joystick_axis" )
		{
			int axis(-1);
			std::string action("");

			// calibration settings
			int min(-1);
			int max(1);
			float map_min(-1.0f);
			float map_max(1.0f);

			BeXmlHelper xmlHelper;
			xmlHelper.addInt("axis", axis);
			xmlHelper.addString("action", action);
			xmlHelper.addInt("min", min);
			xmlHelper.addInt("max", max);
			xmlHelper.addFloat("map_min", map_min);
			xmlHelper.addFloat("map_max", map_max);
			xmlHelper.parse(element);

			if ( axis > -1 )
			{
				// never repeat for axis
				BeCommand cmd = BeCommand( action, 0.0f );
				boost::shared_ptr<Event> event(new Event(cmd, EVENT_NOREPEAT));
				boost::shared_ptr<VirtualAxis> virtualaxis(new VirtualAxis(event, min, max, map_min, map_max));

				eventsystem.registerEvent( axis+6144, virtualaxis );
			}
		}
		else if ( element.ValueTStr()=="input_joystick_button" )
		{
			int button(-1);
			std::string keystate("down");
			std::string action("");
			std::string param("");
			float value(-2.0f);

			// timing
			int repeat(0);
			int delay(0);

			BeXmlHelper xmlHelper;
			xmlHelper.addInt("button", button);
			xmlHelper.addString("keystate", keystate);
			xmlHelper.addString("action", action);
			xmlHelper.addString("param", param);
			xmlHelper.addFloat("value", value);
			xmlHelper.addInt("repeat", repeat);
			xmlHelper.addInt("delay", delay);
			xmlHelper.parse(element);

			if ( button > -1 )
			{
				EventProperty_Repeat EP_Repeat = EVENT_NOREPEAT;
				if ( repeat == 1 )
					EP_Repeat = EVENT_REPEAT;

				// construct command settings
				BeCommand cmdDown;
				if ( param.size() > 0 )
					cmdDown = BeCommand(action, param);
				else if ( value >= -1.0f && value <= 1.0f )
					cmdDown = BeCommand(action, value);
				else
					cmdDown = BeCommand(action);

				if ( keystate == "up" )
					eventsystem.registerEvent( UP,   button+4096, boost::shared_ptr<Event>(new Event(cmdDown, EP_Repeat, delay)) );
				else if ( keystate == "down" )
					eventsystem.registerEvent( DOWN, button+4096, boost::shared_ptr<Event>(new Event(cmdDown, EP_Repeat, delay)) );
				//TODO log
			}		
		}
		elementPtr=element.NextSiblingElement();
	}
}