#ifndef _MODULEGEOMETRY_H
#define _MODULEGEOMETRY_H

#include "Globals.h"
#include "Module.h"

#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"
#include "Assimp\include\cfileio.h"

class GameObject;



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


	//Local Transform
	float3 translation;
	float3 scaling;
	Quat rotation;

	//Hierarchy
	int num_children = 0;
	const char* parent = NULL;
	const char* name_node;

	//Texture
	uint num_image_textures = 0;
	uint id_image_texture;
	string name_texture;
	string dir_texture;

};


class ModuleGeometry : public Module
{
   public:
	   ModuleGeometry(Application* app, bool start_enabled = true);
	   ~ModuleGeometry();

	   bool Init();
	   update_status Update(float dt);
	   bool CleanUp();

	   bool Load_Geometry(const char* path);

	   void Load_Nodes_For_Hierarchy(aiNode* node_child, const aiScene* scene, GameObject* parent, const char* path);
	   



};

#endif
