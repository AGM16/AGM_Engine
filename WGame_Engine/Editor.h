#ifndef _EDITOR_H_
#define _EDITOR_H_


#include "Module.h"
#include "Globals.h"
#include "Panel_Windows.h"


class Editor : public Module
{
public:
	Editor(bool start_enabled = true);
	~Editor();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

private:

	//ImGUi Variable
	bool show_test_window;
	Primitive_Plane plane;

	//Panels
	Panel_Windows panels;


	

};

#endif