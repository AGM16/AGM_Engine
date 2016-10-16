#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "Imgui\imgui.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include "Random.h"

#include "Glew\include\glew.h"
#include <gl/GL.h>
#include "glut/glut.h"

#include "Devil\include\il.h"
#include "Devil\include\ilu.h"
#include "Devil\include\ilut.h"




using namespace std;

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
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

	App->camera->Move(vec(140.0f, 40.0f, 0.0f));
	App->camera->LookAt(vec(0, 0, 0));	
	
	//Load 3D model
	 App->geometry->Load_Geometry("Assets/3D_Models/Street environment_V01.fbx");



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


void ModuleSceneIntro::Cube_Direct_OPENGL()
{
	glLineWidth(2.0f);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLES);
	

	//glBindTexture(GL_TEXTURE_2D, 0);	
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);


	//----------------FRONT--------------------
	//Generating Uvs 
	glTexCoord2f(0.f, 0.f);
	glVertex3f(0.f, 0.f, 0.f);//A
	glTexCoord2f(1.f, 0.f);
	glVertex3f(5.f, 0.f, 0.f);//B
	glTexCoord2f(1.f, 1.f);
	glVertex3f(5.f, 5.f, 0.f);//C
	glTexCoord2f(1.f, 1.f);	
	glVertex3f(5.f, 5.f, 0.f);//C
	glTexCoord2f(0.f, 1.f);
	glVertex3f(0.f, 5.f, 0.f);//D
	glTexCoord2f(0.f, 0.f);
	glVertex3f(0.f, 0.f, 0.f);//A


	//----------------RIGHT_SIDE--------------------
	//Generating Uvs 
	glTexCoord2f(1.f, 0.f);
	glVertex3f(5.f, 0.f, 0.f);//B
	glTexCoord2f(0.f, 0.f);
	glVertex3f(5.f, 0.f, -5.f);//E
	glTexCoord2f(0.f, 1.f);
	glVertex3f(5.f, 5.f, -5.f);//F
	glTexCoord2f(0.f, 1.f);
	glVertex3f(5.f, 5.f, -5.f);//F
	glTexCoord2f(1.f, 1.f);
	glVertex3f(5.f, 5.f, 0.f);//C
	glTexCoord2f(1.f, 0.f);
	glVertex3f(5.f, 0.f, 0.f);//B

	//-----------------Back_Side-------------------

	//Generating Uvs 
	glTexCoord2f(0.f, 0.f);
	glVertex3f(5.f, 0.f, -5.f);//E
	glTexCoord2f(1.f, 0.f);
	glVertex3f(0.f, 0.f, -5.f);//G
	glTexCoord2f(1.f, 1.f);
	glVertex3f(0.f, 5.f, -5.f);//H
	glTexCoord2f(1.f, 1.f);
	glVertex3f(0.f, 5.f, -5.f);//H
	glTexCoord2f(0.f, 1.f);
	glVertex3f(5.f, 5.f, -5.f);//F
	glTexCoord2f(0.f, 0.f);
	glVertex3f(5.f, 0.f, -5.f);//E


	//-------------LEFT_SIDE-----------------------

	//Generating Uvs 
	glTexCoord2f(1.f, 0.f);
	glVertex3f(0.f, 0.f, -5.f);//G
	glTexCoord2f(0.f, 0.f);
	glVertex3f(0.f, 0.f, 0.f);//A
	glTexCoord2f(0.f, 1.f);
	glVertex3f(0.f, 5.f, 0.f);//D
	glTexCoord2f(0.f, 1.f);
	glVertex3f(0.f, 5.f, 0.f);//D
	glTexCoord2f(1.f, 1.f);
	glVertex3f(0.f, 5.f, -5.f);//H
	glTexCoord2f(1.f, 0.f);
	glVertex3f(0.f, 0.f, -5.f);//G

	//--------------TOP----------------------

	//Generating Uvs 
	glTexCoord2f(0.f, 0.f);
	glVertex3f(5.f, 5.f, 0.f);//C
	glTexCoord2f(1.f, 0.f);
	glVertex3f(5.f, 5.f, -5.f);//F
	glTexCoord2f(1.f, 1.f);
	glVertex3f(0.f, 5.f, -5.f);//H
	
	glTexCoord2f(1.f, 1.f);
	glVertex3f(0.f, 5.f, -5.f);//H
	glTexCoord2f(0.f, 1.f);
	glVertex3f(0.f, 5.f, 0.f);//D
	glTexCoord2f(0.f, 0.f);
	glVertex3f(5.f, 5.f, 0.f);//C


	//---------------BASE---------------------
	//Generating Uvs 
	glTexCoord2f(0.f, 0.f);
	glVertex3f(5.f, 0.f, 0.f);//B
	glTexCoord2f(1.f, 0.f);
	glVertex3f(0.f, 0.f, 0.f);//A
	glTexCoord2f(1.f, 1.f);
	glVertex3f(0.f, 0.f, -5.f);//G
	glTexCoord2f(1.f, 1.f);
	glVertex3f(0.f, 0.f, -5.f);//G
	glTexCoord2f(0.f, 1.f);
	glVertex3f(5.f, 0.f, -5.f);//E
	glTexCoord2f(0.f, 0.f);
	glVertex3f(5.f, 0.f, 0.f);//B

	glEnd();
	glLineWidth(1.0f);
	glDisable(GL_TEXTURE_2D);

}

