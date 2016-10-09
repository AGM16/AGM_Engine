#ifndef _GAMEOBJECT_MANAGER_H_
#define _GAMEOBJECT_MANAGER_H_

#include <list>
#include "Module.h"
#include "Globals.h"

class GameObject;
struct Mesh;

class GameObject_Manager : public Module
{
public:

	GameObject_Manager(Application* app, bool start_enabled = true);
	~GameObject_Manager();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	bool Create_Game_Object(Mesh m);
	GameObject* Search_Game_Object(const char* name);
	GameObject* Get_Root_Node();

private:

	GameObject* root_game_obj;

};


#endif // !_GAMEOBJECT_MANAGER_H_

