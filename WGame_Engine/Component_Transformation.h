#ifndef  _COMPONENT_TRANSFORMATION_H_
#define  _COMPONENT_TRANSFORMATION_H_

#include "Components.h"

class Component_Transformation : public Components
{

public:
	Component_Transformation(Components_Type type, GameObject* game_object, float3 pos,  float3 scale_, Quat rot_quat, float3 angles);
	~Component_Transformation();

	void Update();

	void Set_Position();
	void Set_Rotation();
	void Set_Scale();

	math::float3 Get_Position()const;

	math::float4x4 Create_New_Matrix_Transformation();
	math::float4x4 Get_Tranformation_Matrix()const;


private:

	//Coordenates we show in the pannel
	math::float3 position = math::float3::zero;
	math::float3 rotation_degrees = math::float3::zero;
	math::float3 scale = math::float3::zero;
	math::Quat quat_rotation = math::Quat::identity;

	math::float3 rotation_radians = math::float3::zero;

	//Last Information of matrix
	math::float4x4 transformation_matrix = math::float4x4::identity;

	bool start = true;

};
#endif // ! _COMPONENT_TRANSFORMATION_H_

