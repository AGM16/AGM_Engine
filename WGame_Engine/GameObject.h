#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include <vector>
#include "MathGeoLib\include\MathGeoLib.h"
class Components;
enum Components_Type;
struct Mesh;

class GameObject
{
public:
	GameObject(GameObject* Parent_, const char* name_);
	~GameObject();
	//Add Functions
	bool Add_Component_Mesh( Mesh* m);
	bool Add_Component_Transformation(math::float3 pos, math::float3 scale_, math::Quat rot_quat, math::float3 angles);
	bool Add_Component_Material(const char* name_textu, const char* path_texture, unsigned int num_textu, unsigned int id_textu);
	bool Add_Child(GameObject* child);

	void Update_Go_Components();
	bool Exist_Component(Components_Type type);
	Components* Get_Component(Components_Type type);

	//Getters
	std::vector<GameObject*>* Get_Children();
	std::vector<Components*>* Get_Components();
	GameObject* Get_Parent()const;
	const char* Get_Name()const;

private:

	//Variables
	GameObject* Parent = nullptr;
	std::vector<GameObject*> children;
    std::vector<Components*> components_list;
	const char* name = NULL;
	bool Open_components_panel = true;

};

#endif // !_GAMEOBJECT_H_
