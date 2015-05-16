#ifndef BE_SETTINGS_LOADER_H_INCLUDED
#define BE_SETTINGS_LOADER_H_INCLUDED

#include "filesystem/be_file.h"
#include "common/be_event_system.h"
#include <string>
#include <map>

class TiXmlElement;

struct AxisRange
{
	public:
	AxisRange(float min, float max) : m_min(min), m_max(max)
	{
	}
	
	void setMin( float min ) { m_min = min; }
	void setMax( float max ) { m_max = max; }

	float getMin() const { return m_min; }
	float getMax() const { return m_max; }

	private:
	float		m_min;
	float		m_max;
};

class BeSettingsLoader
{
	public:
		bool load(BeEventSystem* eventsystem, const BeFile* befile);
		std::map<std::string, boost::shared_ptr<AxisRange> > axisrangemap;
	private:
		void parseJoystick(BeEventSystem& eventsystem, const TiXmlElement& rootElement);

};

#endif
