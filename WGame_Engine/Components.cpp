#include "Components.h"

Components::Components(Components_Type type) : active_component(false), type_c(type)
{

}

Components::~Components()
{

}

bool Components::Enable()
{
	active_component = true;
	return active_component;
}
void Components::Update()
{

}

bool Components::Disable()
{
	active_component = false;
	return active_component;
}

bool Components::Is_Active()const
{
	return active_component;
}

Components_Type Components::Get_Type()const
{
	return type_c;
}