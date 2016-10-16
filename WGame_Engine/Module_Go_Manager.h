#ifndef _MODULE_GO_MANAGER_H_
#define _MODULE_GO_MANAGER_H_

#include "Module.h"
#include "Globals.h"

class GameObject;
struct Mesh;

class Module_Go_Manager : public Module
{
public:

	Module_Go_Manager(Application* app, bool start_enabled = true);
	~Module_Go_Manager();


	GameObject* Create_Game_Object(Mesh* m, GameObject* Parent);
	update_status Update(float dt);
	void Search_GameObject_To_Active( GameObject* root_go);
	void Search_GameObject_To_Disactive( GameObject* root_go);

	void Window_Hierarchy(GameObject* Root_node);


private:

	GameObject* root_game_object = nullptr;
	GameObject* game_object_selected = nullptr;
	GameObject* last_game_object_selected = nullptr;
	bool open_hierarchy_window = true;
	
	
};


#endif // !_MODULE_GO_MANAGER_H_

