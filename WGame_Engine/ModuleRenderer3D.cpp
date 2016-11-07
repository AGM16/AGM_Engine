#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "Component_Transformation.h"
#include "Glew\include\glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>


#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/libx86/glew32.lib") 

#include "Imgui\imgui.h"
#include "Imgui\imgui_impl_sdl_gl3.h"


ModuleRenderer3D::ModuleRenderer3D(bool start_enabled) : Module(start_enabled)
{
	Set_Name_Module("Render");
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;
	
	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	GLenum gl_enum = glewInit();

	if (GLEW_OK != gl_enum)
	{
		LOG("Glew failed");
	}
	
	if(ret == true)
	{
		//Use Vsync
		if(VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_TEXTURE_2D);
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
	}

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	ImGui_ImplSdlGL3_Init(App->window->window);

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

    //Matrix Projection
	Update_Matrix_Projection();

	//Matrix View
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	
	// light 0 on cam pos
	lights[0].SetPos(App->camera->Get_Camera_Component()->Get_Component_Transformation_Camera()->Get_Position().x, App->camera->Get_Camera_Component()->Get_Component_Transformation_Camera()->Get_Position().y, App->camera->Get_Camera_Component()->Get_Component_Transformation_Camera()->Get_Position().z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	ImGui::Render();
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");
	ImGui_ImplSdlGL3_Shutdown();
	SDL_GL_DeleteContext(context);

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);
	Update_Matrix_Projection();
}

void ModuleRenderer3D::Update_Matrix_Projection()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glLoadMatrixf(App->camera->Get_Projection_Matrix());
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleRenderer3D::Draw_Geometry(const Mesh* mesh, uint texture, float4x4 transformation_matrix, bool wire)
{	
	if (wire == true)
	{
       glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glMultMatrixf(*transformation_matrix.v);
	
	//Enables
	glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	//Vertexs
	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertices);
	glVertexPointer(3, GL_FLOAT, 0, nullptr);

	//Normals
	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_normals);
	glNormalPointer(GL_FLOAT, 0, nullptr);

	
	//Uvs
	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_uvs_texture_coords);
	glTexCoordPointer(2, GL_FLOAT, 0, nullptr);


	if (mesh->num_uvs_texture_coords > 0)
	{
		if (mesh->id_image_texture != 0)
		{
			//Put Texture in buffer
			glBindTexture(GL_TEXTURE_2D, texture);
		}
	}

	//Index
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indices);
	glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, nullptr);

    glBindTexture(GL_TEXTURE_2D, 0); 
    

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	glPopMatrix();
	
}


void ModuleRenderer3D::Render_AABB_Cube(math::AABB aabb_box)
{
	glDisable(GL_LIGHTING);
	vec corners[8];
	aabb_box.GetCornerPoints(corners);

	glLineWidth(2.0f);
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);


	if (corners->Length() > 0)
	{
		
		glBegin(GL_LINES);

		//Back Lines
		glVertex3f(corners[4].x, corners[4].y, corners[4].z);
		glVertex3f(corners[0].x, corners[0].y, corners[0].z);
		glVertex3f(corners[2].x, corners[2].y, corners[2].z);
		glVertex3f(corners[6].x, corners[6].y, corners[6].z);

		//Front Lines
		glVertex3f(corners[1].x, corners[1].y, corners[1].z);
		glVertex3f(corners[5].x, corners[5].y, corners[5].z);
		glVertex3f(corners[7].x, corners[7].y, corners[7].z);
		glVertex3f(corners[3].x, corners[3].y, corners[3].z);

		//Left Lines
		glVertex3f(corners[5].x, corners[5].y, corners[5].z);
		glVertex3f(corners[4].x, corners[4].y, corners[4].z);
		glVertex3f(corners[6].x, corners[6].y, corners[6].z);
		glVertex3f(corners[7].x, corners[7].y, corners[7].z);

		//Right Lines
		glVertex3f(corners[0].x, corners[0].y, corners[0].z);
		glVertex3f(corners[1].x, corners[1].y, corners[1].z);
		glVertex3f(corners[3].x, corners[3].y, corners[3].z);
		glVertex3f(corners[2].x, corners[2].y, corners[2].z);

		//Vertical Back Lines
		glVertex3f(corners[2].x, corners[2].y, corners[2].z);
		glVertex3f(corners[0].x, corners[0].y, corners[0].z);
		glVertex3f(corners[6].x, corners[6].y, corners[6].z);
		glVertex3f(corners[4].x, corners[4].y, corners[4].z);

		//Vertical Front Lines
		glVertex3f(corners[3].x, corners[3].y, corners[3].z);
		glVertex3f(corners[1].x, corners[1].y, corners[1].z);
		glVertex3f(corners[5].x, corners[5].y, corners[5].z);
		glVertex3f(corners[7].x, corners[7].y, corners[7].z);

		glEnd();
	}

	glEnable(GL_LIGHTING);
    
}



