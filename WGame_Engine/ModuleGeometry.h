#ifndef _MODULEGEOMETRY_H
#define _MODULEGEOMETRY_H

#include "Globals.h"
#include "Module.h"
#include "p2Defs.h"

#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"
#include "Assimp\include\cfileio.h"

class GameObject;



struct Mesh
{
	void Clean_Up()
	{
		RELEASE(vertices);
		RELEASE(indices);
		RELEASE(uvs_texture_coords);
		RELEASE(normals);
	}

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
	float3 translation = float3::zero;
	float3 scaling = float3::zero;
	Quat rotation = Quat::identity;

	//Hierarchy
	int num_children = 0;
	string parent;
	string name_node;

	//Texture
	uint num_image_textures = 0;
	uint id_image_texture = 0;
	string name_texture = "";
	string dir_texture = "";



};


class ModuleGeometry : public Module
{
   public:
	   ModuleGeometry(Application* app, bool start_enabled = true);
	   ~ModuleGeometry();

	   bool Init();
	   update_status Update(float dt);
	   bool CleanUp();

	   bool Load_Geometry(const char* path, bool drop);

	   void Load_Nodes_For_Hierarchy(aiNode* node_child, const aiScene* scene, GameObject* parent, const char* path, const char* dir_scene);
	   
	   std::string Delete_$Assimp$_word(std::string str);

	   void Hierarchy_And_Local_Transform(Mesh* m, aiNode* node);


	   //Functions Own Format
	   //IMPORT
	   bool Import_Mesh(const aiMesh* new_mesh, std::string& out_p, const char* name_mesh, const char* dir_scene);
	   bool Import_Material(const char* name_file, const char* path, std::string& out_p, const char* dir_scene);
       void Import_And_Load_Texture(Mesh* m, aiMesh* new_mesh, const aiScene* scene, const char* dir_scene);
	   //Save
	   bool Save(const Mesh& mesh, std::string& out_p, const char* name_mesh, const char* dir_scene);
	   //LOAD
	   bool Load_Mesh(Mesh* mesh, const char* file_path);

 private:

	 bool is_dir_scene_exist = false;

};

#endif
