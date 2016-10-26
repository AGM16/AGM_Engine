#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "Glew\include\glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>


#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/libx86/glew32.lib") 

#include "Imgui\imgui.h"
#include "Imgui\imgui_impl_sdl_gl3.h"


ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
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
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT, 20.0f);

	ImGui_ImplSdlGL3_Init(App->window->window);

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

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


void ModuleRenderer3D::OnResize(int width, int height, float fovy)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Calculate perspective
	float4x4 perspective;
	float _near = 0.125f;
	float _far = 512.0f;

	perspective.SetIdentity();
	float tan_theta_over2 = tan(fovy * pi / 360.0f);

	perspective[0][0] = (1.0f / tan_theta_over2) / ((float)width / (float)height);
	perspective[1][1] = 1.0f / tan_theta_over2;
	perspective[2][2] = (_near + _far) / (_near - _far);
	perspective[3][2] = 2 * _near * _far / (_near - _far);
	perspective[2][3] = -1;
	perspective[3][3] = 0;

	ProjectionMatrix = perspective;
	glLoadMatrixf(*ProjectionMatrix.v);

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


void ModuleRenderer3D::Render_Cube(float4x4 transform, math::float3 size, math::float3 center) const
{
	glPushMatrix();
	transform.v[3][1] *= 2;
	glMultMatrixf(*transform.v);

	float sx = size.x * 0.5f;
	float sy = size.y * 0.5f;
	float sz = size.z * 0.5f;

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBegin(GL_QUADS);

	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-sx, -sy, sz);
	glVertex3f(sx, -sy, sz);
	glVertex3f(sx, sy, sz);
	glVertex3f(-sx, sy, sz);

	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(sx, -sy, -sz);
	glVertex3f(-sx, -sy, -sz);
	glVertex3f(-sx, sy, -sz);
	glVertex3f(sx, sy, -sz);

	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(sx, -sy, sz);
	glVertex3f(sx, -sy, -sz);
	glVertex3f(sx, sy, -sz);
	glVertex3f(sx, sy, sz);

	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-sx, -sy, -sz);
	glVertex3f(-sx, -sy, sz);
	glVertex3f(-sx, sy, sz);
	glVertex3f(-sx, sy, -sz);

	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-sx, sy, sz);
	glVertex3f(sx, sy, sz);
	glVertex3f(sx, sy, -sz);
	glVertex3f(-sx, sy, -sz);

	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(-sx, -sy, -sz);
	glVertex3f(sx, -sy, -sz);
	glVertex3f(sx, -sy, sz);
	glVertex3f(-sx, -sy, sz);

	glEnd();

	glPopMatrix();
}
