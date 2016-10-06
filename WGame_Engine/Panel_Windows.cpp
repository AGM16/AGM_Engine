#include "Panel_Windows.h"


Panel_Windows::Panel_Windows():active_window(false)
{

}

Panel_Windows::~Panel_Windows()
{

}

void Panel_Windows::Visualizate_Window(bool on)
{
	active_window = on;
}

bool Panel_Windows::Is_Active()
{
	return active_window;
}