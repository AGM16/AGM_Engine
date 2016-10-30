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
	GameObject* Create_Camera_Game_Object(GameObject* Parent, const char* name_camera);
	update_status Update(float dt);

	//Functions used to active and deactive components
	void Search_GameObject_To_Active(GameObject* root_go)const;
	void Search_GameObject_To_Deactive(GameObject* root_go)const;

	void Window_Hierarchy(GameObject* Root_node);

	GameObject* Get_Root()const;

private:

	GameObject* root_game_object = nullptr;
	GameObject* game_object_selected = nullptr;
	GameObject* last_game_object_selected = nullptr;

	
	
};


#endif // !_MODULE_GO_MANAGER_H_

