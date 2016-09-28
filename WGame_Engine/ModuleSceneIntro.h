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


	void Cube_Direct_OPENGL();
	void Cube_Vertex_Arrays_OPENGL();
	void Cube_Indice_OPENGL();
	void Draw_Array_Vertex(vector<float3> &vertices, uint id);
	void Draw_Indices_Vertex(vector<uint> &vertices, uint id);

public:
	list<Primitive_Cylinder> spheres;
	list<PhysBody3D*> bodies;
	bool show_test_window;
	
	//Vertex Array
	vector<float3> vertexs;
	uint my_id = 0;

	//Vectex Indices
	vector<uint> indices;
	uint my_indices = 0;

};
