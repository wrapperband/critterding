#include "logbuffermessage.h"
#include "textprinter.h"

Logbuffermessage::Logbuffermessage( boost::shared_ptr<Logbuffer> logBuffer ) : Panel(false), m_logBuffer(logBuffer)
{
	isMovable = true;
	isAlwaysOnTop = true;
	m_active = true;
	
	vpadding = 10;
	hpadding = 10;

	m_localposition.set(2, 24);
}

void Logbuffermessage::draw()
{
	if ( m_active && !m_logBuffer->getMessages().empty() )
	{
// 		m_active = true;
		
		unsigned int height = 5;

		m_dimensions.set(m_logBuffer->getLongestLength() + ( hpadding*2 ), (15 * (m_logBuffer->getMessages().size()-1)) + height + ( vpadding*2 ));
		
	// draw background box and border
		drawBackground();
		drawBorders();

	// render text
// 		glColor3f(1.0f, 1.0f, 1.0f);
		for ( unsigned int i = 0; i < m_logBuffer->getMessages().size(); i++ )
			Textprinter::Instance()->print(Vector2i(m_absposition.m_x + hpadding, m_absposition.m_y + height + (i*15) + vpadding), m_logBuffer->getMessages()[i].getMsg().c_str());
	}
/*	else
		m_active = false;*/
}


void Logbuffer::add(const std::string& msgstring)
{
	m_messages.push_back( Msg( msgstring, Textprinter::Instance()->getWidth( msgstring ) ) );
	
	if ( m_messages.size() > m_maxMessages )
		m_messages.erase(m_messages.begin());

	getLongest();
}
void Logbuffer::deleteExpiredMsg(float timeDelta)
{
	for ( unsigned int i=0; i < m_messages.size(); ++i )
		m_messages[i].age(timeDelta);

	if ( !m_messages.empty() )
		if ( m_maxAge > 0.0f && m_messages[0].getAge() > m_maxAge )
			m_messages.erase(m_messages.begin()+0);
}

