#include "Component_Camera.h"
#include "Application.h"
#include "Components.h"
#include "Component_Transformation.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include "GameObject.h"


Component_Camera::Component_Camera(Components_Type type, GameObject* game_object, const char* name_id_camera) : Components(type, game_object), id_camera(name_id_camera)
{
	Initialize_Frustum_Components();
	transform = (Component_Transformation*)Get_Game_Object()->Get_Component(TRANSFORMATION);
}

Component_Camera::~Component_Camera()
{

}


void Component_Camera::Update()
{
	if (last_fst_position.Equals(transform->Get_Position()) == false)
	{
		frustum.SetPos(transform->Get_Position());
		last_fst_position = transform->Get_Position();
	}

	//Render Camera
	App->renderer3D->Render_Frustum_Cube(frustum);
}

void Component_Camera::Clean_Up()
{

}

void Component_Camera::Initialize_Frustum_Components()
{
	//Initialize Components Frustum
	frustum.SetKind(FrustumSpaceGL, FrustumLeftHanded);
	frustum.SetPos(float3::zero);
	frustum.SetFront(float3::unitZ);
	frustum.SetUp(float3::unitY);

	frustum.SetViewPlaneDistances(10.f, 80.f);
	frustum.SetPerspective(DegToRad(60.f), DegToRad(60.f));

}