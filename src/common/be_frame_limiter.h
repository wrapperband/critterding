#ifndef BE_FRAME_LIMITER_H_INCLUDED
#define BE_FRAME_LIMITER_H_INCLUDED

#include "utils/settings.h"
#include "be_timer.h"

class BeFrameLimiter
{
	public:
		BeFrameLimiter()
		 : settings(Settings::Instance())
		 , active(Settings::Instance()->getCVarPtr("fpslimit"))
		 , m_optimal(Settings::Instance()->getCVarPtr("fpslimit_frames"))
// 		 , m_optimal(60)
		 , m_sleeptime(1000.0f / *m_optimal)
		{
		}
		~BeFrameLimiter() {};

		void mark(BeTimer *t)
		{
			if ( *active == 1 )
			{
				float averageValue(0.0f);
				float depth(0.05f * *m_optimal);
				if (depth < 2.0f)
					depth = 2.0f;
				
				averageValue += m_tracker.process( 0.001f * t->getMilliSeconds(), depth );

				const float difference(1000.0f * averageValue - (1000.0f / *m_optimal));

				const float closegap(0.1f * depth); // ( 1.0f = close gap 100% in 1 frame)
				if ( *m_optimal > 0 )
					m_sleeptime -= difference * closegap;

				// don't let the sleeptime sink too deep, but deep enough to allow for accurate fps limiting at close to max framerates
				if ( m_sleeptime < -10.0f )
					m_sleeptime = -10.0f;
				else if (m_sleeptime > 0.0f )
					SDL_Delay(m_sleeptime);
			}
		}

	private:
		Settings*		settings;
		const int*		active;

		const int*		m_optimal;
		float m_sleeptime;
		
		
		struct BeFpsTracker
		{
			BeFpsTracker()
			{
				reset();
			};
			virtual ~BeFpsTracker() {};
			void reset()
			{
				m_averageCount = 0;
				m_averageValueTotal = 0.0f;
				m_averageValue = 0.0f;
			}
			float process( const float timeDelta, const unsigned int depth  )
			{
				// calculate new "average" timeDelta smoothed out over depth
				if ( m_averageCount < depth ) 
					m_averageCount++;
				else
					m_averageValueTotal -= m_averageValue;

				m_averageValueTotal += timeDelta;
				m_averageValue = m_averageValueTotal / m_averageCount;
				return m_averageValue;
			}
			unsigned int m_averageCount;
			float m_averageValueTotal;
			float m_averageValue;
		};

		
		BeFpsTracker m_tracker;

};



// class BeFrameLimiter
// {
// 	public:
// 		BeFrameLimiter() :
// 		 settings(Settings::Instance()),
// 		 active(Settings::Instance()->getCVarPtr("fpslimit")),
// 		 optimal(Settings::Instance()->getCVarPtr("fpslimit_frames")),
// 		 stepsize(50),
// 		 sleeptime(0)
// 		{
// 		}
// 		~BeFrameLimiter() {};
// 
// 		void mark(BeTimer *t)
// 		{
// 			if ( *active == 1 )
// 			{
// 			// new cps
//                 int cpsi= *optimal;
// 				float cpsf;
// 				if ( t->getMilliSeconds() == 0 ) cpsf=static_cast<float>(cpsi) ;
// 				else cpsf = 1000.f / t->getMilliSeconds();
// 		
// 			// calc sleeptime & sleep
// 				if ( cpsf > *optimal )
// 					sleeptime += stepsize;
// 				else if ( cpsf < *optimal )
// 					sleeptime -= stepsize;
// 
// 				if (sleeptime > 0 ) SDL_Delay(static_cast<Uint32>(0.01f*sleeptime));
// 			}
// 		}
// 
// 	private:
// 		Settings*		settings;
// 		const int*		active;
// 		const int*		optimal;
// 		unsigned int		stepsize;
// 		int			sleeptime;
// };

#endif
