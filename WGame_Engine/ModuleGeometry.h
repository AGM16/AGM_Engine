#ifndef _MODULEGEOMETRY_H
#define _MODULEGEOMETRY_H
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

	//Uvs Variables
	uint uvs_index_texture_coords = 0;
	uint id_uvs_texture_coords = 0;
	uint num_uvs_texture_coords = 0;
	float2* uvs_texture_coords = nullptr;

	//Normals Variables
	uint id_normals = 0;
	uint num_normals = 0;
	float* normals = nullptr;
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

#endif
