#ifndef LOGBUFFERMESSAGE_H_INCLUDED
#define LOGBUFFERMESSAGE_H_INCLUDED

// #include "src/utils/logbuffer.h"
#include "src/gui/panel.h"

class Logbuffer
{
	public:
		Logbuffer() : m_longestLength(0), m_maxMessages(7), m_maxAge(10.0f) {};
		~Logbuffer() {};

		void add(const std::string& msgstring);
		void deleteExpiredMsg(float timeDelta);
	private:
		struct Msg
		{
			public:
				Msg( const std::string& str, unsigned int length ) : m_str(str), m_age(0), m_length(length) {}
				void age( float time ) { m_age += time; }
				float getAge() const { return m_age; }
				unsigned int getLength() const { return m_length; }
				const std::string& getMsg() const { return m_str; }
			private:
				std::string m_str;
				float m_age;
				unsigned int m_length;
		};
	public:
		const std::vector<Msg>& getMessages() const { return m_messages; }
		unsigned int getLongestLength() const { return m_longestLength; }
	private:
		void getLongest()
		{
			m_longestLength = 0;
			for ( unsigned int i=0; i < m_messages.size(); i++ )
				if ( m_messages[i].getLength() > m_longestLength )
					m_longestLength = m_messages[i].getLength();
		}

		unsigned int m_longestLength;
		unsigned int m_maxMessages;
		float m_maxAge;
		std::vector<Msg> m_messages;
};

class Logbuffermessage : public Panel
{
	public:
		Logbuffermessage( boost::shared_ptr<Logbuffer> logBuffer );
		virtual ~Logbuffermessage() {};

		void		draw();
	private:

		int		vpadding;
		int		hpadding;
		boost::shared_ptr<Logbuffer>	m_logBuffer;
};

#endif
