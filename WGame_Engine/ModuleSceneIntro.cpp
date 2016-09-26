#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

#include "Imgui\imgui.h"

#include "MathGeoLib\include\MathGeoLib.h"
#include "MathGeoLib\include\MathBuildConfig.h"

#include "Random.h"

#include <gl/GL.h>
#include <gl/GLU.h>
#include "Primitive.h"
#include "glut/glut.h"
//#include "Glew\include\glew.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec(0, 0, 0));	
	
	show_test_window = false;

	/*uint my_id = 0;
	glGenBuffers(1, (GLuint*) &(my_id));
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* 36 * 3, )*/

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
	glLineWidth(2.0f);

	/*glBegin(GL_LINES);

	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(0.f, 10.f, 0.f);

	glEnd();*/

	glBegin(GL_TRIANGLES);

	//------------------------------------
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(5.f, 5.f, 0.f);
	glVertex3f(0.f, 5.f, 0.f);

  
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(5.f, 0.f, 0.f);
	glVertex3f(5.f, 5.f, 0.f);

	vertex.push_back(float3(0.f, 0.f, 0.f));
	vertex.push_back(float3(5.f, 5.f, 0.f));
	vertex.push_back(float3(0.f, 5.f, 0.f));

	vertex.push_back(float3(0.f, 0.f, 0.f));
	vertex.push_back(float3(5.f, 5.f, 0.f));
	vertex.push_back(float3(0.f, 5.f, 0.f));

	//------------------------------------
	
	glVertex3f(5.f, 0.f, 0.f);
	glVertex3f(5.f, 0.f, -10.f);
	glVertex3f(5.f, 5.f, -10.f);

	
	glVertex3f(5.f, 5.f, -10.f);
	glVertex3f(5.f, 5.f, 0.f);
	glVertex3f(5.f, 0.f, 0.f);

	//------------------------------------
	
	glVertex3f(5.f, 5.f,-10.f);
	glVertex3f(0.f, 0.f, -10.f);
	glVertex3f(0.f, 5.f, -10.f);


    glVertex3f(5.f, 0.f, -10.f);
	glVertex3f(0.f, 0.f, -10.f);
	glVertex3f(5.f, 5.f, -10.f);

	//------------------------------------

	
	glVertex3f(0.f, 0.f, -10.f);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(0.f, 5.f, -10.f);


    glVertex3f(0.f, 5.f, 0.f);
	glVertex3f(0.f, 5.f, -10.f);
	glVertex3f(0.f, 0.f, 0.f);
	
	//------------------------------------

	glVertex3f(0.f, 5.f, 0.f);
	glVertex3f(5.f, 5.f, -10.f);
	glVertex3f(0.f, 5.f, -10.f);

	glVertex3f(5.f, 5.f, -10.f);
	glVertex3f(0.f, 5.f, 0.f);
	glVertex3f(5.f, 5.f, 0.f);


	//------------------------------------

	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(0.f, 0.f, -10.f);
	glVertex3f(5.f, 0.f, -10.f);
	

	glVertex3f(5.f, 0.f, -10.f);
	glVertex3f(5.f, 0.f, 0.f);
	glVertex3f(0.f, 0.f, 0.f);
	



	glEnd();

	glLineWidth(1.0f);

	return UPDATE_CONTINUE;
}




