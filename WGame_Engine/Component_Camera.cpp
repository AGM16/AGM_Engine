#include "Component_Camera.h"
#include "Application.h"
#include "Components.h"
#include "Component_Transformation.h"
#include "MathGeoLib\include\Math\MathFunc.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include "GameObject.h"

using namespace std;

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
			ImGui::Checkbox("Active Culling##foo1", &active_culling);
			ImGui::Checkbox("Active AABB Bounding Boxes##fee2", &active_all_aabb_bounding_boxes);
			ImGui::Checkbox("Active OBB Bounding Boxes##fee3", &active_all_obb_bounding_boxes);
			ImGui::Checkbox("Active Draw Frustum##fee4", &draw_frustum);
		}

		if (App->camera->camera_go_camera_test == Get_Game_Object())
		{
			if (ImGui::CollapsingHeader("Quadtree", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::Text("Root:");
				if (ImGui::Button("Create QuadtreeNode Root"))
				{
					App->go_manager->Create_Quadtree_Root_Function(float2(100.f, 100.f), float2(0.f, 0.f));
				}

				ImGui::Text("To insert again GO you must clear the quadtree");
				if (ImGui::Button("Insert Game Objects"))
				{
					App->go_manager->Insert_GO_To_Quadtree();
					App->go_manager->Set_Game_Obj_Inserted(true);
				}

				ImGui::Text("Erase Quadtree:");
				if (ImGui::Button("Clear Quadtree"))
				{
					App->go_manager->Clear_Quadtree_Function();
				}

				float size[2];
				size[0] = App->go_manager->Get_Boundaries_Quadtree_Root_Function().x;
				size[1] = App->go_manager->Get_Boundaries_Quadtree_Root_Function().y;
				if (ImGui::InputFloat2("Root Size:", size))
				{
					App->go_manager->Set_Boundaries_Quadtree_Root_Function(float2(size[0], size[1]));
				}


			}
		}
	}
		
	//Render Camera
	if (draw_frustum)
	{
		App->renderer3D->Render_Frustum_Cube(frustum);
	}
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
void Component_Camera::Set_Near_Plane(const float &new_near_plane)
{
	if(new_near_plane > 1.0f && new_near_plane < frustum.FarPlaneDistance())
		frustum.SetViewPlaneDistances(new_near_plane, frustum.FarPlaneDistance());

	//To update the information panel of the editor
	near_plane = new_near_plane;
}

void Component_Camera::Set_Far_Plane(const float &new_far_plane)
{
	float f_plane = 0;
	if (new_far_plane > 400.f)
		f_plane = 400.f;
	else
		f_plane = new_far_plane;

	if (new_far_plane > frustum.NearPlaneDistance())
		frustum.SetViewPlaneDistances(frustum.NearPlaneDistance(), f_plane);

	//To update the information panel of the editor
	far_plane = f_plane;
	
}

void Component_Camera::Set_FOV(const float &new_fov)
{
	frustum.SetVerticalFovAndAspectRatio(new_fov, frustum.AspectRatio());
}

void Component_Camera::Set_Up(const float3 &new_up)
{
	frustum.SetUp(new_up);
}

void Component_Camera::Set_Front(const float3 &new_front)
{
	frustum.SetFront(new_front);
}

void Component_Camera::Frustum_Translate(const float3 &position)
{
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

float3 Component_Camera::Get_Position()const
{
	return transformation->Get_Position();
}

const Frustum Component_Camera::Get_Frustum()const
{
	return frustum;
}

bool Component_Camera::Intersect_Frustum_AABB(const AABB &b)
{
	if (active_culling == true)
	{
		float3 corners_aabb[8];
		b.GetCornerPoints(corners_aabb);

		Plane planes_fst[6];
		frustum.GetPlanes(planes_fst);

		//Check every plane of the frustum
		for (int n_planes = 0; n_planes < 6; ++n_planes)
		{
			int in_count_corners = 8;
			
          //If all the bounding box is not in front of the plane.
			//If it is samller than  0.f means that is inside/front the plane the min and max of bounding box.
			//As a result, it won't be necessary check each corner of the bounding box with this plane.
			if (planes_fst[n_planes].SignedDistance(b) >= 0.f)
			{
				for (int i = 0; i < 8; ++i)
				{
					//If the corner of the bounding box is behind we dont consider this corner in this plane
					if (planes_fst[n_planes].SignedDistance(corners_aabb[i]) >= 0.f)
					{
						--in_count_corners;
					}
				}
			}

			if (in_count_corners == 0)
			{
				return false;
			}
		}

		return true;

	}
	else
	{
		return false;
	}
}


void Component_Camera::Set_Draw_Frustum(bool on)
{
	draw_frustum = on;
}

//Check Box that active all the bounding boxes
//AABB
void Component_Camera::Set_Checkbox_AABB_Bounding_Boxes(bool on)
{
	active_all_aabb_bounding_boxes = on;
}

bool Component_Camera::Get_Checkbox_AABB_Bounding_Boxes()const
{
	return active_all_aabb_bounding_boxes;
}

void Component_Camera::Set_Checkbox_OBB_Bounding_Boxes(bool on)
{
	active_all_obb_bounding_boxes = on;
}

bool Component_Camera::Get_Checkbox_OBB_Bounding_Boxes()const
{
	return active_all_obb_bounding_boxes;
}






