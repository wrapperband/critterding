#include "be_event_system.h"
#include <iostream>

void Event::process( BeCommandSystem* commandsystem, unsigned int elapsed_ms )
{
	if ( m_active )
	{
		if ( m_triggerdelay > 0 )
		{
			while ( m_currentdelay >= m_triggerdelay )
			{
				m_currentdelay -= m_triggerdelay;
				commandsystem->executeCommand( m_command );
			}
			
			m_currentdelay += elapsed_ms;
		}
		else
		{
			// no m_triggerdelay means always
			commandsystem->executeCommand( m_command );
		}

		// disable event if NOREPEAT
		if ( !m_repeat )
			m_active = false;
	}
}

float VirtualAxis::getMappedValue( int value ) const
{
// 	cerr << value << endl;

	if ( m_map_min == 0 && m_map_max == 0 )
		return 1.0f*value;
	
	// adjust value to minimum
		value -=  m_min;

	// flip sign if descending range
		if ( m_min > m_max )
			value *= -1;

	// map m_range to m_map_range
		float convertedValue = m_range_ratio * value;

	// FIXME first 2 can be shortened / combined
	// align convertedValue to m_min
		if ( m_map_min < m_map_max )
			convertedValue += m_map_min;
		else
			convertedValue -= m_map_min;

	// flip sign if map_min > map_max
		if ( m_map_min > m_map_max )
			convertedValue *= -1;

	// clamp converted value
		if ( m_map_min <= m_map_max ) {
			if ( convertedValue < m_map_min )
				convertedValue = m_map_min;
			else if ( convertedValue > m_map_max )
				convertedValue = m_map_max;
		}
		else {
			if ( convertedValue < m_map_max )
				convertedValue = m_map_max;
			else if ( convertedValue > m_map_min )
				convertedValue = m_map_min;
		}

	return convertedValue;
}

BeEventSystem* BeEventSystem::Instance() 
{
	static BeEventSystem t;
	return &t;
}

// int key
long unsigned int BeEventSystem::registerEvent( BindKey_State state, long unsigned int bindkey, boost::shared_ptr<Event> event )
{
	if ( state == UP )
		bindkey += 1024;

	m_keystateMap[ bindkey ] = event;
	m_eventList.push_back(event);

	return bindkey;
}

void BeEventSystem::activateKeystate( const long unsigned int key )
{
	// activate events waiting for key DOWN
	if ( m_keystateMap[key] )
	{
		m_keystateMap[key]->activate();
		m_keystateMap[key]->process( m_commandsystem, 0 );
	}

	// deactivate events waiting for key DOWN
	if ( m_keystateMap[key+1024] )
		m_keystateMap[key+1024]->deactivate();
}

void BeEventSystem::deactivateKeystate( const long unsigned int key )
{
	// deactivate events waiting for key UP
	if ( m_keystateMap[key] )
		m_keystateMap[key]->deactivate();

	// activate events waiting for key UP
	if ( m_keystateMap[key+1024] )
		m_keystateMap[key+1024]->activate();
}

// string key
long unsigned int BeEventSystem::registerEvent( BindKey_State state, const std::string& name, boost::shared_ptr<Event> event )
{
	if ( !m_stringKeyLookupMap[name] )
	{
		// find the first available key in m_keystateMap
		long unsigned int i = 8192;
		while ( m_keystateMap[i] )
			++i;

		// register in m_stringKeyLookupMap
		m_stringKeyLookupMap[name] = i;
		registerEvent( state, i, event );
		return i;
	}
	else
	{
		long unsigned int i = m_stringKeyLookupMap[name];
		registerEvent( state, i, event );
		return i;
	}
	return 0;
}

void BeEventSystem::activateKeystate( const std::string& name )
{
	if ( m_stringKeyLookupMap[name] )
		activateKeystate( m_stringKeyLookupMap[name] );
}

void BeEventSystem::deactivateKeystate( const std::string& name )
{
	if ( m_stringKeyLookupMap[name] )
		deactivateKeystate( m_stringKeyLookupMap[name] );
}

// Axis
long unsigned int BeEventSystem::registerEvent( long unsigned int bindaxis, boost::shared_ptr<VirtualAxis> virtualaxis )
{
	// create axis if it doesn't exist
		if ( !m_axisMap[ bindaxis ] )
			m_axisMap[ bindaxis ] = boost::shared_ptr<Axis>(new Axis());

	// register Virtual Axis
		m_axisMap[ bindaxis ]->registerVirtualAxis( virtualaxis );

	// copy event pointer to eventlist
		m_eventList.push_back( virtualaxis->getEvent() );

	return bindaxis;
}


void BeEventSystem::setAxisstate( const long unsigned int key, int value )
{
 	//std::cerr << "axis: " << key-6144 << " " << value << std::endl;
	if ( m_axisMap[key] )
	{
		// find correct VirtualAxis
		VirtualAxis* virtualaxis = m_axisMap[key]->getVirtualAxis( value );
		
		if ( virtualaxis )
		{
			// map range
			float mappedvalue = virtualaxis->getMappedValue(value);
// 			cerr << "  => " << mappedvalue << endl;

			// update & activate command
			virtualaxis->getEvent()->getCommand()->m_argFloat = mappedvalue;
			virtualaxis->getEvent()->activate();
// 			virtualaxis->getEvent()->process( m_commandsystem, 0 );
		}
	}
}

void BeEventSystem::processEvents( unsigned int elapsed_ms )
{
	for ( unsigned int i=0; i < m_eventList.size(); ++i )
	{
		// check delay timers
		m_eventList[i]->process( m_commandsystem, elapsed_ms );
	}
}
