#ifndef BE_EVENT_SYSTEM_H_INCLUDED
#define BE_EVENT_SYSTEM_H_INCLUDED

#include "be_command_system.h"
#include "src/common/be_command_system.h"
#include <math.h>

enum EventProperty_Repeat
{
	EVENT_NOREPEAT = 0,
	EVENT_REPEAT
};

enum BindKey_State
{
	DOWN = 0,
	UP
};

struct Event
{
	Event( const BeCommand& command, EventProperty_Repeat repeat, unsigned int delay ) : 
		m_active(false),
		m_command(command),
		m_repeat(repeat),
		m_triggerdelay(delay),
		m_currentdelay(0)
	{
	};
	Event( const BeCommand& command, EventProperty_Repeat repeat ) :
		m_active(false),
		m_command(command),
		m_repeat(repeat),
		m_triggerdelay(0),
		m_currentdelay(0)
	{};

	// activate & deactivate Events
	void activate(  )
	{
		m_active = true;
		m_currentdelay = m_triggerdelay;
	}
	void deactivate(  ) { m_active = false; }

	BeCommand* getCommand() { return &m_command; }
	void process( BeCommandSystem* commandsystem, unsigned int elapsed_ms );

	private:
		float			m_active;
		BeCommand		m_command;

		// repeat & timing
		EventProperty_Repeat	m_repeat;
		unsigned int		m_triggerdelay;
		unsigned int		m_currentdelay;
};

struct VirtualAxis
{
	VirtualAxis( boost::shared_ptr<Event> event, int min, int max, float map_min, float map_max ) : 
		m_event(event),
		m_min(min),
		m_max(max),
		m_range( abs(m_max - m_min) ),
		m_map_min(map_min),
		m_map_max(map_max),
		m_map_range( fabs(m_map_max - m_map_min) ),
		m_range_ratio(0)
	{
		if ( m_range != 0 )
			m_range_ratio = m_map_range / m_range;
	}
	
	bool valueInRange( int value ) const
	{
		if ( m_min <= m_max )
		{
			if ( value < m_min || value > m_max )
				return false;
		}
		else
		{
			if ( value < m_max || value > m_min )
				return false;
		}
		return true;
	}
	
	float getMappedValue( int value ) const;
	boost::shared_ptr<Event> getEvent( ) const { return m_event; }

	private :
		boost::shared_ptr<Event> m_event;
		int m_min;
		int m_max;
		int m_range;
		float m_map_min;
		float m_map_max;
		float m_map_range;
		float m_range_ratio;
};

struct Axis
{
	Axis() {};
	void registerVirtualAxis( boost::shared_ptr<VirtualAxis> virtualaxis ) { m_virtualaxisList.push_back( virtualaxis ); }

	VirtualAxis* getVirtualAxis( const int value ) const
	{
		for ( unsigned int i=0; i < m_virtualaxisList.size(); ++i )
			if ( m_virtualaxisList[i]->valueInRange(value) )
				return m_virtualaxisList[i].get();
		return 0;
	}
	private:
		std::vector<boost::shared_ptr<VirtualAxis> > m_virtualaxisList;
};

class BeEventSystem
{
	public:
		static BeEventSystem* Instance();
		~BeEventSystem() {};

		// int key
		long unsigned int registerEvent( BindKey_State state, long unsigned int bindkey, boost::shared_ptr<Event> event );
		void activateKeystate( const long unsigned int key );
		void deactivateKeystate( const long unsigned int key );

		// string key (maps to int key)
		long unsigned int registerEvent( BindKey_State state, const std::string& name, boost::shared_ptr<Event> event );
		void activateKeystate( const std::string& name );
		void deactivateKeystate( const std::string& name );

		// axis
		long unsigned int registerEvent( long unsigned int axis, boost::shared_ptr<VirtualAxis> virtualaxis );
		void setAxisstate( const long unsigned int bindaxis, int value );
		
		Event* getEvent( const long unsigned int key ) { return m_keystateMap[key].get(); }
		void processEvents( unsigned int elapsed_ms );
		BeCommandSystem* getCommandSystem() { return m_commandsystem; }
		
	protected:
		BeEventSystem()
		{
			m_commandsystem = BeCommandSystem::Instance();
		};
	private:

		// int key
// 		map<long unsigned int, boost::shared_ptr<Event> > m_modifierstatestateMap;
		std::map<long unsigned int, boost::shared_ptr<Event> > m_keystateMap;
		std::map<long unsigned int, boost::shared_ptr<Axis> > m_axisMap;
		std::vector<boost::shared_ptr<Event> >	m_eventList;

		// string key (maps to int key)
		std::map<std::string, long unsigned int> m_stringKeyLookupMap;

		BeCommandSystem* m_commandsystem;
};

#endif
