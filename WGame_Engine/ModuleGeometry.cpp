#include"Application.h"
#include "ModuleGeometry.h"
#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"
#include "Assimp\include\cfileio.h"
#include "p2Defs.h"
#include "Glew\include\glew.h"
#include <gl/GL.h>

#pragma comment (lib, "Devil/libx86/DevIL.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "Devil/libx86/ILU.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Devil/libx86/ILUT.lib") 

#include "Devil\include\il.h"
#include "Devil\include\ilu.h"
#include "Devil\include\ilut.h"

#pragma comment(lib, "Assimp/libx86/assimp.lib")

using namespace std;


ModuleGeometry::ModuleGeometry(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}
ModuleGeometry::~ModuleGeometry()
{

}

bool ModuleGeometry::Init()
{
	//Assimps: Turn on the debugger
	aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	LOG("Assimp debugger has turned on");

	return true;
}
update_status ModuleGeometry::Update(float dt)
{

	return UPDATE_CONTINUE;
}
bool ModuleGeometry::CleanUp()
{
	aiDetachAllLogStreams();
	return true;
}

bool ModuleGeometry::Load_Geometry(const char* path, bool drop)
{
	bool ret = false;
	const aiScene* scene = nullptr;
	string dir_scene; 
	char* buffer;
	if (drop == false)
	{
		uint size_file = App->filesystem->Load(path, &buffer);

		if (size_file != 0)
		{
			
			//Create the scene folder
			 dir_scene = "/Library/";
			 dir_scene.append(App->filesystem->Get_FileName_From_Path(path));
			 size_t size = dir_scene.find(".fbx");
			 if (size != string::npos)
			 {
				 dir_scene = dir_scene.substr(0, size);
			 }
			  
			 //Chek is the scene folder already exists
			 if (App->filesystem->Exists(dir_scene.c_str()) == false)
			 {

				 //Create the scene folder
				 App->filesystem->Create_Dir(dir_scene.c_str());
				 
				 //Create the mesh folder of this scene
				 string dir_meshes = dir_scene;
				 dir_meshes.append("/Meshes");
				 App->filesystem->Create_Dir(dir_meshes.c_str());

				 //Create the texture folder of this scene
				 string dir_textures = dir_scene;
				 dir_textures.append("/Textures");
				 App->filesystem->Create_Dir(dir_textures.c_str());
			 }
			 else
			 {
				 //We will import the scene that already exists
				 LOG("The dir scene %s already exists", dir_scene.c_str()); 
				 is_dir_scene_exist = true;
			 }


			//Import Geometry File
			scene = aiImportFileFromMemory(buffer, size_file, aiProcessPreset_TargetRealtime_MaxQuality, nullptr);
		}
	}
	else
	{       //For doctor memory when I have to drop a FBX in the scene to find memory leaks
			scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	}

	if(scene != nullptr)
	{
		//Check the state of the import
		if (scene->HasMeshes() == true)
		{
			//RootNode of the scene
			aiNode* parent = scene->mRootNode;//At the begining this will be the RootNode 

			for( int i = 0; i < parent->mNumChildren; i++)
			{
				//Visit each child to obtain the mesh information using Load
				Load_Nodes_For_Hierarchy(parent->mChildren[i], scene, nullptr, path, dir_scene.c_str());
			}
		
			ret = true;
			is_dir_scene_exist = false;

			aiReleaseImport(scene);		
		}
		else
		{
			LOG("Error loading scene %s", path);
			ret = false;
		}
		
		if (drop == false)
		{
			delete[] buffer;
		}
	}
	else
	{
		LOG("Error loading : The file is empty %s", path);
		ret = false;
	}

	

	return ret;
	

}

