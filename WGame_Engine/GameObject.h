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
	bool Add_Component_Transformation(const math::float3 &pos, const math::float3 &scale_, const math::Quat &rot_quat, const math::float3 &angles);
	bool Add_Component_Material(const char* name_textu, const char* path_texture, const unsigned int &num_textu,const unsigned int &id_textu);
	bool Add_Component_Camera(const char* name_camera);
	bool Add_Child(GameObject* child);

	void Update_Go_Components();
	bool Exist_Component(Components_Type type)const;
	Components* Get_Component(Components_Type type)const;

	//Getters
	const std::vector<GameObject*>* Get_Children();
	const std::vector<Components*>* Get_List_Components();
	GameObject* Get_Parent()const;
	const char* Get_Name()const;

private:

	GameObject* Parent = nullptr;
	std::vector<GameObject*> children;
    std::vector<Components*> components_list;
	const char* name = NULL;

};

#endif // !_GAMEOBJECT_H_
