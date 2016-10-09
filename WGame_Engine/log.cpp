
#include "Globals.h"
#include "Application.h"

void log(const char file[], int line, const char* format, ...)
{
	static char tmp_string[4096];
	static char tmp_string2[4096];
	static va_list  ap;

	
	// Construct the string from variable arguments
	va_start(ap, format);
	vsprintf_s(tmp_string, 4096, format, ap);
	va_end(ap);
	sprintf_s(tmp_string2, 4096, "\n%s(%d) : %s", file, line, tmp_string);
	OutputDebugString(tmp_string2);

	if (App)
	{
		App->Log_Console(tmp_string2);
	}
	
}

/*using namespace std;

GameObject_Manager::GameObject_Manager(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

GameObject_Manager::~GameObject_Manager()
{

}

bool GameObject_Manager::Start()
{
	root_game_obj = new GameObject(nullptr, "Empty_Root_Node");
	root_game_obj->Add_Component(COMPONENT_TYPE::TRANSFORMATION, float3(0.f, 0.f, 0.f), float3(0.f, 0.f, 0.f), float3(0.f, 0.f, 0.f));

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
	game_obj->Add_Component(COMPONENT_TYPE::TRANSFORMATION, m.translation, RadToDeg(m.rotation.ToEulerYXZ()), m.scaling);

	if (m.parent != NULL)
	{
		game_obj->Get_Parent()->Add_Child(game_obj);
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

*/