void ModuleSceneIntro::Cube_Vertex_Arrays_OPENGL()
{
	
	
	//----------------FRONT--------------------


	vertexs.push_back(float3(0.f, 0.f, 0.f));//A
	vertexs.push_back(float3(5.f, 0.f, 0.f));//B
	vertexs.push_back(float3(5.f, 5.f, 0.f));//C


	vertexs.push_back(float3(5.f, 5.f, 0.f));//C
	vertexs.push_back(float3(0.f, 5.f, 0.f));//D
	vertexs.push_back(float3(0.f, 0.f, 0.f));//A


	//----------------RIGHT_SIDE--------------------
	

	vertexs.push_back(float3(5.f, 0.f, 0.f));//B
	vertexs.push_back(float3(5.f, 0.f, -10.f));//E
	vertexs.push_back(float3(5.f, 5.f, -10.f));//F


	vertexs.push_back(float3(5.f, 5.f, -10.f));//F
	vertexs.push_back(float3(5.f, 5.f, 0.f));//C
	vertexs.push_back(float3(5.f, 0.f, 0.f));//B

	//-----------------Back_Side-------------------
   
	vertexs.push_back(float3(5.f, 0.f, -10.f));//E
	vertexs.push_back(float3(0.f, 0.f, -10.f));//G
	vertexs.push_back(float3(0.f, 5.f, -10.f));//H


	vertexs.push_back(float3(0.f, 5.f, -10.f));//H
	vertexs.push_back(float3(5.f, 5.f, -10.f));//F
	vertexs.push_back(float3(5.f, 0.f, -10.f));//E


	//-------------LEFT_SIDE-----------------------


	vertexs.push_back(float3(0.f, 0.f, -10.f));//G
	vertexs.push_back(float3(0.f, 0.f, 0.f));//A
	vertexs.push_back(float3(0.f, 5.f, 0.f));//D


	vertexs.push_back(float3(0.f, 5.f, 0.f));//D
	vertexs.push_back(float3(0.f, 5.f, -10.f));//H
	vertexs.push_back(float3(0.f, 0.f, -10.f));//G

	//--------------TOP----------------------

	vertexs.push_back(float3(5.f, 5.f, 0.f));//C
	vertexs.push_back(float3(5.f, 5.f, -10.f));//F
	vertexs.push_back(float3(0.f, 5.f, -10.f));//H

	vertexs.push_back(float3(0.f, 5.f, -10.f));//H
	vertexs.push_back(float3(0.f, 5.f, 0.f));//D
	vertexs.push_back(float3(5.f, 5.f, 0.f));//C


	//---------------BASE---------------------

	vertexs.push_back(float3(5.f, 0.f, 0.f));//B
	vertexs.push_back(float3(0.f, 0.f, 0.f));//A
	vertexs.push_back(float3(0.f, 0.f, -10.f));//G


	vertexs.push_back(float3(0.f, 0.f, -10.f));//G
	vertexs.push_back(float3(5.f, 0.f, -10.f));//E
	vertexs.push_back(float3(5.f, 0.f, 0.f));//B

	
	glGenBuffers(1, (GLuint*) &(my_id));//Generate the buffer
	glBindBuffer(GL_ARRAY_BUFFER, my_id);//Start using that buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexs.size() * 3, vertexs.data(), GL_STATIC_DRAW);

}


