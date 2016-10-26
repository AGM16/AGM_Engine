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

	bool Is_Checkbox_Active()const;
	bool Set_Checkbox(bool on);

private:

	Mesh* mesh = nullptr;
	unsigned int id_image = 0;

	//Booleans use to parents
	bool active_checkbox = false;//Use it to the checkbox
	bool last_active_mesh = false;
	bool last_active_texture = false;
	bool last_active_wireframe = false;
	bool wireframe = false;
};

#endif // !_COMPONENT_MESH_H_

