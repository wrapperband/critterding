#ifndef CANVAS_H_INCLUDED
#define CANVAS_H_INCLUDED

#include "container.h"

struct SDL_KeyboardEvent;

class Canvas : public BeWidgetContainer
{
	public:
		Canvas(BeFilesystem& filesystem);
		virtual ~Canvas();

		void setDefaultZAxis();
		
		void moveMouse(unsigned int x, unsigned int y);
		bool mouseFocus;

		bool buttonPress(const unsigned int& button);
		void buttonRelease(const unsigned int& button);

		bool keyPress(const SDL_KeyboardEvent& event);
		bool keyRelease(const SDL_KeyboardEvent& event);

		void swapChild(const std::string& child);
		void raisePanel(BeWidget* w);
		
	private:
		virtual void drawChildren();
		bool mouseOverChild( BeWidgetPtr* fWidget, int x, int y );

		unsigned int m_x;
		unsigned int m_y;

		BeWidgetPtr m_pickedwidget;
		BeWidgetPtr m_focussedwidget;

		std::vector<std::string> m_sortedindices;
};

#endif
