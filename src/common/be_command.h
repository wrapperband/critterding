#ifndef BE_COMMAND_H_INCLUDED
#define BE_COMMAND_H_INCLUDED

#include <string>

	class BeCommand
	{
		public:
			enum cmdargtype {
				A_NOARG = 1,
				A_INT,
				A_FLOAT,
				A_STRING
			};
			BeCommand(const std::string& name) : m_name(name), m_argtype(A_NOARG) {};
			BeCommand(const std::string& name, const std::string& argString) : m_name(name), m_argtype(A_STRING), m_argString(argString) {};
			BeCommand(const std::string& name, float argFloat) : m_name(name), m_argtype(A_FLOAT), m_argFloat(argFloat) {};
			BeCommand(const std::string& name, int argInt) : m_name(name), m_argtype(A_INT), m_argInt(argInt) {};
			BeCommand() {};
			~BeCommand() {};

			std::string m_name;
			cmdargtype m_argtype;
			int m_argInt;
			float m_argFloat;
			std::string m_argString;
	};

#endif
