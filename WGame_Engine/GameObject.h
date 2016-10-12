#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include <list>
class Components;
enum Components_Type;
struct Mesh;

class GameObject
{
public:
	GameObject(GameObject* Parent_, char* name_);
	~GameObject();

	bool Add_Component_Mesh( Mesh* m);
	bool Add_Component_Transformation(Mesh* m);
	bool Add_Child(GameObject* child);

	void Update_Go_Components();
	bool Exist_Component(Components_Type type);
	Components* Get_Component(Components_Type type);


	std::list<GameObject*>* Get_Children();
	std::list<Components*>* Get_Components();
	GameObject* Get_Parent()const;
	char* Get_Name()const;

	
	

private:

	//Variables
	GameObject* Parent = nullptr;
	std::list<GameObject*> children;
    std::list<Components*> components_list;
	char* name = NULL;

};

#endif // !_GAMEOBJECT_H_
