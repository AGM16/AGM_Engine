#include "Component_Transformation.h"
#include "Application.h"
#include "Imgui\imgui.h"

Component_Transformation::Component_Transformation(COMPONENT_TYPE type, GameObject* go, float3 position_, float3 rotation_, float3 scale_) : Components(type, go), position(position_), rotation(rotation_), scale(scale_), angle(rotation_)
{

}

Component_Transformation::~Component_Transformation()
{

}

//Functions
void Component_Transformation::Update()
{
	if (Get_Active_Component())
	{

		if (ImGui::CollapsingHeader("Local Transform"))
		{
			ImGui::DragFloat3("Postion", position.ptr());
			ImGui::DragFloat3("Rotation", angle.ptr());
			ImGui::DragFloat3("Scale", scale.ptr());
		}

	}
}



//Get Variables
float3 Component_Transformation::Get_Position()
{
	return position;
}

float3 Component_Transformation::Get_Rotation_Axis()
{
	return axis_rotation;
}

float3  Component_Transformation::Get_Angle_Rotation()
{
	return angle;
}

float3 Component_Transformation::Get_Scale()
{
	return scale;
}