void ModuleSceneIntro::Cube_Indice_OPENGL()
{
	vector<float3> ver;

	ver.push_back(vec(10.0f, 5.0f, 0.0f)); //0
	ver.push_back(vec(15.0f, 5.0f, 5.0f)); //1
	ver.push_back(vec(15.0f, 5.0f, 5.0f)); //2
	ver.push_back(vec(10.0f, 5.0f, 5.0f)); //3
	ver.push_back(vec(10.0f, 5.0f, 5.0f)); //4
	ver.push_back(vec(15.0f, 5.0f, 0.0f)); //5
	ver.push_back(vec(15.0f, 0.0f, 5.0f)); //6
	ver.push_back(vec(15.0f, 0.0f, 5.0f)); //7
	ver.push_back(vec(10.0f, 0.0f, 5.0f)); //8
	ver.push_back(vec(10.0f, 0.0f, 5.0f)); //9
	ver.push_back(vec(15.0f, 0.0f, 0.0f)); //10
	ver.push_back(vec(15.0f, 0.0f, 0.0f)); //11
	ver.push_back(vec(15.0f, 0.0f, 0.0f)); //12
	ver.push_back(vec(10.0f, 0.0f, 0.0f)); //13
	ver.push_back(vec(10.0f, 0.0f, 0.0f)); //14
	ver.push_back(vec(10.0f, 0.0f, 0.0f)); //15


	glGenBuffers(1, (GLuint*) &(my_id));//Generate the buffer
	glBindBuffer(GL_ARRAY_BUFFER, my_id);//Start using that buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ver.size() * 3, ver.data(), GL_STATIC_DRAW);


	uvs.push_back(float2(1.0f, 1.0f));
	uvs.push_back(float2(0.0f, 0.0f));
	uvs.push_back(float2(1.0f, 1.0f));
	uvs.push_back(float2(1.0f, 0.0f));
	uvs.push_back(float2(0.0f, 1.0f));
	uvs.push_back(float2(0.0f, 1.0f));
	uvs.push_back(float2(1.0f, 0.0f));
	uvs.push_back(float2(0.0f, 0.0f));
	uvs.push_back(float2(0.0f, 0.0f));
	uvs.push_back(float2(1.0f, 0.0f));
	uvs.push_back(float2(0.0f, 0.0f));
	uvs.push_back(float2(1.0f, 1.0f));
	uvs.push_back(float2(0.0f, 1.0f));
	uvs.push_back(float2(0.0f, 1.0f));
	uvs.push_back(float2(1.0f, 0.0f));
	uvs.push_back(float2(1.0f, 1.0f));

    //Uvs
	glGenBuffers(1, (GLuint*) &(my_uvs));//Generate the buffer
	glBindBuffer(GL_ARRAY_BUFFER, my_uvs);//Start using that buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * uvs.size() * 2, uvs.data(), GL_STATIC_DRAW);


	//Front
	indices.push_back(10);
	indices.push_back(14);
	indices.push_back(0);

	indices.push_back(10);
	indices.push_back(0);
	indices.push_back(5);

	//Left 
	indices.push_back(13);
	indices.push_back(8);
	indices.push_back(3);

	indices.push_back(13);
	indices.push_back(3);
	indices.push_back(0);

	//Right 
	indices.push_back(6);
	indices.push_back(11);
	indices.push_back(5);

	indices.push_back(6);
	indices.push_back(5);
	indices.push_back(1);

	//Back 
	indices.push_back(8);
	indices.push_back(6);
	indices.push_back(2);

	indices.push_back(8);
	indices.push_back(2);
	indices.push_back(4);

	//Upper 
	indices.push_back(5);
	indices.push_back(3);
	indices.push_back(1);

	indices.push_back(5);
	indices.push_back(0);
	indices.push_back(3);

	//Bottom 
	indices.push_back(15);
	indices.push_back(12);
	indices.push_back(7);

	indices.push_back(15);
	indices.push_back(7);
	indices.push_back(9);
	

	//Indices
	glGenBuffers(1, (GLuint*) &(my_indices));//Generate the buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);//Start using that buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * indices.size(), indices.data(), GL_STATIC_DRAW);

}


void ModuleSceneIntro::Draw_Array_Vertex(vector<float3> &vertices, uint id)
{
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    //DRAW Vertex ARRAY
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size() * 3);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void ModuleSceneIntro::Draw_Indices_UVS(vector<uint> vec_indices, uint indices, uint vertex_id, uint uvs_id, uint texture)
{
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	//Vertexs
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	//Uvs
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, uvs_id);
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	//Index
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices);

	//Put Texture in buffer
	glBindTexture(GL_TEXTURE_2D, texture);

	glDrawElements(GL_TRIANGLES, vec_indices.size(), GL_UNSIGNED_INT, NULL);

	//Disable
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindTexture(GL_TEXTURE_2D, 0);

}





