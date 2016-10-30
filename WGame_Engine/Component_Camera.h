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



private:

	math::Frustum frustum;
	const char* id_camera;
	Component_Transformation* transform;
	math::float3 last_fst_position;

};



#endif
