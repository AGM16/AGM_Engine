#include "Component_Transformation.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include "Components.h"
#include "Imgui\imgui.h"


Component_Transformation::Component_Transformation(Components_Type type, GameObject* game_object, float3 pos, float3 scale_, Quat rot_quat, float3 angles) : Components(type, game_object), position(pos),  scale(scale_), quat_rotation(rot_quat), rotation_degrees(angles)
{
	
}

Component_Transformation::~Component_Transformation()
{

}

void Component_Transformation::Update()
{
	//I do that to put everything at correct position at the begining
	if (start)
	{
	   Set_Position();
       Set_Rotation();
	   Set_Scale();
       Create_New_Matrix_Transformation();
	   start = false;
	}

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

}

void Component_Transformation::Set_Rotation()
{

	rotation_radians = DegToRad(rotation_degrees);

	quat_rotation = quat_rotation.FromEulerXYZ(rotation_radians.x, rotation_radians.y, rotation_radians.z);

	transformation_matrix = Create_New_Matrix_Transformation();
}

void Component_Transformation::Set_Scale()
{
	transformation_matrix = Create_New_Matrix_Transformation();
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