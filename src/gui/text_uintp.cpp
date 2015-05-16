#include "text_uintp.h"
#include "textprinter.h"

BeWidgetTextCVARuint::BeWidgetTextCVARuint(const int* cvarpointer) : m_cvarpointer(cvarpointer)
{
}

void BeWidgetTextCVARuint::draw()
{
// 	glColor3f(1.0f, 1.0f, 1.0f);
	textprinter->print( m_absposition, m_cvarpointer );
}
