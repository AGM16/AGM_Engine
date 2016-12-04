#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "Imgui\imgui.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include "Random.h"
#include "p2QuadTree.h"

#include "Glew\include\glew.h"
#include <gl/GL.h>
#include "glut/glut.h"

#include "Devil\include\il.h"
#include "Devil\include\ilu.h"
#include "Devil\include\ilut.h"




using namespace std;

ModuleSceneIntro::ModuleSceneIntro(bool start_enabled) : Module(start_enabled)
{
	Set_Name_Module("Scene_Intro");
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

bool ModuleSceneIntro::Init()
{
	return true;
}

// Load assets
bool ModuleSceneIntro::Start()
{

	LOG("Loading Intro assets");
	bool ret = true;

	//App->camera->Move(vec(140.0f, 40.0f, 0.0f));
	//App->camera->LookAt(vec(0, 0, 0));	
	
	//Load 3D model
	App->geometry->Load_Geometry("Assets/3D_Models/Street environment_V01.fbx",false);



	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");


	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{	
	if (App->input->GetKey(SDL_SCANCODE_B) == KEY_UP)
	{
		App->go_manager->Insert_GO_To_Quadtree();
		App->go_manager->Set_Game_Obj_Inserted(true);
	}	

	if (App->input->GetKey(SDL_SCANCODE_N) == KEY_UP)
	{
		App->go_manager->Clear_Quadtree_Function();
	}

	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_UP)
	{
		App->go_manager->Create_Quadtree_Root_Function(float2(100.f, 100.f), float2(0.f, 0.f));
	}

	App->go_manager->Intersect_Camera_Culling_Quadtree_Function(*App->camera->camera_component_test);
	    
	return UPDATE_CONTINUE;
}