void ModuleRenderer3D::Render_OBB_Cube(math::OBB aabb_box)
{

	vec corners[8];
	aabb_box.GetCornerPoints(corners);

	glDisable(GL_LIGHTING);

	glLineWidth(2.0f);
	glColor4f(0.0f, 0.0f, 9.0f, 1.0f);


	if (corners->Length() > 0)
	{

		glBegin(GL_LINES);

		//Back Lines
		glVertex3f(corners[4].x, corners[4].y, corners[4].z);
		glVertex3f(corners[0].x, corners[0].y, corners[0].z);
		glVertex3f(corners[2].x, corners[2].y, corners[2].z);
		glVertex3f(corners[6].x, corners[6].y, corners[6].z);

		//Front Lines
		glVertex3f(corners[1].x, corners[1].y, corners[1].z);
		glVertex3f(corners[5].x, corners[5].y, corners[5].z);
		glVertex3f(corners[7].x, corners[7].y, corners[7].z);
		glVertex3f(corners[3].x, corners[3].y, corners[3].z);

		//Left Face
		glVertex3f(corners[5].x, corners[5].y, corners[5].z);
		glVertex3f(corners[4].x, corners[4].y, corners[4].z);
		glVertex3f(corners[6].x, corners[6].y, corners[6].z);
		glVertex3f(corners[7].x, corners[7].y, corners[7].z);

		//Right Lines
		glVertex3f(corners[0].x, corners[0].y, corners[0].z);
		glVertex3f(corners[1].x, corners[1].y, corners[1].z);
		glVertex3f(corners[3].x, corners[3].y, corners[3].z);
		glVertex3f(corners[2].x, corners[2].y, corners[2].z);

		//Vertical Back Lines
		glVertex3f(corners[2].x, corners[2].y, corners[2].z);
		glVertex3f(corners[0].x, corners[0].y, corners[0].z);
		glVertex3f(corners[6].x, corners[6].y, corners[6].z);
		glVertex3f(corners[4].x, corners[4].y, corners[4].z);

		//Vertical Front Lines
		glVertex3f(corners[3].x, corners[3].y, corners[3].z);
		glVertex3f(corners[1].x, corners[1].y, corners[1].z);
		glVertex3f(corners[5].x, corners[5].y, corners[5].z);
		glVertex3f(corners[7].x, corners[7].y, corners[7].z);

		glEnd();


	}

	glEnable(GL_LIGHTING);
}

void ModuleRenderer3D::Render_Frustum_Cube(math::Frustum fst)
{
	vec corners[8];
	fst.GetCornerPoints(corners);

	glDisable(GL_LIGHTING);

	glLineWidth(2.0f);
	glColor4f(9.0f, 0.0f, 0.0f, 1.0f);

	if (corners->Length() > 0)
	{

		glBegin(GL_LINES);

		//Back Lines
		glVertex3f(corners[4].x, corners[4].y, corners[4].z);
		glVertex3f(corners[0].x, corners[0].y, corners[0].z);
		glVertex3f(corners[2].x, corners[2].y, corners[2].z);
		glVertex3f(corners[6].x, corners[6].y, corners[6].z);

		//Front Lines
		glVertex3f(corners[1].x, corners[1].y, corners[1].z);
		glVertex3f(corners[5].x, corners[5].y, corners[5].z);
		glVertex3f(corners[7].x, corners[7].y, corners[7].z);
		glVertex3f(corners[3].x, corners[3].y, corners[3].z);

		//Left Face
		glVertex3f(corners[5].x, corners[5].y, corners[5].z);
		glVertex3f(corners[4].x, corners[4].y, corners[4].z);
		glVertex3f(corners[6].x, corners[6].y, corners[6].z);
		glVertex3f(corners[7].x, corners[7].y, corners[7].z);

		//Right Lines
		glVertex3f(corners[0].x, corners[0].y, corners[0].z);
		glVertex3f(corners[1].x, corners[1].y, corners[1].z);
		glVertex3f(corners[3].x, corners[3].y, corners[3].z);
		glVertex3f(corners[2].x, corners[2].y, corners[2].z);

		//Vertical Back Lines
		glVertex3f(corners[2].x, corners[2].y, corners[2].z);
		glVertex3f(corners[0].x, corners[0].y, corners[0].z);
		glVertex3f(corners[6].x, corners[6].y, corners[6].z);
		glVertex3f(corners[4].x, corners[4].y, corners[4].z);

		//Vertical Front Lines
		glVertex3f(corners[3].x, corners[3].y, corners[3].z);
		glVertex3f(corners[1].x, corners[1].y, corners[1].z);
		glVertex3f(corners[5].x, corners[5].y, corners[5].z);
		glVertex3f(corners[7].x, corners[7].y, corners[7].z);

		glEnd();


	}

	glEnable(GL_LIGHTING);
}
