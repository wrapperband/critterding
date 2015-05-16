#ifndef BE_COMMAND_SYSTEM_H_INCLUDED
#define BE_COMMAND_SYSTEM_H_INCLUDED

#include <map>
#include <string>
#include "common/be_command.h"
#include "utils/settings.h"
#include <boost/function.hpp>



class BeCommandSystem
{
public:
	typedef boost::function<void ()>			FunctionVoid;
	typedef boost::function<void (const int)>		FunctionInt;
	typedef boost::function<void (const float)>		FunctionFloat;
	typedef boost::function<void (const std::string&)>	FunctionString;

	static BeCommandSystem* Instance();

	void executeCommand(const BeCommand& cmds);

	void registerVoidCommand(const std::string& name, FunctionVoid member);
	void registerIntCommand(const std::string& name, FunctionInt member);
	void registerFloatCommand(const std::string& name, FunctionFloat member);
	void registerStringCommand(const std::string& name, FunctionString member);

	/*void execCmd(const std::string& name);
	void execCmd(const std::string& name, const int value);
	void execCmd(const std::string& name, const float value);
	void execCmd(const std::string& name, const std::string& value);*/

protected:
	BeCommandSystem();

private:
	typedef std::map< std::string, FunctionVoid > MapFunctionVoid;
	typedef std::map< std::string, FunctionInt > MapFunctionInt;
	typedef std::map< std::string, FunctionFloat > MapFunctionFloat;
	typedef std::map< std::string, FunctionString > MapFunctionString;
	
	MapFunctionVoid		m_mapFunctionVoid;
	MapFunctionInt		m_mapFunctionInt;
	MapFunctionFloat	m_mapFunctionFloat;
	MapFunctionString	m_mapFunctionString;

	static BeCommandSystem*	_instance;
};

#endif
