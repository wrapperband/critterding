#ifndef BE_FRAMECOUNTER_H_INCLUDED
#define BE_FRAMECOUNTER_H_INCLUDED

// #include "be_timer.h"
// #include <iostream>

// TODO REMOVE WHEN STUNTCOUREUR WENT INTO SANDBOX
class BeFrameCounter
{
public:
	BeFrameCounter() {
		reset();
	};
	~BeFrameCounter() { };
	void reset()
	{
		m_totalFrames=0;
		m_totalTime=0.f;
		m_time=0.f;
		m_frames=0;
		m_currentFramesPerSecond=0;
		m_averageFramesPerSecond=0;
		m_currentFramesPerSecondFloat=0.0f;
		m_averageFramesPerSecondFloat=0.0f;
	}
	void mark(const float timeDelta)
	{
		++m_frames;
		m_time+=timeDelta;
		if(m_time>=1.0f)
		{
			m_totalFrames+=m_frames;
			m_totalTime+=m_time;
			m_averageFramesPerSecond=m_totalFrames/m_totalTime;
			m_currentFramesPerSecond=m_frames;
			m_averageFramesPerSecondFloat=(float)m_totalFrames/m_totalTime;
			m_currentFramesPerSecondFloat=(float)m_frames/m_time;
			m_frames=0;
			m_time=0.0f;
		}
	}
	unsigned int getCurrentFramesPerSecond() const { return m_currentFramesPerSecond; }
	unsigned int getAverageFramesPerSecond() const { return m_averageFramesPerSecond; }
	float getCurrentFramesPerSecondf() const { return m_currentFramesPerSecondFloat; }
	float getAverageFramesPerSecondf() const { return m_averageFramesPerSecondFloat; }

private:
	unsigned int m_totalFrames;
	float m_totalTime;
	float m_time;
	unsigned int m_frames;
	unsigned int m_currentFramesPerSecond;
	unsigned int m_averageFramesPerSecond;
	float m_currentFramesPerSecondFloat;
	float m_averageFramesPerSecondFloat;
};

#endif


// #ifndef FPS_H
// #define FPS_H
// 
// #include "be_timer.h"
// #include <iostream>
// 
// class BeFrameCounter
// {
// public:
// 	BeFrameCounter() : m_totalFrames(0), m_totalTime(0.f), m_time(0.f), m_frames(0), m_currentFramesPerSecond(0), m_averageFramesPerSecond(0)
// 	{
// 	}
// 	void reset()
// 	{
// 		m_totalFrames=0;
// 		m_totalTime=0.f;
// 		m_time=0.f;
// 		m_frames=0;
// 		m_currentFramesPerSecond=0;
// 		m_averageFramesPerSecond=0;
// 	}
// 	void mark(const float timeDelta)
// 	{
// 		++m_frames;
// 		m_time+=timeDelta;
// 		if(m_time>1.0f)
// 		{
// 			m_totalFrames+=m_frames;
// 			m_totalTime+=m_time;
// 			m_averageFramesPerSecond=static_cast<unsigned int>(m_totalFrames/m_totalTime);
// 			m_currentFramesPerSecond=m_frames;
// 			m_frames=0;
// 			m_time-=1.0f;
// 		}
// 	}
// 	unsigned int getCurrentFramesPerSecond()
// 	{
// 		return m_currentFramesPerSecond;
// 	}
// 	unsigned int getAverageFramesPerSecond()
// 	{
// 		return m_averageFramesPerSecond;
// 	}
// 	~BeFrameCounter()
// 	{
// 	}
// private:
// 	unsigned int m_totalFrames;
// 	float m_totalTime;
// 	float m_time;
// 	unsigned int m_frames;
// 	unsigned int m_currentFramesPerSecond;
// 	unsigned int m_averageFramesPerSecond;
// };
// 
// #endif