void ModuleGeometry::Load_Nodes_For_Hierarchy(aiNode* node_child, const aiScene* scene, GameObject* parent, const char* path, const char* dir_scene)
{ 
	
    GameObject* game_obj = nullptr;

	if (node_child->mNumMeshes > 0)
	{
		for (int i = 0; i < node_child->mNumMeshes; i++)
		{
			aiMesh* new_mesh = scene->mMeshes[node_child->mMeshes[i]];
			Mesh* m = new Mesh();

			
            //Check Hierarchy and local transform of every mesh
			if (new_mesh->HasPositions() == true)
			{	
				Hierarchy_And_Local_Transform(m, node_child);	
			}

			//Import Mesh
			string path_file;
			string path_file_2;

			if (is_dir_scene_exist == false)
			{
				Import_Mesh(new_mesh, path_file, m->name_node.c_str(), dir_scene);
				path_file_2 = path_file;
			}
			else
			{
				//if there is more than one mesh it means that there is a copy of this name with "_m_%d"
				path_file_2 = dir_scene;
				path_file_2.append("/Meshes/");
				path_file_2.append(m->name_node.c_str());
				if (i > 0)
				{
					char copy_name[10];
					sprintf_s(copy_name, 10, "_m_%d", i);
					path_file_2.append(copy_name);
				}

				path_file_2.append(".wge");
			}

			//Load Mesh
			Load_Mesh(m, path_file_2.c_str());
			

			//Texture
			Import_And_Load_Texture(m, new_mesh, scene, dir_scene);


			//Load Mesh buffer to the VRAM
			//Vertices Buffer
			glGenBuffers(1, (GLuint*) &(m->id_vertices));//Generate the buffer
			glBindBuffer(GL_ARRAY_BUFFER, m->id_vertices);//Start using that buffer
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m->num_vertices * 3, m->vertices, GL_STATIC_DRAW);

			//Normals Buffer
			glGenBuffers(1, (GLuint*) &(m->id_normals));
			glBindBuffer(GL_ARRAY_BUFFER, m->id_normals);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m->num_normals * 3, m->normals, GL_STATIC_DRAW);

			//Texture_coords Buffer
			glGenBuffers(1, (GLuint*) &(m->id_uvs_texture_coords));
			glBindBuffer(GL_ARRAY_BUFFER, m->id_uvs_texture_coords);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m->num_uvs_texture_coords * 2, m->uvs_texture_coords, GL_STATIC_DRAW);

			//Indices Buffer
			glGenBuffers(1, (GLuint*) &(m->id_indices));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->id_indices);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) *  m->num_indices, m->indices, GL_STATIC_DRAW);


			
			
			game_obj = App->go_manager->Create_Game_Object(m, parent);

			
		}
	}
	else
	{
		Mesh* m = new Mesh();
		
		Hierarchy_And_Local_Transform(m, node_child);

		game_obj = App->go_manager->Create_Game_Object(m, parent);
	}


	if (node_child->mNumChildren > 0)
	{
		for (int i = 0; i < node_child->mNumChildren; i++)
		{
			Load_Nodes_For_Hierarchy(node_child->mChildren[i], scene, game_obj, path, dir_scene);
		}

	}

}


//-------------------------------LOAD_FUNCTIONS-------------------------------------

void ModuleGeometry::Hierarchy_And_Local_Transform(Mesh* m, aiNode* node)
{

	m->parent = node->mParent->mName.C_Str();
	m->num_children = node->mNumChildren;
	m->name_node = node->mName.C_Str();

	//Delete the assimp from the strings
	m->name_node = Delete_$Assimp$_word(m->name_node);
	m->parent = Delete_$Assimp$_word(m->parent);

	aiVector3D translation;
	aiVector3D scaling;
	aiQuaternion rotation;
	node->mTransformation.Decompose(scaling, rotation, translation);

	//Position
	m->translation.x = translation.x;
	m->translation.y = translation.y;
	m->translation.z = translation.z;

	//Scale
	m->scaling.x = scaling.x;
	m->scaling.y = scaling.y;
	m->scaling.z = scaling.z;

	//Rotation
	m->rotation.x = rotation.x;
	m->rotation.y = rotation.y;
	m->rotation.z = rotation.z;
	m->rotation.w = rotation.w;

}

