#include "be_command_system.h"
#include <boost/bind/bind.hpp>

BeCommandSystem* BeCommandSystem::Instance () 
{
	static BeCommandSystem t;
	return &t;
}

BeCommandSystem::BeCommandSystem()
{
	Settings* const	settings = Settings::Instance();
	registerStringCommand("settings_saveprofile", boost::bind(&Settings::saveProfile, settings, _1));
	registerStringCommand("settings_increase", boost::bind(&Settings::increaseCVar, settings, _1, 1));
	registerStringCommand("settings_decrease", boost::bind(&Settings::decreaseCVar, settings, _1, 1));
}

void BeCommandSystem::registerVoidCommand(const std::string& name, FunctionVoid function)
{
	m_mapFunctionVoid[name]		= function;
}

void BeCommandSystem::registerIntCommand(const std::string& name, FunctionInt function)
{
	m_mapFunctionInt[name]		= function;
}

void BeCommandSystem::registerFloatCommand(const std::string& name, FunctionFloat function)
{
	m_mapFunctionFloat[name]	= function;
}

void BeCommandSystem::registerStringCommand(const std::string& name, FunctionString function)
{
	m_mapFunctionString[name]	= function;
}

void BeCommandSystem::executeCommand(const BeCommand& cmds)
{
	const std::string& name=cmds.m_name;
	const BeCommand::cmdargtype type=cmds.m_argtype;
	switch(type)
	{
	case BeCommand::A_NOARG:
		{
			const MapFunctionVoid::iterator iteratorVoid=m_mapFunctionVoid.find(name);
			if(iteratorVoid!=m_mapFunctionVoid.end())
			{
				iteratorVoid->second();
			}
			break;
		}
	case BeCommand::A_INT:
		{
			const MapFunctionInt::iterator iteratorInt=m_mapFunctionInt.find(name);
			if(iteratorInt!=m_mapFunctionInt.end())
			{
				iteratorInt->second(cmds.m_argInt);
			}
			break;
		}
	case BeCommand::A_FLOAT:
		{
			const MapFunctionFloat::iterator iteratorFloat=m_mapFunctionFloat.find(name);
			if(iteratorFloat!=m_mapFunctionFloat.end())
			{
				iteratorFloat->second(cmds.m_argFloat);
			}
			break;
		}
	case BeCommand::A_STRING:
		{
			const MapFunctionString::iterator iteratorString=m_mapFunctionString.find(name);
			if(iteratorString!=m_mapFunctionString.end())
			{
				iteratorString->second(cmds.m_argString);
			}
			break;
		}
	default:
		break;
	}
}
