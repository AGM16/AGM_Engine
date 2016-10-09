#pragma once
#include "Module.h"
#include "Globals.h"
#include "Primitive.h"
#include "ModuleGeometry.h"
#include <list>
#include <vector>

class GameObject;


class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Init();
	bool Start();
	update_status Update(float dt);
	bool CleanUp();


	void Cube_Direct_OPENGL();
	void Cube_Vertex_Arrays_OPENGL();
	void Cube_Indice_OPENGL();
	void Draw_Array_Vertex(vector<float3> &vertices, uint id);
	void Draw_Indices_UVS(vector<uint> vec_indices, uint indices, uint vertex_id, uint uvs_id, uint texture);

public:
	list<Primitive_Cylinder> spheres;
	list<PhysBody3D*> bodies;
	
	//Vertex Array
	vector<float3> vertexs;
	uint my_id = 0;

	//Vectex Indices
	vector<uint> indices;
	uint my_indices = 0;

	//Vertex Uvs
	vector<float2> uvs;
	uint my_uvs = 0;

	//FBX
	vector<Mesh> geometry_object;

	//Texture Check Image
	uint image_name;

	//Texture Elena
	uint  lena_image;

	GameObject* go;



};
