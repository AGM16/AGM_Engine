#ifndef _COMPONENTS_H_
#define _COMPONENTS_H_


struct Mesh;

enum COMPONENT_TYPE
{
	TRANSFORMATION = 0,
	MESH,
	MATERIAL
};

class Components
{
public:

	Components(COMPONENT_TYPE type, Mesh* mesh_go);
	~Components();

	bool Enable();
	virtual void Update();
	bool Disable();

	COMPONENT_TYPE Get_Type();
	bool Get_Active_Component();
	Mesh* Get_Mesh_Game_Object();

private:
	
	bool active_component;
	//Create gameobject to get the information of it
	COMPONENT_TYPE type_c;
	Mesh* mesh_game_obj;


};

#endif
