#ifndef TEXTINPUT_H_INCLUDED
#define TEXTINPUT_H_INCLUDED

#include "panel.h"
#include "text.h"

struct SDL_KeyboardEvent;

class BeWidgetTextInput : public Panel
{
	public:
		BeWidgetTextInput();
		virtual ~BeWidgetTextInput() {};

		virtual void acceptKey(const SDL_KeyboardEvent& event);
		virtual void click(const int& button);
		virtual void release(const int& button);
		virtual void draw();
		virtual void set(const std::string& newstring);
		virtual void genSubmitEvent(const BeCommand& cmds);
		void emtpyAfterSubmit() { m_emtpyAfterSubmit = true; }
		void emtpyOnClick() { m_emtpyOnClick = true; }
	protected:
	private:
        char getAsciiValue( const SDL_KeyboardEvent &key );
		BeWidgetTextPtr m_text;
		Vector2i m_textposition;
		std::string m_stringbackup;
// 		long unsigned int submitEvent;
		Event* submitEvent;

		bool m_emtpyAfterSubmit;
		bool m_emtpyOnClick;
        //bool m_history;

        size_t              m_historyIndex;
        std::vector<std::string> m_historyStrings;

};

#endif
