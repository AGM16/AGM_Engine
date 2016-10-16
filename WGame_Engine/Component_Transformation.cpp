#include "Component_Transformation.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include "Components.h"
#include "GameObject.h"
#include "Imgui\imgui.h"


Component_Transformation::Component_Transformation(Components_Type type, GameObject* game_object, float3 pos, float3 scale_, Quat rot_quat, float3 angles) : Components(type, game_object), position(pos), scale(scale_), quat_rotation(rot_quat), rotation_degrees(angles)
{
	transformation_matrix = Create_New_Matrix_Transformation();
	Modify_Children();
}

Component_Transformation::~Component_Transformation()
{

}

void Component_Transformation::Update()
{

	if (Is_Active())
	{
		if (ImGui::CollapsingHeader("Local Transform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (active == false)
			{
				if (ImGui::DragFloat3("Postion", position.ptr()))
				{
					Set_Position();
				}

				if (ImGui::DragFloat3("Rotation", rotation_degrees.ptr()))
				{
					Set_Rotation();
				}


				if (ImGui::DragFloat3("Scale", scale.ptr()))
				{
					Set_Scale();
				}
			}
			else
			{
				ImGui::DragFloat3("Postion", float3(0.f, 0.f, 0.f).ptr());
				

				ImGui::DragFloat3("Rotation", float3(0.f, 0.f, 0.f).ptr());
				

				ImGui::DragFloat3("Scale", float3(0.f, 0.f, 0.f).ptr());	
			}

			ImGui::Checkbox("Active##foo2", &active);
		}
	}
}



void Component_Transformation::Set_Position()
{

	transformation_matrix = Create_New_Matrix_Transformation();
	Modify_Children();

}

void Component_Transformation::Set_Rotation()
{
	rotation_radians = DegToRad(rotation_degrees);

	quat_rotation = quat_rotation.FromEulerXYZ(rotation_radians.x, rotation_radians.y, rotation_radians.z);

	transformation_matrix = Create_New_Matrix_Transformation();

	Modify_Children();
}

void Component_Transformation::Set_Scale()
{
	transformation_matrix = Create_New_Matrix_Transformation();
	Modify_Children();
}


void Component_Transformation::Modify_Children()
{
	GameObject* go = Get_Game_Object();
	if (go)
	{
		if (go->Get_Parent())
		{
			
			if (go->Get_Parent()->Exist_Component(TRANSFORMATION))
			{
				Component_Transformation* par_comp_transform = (Component_Transformation*)go->Get_Parent()->Get_Component(TRANSFORMATION);

				transformation_matrix_to_draw = par_comp_transform->transformation_matrix_to_draw * transformation_matrix;
			}
						
				for (std::vector<GameObject*>::iterator g_object_children = go->Get_Children()->begin(); g_object_children != go->Get_Children()->end(); g_object_children++)
				{
					if ((*g_object_children)->Exist_Component(TRANSFORMATION))
					{
						Component_Transformation* transform = (Component_Transformation*)(*g_object_children)->Get_Component(TRANSFORMATION);

						transform->Modify_Children();
					}
				}	
		}
		else
		{
			transformation_matrix_to_draw = transformation_matrix;
		}
	}
}

math::float4x4 Component_Transformation::Create_New_Matrix_Transformation()
{
	 return transformation_matrix.FromTRS(position, quat_rotation, scale);	
}

math::float4x4 Component_Transformation::Get_Tranformation_Matrix()const
{
	return transformation_matrix_to_draw;
}

math::float3 Component_Transformation::Get_Position()const
{
	return position;
}