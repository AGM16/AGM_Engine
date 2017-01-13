#ifndef _MODULE_GO_MANAGER_H_
#define _MODULE_GO_MANAGER_H_

#include "Module.h"
#include "p2QuadTree.h"
#include "Globals.h"
#include "PugiXml\src\pugixml.hpp"

class GameObject;
struct Mesh;

class Module_Go_Manager : public Module
{
public:

	Module_Go_Manager( bool start_enabled = true);
	~Module_Go_Manager();

	GameObject* Create_Game_Object(Mesh* m, GameObject* Parent);
	GameObject* Create_Camera_Game_Object(GameObject* Parent, const char* name_camera);
	GameObject* Create_Empty_Game_Object(const char* name_go, GameObject* Parent);
	GameObject* Create_Particle_Game_Object(const char* name_go, GameObject* Parent);
	update_status Update(float dt);

	//Functions used to active and deactive components
	void Search_GameObject_To_Active(GameObject* root_go)const;
	void Search_GameObject_To_Deactive(GameObject* root_go)const;

	bool Load(pugi::xml_node& node);
	bool Save(pugi::xml_node& node)const;
	

	void Window_Hierarchy(GameObject* Root_node);

	GameObject* Get_Root()const;
	GameObject* Get_Selected_GO()const;

	GameObject* Obtain_GO_By_Raycast(const LineSegment& r, vector<GameObject*> list_go)const;
	std::vector<GameObject*> Collect_GO_Candidates(const math::LineSegment& r)const;

    void Insert_GO_To_Quadtree();

	//--------------Quadtree functions----------------
	void Create_Quadtree_Root_Function(const float2 size_rect, const float2 center);
	bool Insert_Quadtree_Function(GameObject& go);
	bool Clear_Quadtree_Function();
	void Intersect_Camera_Culling_Quadtree_Function(Component_Camera& camera);
	vector<GameObject*> Ray_Intersects_Quadtree_Function(const LineSegment& ray);
	void Set_Boundaries_Quadtree_Root_Function(const float2 size_rect);
	float2 Get_Boundaries_Quadtree_Root_Function()const;
	void Deactivate_GO_Render();

	void Set_Game_Obj_Inserted(bool on);
  
private:

	GameObject* root_game_object = nullptr;
	GameObject* game_object_selected = nullptr;
	GameObject* last_game_object_selected = nullptr;
	p2QuadTree quadtree_go;

	/*To be sure that we only can insert the Go once. If we clear the Quadtree 
	we can creater another Quadtree and insert new go*/
	bool game_obj_Inserted = false;
	
	
};


#endif // !_MODULE_GO_MANAGER_H_

