#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include <string>
#include <list>
#include "MathGeoLib\include\MathGeoLib.h"

class Components;
class Component_Transformation;
enum COMPONENT_TYPE;


struct Mesh;

class GameObject
{
public:

	GameObject();
	GameObject(GameObject* parent_, const char* name_);
	~GameObject();

	//Components
	bool Add_Component(COMPONENT_TYPE type, GameObject* go, Mesh mesh_go, float3 position_, float3 rotation_, float3 scale_);
	/*bool Delete_Component(COMPONENT_TYPE type);*/
	bool Active_Component(COMPONENT_TYPE type);

	//Gameobject Childs
	bool Add_Child(GameObject* child);
	//bool Delete_Child(GameObject* child);

	//Getter and setters
	void Set_Name(const char* new_name);
	const char* Get_Name()const;
	GameObject* Get_Parent();

	void Render_Components_Object();
	Component_Transformation* Get_Component_Transformation();//To pass the information to the function draw
	std::list<GameObject*> Get_Children();


private:

	GameObject* parent;
	const char* name;
	std::list<Components*> componets_go;
	std::list<GameObject*> children;

};

#endif // !

