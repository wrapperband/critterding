#include "be_log.h"
#include "common/be_parser.h"
#include "utils/settings.h"
#include <iostream>
#ifdef WIN32
	#include <Windows.h>
#endif

void BeLogDebug::log(const std::string& message)
{
	if ( 
		Settings::Instance()->getCVar(m_chapter) == 1
		|| Settings::Instance()->getCVar("debug_ALL") == 1
		|| m_chapter == "debug_ERROR"
	)
	{
		#ifndef WIN32
			std::string newmessage(message);
			BeParser parseH;

			// color wrap chapter
			if ( parseH.beginMatchesStrip("::", newmessage ) )
				parseH.wrap( newmessage, parseH.returnUntillStrip(" ", newmessage), ANSI_CYAN_BOLD, ANSI_NORMAL );

			// color wrap errors and warnings
			parseH.wrap( newmessage, " error", ANSI_RED_BOLD, ANSI_NORMAL );
			parseH.wrap( newmessage, " warning", ANSI_YELLOW_BOLD, ANSI_NORMAL );
			std::cout << newmessage;
		#else
			OutputDebugString(message.c_str());
			std::cout << message;
		#endif
	}
}
