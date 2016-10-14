#include "GameObject.h"
#include "Application.h"
#include "Components.h"
#include "Component_Mesh.h"
#include "Component_Transformation.h"

using namespace std;

GameObject::GameObject(GameObject* Parent_, const char* name_): Parent(Parent_), name(name_)
{

}

GameObject::~GameObject()
{

}

bool GameObject::Add_Component_Mesh(Mesh* m)
{

	if (Exist_Component(MESH) == false)
	{
		Components* new_component = new Component_Mesh(MESH, this , m);
		LOG("The GameObject %s component %s has been created", name, "MESH");

		components_list.push_back(new_component);
		return true;
	}

	return false;
}

bool GameObject::Add_Component_Transformation(math::float3 pos, math::float3 scale_, math::Quat rot_quat, math::float3 angles)
{
	if (Exist_Component(MESH) == false)
	{
		Components* new_component = new Component_Transformation(TRANSFORMATION, this, pos, scale_, rot_quat, angles);
		LOG("The GameObject %s component %s has been created", this->name, "TRANSFORMATION");

		components_list.push_back(new_component);
		return true;
	}

	return false;
}

bool GameObject::Add_Child(GameObject* child)
{
	if (child != NULL)
	{
		children.push_back(child);
		LOG("The GameObject %s is the child of GameObject %s", child->name, name);
		return true;
	}
	else
	{
		LOG("The GameObject child is NULL");
	}

	return false;
}

void GameObject::Update_Go_Components()
{
	if (children.size() > 0)
	{
		list<GameObject*>::iterator node_children = children.begin();
		while (node_children != children.end())
		{
			(*node_children)->Update_Go_Components();
			node_children++;
		}
	}

	if (components_list.size() > 0)
	{
		list<Components*>::iterator node_comp = components_list.begin();
		ImGui::Begin("Components",&Open_components_panel);
		while (node_comp != components_list.end())
		{
		    (*node_comp)->Update();
			
			node_comp++;
		}
		
		ImGui::End();
	}
}

bool GameObject::Exist_Component(Components_Type type)
{
	list<Components*>::iterator node_com = components_list.begin();

	while (node_com != components_list.end())
	{
		if ((*node_com)->Get_Type() == type)
		{
			LOG("The GameObject %s has already a %s component", name, (char*)type);
			return true;
		}

		node_com++;
	}

	return false;
}


Components* GameObject::Get_Component(Components_Type type)
{
	
		list<Components*>::iterator itme_component = components_list.begin();

		while (itme_component != components_list.end())
		{
			if ((*itme_component)->Get_Type() == type)
			{
				LOG("The GameObject %s has already a %s component", name, (char*)type);
				return (*itme_component);
			}

			itme_component++;
		}
	

	return NULL;
}

std::list<GameObject*>* GameObject::Get_Children()
{
	return &children;
}

std::list<Components*>* GameObject::Get_Components()
{
	return &components_list;
}

GameObject* GameObject::Get_Parent()const
{
	return Parent;
}

const char* GameObject::Get_Name()const
{
	return name;
}