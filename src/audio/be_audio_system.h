#ifndef BE_AUDIO_SYSTEM_H_INCLUDED
#define BE_AUDIO_SYSTEM_H_INCLUDED

#ifdef WIN32
	#include "FMOD/fmod_errors.h"
	//#include "FMOD/fmod.hpp"
	#include "FMOD/fmod_event.hpp"
#else
	#ifdef ENABLE_FMOD
		#include <fmodex/fmod_errors.h>
		#include <fmodex/fmod_event.hpp>
	#endif
#endif
#include "filesystem/be_filesystem.h"

#include <string>

class BeAudioBank;

class BeAudioSystem
{

	friend class BeAudioBank;

#ifdef ENABLE_FMOD
	void ERRCHECK(FMOD_RESULT result)
	{
		if (result != FMOD_OK)
		{
			printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
			exit(-1);
		}
	}
#endif
public:

	BeAudioSystem(const std::vector<std::string>& rootPaths) : m_initialised(true)
	{
#ifdef ENABLE_FMOD
		m_filesystem.reset(new BeFilesystem);
		for (unsigned i=0; i < rootPaths.size(); i++) { m_filesystem->getRootPaths().push(rootPaths[i]); }
#if 0
		FMOD_RESULT result = FMOD::System_Create(&m_system);
		if(result==FMOD_OK)
		{
			unsigned int version;
			result = m_system->getVersion(&version);
			if(result==FMOD_OK)
			{
				if (version < FMOD_VERSION)
				{
					printf("Error! You are using an old version of FMOD %08x. This program requires %08x\n", version, FMOD_VERSION);
					m_initialised=false;
				}
				else
				{
					printf("FMOD Version %08x\n", version);
				}
			}
			else
			{
				printf("Error! Cannot get FMOD version\n");
				m_initialised=false;
			}
		}
		else
		{
			printf("Error! Cannot create FMOD system\n");
			m_initialised=false;
		}
#endif
		FMOD_RESULT result;
		ERRCHECK(result = FMOD::EventSystem_Create(&m_eventSystem));
		ERRCHECK(result = m_eventSystem->init(64, FMOD_INIT_NORMAL, 0, FMOD_EVENT_INIT_NORMAL));
		//ERRCHECK(result = m_eventSystem->setMediaPath(""));
		ERRCHECK(result = m_eventSystem->load(m_filesystem->getPath("fmod/stuntcoureur.fev").c_str(), 0, 0));

		ERRCHECK(result = m_eventSystem->getGroup("stuntcoureur/StuntCoureur", FMOD_EVENT_DEFAULT, &m_eventGroup));
		ERRCHECK(result = m_eventGroup->getEvent("Car", FMOD_EVENT_DEFAULT, &m_event));

		//ERRCHECK(result = m_eventSystem->getCategory("master", &mastercategory));

		ERRCHECK(result = m_event->getParameter("load", &m_load));
		//ERRCHECK(result = load->getRange(&load_min, &load_max));
		//ERRCHECK(result = load->setValue(load_max));

		ERRCHECK(result = m_event->getParameterByIndex(0, &m_rpm));
		//ERRCHECK(result = rpm->getRange(&rpm_min, &rpm_max));
		ERRCHECK(result = m_event->start());
#endif
	}

	void update(int rpm,float load,float vol)
	{
#ifdef ENABLE_FMOD
		FMOD_RESULT result;
		if(m_event)
		{
			m_event->setVolume(vol);
		}
		if(m_rpm)
		{
			if(rpm<0)
			{
				rpm*=-1;
			}
			m_rpm->setValue(rpm+800);
		}
		if(m_load)
		{
			m_load->setValue(load);
		}
		if(m_eventSystem)
		{
			ERRCHECK(result = m_eventSystem->update());
			//FMOD_EVENT_STATE state;
			//ERRCHECK(result = m_event->getState(&state));
			//printf("Event is %s       \r", (state & FMOD_EVENT_STATE_PLAYING) ? "playing" : "stopped");
		}
#endif
	}

	~BeAudioSystem()
	{
#ifdef ENABLE_FMOD
		if(m_event)
		{
			m_event->stop();
		}
#if 0
		if(m_system)
		{
			m_system->release();
		}
#endif
		if(m_eventGroup)
		{
			m_eventGroup->freeEventData();
		}
		if(m_eventSystem)
		{
			m_eventSystem->release();
		}
#endif
	}

protected:
	boost::shared_ptr<BeFilesystem> m_filesystem;
	bool m_initialised;
#ifdef ENABLE_FMOD
#if 0
	FMOD::System*	m_system;
#endif
	FMOD::EventSystem*	m_eventSystem;
	FMOD::EventGroup     *m_eventGroup;
	//FMOD::EventCategory  *mastercategory;
	FMOD::Event*			m_event;
	FMOD::EventParameter *m_rpm;
	FMOD::EventParameter *m_load;
#endif
};

#if 0
class BeAudioBank
{
public:
	BeAudioBank(BeAudioSystem& system, const std::string path)
	{
		const FMOD_RESULT result = system.m_system->createSound(path.c_str(), FMOD_DEFAULT, 0, &m_sound);
		if(result==FMOD_OK)
		{
			printf("Audio bank loaded %s\n", path.c_str());
			int count=0;
			//result = 
			m_sound->getNumSubSounds(&count);
			printf("%d sounds\n", count);
			for (int i = 0; i < count; ++i)
			{
				FMOD::Sound *sound;
				char name[256];

				//result = 
				m_sound->getSubSound(count, &sound);

				//result = 
				sound->getName(name, 256);

				printf("%d. %s\n", i, name);
			}
		}
		else
		{
			printf("Error loading audio bank %s\n", path.c_str());
		}
		
	}
	/*FMOD::Sound* findSound(const std::string &name)
	{

	}*/
	~BeAudioBank()
	{
		if(m_sound)
		{
			m_sound->release();
		}
	}
private:
	FMOD::Sound* m_sound;
};
#endif

#endif
