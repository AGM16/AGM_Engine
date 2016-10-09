#include "Application.h"
#include "GameObject_Manager.h"
#include "GameObject.h"
#include "Components.h"
#include "MathGeoLib\include\MathGeoLib.h"

using namespace std;


GameObject_Manager::GameObject_Manager(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

GameObject_Manager::~GameObject_Manager()
{

}

bool GameObject_Manager::Start()
{
	root_game_obj = new GameObject();

	Mesh m;
	root_game_obj->Add_Component(COMPONENT_TYPE::TRANSFORMATION, m, float3(0.f, 0.f, 0.f), float3(0.f, 0.f, 0.f), float3(0.f, 0.f, 0.f));

	root_game_obj->Get_Name();

	return true;
}

update_status GameObject_Manager::Update(float dt)
{
	//Render Active Components
	root_game_obj->Render_Components_Object();

	return UPDATE_CONTINUE;
}


bool GameObject_Manager::CleanUp()
{
	delete root_game_obj;
	return true;
}

//Create
bool GameObject_Manager::Create_Game_Object(Mesh m)
{
	GameObject* go_parent = nullptr;

	//New GameObject
	if (m.parent != NULL)
	{
		go_parent = Search_Game_Object(m.parent);
	}
	else
	{
		go_parent = root_game_obj;
	}

	GameObject* game_obj = new GameObject(go_parent, m.name_node);

	//Add Components
	game_obj->Add_Component(COMPONENT_TYPE::TRANSFORMATION, m, m.translation, RadToDeg(m.rotation.ToEulerYXZ()), m.scaling);

	if (m.parent != NULL)
	{
		game_obj->Get_Parent()->Add_Child(game_obj);
	}
	else
	{
		root_game_obj = game_obj;
	}



	return true;
}

GameObject* GameObject_Manager::Search_Game_Object(const char* name)
{
	if (root_game_obj->Get_Children().size() > 0)
	{
		list<GameObject*>::iterator item_game_obj = root_game_obj->Get_Children().begin();

		while (item_game_obj != root_game_obj->Get_Children().end())
		{
			if ((*item_game_obj)->Get_Name() == name)
			{
				LOG("Error: The GameObject %s has been found", name);
				return (*item_game_obj);
			}

			item_game_obj++;
		}
	}

	return NULL;
}

GameObject*  GameObject_Manager::Get_Root_Node()
{
	 return root_game_obj;
}

