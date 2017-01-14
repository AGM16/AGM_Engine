#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "Imgui\imgui.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include "Component_Transformation.h"
#include "Component_Transformation.h"
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
	
	//Load 3D model
	App->geometry->Load_Geometry("Assets/3D_Models/Street environment_V01.fbx",false);


	//Creation Particles
	GameObject* particle_1 = App->go_manager->Create_Particle_Game_Object("Particle_1", nullptr);
	Component_Transformation* trans1 = (Component_Transformation*)particle_1->Get_Component(TRANSFORMATION);
	trans1->Set_Position(float3(32.75, 11, 42.5));

	GameObject* particle_2 = App->go_manager->Create_Particle_Game_Object("Particle_2", nullptr);
	Component_Transformation* trans2 = (Component_Transformation*)particle_2->Get_Component(TRANSFORMATION);
	trans2->Set_Position(float3(43.8, 11, 37.8));

	GameObject* particle_3 = App->go_manager->Create_Particle_Game_Object("Particle_3", nullptr);
	Component_Transformation* trans3 = (Component_Transformation*)particle_3->Get_Component(TRANSFORMATION);
	trans3->Set_Position(float3(-32.9, 7.5, -43.2));

	GameObject* particle_4 = App->go_manager->Create_Particle_Game_Object("Particle_4", nullptr);
	Component_Transformation* trans4 = (Component_Transformation*)particle_4->Get_Component(TRANSFORMATION);
	trans4->Set_Position(float3(-43.1, 7.5, -32.6));

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




