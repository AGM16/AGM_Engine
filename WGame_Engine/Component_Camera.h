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
	void Set_Near_Plane(const float &new_near_plane);
	void Set_Far_Plane(const float &new_far_plane);
	void Set_FOV(const float &new_fov);
	void Set_Up(const float3 &new_up);
	void Set_Front(const float3 &new_front);
	void Frustum_Translate(const float3 &position);
	void Set_Draw_Frustum(bool on);

	//Getters
	float* Get_View_Matrix()const;
	float4x4 Get_Project_Matrix()const;
	float Get_Near_Plane()const;
	float Get_Far_Plane()const;
	float Get_Fov()const;
	float3 Get_Up()const;
	float3 Get_Front()const;
	float3 Get_World_Right()const;
	float3 Get_Position()const;
	const math::Frustum Get_Frustum()const;
	bool Get_Draw_Frustum()const;
	bool Get_Deactive_Qaudtree_Draw()const;

	Component_Transformation* Get_Component_Transformation_Camera()const;
	void Look_At(const math::float3 &position);
	bool Intersect_Frustum_AABB(const AABB &b);


	//Check Box that active all the bounding boxes
	//AABB
	void Set_Checkbox_AABB_Bounding_Boxes(bool on);
	bool Get_Checkbox_AABB_Bounding_Boxes()const;
	//OBB
	void Set_Checkbox_OBB_Bounding_Boxes(bool on);
	bool Get_Checkbox_OBB_Bounding_Boxes()const;

private:

	math::Frustum frustum;
	const char* id_camera;
	Component_Transformation* transformation;
	math::float4x4 last_fst_transformation = float4x4::identity;
	math::float3 reference = float3::zero;

	//Frustum Variables
	float near_plane;
	float far_plane;
	float vertical_fov;
	float aspect_ratio;
	bool active_culling = true;
	bool active_all_aabb_bounding_boxes = false;
	bool active_all_obb_bounding_boxes = false;
	bool draw_frustum = false;
	bool deactivate_quadtree_draw = false;

};



#endif
