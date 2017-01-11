#ifndef _COMPONENT_BILLBOARD_H_
#define _COMPONENT_BILLBOARD_H_

#include "Components.h"
#include "MathGeoLib\include\MathGeoLib.h"

class Component_Billboard : public Components
{
public:

	Component_Billboard(Components_Type type, GameObject* game_object);
	~Component_Billboard();

	void Update_BillBoard(const float3 look_to_point, const float3 up_vector);


private:

	float3 localForward = float3(0, 0, 1);
	float3 localUp = float3(0, 1, 0);

};

#endif // !_COMPONENT_MESH_H_