#ifndef _GAMEOBJECT_H_
#define _GAMEOBJECT_H_

#include <vector>
#include "MathGeoLib\include\MathGeoLib.h"
#include "PugiXml\src\pugixml.hpp"

class Components;
enum Components_Type;
struct Mesh;

class GameObject
{
public:
	GameObject(GameObject* Parent_, const char* name_, int id_number);
	~GameObject();

	//Add Functions
	bool Add_Component_Mesh( Mesh* m, bool active);
	bool Add_Component_Transformation(const math::float3 &pos, const math::float3 &scale_, const math::Quat &rot_quat, const math::float3 &angles, bool active);
	bool Add_Component_Material(const char* name_textu, const char* path_texture, const unsigned int &num_textu,const unsigned int &id_textu,bool is_a_particle_tex, bool active);
	bool Add_Component_Camera(const char* name_camera);
	bool Add_Component_Billboard(bool active);
	bool Add_Component_Emitter(bool active);
	bool Add_Child(GameObject* child);

	void Update_Go_Components();
	bool Exist_Component(Components_Type type)const;
	Components* Get_Component(Components_Type type)const;

	//Getters
	const std::vector<GameObject*>* Get_Children();
	const std::vector<Components*>* Get_List_Components();
	GameObject* Get_Parent()const;
	const char* Get_Name()const;
	float3 Get_Distance_To_Ray()const;

	void Set_Distance_To_Ray(float3 new_distance);
	//Load and save
	bool Load(pugi::xml_node& node);
	bool Save(pugi::xml_node& node)const;

	int Is_Name_Repaeated(const char* name_rep);

	//------------------------Mouse Picking-----------------------------
	bool Check_Collision_With_RayCast(const LineSegment& ray, float& min_distance);
	void GO_Candidates_Raycast(GameObject* go, const LineSegment& ray, std::vector<GameObject*>& list_go);

	//------------------------Quadtree-----------------------------
	void Insert_To_Quadtree();

private:

	GameObject* Parent = nullptr;
	std::vector<GameObject*> children;
    std::vector<Components*> components_list;
	float3 distance_to_ray = float3::zero;
	const char* name = NULL;
	int id = 0;
	int name_repeated = 0;


};

#endif // !_GAMEOBJECT_H_
