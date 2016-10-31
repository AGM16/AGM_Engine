#ifndef _COMPONENT_CAMERA_H_
#define _COMPONENT_CAMERA_H_

#include "MathGeoLib\include\MathGeoLib.h"
#include "Components.h"

class Component_Transformation;


class Component_Camera : public Components
{
public:

	Component_Camera(Components_Type type, GameObject* game_object, const char* name_id_camera);
	~Component_Camera();

	void Update();
	void Clean_Up();

	//Initialize Frustum
	void Initialize_Frustum_Components();

	//Setters
	void Set_Near_Plane(float new_near_plane);
	void Set_Far_Plane(float new_far_plane);
	void Set_FOV(float new_fov);
	void Set_Up(float3 new_up);
	void Set_Front(float3 new_front);
	void Frustum_Translate(float3 position);

	//Getters
	float* Get_View_Matrix()const;
	float4x4 Get_Project_Matrix()const;
	float Get_Near_Plane()const;
	float Get_Far_Plane()const;
	float Get_Fov()const;
	float3 Get_Up()const;
	float3 Get_Front()const;
	float3 Get_World_Right()const;


	Component_Transformation* Get_Component_Transformation_Camera()const;

	void Look_At(const math::float3 &position);

    math::float3 reference = float3::zero;

private:

	math::Frustum frustum;
	const char* id_camera;
	Component_Transformation* transformation;
	math::float4x4 last_fst_transformation = float4x4::identity;
	

	//Frustum Variables
	float near_plane;
	float far_plane;
	float vertical_fov;
	float aspect_ratio;

};



#endif
