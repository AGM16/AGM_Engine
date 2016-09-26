#ifndef _MODULEINFOWINDOW_
#define _MODULEINFOWINDOW_


#include "Imgui\imgui.h"

class ModuleInfoWindow
{
 public:
	ModuleInfoWindow();
	~ModuleInfoWindow();

	bool Set_Active(bool on);
	bool Is_Active();

private:

	bool active;


};

#endif