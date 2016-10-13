#include "Component_Transformation.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include "Components.h"
#include "GameObject.h"
#include "Imgui\imgui.h"


Component_Transformation::Component_Transformation(Components_Type type, GameObject* game_object, float3 pos, float3 scale_, Quat rot_quat, float3 angles) : Components(type, game_object), position(pos), last_position(pos),  scale(scale_), last_scale(scale_), quat_rotation(rot_quat), rotation_degrees(angles)
{
	transformation_matrix = Create_New_Matrix_Transformation();
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
	}
}

void Component_Transformation::Set_Position()
{

	transformation_matrix = Create_New_Matrix_Transformation();
	Modify_Children();
	last_position = position;

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
		if (go->Get_Children()->size() > 0)
		{
			std::list<GameObject*>::iterator node = go->Get_Children()->begin();
			
			for(node; node != go->Get_Children()->end(); node++)
			{
			    Component_Transformation* comp_trans = (Component_Transformation*)(*node)->Get_Component(TRANSFORMATION);

			
				comp_trans->position += (position - last_position);
				comp_trans->Set_Position();
				
				comp_trans->rotation_degrees = rotation_degrees;
				comp_trans->Set_Rotation();

				if (comp_trans->scale.x != scale.x && comp_trans->scale.y != scale.y && comp_trans->scale.z != scale.z)
				{
					comp_trans->scale = float3(scale.x * comp_trans->scale.x, scale.y * comp_trans->scale.y, scale.z * comp_trans->scale.z);
				}
				else
				{
					comp_trans->scale = scale;
				}

				comp_trans->Set_Scale();


		       if((*node)->Get_Children()->size() > 0)
               comp_trans->Modify_Children();
			}
			

		}
	}
}

math::float4x4 Component_Transformation::Create_New_Matrix_Transformation()
{
	 return transformation_matrix.FromTRS(position, quat_rotation, scale);	
}

math::float4x4 Component_Transformation::Get_Tranformation_Matrix()const
{
	return transformation_matrix;
}

math::float3 Component_Transformation::Get_Position()const
{
	return position;
}