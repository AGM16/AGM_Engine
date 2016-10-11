#include "GameObject.h"
#include "Application.h"
#include "Components.h"
#include "Component_Mesh.h"

using namespace std;

GameObject::GameObject(GameObject* Parent_, char* name_): Parent(Parent_), name(name_)
{

}

GameObject::~GameObject()
{

}

bool GameObject::Add_Component_Mesh(Mesh* m)
{
	bool ret = true;
	if (components_list.size() > 0)
	{
		ret = Exist_Component(MESH);
	}

	if (ret)
	{
		Components* new_component = new Component_Mesh(MESH,m);
		LOG("The GameObject %s component %s has been creqated", name, (char*)MESH);

		components_list.push_back(new_component);
	}

	return ret;
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
		ImGui::Begin("Components");
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
			return false;
		}

		node_com++;
	}

	return true;
}