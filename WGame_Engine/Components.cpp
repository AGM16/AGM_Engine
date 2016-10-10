#include "Components.h"
#include "Application.h"


Components::Components(COMPONENT_TYPE type, GameObject* go): active_component(true), type_c(type), game_obj_c(go)
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

GameObject* Components::Get_Game_Object()
{
	return game_obj_c;
}
