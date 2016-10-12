#include "Application.h"
#include "Module_Go_Manager.h"
#include "GameObject.h"

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
			//Add Component
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

	if (root_game_object->Get_Children().size() > 0)
	{
		list<GameObject*>::iterator node_game_obj = root_game_object->children.begin();

		while (node_game_obj != root_game_object->children.end())
		{
			//Render Components
			(*node_game_obj)->Update_Go_Components();
			node_game_obj++;
		}

	}


	return UPDATE_CONTINUE;
}

GameObject* Module_Go_Manager::Search_parent(char* name)
{
	/*if (root_game_object->Get_Children().size() > 0)
	{
		list<GameObject*>::iterator node_game_obj = root_game_object->Get_Children().begin();

		while (node_game_obj != root_game_object->Get_Children().end())
		{
			if ((*node_game_obj)->Get_Children().size() > 0)
			{

			}
			node_game_obj++;
		}

	}*/

	return NULL;
}