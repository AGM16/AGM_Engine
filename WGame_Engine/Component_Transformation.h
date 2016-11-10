#ifndef  _COMPONENT_TRANSFORMATION_H_
#define  _COMPONENT_TRANSFORMATION_H_

#include "Components.h"

class Component_Transformation : public Components
{

public:

	Component_Transformation(Components_Type type, GameObject* game_object, float3 pos,  float3 scale_, Quat rot_quat, float3 angles);
	~Component_Transformation();

	void Update();

	void Set_Position(const math::float3 &pos);
	void Set_Rotation(const math::float3 &rot_degrees);
	void Set_Scale(const math::float3 &scal);

	math::float3 Get_Position()const;
	math::float3 Get_Rotation()const;
	math::float3 Get_Scale()const;
	math::float4x4 Get_Tranformation_Matrix()const;


	math::float4x4 Create_New_Matrix_Transformation(const math::float3 &pos, const math::Quat &q_rotation, const math::float3 &scal);
	void Modify_Children();


	//Checkbox bool
	bool Is_Checkbox_Active()const;
	bool Set_Checkbox(bool on);


private:

	//Coordenates we show in the pannel
	math::float3 position = math::float3::zero;
	math::float3 rotation_degrees = math::float3::zero;
	math::float3 scale = math::float3::one;
	math::Quat quat_rotation = math::Quat::identity;
	math::float3 rotation_radians = math::float3::zero;

	//Last Information of matrix
	math::float4x4 transformation_matrix = math::float4x4::identity;
	math::float4x4 transformation_matrix_to_draw = math::float4x4::identity;

	bool active_checkbox = false;

};
#endif // ! _COMPONENT_TRANSFORMATION_H_

