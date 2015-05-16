#ifndef BE_TIMER_H_INCLUDED
#define BE_TIMER_H_INCLUDED
#include <SDL/SDL.h>
#include <iostream>

class BeTimer
{
	public:
		BeTimer() :
		 m_totalMilliSeconds(0), 
		 m_elapsedMilliSeconds(0), 
		 m_elapsedSeconds(0)
		{
			SDL_Init(0); 
		};
		~BeTimer() {};

		void mark()
		{
			// get now
			int sdl_now = SDL_GetTicks();

			// calc diff between now and lasttime
			m_elapsedMilliSeconds = sdl_now - m_totalMilliSeconds;
			m_elapsedSeconds = 0.001f * m_elapsedMilliSeconds;

			m_totalMilliSeconds = sdl_now;
		}

		unsigned int getMilliSeconds() const { return m_elapsedMilliSeconds; }
		float getSeconds() const { return m_elapsedSeconds; }
		unsigned int getTotalMilliSeconds() const { return m_totalMilliSeconds; }
	private:
		unsigned int m_totalMilliSeconds;
		unsigned int m_elapsedMilliSeconds;
		float m_elapsedSeconds;
};

#endif
