#include "Component_Camera.h"
#include "Application.h"
#include "Components.h"
#include "Component_Transformation.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include "GameObject.h"


Component_Camera::Component_Camera(Components_Type type, GameObject* game_object, const char* name_id_camera) : Components(type, game_object), id_camera(name_id_camera), vertical_fov(40.f), near_plane(10.f), far_plane(180.f), aspect_ratio(0.f)
{
	transformation = (Component_Transformation*)Get_Game_Object()->Get_Component(TRANSFORMATION);
	Initialize_Frustum_Components();
}

Component_Camera::~Component_Camera()
{

}


void Component_Camera::Update()
{
	if (last_fst_transformation.Equals(transformation->Get_Tranformation_Matrix()) == false)
	{
		float4x4 matrix = transformation->Get_Tranformation_Matrix();

		frustum.SetPos(matrix.TranslatePart());

		last_fst_transformation = transformation->Get_Tranformation_Matrix();		
	}
 
	if (Is_Active())
	{
		if (ImGui::CollapsingHeader("Camera Options", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::SliderFloat("Near Plane", &near_plane, 1.0f, far_plane))
			{
				Set_Near_Plane(near_plane);
			}

			if (ImGui::SliderFloat("Far Plane", &far_plane, near_plane, 400.f))
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

			ImGui::Text("Aspect_Ratio :  %f", aspect_ratio);

		
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
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetPos(float3(0.f, 20.f, -50.f));
	transformation->Set_Position(frustum.Pos());
	frustum.SetFront(float3::unitZ);
	frustum.SetUp(float3::unitY);
	frustum.SetViewPlaneDistances(near_plane, far_plane);
	frustum.SetPerspective(DegToRad(60.f), DegToRad(vertical_fov));
    
	aspect_ratio = frustum.AspectRatio();
	

}

void Component_Camera::Look_At(const float3 &position)
{
	reference = position;
	float3 look_at_dir = reference - transformation->Get_Position();
	float3x3 look_at_matrix = float3x3::LookAt(frustum.Front(), look_at_dir, frustum.Up(), float3::unitY);

	//Frustum Modifications: Front and Up
	frustum.SetFront(look_at_matrix.MulDir(frustum.Front()).Normalized());
	frustum.SetUp(look_at_matrix.MulDir(frustum.Up()).Normalized());
}

//Set Functions
void Component_Camera::Set_Near_Plane(float new_near_plane)
{
	if(new_near_plane > 1.0f && new_near_plane < frustum.FarPlaneDistance())
		frustum.SetViewPlaneDistances(new_near_plane, frustum.FarPlaneDistance());
}

void Component_Camera::Set_Far_Plane(float new_far_plane)
{
	if (new_far_plane > frustum.NearPlaneDistance())
		frustum.SetViewPlaneDistances(frustum.NearPlaneDistance(), new_far_plane);
	
}

void Component_Camera::Set_FOV(float new_fov)
{
	frustum.SetVerticalFovAndAspectRatio(new_fov, frustum.AspectRatio());
}

void Component_Camera::Set_Up(float3 new_up)
{
	frustum.SetUp(new_up);
}

void Component_Camera::Set_Front(float3 new_front)
{
	frustum.SetFront(new_front);
}

void Component_Camera::Frustum_Translate(float3 position)
{
	frustum.Translate(position);
	transformation->Set_Position(position);
}


//Get Functions
float* Component_Camera::Get_View_Matrix()const
{
	float4x4 view_matrix = frustum.ViewMatrix();
	return *view_matrix.Transposed().v;
	
}

float4x4 Component_Camera::Get_Project_Matrix()const
{
	float4x4 project_matrix = frustum.ProjectionMatrix();
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

Component_Transformation* Component_Camera::Get_Component_Transformation_Camera()const
{
	return transformation;
}

float3 Component_Camera::Get_Up()const
{
	return frustum.Up();
}

float3 Component_Camera::Get_Front()const
{
	return frustum.Front();
}

float3 Component_Camera::Get_World_Right()const
{
	return frustum.WorldRight();
}



