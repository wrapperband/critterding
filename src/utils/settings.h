#ifndef SETTINGS_H_INCLUDED
#define SETTINGS_H_INCLUDED

#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <string>
#include "common/be_parser.h"
#include "filesystem/be_dir.h"
#include "kernel/be_log.h"
#include "filesystem/be_filesystem.h"

class Logbuffer;

enum cvartype {
	T_INT = 1,
	T_FLOAT,
	T_BOOL,
	T_STRING,
};

struct CVar
{
	public:
		CVar( const std::string& string_value, const std::string& comment )
		 : m_type(T_STRING), m_string_value(string_value), m_comment(comment), m_logDebug("CVAR") {};
		CVar( const int& value, const int& min_val, const int& max_val, bool loop, const std::string& comment )
		 : m_type(T_INT), m_int_value(value), m_int_min(min_val), m_int_max(max_val), m_loop(loop), m_comment(comment), m_logDebug("CVAR") {};
		~CVar() {};

		bool set(const int& value)
		{
			if ( value >= m_int_min && value <= m_int_max )
			{
				m_int_value = value;
				return true;
			}
			else
			{
				return false;
			}
		}
		bool set(const std::string& value)
		{
			if ( m_type == T_INT )
				return set( atoi( value.c_str() ) );
			else if ( m_type == T_STRING )
			{
				m_string_value = value;
				return true;
			}
            return false;
		}
		void increase(const int& value)
		{
			int diff = m_int_value + value;
			if ( diff >= m_int_min && diff <= m_int_max )
				m_int_value += value;
			else if ( m_loop )
				m_int_value = m_int_min;
		}
		void decrease(const int& value)
		{
			int diff = m_int_value - value;
			if ( diff >= m_int_min )
				m_int_value = diff;
		}

		int		getIntValue() const { return m_int_value; }
		int		getIntMin() const { return m_int_min; }
		int		getIntMax() const { return m_int_max; }
		const int*		getIntValuePointer() const { return &m_int_value; }
		const std::string& 	getStringValue() const { return m_string_value; }
		cvartype		getType() const { return m_type; }
		const std::string&	getComment() const { return m_comment; }
		void			setLocal() { m_local = true; }
		bool		isLocal() const { return m_local; }
	private:
		cvartype m_type;
		std::string m_string_value;
		int m_int_value;
		int m_int_min;
		int m_int_max;
		bool m_loop;
		std::string m_comment;
		bool m_local;
		BeLogDebug m_logDebug;
};

class Settings
{
	public:
		static Settings* Instance();

		// General
		void			registerCVar(const std::string& name, CVar* cvar);
		void			registerLocalCVar(const std::string& name, CVar* cvar);
		void			unregisterCVar(const std::string& name);
		bool			isCVar(const std::string& name);

		// Int
		int			getCVar(const std::string& name);
		const int*		getCVarPtr(const std::string& name);
		bool			setCVar(const std::string& name, const unsigned int& value);
		void			decreaseCVar(const std::string& name, const unsigned int& value);
		void			increaseCVar(const std::string& name, const unsigned int& value);

		// String
		const std::string&	getCVarS(const std::string& name);
		bool			setCVar(const std::string& name, const std::string& value);

		void			checkCommandLineOptions(int argc, char *argv[]);
		void			loadProfile(const std::string& filename);
		void			saveProfile(const std::string& savedir);

		int*			winWidth;
		int*			winHeight;
		
		void setLogbuffer( boost::shared_ptr<Logbuffer> logBuffer ) { m_logBuffer = logBuffer; }
	protected:
		Settings();
		~Settings();
	private:
		static Settings* _instance;

		std::map<std::string, CVar*> cvarlist;

		Dir			dirH;
		std::stringstream helpinfo;
		void createHelpInfo();

		BeParser	parseH;
		BeFilesystem	m_filesystem;
		BeLogDebug	m_logDebug;
		boost::shared_ptr<Logbuffer> m_logBuffer;
};

#endif
