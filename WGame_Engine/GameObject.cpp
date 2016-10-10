#include "GameObject.h"
#include "Components.h"
#include "Application.h"
#include "Component_Transformation.h"
#include "Component_Mesh.h"



using namespace std;

GameObject::GameObject() : name("Root_Node_GameObject"), parent(NULL)
{

}


GameObject::GameObject(GameObject* parent_, const char* name_): parent(parent_), name(name_)
{

}

GameObject::~GameObject()
{

}

void GameObject::Render_Components_Object()
{

	if (children.size() > 0)
	{
		list<GameObject*>::iterator item_chilren_go = children.begin();
		while (item_chilren_go != children.end())
		{
            Render_Components_Object();
			item_chilren_go++;
		}
		
	}
	if (componets_go.size() > 0)
	{
		list<Components*>::iterator item_comp = componets_go.begin();

		ImGui::Begin("Components");

		(*item_comp)->Update();

		ImGui::End();
	}
	
}

//Components
bool GameObject::Add_Component(COMPONENT_TYPE type, GameObject* go, Mesh mesh_go, float3 position_, float3 rotation_, float3 scale_)
{
	//Check if this component is already done
	if (componets_go.size() > 0)
	{		
		list<Components*>::iterator item_comp = componets_go.begin(); 
		
		while (item_comp != componets_go.end())
		{
			if ((*item_comp)->Get_Type() == type)
			{
				LOG("Error: The component %s is already done", (char*)type);
				return false;
			}

			item_comp++;
		}

	}
	
	//Create the new component
		Components* new_component = nullptr;

		if (type == TRANSFORMATION)
		{
			new_component = new Component_Transformation(type,go,position_,rotation_,scale_);
			LOG("Error: The component %s has been created", (char*)type);
		}
		if (type == MESH)
		{
			new_component = new Component_Mesh(type, go, &mesh_go);
			LOG("Error: The component %s has been created", (char*)type);
		}

		componets_go.push_back(new_component);

	

	return true;
}

/*bool GameObject::Delete_Component(COMPONENT_TYPE type)
{

	return true;
}*/

bool GameObject::Active_Component(COMPONENT_TYPE type)
{
	if (componets_go.size() > 0)
	{
		list<Components*>::iterator item_comp = componets_go.begin();

		while (item_comp != componets_go.end())
		{
			if ((*item_comp)->Get_Type() == type)
			{
				(*item_comp)->Enable();
				LOG("Error: The component %s is enable", (char*)type);
				return true;
			}

			item_comp++;
		}
	}

	LOG("Error: The component %s hasn't found", (char*)type);
	return false;

}

//Gameobject Childs
bool GameObject::Add_Child(GameObject* child)
{
	if (child != NULL)
	{
		child->parent = this;
		children.push_back(child);
		LOG("Error: The child %s has a new parent %s", child->name, name);
	}
	else
	{
		LOG("Error: The child %s doesn't exist", child->name, name);
		return false;
	}

	return true;
}

/*bool GameObject::Delete_Child(GameObject* child)
{

	return true;
}*/


void GameObject::Set_Name(const char* new_name)
{
	if (new_name == NULL)
		name = "Wihtout_Name";
	else
		name = new_name;
}

const char* GameObject::Get_Name()const
{
	return name;
}

GameObject* GameObject::Get_Parent()
{
	return parent;
}

Component_Transformation* GameObject::Get_Component_Transformation()
{
	if (componets_go.size() > 0)
	{
		list<Components*>::iterator item_comp = componets_go.begin();

		while (item_comp != componets_go.end())
		{
			if ((*item_comp)->Get_Type() == TRANSFORMATION)
			{

				LOG("The component %s of %s has been found", (char*)TRANSFORMATION, name);
				return (Component_Transformation*)(*item_comp);
			}

			item_comp++;
		}
	}

	LOG("Error: The component %s of %s hasn't found", (char*)TRANSFORMATION, name);
	return NULL;
}

list<GameObject*> GameObject::Get_Children()
{
	return children;
}