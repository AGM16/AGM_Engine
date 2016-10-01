#pragma once
#include "Globals.h"
#include "Module.h"




struct Mesh
{
   //Faces Variables
	uint id_indices = 0;
	uint num_indices = 0;
	uint* indices = nullptr;

	//Vertices Variables
	uint id_vertices = 0;
	uint num_vertices = 0;
	float* vertices = nullptr;
};


class ModuleGeometry : public Module
{
   public:
	   ModuleGeometry(Application* app, bool start_enabled = true);
	   ~ModuleGeometry();

	   bool Init();
	   update_status Update(float dt);
	   bool CleanUp();

	   vector<Mesh> Load_Geometry(const char* path);



};
