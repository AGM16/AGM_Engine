#ifndef _MODULECAMERA3D_H_
#define _MODULECAMERA3D_H_

#include "Module.h"
#include "Globals.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include <list>
#include "p2Point.h"
#include "Component_Camera.h"

class GameObject;


#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

enum Direction
{
	GO_RIGHT,
	GO_LEFT,
	GO_UP,
	GO_DOWN
};

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Init();
	update_status Update(float dt);
	bool CleanUp();


	void LookAt(const vec &Spot);
	void Set_Position(const vec &position);


	float* GetViewMatrix();
	float* Get_Projection_Matrix()const;
	Component_Camera* Get_Camera_Component()const;

private:

	GameObject* camera_go = nullptr;
	Component_Camera* camera_component = nullptr;
};

#endif