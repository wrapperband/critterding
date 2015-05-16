#include "textinput.h"

#include <SDL/SDL_events.h>
#include <SDL/SDL_keysym.h>
#include "textprinter.h"

BeWidgetTextInput::BeWidgetTextInput() : Panel(false),
m_textposition( 5, 12 ),
submitEvent(0),
m_emtpyAfterSubmit(false),
m_emtpyOnClick(false)
{
	m_active = true;
	m_dimensions.set(60, 40);

	m_text = BeWidgetTextPtr(new BeWidgetText());
		m_text->translate(m_textposition);
	
	addWidget("btext", m_text);
// 	m_text = static_cast<Text*>(addWidgetText( "btext", m_textposition.m_x, m_textposition.m_y, "" ));
}

void BeWidgetTextInput::genSubmitEvent(const BeCommand& cmds )
{
	submitEvent = new Event( cmds, EVENT_NOREPEAT, 0);
// 	submitEvent = BeEventSystem::Instance()->registerEvent( DOWN, name, boost::shared_ptr<Event>(new Event( cmds, repeat, delay) ) );
}

void BeWidgetTextInput::set(const std::string& newstring)
{
	m_text->set(newstring);
}


char BeWidgetTextInput::getAsciiValue( const SDL_KeyboardEvent &key )
{    
    // magic numbers courtesy of SDL docs :)    
    const int INTERNATIONAL_MASK = 0xFF80, UNICODE_MASK = 0x7F;    
    int uni = key.keysym.unicode;    
    if( uni == 0 ) // not translatable key (like up or down arrows)    
    {        // probably not useful as string input        
        // we could optionally use this to get some value        
        // for it: 
        //SDL_GetKeyName( key );        
        return 0;    
    }    
    if( ( uni & INTERNATIONAL_MASK ) == 0 )    
    {        
        if( SDL_GetModState() & KMOD_SHIFT )        
        {            
            return (char)(toupper(uni & UNICODE_MASK));        
        }        
        else       
        {            
            return (char)(uni & UNICODE_MASK);        
        }    
    }    
    else // we have a funky international character. one we can't read :(    
    {        
        // we could do nothing, or we can just show some sign of input, like so:        
        return '?';    
    }
}

void BeWidgetTextInput::acceptKey(const SDL_KeyboardEvent& event)
{
    char str[2]={getAsciiValue(event),0};
    const std::string key=SDL_GetKeyName( event.keysym.sym );
	if ( key == "escape" )
	{
		isKeyboardGrabbing = false;
		m_text->set(m_stringbackup);
        SDL_EnableKeyRepeat(0,0);
	}
    else if ( key == "up" )
    {
        //Sanity check
        if(m_historyIndex>=m_historyStrings.size())
        {
            m_historyStrings.push_back(m_text->getString());
            m_historyIndex=m_historyStrings.size()-1;
        }
        if(m_historyIndex>0)
        {
            m_historyIndex--;
            m_text->set(m_historyStrings[m_historyIndex]);
        }
    }
    else if ( key == "down" )
    {
        if(m_historyIndex<m_historyStrings.size()-1)
        {
            m_historyIndex++;
            m_text->set(m_historyStrings[m_historyIndex]);
        }
    }
	else if ( key == "enter" || key == "return" )
	{
		if ( submitEvent )
		{
			submitEvent->getCommand()->m_argString = m_text->getString();
			submitEvent->activate();
			submitEvent->process( BeEventSystem::Instance()->getCommandSystem(), 0 );

            m_historyStrings.push_back(m_text->getString());
            m_historyIndex=m_historyStrings.size();

			if ( m_emtpyAfterSubmit )
				m_text->set("");
        }
        SDL_EnableKeyRepeat(0,0);
		isKeyboardGrabbing = false;
	}
	else if ( key == "backspace" )
	{
		m_text->set( m_text->getString().substr( 0, m_text->getString().size()-1 ) );
	}
    else if(str[0]!=0)  //TODO: Allow ; character!
    {
        if(str[0]!=';')
        {
            m_text->set( m_text->getString()+std::string(str) );
        }
    }
	//else if ( key == "space" )
	//{
	//	m_text->set( m_text->getString()+" " );
	//}
	//else if ( key == ";" )
	//{
	//}
	//else if ( key == "left shift" || key == "right shift" )
	//{
	//}
	//else //if ( key.size() == 1 )
	//{
        
	//	m_text->set( m_text->getString()+std::string(str) );
	//}
}

void BeWidgetTextInput::click(const int& button)
{
	// backup string when activating grabbing
	if ( !isKeyboardGrabbing )
		m_stringbackup = m_text->getString();

	if ( m_emtpyOnClick )
		m_text->set("");
	
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,SDL_DEFAULT_REPEAT_INTERVAL);
	isKeyboardGrabbing = true;
}

void BeWidgetTextInput::release(const int& button)
{

}

void BeWidgetTextInput::draw()
{
	if (m_active)
	{
		drawBackground();
		drawBorders();
		drawChildren();

		// render cursor
		if ( isKeyboardGrabbing )
		{
			Vector2i position = m_absposition + m_textposition;
			position.m_x += m_text->getDimensions().m_x;
			textprinter->print( position, "|" );
		}
	}
}
