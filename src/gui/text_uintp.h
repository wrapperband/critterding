#ifndef TEXT_UINTP_H_INCLUDED
#define TEXT_UINTP_H_INCLUDED

#include "widget.h"

class BeWidgetTextCVARuint : public BeWidget
{
	public:
		BeWidgetTextCVARuint(const int* cvarpointer);
		virtual ~BeWidgetTextCVARuint() {};
		void		draw();
	protected:
	private:
		const int*	m_cvarpointer;
};

#endif
