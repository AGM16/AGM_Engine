#include "Application.h"
#include "Module_Go_Manager.h"
#include "GameObject.h"
#include "Components.h"

using namespace std;

Module_Go_Manager::Module_Go_Manager(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	root_game_object = new GameObject(NULL, "Root_Game_Object");
}

Module_Go_Manager::~Module_Go_Manager()
{

}


GameObject* Module_Go_Manager::Create_Game_Object(Mesh* m, GameObject* Parent)
{
	if (Parent == NULL)
	{
		Parent = root_game_object;
    }
		
		GameObject* new_game_object = new GameObject(Parent, m->name_node);

		//Add Child to the parent
		new_game_object->Get_Parent()->Add_Child(new_game_object);
		LOG("The GameObject %s has a new child : %s ", new_game_object->Get_Parent()->Get_Name(), new_game_object->Get_Name());

	if (m != NULL)
	{
			//Add Components Transformation
			new_game_object->Add_Component_Transformation(m);
			LOG("The GameObject %s has a new component : %s ", new_game_object->Get_Name(), "TRANSFORMATION");
			
			//Add Components Mesh
			new_game_object->Add_Component_Mesh(m);
			LOG("The GameObject %s has a new component : %s ", new_game_object->Get_Name(), "MESH");

	}
	else
	{
		LOG("The GameObject mesh is empty ");
	}

	return new_game_object;


}

update_status Module_Go_Manager::Update(float dt)
{

	if (root_game_object->Get_Children()->size() > 0)
	{
		list<GameObject*>::iterator node_game_obj = root_game_object->Get_Children()->begin();

		while (node_game_obj != root_game_object->Get_Children()->end())
		{
			//Render Components
			(*node_game_obj)->Update_Go_Components();
			node_game_obj++;
		}

	}

	
	ImGui::Begin("Hierarchy", &open_hierarchy_window);
	if (root_game_object->Get_Children()->size() > 0)
	{
		Window_Hierarchy(root_game_object);
	}
	ImGui::End();

	return UPDATE_CONTINUE;
}

void Module_Go_Manager::Window_Hierarchy(GameObject* Root_node)
{

	for (list<GameObject*>::iterator node_go = Root_node->Get_Children()->begin(); node_go != Root_node->Get_Children()->end(); node_go++)
	{
		uint flags = 0;
		if ((*node_go) == game_object_selected)
			flags = ImGuiTreeNodeFlags_Selected;

		if ((*node_go)->Get_Children()->size() > 0)
		{
			if (ImGui::TreeNodeEx((*node_go)->Get_Name(), flags))
			{
				if (ImGui::IsItemClicked())
				{   
					if (game_object_selected != NULL)
					{
						Search_GameObject_To_Disactive(game_object_selected);
					}
					game_object_selected = (*node_go);
					Search_GameObject_To_Active( game_object_selected);

					last_game_object_selected = game_object_selected;
				}

				Window_Hierarchy((*node_go));
				ImGui::TreePop();
			}
		}
		else
		{
			if (ImGui::TreeNodeEx((*node_go)->Get_Name(), flags | ImGuiTreeNodeFlags_Leaf))
			{
				if (ImGui::IsItemClicked())
				{
					if (game_object_selected != NULL)
					{
						Search_GameObject_To_Disactive(game_object_selected);
					}
					game_object_selected = (*node_go);
					Search_GameObject_To_Active( game_object_selected);

					last_game_object_selected = game_object_selected;
				}

				ImGui::TreePop();
			}
		}
	}

}

void Module_Go_Manager::Search_GameObject_To_Active( GameObject* root_go)
{
	list<Components*>::iterator node_comp = root_go->Get_Components()->begin();
	while (node_comp != root_go->Get_Components()->end())
	{
		(*node_comp)->Enable();
		node_comp++;
	}
}

void Module_Go_Manager::Search_GameObject_To_Disactive( GameObject* root_go)
{
	if (last_game_object_selected != NULL)
	{
		list<Components*>::iterator node_comp = last_game_object_selected->Get_Components()->begin();
		while (node_comp != last_game_object_selected->Get_Components()->end())
		{
			(*node_comp)->Disable();
			node_comp++;
		}
	}
	
}