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
	App->geometry->Load_Geometry("Assets/3D_Models/Brutus.fbx",false);

	scene.Create(float2(500, 500), float2::zero);


	for (vector<GameObject*>::const_iterator go = App->go_manager->Get_Root()->Get_Children()->begin(); go != App->go_manager->Get_Root()->Get_Children()->end(); go++)
	{
		scene.Insert((*go));
	}




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
		

     

	
	return UPDATE_CONTINUE;
}




