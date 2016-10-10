#ifndef _COMPONENTS_H_
#define _COMPONENTS_H_


struct Mesh;
class GameObject;

enum COMPONENT_TYPE
{
	TRANSFORMATION = 0,
	MESH,
	MATERIAL
};

class Components
{
public:

	Components(COMPONENT_TYPE type, GameObject* go);
	~Components();

	bool Enable();
	virtual void Update();
	bool Disable();

	COMPONENT_TYPE Get_Type();
	bool Get_Active_Component();
	GameObject* Get_Game_Object();

private:
	
	bool active_component;
	//Create gameobject to get the information of it
	COMPONENT_TYPE type_c;
	GameObject* game_obj_c;


};

#endif
