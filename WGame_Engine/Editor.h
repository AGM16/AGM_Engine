#pragma once
#include "Module.h"
#include "Globals.h"
#include "Random.h"


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

	//Bounding Box
	void Create_AABB_Box();
	void Rotate_Bounding_Box(float x, float y, float z);
	void Sphere_Bounding_Box();


private:

	//ImGUi Variable
	bool show_test_window;

	//MathGeolib Variables
	list<Primitive_Sphere> s_points;

	Primitive_Plane plane;
	Primitive_Cube aabb_bounding_box;
	Primitive_Sphere bounding_sphere;

	bool render_aabb;
	bool render_bounding_sphere;



};