#include "Component_Camera.h"
#include "Application.h"
#include "Components.h"
#include "Component_Transformation.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include "GameObject.h"


Component_Camera::Component_Camera(Components_Type type, GameObject* game_object, const char* name_id_camera) : Components(type, game_object), id_camera(name_id_camera)
{
	transformation = (Component_Transformation*)Get_Game_Object()->Get_Component(TRANSFORMATION);
	Initialize_Frustum_Components();
}

Component_Camera::~Component_Camera()
{

}


void Component_Camera::Update()
{
	if (last_fst_position.Equals(transformation->Get_Position()) == false)
	{
		frustum.SetPos(transformation->Get_Position());
		last_fst_position = transformation->Get_Position();
	}

	if (Is_Active())
	{
		if (ImGui::CollapsingHeader("Camera Options", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::SliderFloat("Near Plane", &near_plane, 1.0f, far_plane))
			{
				Set_Near_Plane(near_plane);
			}

			if (ImGui::SliderFloat("Far Plane", &far_plane, near_plane, 250.f))
			{
				Set_Far_Plane(far_plane);
			}

			if (ImGui::SliderFloat("Fov", &vertical_fov, 1.f, 100.f))
			{
				//Calculate the new aspect of ratio
				aspect_ratio = (tanf(frustum.HorizontalFov() / 2)) / (tanf(DegToRad(vertical_fov) / 2));
				//Set the FOV
				Set_FOV(DegToRad(vertical_fov));
			}

			ImGui::Text("Aspect_Ratio :  %f", frustum.AspectRatio());

		
		}
	}
		
	//Render Camera
	App->renderer3D->Render_Frustum_Cube(frustum);
}

void Component_Camera::Clean_Up()
{

}

void Component_Camera::Initialize_Frustum_Components()
{
	//Initialize Components Frustum
	frustum.SetKind(FrustumSpaceGL, FrustumLeftHanded);
	frustum.SetPos(float3(50, 20, -100));
	transformation->Set_Position(float3(50, 20, -100));
	frustum.SetFront(float3::unitZ);
	frustum.SetUp(float3::unitY);
	frustum.SetViewPlaneDistances(near_plane, far_plane);
	frustum.SetPerspective(DegToRad(60.f), DegToRad(vertical_fov));
    
	aspect_ratio = frustum.AspectRatio();

	Look_At(float3::zero);
}

void Component_Camera::Look_At(const math::float3 &position)
{
	float3 look_at_dir = position - transformation->Get_Position();
	float3x3 look_at_matrix = float3x3::LookAt(frustum.Front(), look_at_dir, frustum.Up(), float3::unitY);
	//Frustum Modifications
	frustum.SetFront(look_at_matrix.MulDir(frustum.Front()).Normalized());
	frustum.SetUp(look_at_matrix.MulDir(frustum.Up()).Normalized());
}

//Set Functions
void Component_Camera::Set_Near_Plane(float new_near_plane)
{
	if(new_near_plane >= 1.0f && new_near_plane < frustum.FarPlaneDistance())
		frustum.SetViewPlaneDistances(new_near_plane, frustum.FarPlaneDistance());
}

void Component_Camera::Set_Far_Plane(float new_far_plane)
{
	if (new_far_plane > frustum.NearPlaneDistance())
		frustum.SetViewPlaneDistances(frustum.NearPlaneDistance(), new_far_plane);
}

void Component_Camera::Set_FOV(float new_fov)
{
	//Change the FOV
	frustum.SetVerticalFovAndAspectRatio(new_fov, frustum.AspectRatio());

}


//Get Functions
float4x4 Component_Camera::Get_View_Matrix()const
{
	float4x4 view_matrix = frustum.ViewMatrix();
	return view_matrix.Transposed();
	
}

float4x4 Component_Camera::Get_Project_Matrix()const
{
	math::float4x4 project_matrix = frustum.ProjectionMatrix().Transposed();
	return project_matrix;
}

float Component_Camera::Get_Near_Plane()const
{
	return frustum.NearPlaneDistance();
}

float Component_Camera::Get_Far_Plane()const
{
	return frustum.FarPlaneDistance();
}

float Component_Camera::Get_Fov()const
{
	return  vertical_fov;
}


