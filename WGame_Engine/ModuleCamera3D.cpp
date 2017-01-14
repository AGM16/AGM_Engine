#include "Globals.h"
#include "Application.h"
#include "PhysBody3D.h"
#include "ModuleCamera3D.h"
#include "GameObject.h"
#include "Component_Camera.h"
#include "Component_Transformation.h"

#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

ModuleCamera3D::ModuleCamera3D(bool start_enabled) : Module(start_enabled)
{
	Set_Name_Module("Camera");
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Init()
{
	LOG("Setting up the camera");
	bool ret = true;

	//Main Camera
	main_camera_go = App->go_manager->Create_Camera_Game_Object(nullptr, "Main_Camera");
	camera_component = (Component_Camera*)main_camera_go->Get_Component(Components_Type::CAMERA);

	camera_component->Get_Component_Transformation_Camera()->Set_Position(float3(0.f, 50.f, 150.f));
	camera_component->Get_Component_Transformation_Camera()->Set_Rotation(float3(-20.f, 180.f, 0.f));
	camera_component->Set_Far_Plane(400.f);
	
	
	//Camera Test
	camera_go_camera_test = App->go_manager->Create_Camera_Game_Object(nullptr, "Camera_Test");
	camera_component_test = (Component_Camera*)camera_go_camera_test->Get_Component(Components_Type::CAMERA);
	camera_component_test->Get_Component_Transformation_Camera()->Set_Position(float3(0.f, 15.f, -100.f));

	//Draw frustum camera test
	camera_component_test->Set_Draw_Frustum(true);

	

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	//Camera Controls
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		camera_component->Get_Component_Transformation_Camera()->Set_Position(float3(0.f, 50.f, 150.f));
		camera_component->Get_Component_Transformation_Camera()->Set_Rotation(float3(-20.f, 180.f, 0.f));
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		camera_component->Get_Component_Transformation_Camera()->Set_Position(float3(0.f, 50.f, -150.f));
		camera_component->Get_Component_Transformation_Camera()->Set_Rotation(float3(20.f, 0.f, 0.f));
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		camera_component->Get_Component_Transformation_Camera()->Set_Position(float3(150.f, 20.f, 0.f));
		camera_component->Get_Component_Transformation_Camera()->Set_Rotation(float3(0.f, -90.f, 0.f));
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		camera_component->Get_Component_Transformation_Camera()->Set_Position(float3(-150.f, 20.f, 0.f));
		camera_component->Get_Component_Transformation_Camera()->Set_Rotation(float3(0.f, 90.f, 0.f));
	}

	if (camera_component->Get_Component_Transformation_Camera()->Is_Checkbox_Active() == false && App->editor->Is_Saving() == false && App->editor->Is_Loading() == false && App->editor->Is_Creating_GO() == false)
	{
		//Move Buttons-----------------------------------------------------------
		float3 pos = Get_Camera_Position();
		math::float4x4 matrix = camera_component->Get_Component_Transformation_Camera()->Get_Tranformation_Matrix();

		float3 x = matrix.WorldX();
		float3 y = matrix.WorldY();
		float3 z = matrix.WorldZ();

		math::float3 new_pos = math::float3::zero;
		float speed = 15.0f * dt;

		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			speed = 30.0f * dt;

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) new_pos += z * speed;
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) new_pos -= z * speed;


		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) new_pos += x * speed;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) new_pos -= x * speed;



		//Middle mouse button movement
		if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT)
		{
			int dx = App->input->GetMouseXMotion();
			int dy = App->input->GetMouseYMotion();

			new_pos += x * speed * dx;
			new_pos += z * speed * dy;
		}

		if (new_pos.x != 0 || new_pos.y != 0 || new_pos.z != 0)
		{
			pos += new_pos;
			Set_Position(pos);
		}

		// Mouse motion ------------------------------------------------------

		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		{
			float sensivity = 0.025f;

			float delta_x = (float)dx * sensivity;
			float delta_y = (float)dy * sensivity;

			Quat quaternion, quaternion2;
			quaternion = quaternion.FromEulerXYZ(0.0f, delta_x, 0.0f);
			quaternion2 = quaternion2.FromEulerXYZ(-delta_y, 0.0f, 0.0f);

			Quat result = camera_component->Get_Component_Transformation_Camera()->Get_Quaternion_Rotation();
			result = result.Mul(quaternion);
			result = result.Mul(quaternion2);
			camera_component->Get_Component_Transformation_Camera()->Set_Rotation(RadToDeg(result.ToEulerXYZ()));
		}
	}

	return UPDATE_CONTINUE;
}


// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const vec &Spot)
{
	camera_component->Look_At(Spot);
}


// -----------------------------------------------------------------
void ModuleCamera3D::Set_Position(const vec &position)
{
	camera_component->Frustum_Translate(position);
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return camera_component->Get_View_Matrix();
}

float3 ModuleCamera3D::Get_Camera_Position()const
{
	return camera_component->Get_Position();
}

// -----------------------------------------------------------------
Component_Camera* ModuleCamera3D::Get_Camera_Component()const
{
	return camera_component;
}

float* ModuleCamera3D::Get_Projection_Matrix()const
{
	math::float4x4 matrix = camera_component->Get_Project_Matrix();
	matrix.Transpose();

	return *matrix.v;
}

LineSegment ModuleCamera3D::Create_RayCast()
{
	float2 mouse_pos(App->input->GetMouseX(), App->input->GetMouseY());

	//Pos x and y of the mouse normalized
	float norm_pos_x = 2.0f * mouse_pos.x / App->window->Get_Screen_size().x - 1.0f;
	float norm_pos_y = 1.0f - 2.0f * mouse_pos.y / App->window->Get_Screen_size().y;

	return camera_component->Get_Frustum().UnProjectLineSegment(norm_pos_x, norm_pos_y);
	
}
