#ifndef _COMPONENT_MESH_H_
#define _COMPONENT_MESH_H_

#include "Components.h"

struct Mesh;


class Component_Mesh : public Components
{
public:

	Component_Mesh(Components_Type type, GameObject* game_object, Mesh* mesh_);
	~Component_Mesh();

	void Update();
	void Clean_Up();

public:

	Mesh* mesh = nullptr;
	bool active = false;
	bool last_active_mesh = false;
	bool last_active_texture = false;
	unsigned int id_image = 0;
};

#endif // !_COMPONENT_MESH_H_

