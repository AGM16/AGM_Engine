#ifndef _COMPONENT_MESH_H_
#define _COMPONENT_MESH_H_

#include "Components.h"

struct Mesh;
class Component_Transformation;
class Component_Material;


class Component_Mesh : public Components
{
public:

	Component_Mesh(Components_Type type, GameObject* game_object, Mesh* mesh_);
	~Component_Mesh();

	void Update();
	void Clean_Up();

	//Checkbox Deactive bool
	bool Is_Checkbox_Active()const;
	void Set_Checkbox(bool on);

	//Checkbox Wireframe bool
	bool Is_Checkbox_Wireframe_Active()const;
	void Set_Checkbox_Wireframe(bool on);

	//Checkbox AABB bool
	bool Is_Checkbox_AABB_Active()const;
	void Set_Checkbox_AABB(bool on);

	//Checkbox OBB bool
	bool Is_Checkbox_OBB_Active()const;
	void Set_Checkbox_OBB(bool on);

	void Check_Parent_Checkboxes(Component_Mesh* mesh_parent, Component_Material* material_parent, Component_Material* material);
	void Recalculate_Properties_Bounding_Box(Component_Transformation* transformation);

	void Render_Mesh_Panel();
	void Render_Bounding_Box_Panel();

	Mesh* Get_Mesh()const;
	AABB Get_AABB_Bounding_Box()const;
	OBB Get_OBB_Bounding_Box()const;
	
	bool draw = false;

private:

	Mesh* mesh;
	unsigned int id_image = 0;

	//Bounding Box
	AABB bounding_box;
	AABB new_bounding_box;
	OBB obb_box;
	math::float4x4 last_transformation_matrix = math::float4x4::identity;

	//Booleans use to parents
	bool active_checkbox = false;//Use it to the checkbox
	bool last_active_mesh = false;
	bool last_active_texture = false;
	bool last_active_wireframe = false;
	bool wireframe = false;
	bool aabb_checkbox = false;
	bool obb_checkbox = false;

	
};

#endif // !_COMPONENT_MESH_H_

