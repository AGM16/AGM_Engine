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
	char* buffer;
	if (drop == false)
	{
		uint size_file = App->filesystem->Load(path, &buffer);

		if (size_file != 0)
		{
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
				Load_Nodes_For_Hierarchy(parent->mChildren[i], scene, nullptr);
			}
		
			ret = true;

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

void ModuleGeometry::Load_Nodes_For_Hierarchy(aiNode* node_child, const aiScene* scene, GameObject* parent)
{ 
	
    GameObject* game_obj = nullptr;

	if (node_child->mNumMeshes > 0)
	{
		for (int i = 0; i < node_child->mNumMeshes; i++)
		{
			aiMesh* new_mesh = scene->mMeshes[node_child->mMeshes[i]];
			Mesh* m = new Mesh();

			//Load Mesh
			Load_Info_Mesh(m, new_mesh);

			//Texture
			Load_texture_From_FBX(m, new_mesh, scene);


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


			//Check Hierarchy and local transform of every mesh
			if (new_mesh->HasPositions() == true)
			{	
				Hierarchy_And_Local_Transform(m, node_child);	
			}

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
			Load_Nodes_For_Hierarchy(node_child->mChildren[i], scene, game_obj);
		}

	}

}


//-------------------------------LOAD_FUNCTIONS-------------------------------------
void ModuleGeometry::Load_Info_Mesh(Mesh* m, aiMesh* new_mesh)
{
	//Vertices
	m->num_vertices = new_mesh->mNumVertices;
	m->vertices = new float[m->num_vertices * 3];
	memcpy(m->vertices, new_mesh->mVertices, sizeof(float) * m->num_vertices * 3);
	LOG("New mesh with %d vertices", m->num_vertices);

	//Check if our mesh have normals
	if (new_mesh->HasNormals())
	{

		m->num_normals = new_mesh->mNumVertices;
		m->normals = new float[m->num_normals * 3];
		memcpy(m->normals, new_mesh->mNormals, sizeof(float) * m->num_normals * 3);
		LOG("New mesh with %d normals", m->num_normals);
	}

	//Check if the mesh have uvs coordenates
	if (new_mesh->HasTextureCoords(m->uvs_index_texture_coords))
	{
		m->num_uvs_texture_coords = new_mesh->mNumVertices;
		m->uvs_texture_coords = new float2[m->num_uvs_texture_coords];

		for (int i = 0; i < m->num_uvs_texture_coords; i++)
		{
			//Assign uv to the uvs_array<float2>
			m->uvs_texture_coords[i].x = new_mesh->mTextureCoords[m->uvs_index_texture_coords][i].x;
			m->uvs_texture_coords[i].y = new_mesh->mTextureCoords[m->uvs_index_texture_coords][i].y;
		}

		LOG("New mesh with %d uvs_texture_coords", m->num_uvs_texture_coords);
	}

	//Faces
	if (new_mesh->HasFaces())
	{
		//We assume that each face is a triangle
		m->num_indices = new_mesh->mNumFaces * 3;
		m->indices = new uint[m->num_indices];
		for (uint i = 0; i < new_mesh->mNumFaces; ++i)
		{
			//Check the number of indices of each face
			if (new_mesh->mFaces[i].mNumIndices != 3)
			{
				LOG("WARNING, geometry face with != 3indices!");
			}
			else
				memcpy(&m->indices[i * 3], new_mesh->mFaces[i].mIndices, 3 * sizeof(uint));

		}
	}
}

void ModuleGeometry::Hierarchy_And_Local_Transform(Mesh* m, aiNode* node)
{

	m->parent = node->mParent->mName.C_Str();
	m->num_children = node->mNumChildren;
	m->name_node = node->mName.C_Str();
	LOG("The %s mesh is the child of the gameobject %s", m->name_node.c_str(), m->parent);

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

void ModuleGeometry::Load_texture_From_FBX(Mesh* m, aiMesh* new_mesh, const aiScene* scene)
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
