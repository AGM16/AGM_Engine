#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"
#include "Random.h"

#define MAX_SNAKE 30

struct PhysBody3D;
struct PhysMotor3D;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);
	void Create_Bodys_World(float size_x, float size_y, float size_z, float pos_x, float pos_y, float pos_z, Primitive_Cube primitive_cube[], PhysBody3D* PB_3D[], int PB_Num, float mass, bool sensor, bool rotation, float r_x, float r_y, float r_z, int angle, bool num_primitive, int num_prim);
	void Create_Circuit_World(float size_x, float size_y, float size_z, float pos_x, float pos_y, float pos_z, Primitive_Cube primitive_cube[], PhysBody3D* PB_3D[], int init_Num, int final_num, float parameter, float mass, bool distance, bool height, bool axis_x, bool positive, int value);
	void Create_Curves(float size_x, float size_y, float size_z, float pos_x_a, float pos_y_a, float pos_z_a, Primitive_Cube primitive_cube[], PhysBody3D* PB_3D[], int init_Num, int final_num, float parameter_1, float parameter_2, float parameter_3, float parameter_4, float parameter_5, float value_parameter_1_z, float value_parameter_2_z, float value_parameter_3_z, float value_parameter_4_z, float value_parameter_5_z,
		float value_parameter_1_x, float value_parameter_2_x, float value_parameter_3_x, float value_parameter_4_x, float value_parameter_5_x);
	void Create_Cylinder_Bodys_World(float radius_a, float height_a, float pos_x, float pos_y, float pos_z, Primitive_Cylinder primitive_cube[], PhysBody3D* PB_3D[], int PB_Num, float mass);
	void Create_Sphere_Bodys_World(float radius_a, float pos_x, float pos_y, float pos_z, Primitive_Sphere primitive_cube[], PhysBody3D* PB_3D[], int PB_Num, float mass, bool num_primitive, int num_prim);

public:
	Random ran;
	bool show_test_window;
	int x, y, z;
	
};
