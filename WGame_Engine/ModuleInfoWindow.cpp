#include "ModuleInfoWindow.h"

ModuleInfoWindow::ModuleInfoWindow() : active(false)
{

}
ModuleInfoWindow::~ModuleInfoWindow()
{
	
}

bool ModuleInfoWindow::Set_Active(bool on)
{
	return active = on;
}

bool ModuleInfoWindow::Is_Active()
{
	return active;

}