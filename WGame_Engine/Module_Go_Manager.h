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
	GameObject* Search_parent(char* name);


private:

	GameObject* root_game_object = nullptr;
	

};


#endif // !_MODULE_GO_MANAGER_H_

