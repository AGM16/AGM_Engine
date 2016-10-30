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

	//Getters
	float4x4 Get_View_Matrix()const;
	float4x4 Get_Project_Matrix()const;
	float Get_Near_Plane()const;
	float Get_Far_Plane()const;
	float Get_Fov()const;


	void Look_At(const math::float3 &position);



private:

	math::Frustum frustum;
	const char* id_camera;
	Component_Transformation* transformation;
	math::float3 last_fst_position;

	//Frustum Variables
	float near_plane = 10.0f;
	float far_plane = 80.0f;
	float vertical_fov = 40.f;
	float aspect_ratio = 0.f;

};



#endif
