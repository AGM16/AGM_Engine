#include "Components.h"
#include "Application.h"


Components::Components(COMPONENT_TYPE type, Mesh* mesh_go): active_component(true), type_c(type), mesh_game_obj(mesh_go)
{

}

Components::~Components()
{

}

bool Components::Enable()
{
	if(active_component == false)
	active_component = true;

	return active_component;
}

void Components::Update()
{
	

}

bool Components::Disable()
{
	if (active_component == true)
		active_component = true;

	return active_component;
}

COMPONENT_TYPE Components::Get_Type()
{
	return type_c;
}

bool Components::Get_Active_Component()
{
	return active_component;
}

Mesh* Components::Get_Mesh_Game_Object()
{
	return mesh_game_obj;
}
