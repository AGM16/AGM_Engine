#pragma once
#include "Module.h"
#include "Globals.h"
#include "Primitive.h"
#include <list>


class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();



	

public:
	list<Primitive_Cylinder> spheres;
	list<PhysBody3D*> bodies;
	bool show_test_window;

};