void ModuleGeometry::Import_And_Load_Texture(Mesh* m, aiMesh* new_mesh, const aiScene* scene, const char* dir_scene)
{
	aiMaterial* material = scene->mMaterials[new_mesh->mMaterialIndex];

	if (material)
	{
		//Get the number of textures
		m->num_image_textures = material->GetTextureCount(aiTextureType_DIFFUSE);
		if (m->num_image_textures > 0)
		{
			aiString path_;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &path_);

			//Add the correct path of the texture
			m->dir_texture.assign("Assets/Textures/");
			m->name_texture.assign(App->filesystem->Get_FileName_From_Path(path_.data));
			m->dir_texture.append(m->name_texture.c_str());

			if (is_dir_scene_exist == false)
			{
				//Import Texture
				string dir_texture;
				Import_Material(m->name_texture.c_str(), m->dir_texture.c_str(), dir_texture, dir_scene);
			}


			//Load Texture
			if (path_.length > 0)
			{
				ILuint id;
				ilGenImages(1, &id);
				ilBindImage(id);
				ilLoadImage(m->dir_texture.c_str());

				m->id_image_texture = ilutGLBindTexImage();
			}
		}
	}

}


string ModuleGeometry::Delete_$Assimp$_word(string str)
{
	string name_node = str.c_str();
	size_t size = name_node.find("$AssimpFbx$");
	if (size != string::npos)
	{
		name_node = name_node.substr(0, size);
		
	}
	else
	{
		LOG("There is no $Assimp$ in this string");
	}
	
	return name_node;
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

bool ModuleGeometry::Import_Mesh(const aiMesh* new_mesh, string& out_p, const char* name_mesh, const char* dir_scene)
{
	Mesh m;

	//Vertices
	m.num_vertices = new_mesh->mNumVertices;
	m.vertices = new float[m.num_vertices * 3];
	memcpy(m.vertices, new_mesh->mVertices, sizeof(float) * m.num_vertices * 3);
	LOG("New mesh with %d vertices", m.num_vertices);

	//Check if our mesh have normals
	if (new_mesh->HasNormals())
	{

		m.num_normals = new_mesh->mNumVertices;
		m.normals = new float[m.num_normals * 3];
		memcpy(m.normals, new_mesh->mNormals, sizeof(float) * m.num_normals * 3);
		LOG("New mesh with %d normals", m.num_normals);
	}

	//Check if the mesh have uvs coordenates
	if (new_mesh->HasTextureCoords(m.uvs_index_texture_coords))
	{
		m.num_uvs_texture_coords = new_mesh->mNumVertices;
		m.uvs_texture_coords = new float2[m.num_uvs_texture_coords];

		for (int i = 0; i < m.num_uvs_texture_coords; i++)
		{
			//Assign uv to the uvs_array<float2>
			m.uvs_texture_coords[i].x = new_mesh->mTextureCoords[m.uvs_index_texture_coords][i].x;
			m.uvs_texture_coords[i].y = new_mesh->mTextureCoords[m.uvs_index_texture_coords][i].y;
		}

		LOG("New mesh with %d uvs_texture_coords", m.num_uvs_texture_coords);
	}

	//Faces
	if (new_mesh->HasFaces())
	{
		//We assume that each face is a triangle
		m.num_indices = new_mesh->mNumFaces * 3;
		m.indices = new uint[m.num_indices];
		for (uint i = 0; i < new_mesh->mNumFaces; ++i)
		{
			//Check the number of indices of each face
			if (new_mesh->mFaces[i].mNumIndices != 3)
			{
				LOG("WARNING, geometry face with != 3indices!");
			}
			else
				memcpy(&m.indices[i * 3], new_mesh->mFaces[i].mIndices, 3 * sizeof(uint));

		}
	}

	return Save(m, out_p, name_mesh, dir_scene);

}

bool ModuleGeometry::Save(const Mesh& mesh, string& out_p, const char* name_mesh, const char* dir_scene)
{
	uint amount_infor_mesh[4] = { mesh.num_indices,  mesh.num_vertices,  (mesh.normals) ? mesh.num_vertices : 0,  mesh.num_uvs_texture_coords};

	//Size 
	uint size = sizeof(amount_infor_mesh);
	size += sizeof(uint) * amount_infor_mesh[0] + sizeof(float) * amount_infor_mesh[1] * 3;
	if (amount_infor_mesh[2] != 0)
		size += sizeof(float) * amount_infor_mesh[2] * 3;

	if (amount_infor_mesh[3] != 0)
		size += sizeof(float) * amount_infor_mesh[3] * 2;

	
	//Allocate with size and fill it
	char* data = new char[size];
	char* cursor = data;

	//Store information from mesh to cursor variable
	//amount_infor_mesh
	uint bytes = sizeof(amount_infor_mesh);
	memcpy(cursor, amount_infor_mesh, bytes);
	cursor += bytes;

	//STORE: Indices
	bytes = sizeof(uint) * amount_infor_mesh[0];
	memcpy(cursor, mesh.indices, bytes);
	cursor += bytes;

	//STORE: Vertices
	bytes = sizeof(float) * amount_infor_mesh[1] * 3;
	memcpy(cursor, mesh.vertices, bytes);
	cursor += bytes;

	//STORE: Normals
	if (mesh.normals != nullptr)
	{
		memcpy(cursor, mesh.normals, bytes);
		cursor += bytes;
	}

	if (mesh.num_uvs_texture_coords != 0)
	{
		//Store: Texture Coords(uvs)
		bytes = sizeof(float) * mesh.num_vertices * 2;
		memcpy(cursor, mesh.uvs_texture_coords, bytes);
	}


	string name_file = name_mesh;
	string scene_path = dir_scene;
	scene_path.append("/Meshes/");
	bool ret = App->filesystem->Save_Unique(name_file, data, size, scene_path.c_str(), "wge", out_p);

	delete[] data;
	data = nullptr;

	return ret;

}

bool ModuleGeometry::Load_Mesh(Mesh* mesh, const char* file_path)
{
	char* buffer;

	//Check if we have the correct path_file
	uint size_file = App->filesystem->Load(file_path, &buffer);
	if (size_file == 0) 
	{
		return false;
	}
	else
	{
		uint amount_infor_mesh[4];
		char* cursor = buffer;

		uint bytes = sizeof(amount_infor_mesh);
		memcpy(amount_infor_mesh, cursor, bytes);
        cursor += bytes;

		//Tranfer information to the mesh
		//Indices
		mesh->num_indices = amount_infor_mesh[0];
		bytes = sizeof(uint) * mesh->num_indices;
		mesh->indices = new uint[mesh->num_indices];
		memcpy(mesh->indices, cursor, bytes);
		cursor += bytes;

		//Vertices
		mesh->num_vertices = amount_infor_mesh[1];
		bytes = sizeof(float) * mesh->num_vertices * 3;
		mesh->vertices = new float[mesh->num_vertices * 3];
		memcpy(mesh->vertices, cursor, bytes);
		cursor += bytes;

		//Normals
		mesh->num_normals = amount_infor_mesh[2];
		if (amount_infor_mesh[2] != 0)
		{
			bytes = sizeof(float) * mesh->num_normals * 3;
			mesh->normals = new float[mesh->num_normals * 3];
			memcpy(mesh->normals, cursor, bytes);
			cursor += bytes;
		}

		//Texture Coords(uvs)
		mesh->num_uvs_texture_coords = amount_infor_mesh[3];
		if (mesh->num_uvs_texture_coords != 0)
		{
			bytes = sizeof(float) * mesh->num_uvs_texture_coords * 2;
			mesh->uvs_texture_coords = new float2[mesh->num_uvs_texture_coords * 2];
			memcpy(mesh->uvs_texture_coords, cursor, bytes);
		}

		delete[] buffer;
		buffer = nullptr;
	}

	return true;
}


bool  ModuleGeometry::Import_Material(const char* name_file, const char* path, std::string& out_p, const char* dir_scene)
{
	bool ret = false;

	ILuint id;
	ilGenImages(1, &id);
	ilBindImage(id);
	ilLoadImage(path);

	ILuint size;
	ILubyte* data;
	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);
	size = ilSaveL(IL_DDS, NULL, 0);

	if (size > 0)
	{
		data = new ILubyte[size];
		if (ilSaveL(IL_DDS, data, size) > 0)
		{
			string name_f = name_file;
			string scene_path = dir_scene;
			scene_path.append("/Textures/");
			ret = App->filesystem->Save_Unique(name_f, data, size, scene_path.c_str(), "tex", out_p);
		}

		delete[] data;
		data = nullptr;
	}

	ilDeleteImages(1, &id);

	return ret;
}

