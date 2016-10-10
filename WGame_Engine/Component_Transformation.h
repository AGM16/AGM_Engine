#ifndef _COMPONENT_TRANSFORMATION_H_
#define _COMPONENT_TRANSFORMATION_H_

#include "Components.h"
#include "MathGeoLib\include\MathGeoLib.h"

class Component_Transformation : public Components
{
public:

	Component_Transformation(COMPONENT_TYPE type, GameObject* go, float3 position_, float3 rotation_, float3 scale_);
	~Component_Transformation();

	//Functions
	void Update();

	

	//Get Variables
	float3 Get_Position();
	float3 Get_Rotation_Axis();
	float3  Get_Angle_Rotation();
	float3 Get_Scale();

private:

	float3 position;
	float3 rotation;
	float3 axis_rotation;
	float3  angle;
	float3 scale;


};


#endif
