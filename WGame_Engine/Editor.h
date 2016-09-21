#pragma once

#include "Module.h"
#include "Globals.h"


class Editor : public Module
{
public:
	Editor(Application* app, bool start_enabled = true);
	~Editor();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	//Primitive Function
	void Create_Points(int amount);
	void Render();

	//AABB
	void Create_AABB_Box();

	
private:

	Primitive_Plane plane;
	list<Primitive_Sphere> s_points;
	Primitive_Cube aabb_bounding_box;
	bool render_aabb;
	int x, y, z;
	Random ran;

};
