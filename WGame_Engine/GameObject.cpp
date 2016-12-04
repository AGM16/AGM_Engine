#include "GameObject.h"
#include "Application.h"
#include "Components.h"
#include "Component_Mesh.h"
#include "Component_Transformation.h"
#include "Component_Material.h"
#include "Component_Camera.h"
#include "p2Defs.h"
#include "Imgui\imgui.h"
#include "Module_Go_Manager.h"

using namespace std;

GameObject::GameObject(GameObject* Parent_, const char* name_, int id_number): Parent(Parent_), name(name_), id(id_number)
{

}

GameObject::~GameObject()
{
	for (vector<Components*>::iterator compo = components_list.begin(); compo != components_list.end(); ++compo)
	{
		(*compo)->Clean_Up();
		delete (*compo);
		(*compo) = nullptr;
	}
	components_list.clear();

	for (vector<GameObject*>::iterator child = children.begin(); child != children.end(); ++child)
	{
		delete (*child);
		(*child) = nullptr;
	}

	children.clear();

	Parent = nullptr;
}

bool GameObject::Add_Component_Mesh(Mesh* m)
{

	if (Exist_Component(MESH) == false)
	{
		Components* new_component = new Component_Mesh(MESH, this , m);
		LOG("The GameObject %s component %s has been created", name, "MESH");

		components_list.push_back(new_component);
		return true;
	}

	return false;
}

bool GameObject::Add_Component_Transformation(const math::float3 &pos, const math::float3 &scale_, const math::Quat &rot_quat, const math::float3 &angles)
{
	if (Exist_Component(TRANSFORMATION) == false)
	{
		Components* new_component = new Component_Transformation(TRANSFORMATION, this, pos, scale_, rot_quat, angles);
		LOG("The GameObject %s component %s has been created", this->name, "TRANSFORMATION");

		components_list.push_back(new_component);
		return true;
	}

	return false;
}


bool GameObject::Add_Component_Material(const char* name_textu, const char* path_texture, const unsigned int &num_textu, const unsigned int &id_textu)
{
	if (Exist_Component(MATERIAL) == false)
	{
		Components* new_component = new Component_Material(MATERIAL, this, name_textu, path_texture, num_textu, id_textu);
		LOG("The GameObject %s component %s has been created", this->name, "MATERIAL");

		components_list.push_back(new_component);
		return true;
	}

	return false;
}

bool GameObject::Add_Component_Camera(const char* name_camera)
{
	if (Exist_Component(CAMERA) == false)
	{
		Components* new_component = new Component_Camera(CAMERA, this, name_camera);
		LOG("The GameObject %s component %s has been created", this->name, "CAMERA");

		components_list.push_back(new_component);
		return true;
	}
}

bool GameObject::Add_Child(GameObject* child)
{
	if (child != nullptr)
	{
		children.push_back(child);
		LOG("The GameObject %s is the child of GameObject %s", child->name, name);
		return true;
	}
	else
	{
		LOG("The GameObject child is NULL");
	}

	return false;
}

void GameObject::Update_Go_Components()
{
	if (children.size() > 0)
	{
		vector<GameObject*>::iterator node_children = children.begin();
		while (node_children != children.end())
		{
			(*node_children)->Update_Go_Components();
			node_children++;
		}
	}

	if (components_list.size() > 0)
	{
		vector<Components*>::iterator node_comp = components_list.begin();
		
		ImVec2 size_w;
		float2 pos;
		if (App->Get_Windows_Resized() == false)
		{
			size_w = ImVec2(290.f, 310.f);
			pos = float2(290, 20);
		}
		else
		{
			size_w = ImVec2(380.f, 620.f);
			pos = float2(382, 20);
		}
		ImGui::SetNextWindowPos(ImVec2(App->window->Get_Screen_size().x - pos.x, pos.y));
		ImGui::SetNextWindowSize(size_w);
		ImGui::Begin("Components");
		while (node_comp != components_list.end())
		{
		    (*node_comp)->Update();
			
			node_comp++;
		}
		
		ImGui::End();
	}
}

bool GameObject::Exist_Component(Components_Type type)const
{
	if (components_list.size() > 0)
	{
		vector<Components*>::const_iterator node_com = components_list.begin();

		while (node_com != components_list.end())
		{
			if ((*node_com)->Get_Type() == type)
			{
				return true;
			}

			node_com++;
		}
	}

	return false;
}


Components* GameObject::Get_Component(Components_Type type)const
{
	
	    vector<Components*>::const_iterator itme_component = components_list.begin();

		while (itme_component != components_list.end())
		{
			if ((*itme_component)->Get_Type() == type)
			{
				return (*itme_component);
			}

			itme_component++;
		}
	

	return nullptr;
}

const vector<GameObject*>* GameObject::Get_Children()
{
	return &children;
}

const vector<Components*>* GameObject::Get_List_Components()
{
	return &components_list;
}

GameObject* GameObject::Get_Parent()const
{
	return Parent;
}

const char* GameObject::Get_Name()const
{
	return name;
}

float3 GameObject::Get_Distance_To_Ray()const
{
	return distance_to_ray;
}

void GameObject::Set_Distance_To_Ray(float3 new_distance)
{
	distance_to_ray = new_distance;
}


bool GameObject::Load(pugi::xml_node& node)
{
	Component_Transformation* transform = (Component_Transformation*)Get_Component(TRANSFORMATION);
	Component_Mesh* mesh = (Component_Mesh*)Get_Component(MESH);
	Component_Material* material = (Component_Material*)Get_Component(MATERIAL);
	Component_Camera* comp_camera = nullptr;

	//Check if the GO is the camera
	if (Exist_Component(CAMERA) == true)
		comp_camera = (Component_Camera*)Get_Component(CAMERA);


	pugi::xml_node next_node;

	
	char node_go[100];
	sprintf_s(node_go, 100, "%s_%s", "GameObject", Get_Name());

	//Have access to the child
	next_node = node.child(node_go);
	

	id = next_node.attribute("id").as_int();

	//----------------TRANSFORMATION-----------------------
	//Position
	float pos_x = next_node.attribute("pos_x").as_float();
	float pos_y = next_node.attribute("pos_y").as_float();
	float pos_z = next_node.attribute("pos_z").as_float();
	transform->Set_Position(math::float3(pos_x, pos_y, pos_z));

	if (Exist_Component(CAMERA) == false)
	{
		//Rotation
		float rot_x = next_node.attribute("rot_x").as_float();
		float rot_y = next_node.attribute("rot_y").as_float();
		float rot_z = next_node.attribute("rot_z").as_float();
		transform->Set_Rotation(math::float3(rot_x, rot_y, rot_z));

		//Scale
		float scale_x = next_node.attribute("scale_x").as_float();
		float scale_y = next_node.attribute("scale_y").as_float();
		float scale_z = next_node.attribute("scale_z").as_float();
		transform->Set_Scale(math::float3(scale_x, scale_y, scale_z));
	}
	else
	{
		//Camera
		//Up
		vec vec_up = float3::zero;
		vec_up.x = next_node.attribute("Up_x").as_float();
		vec_up.y = next_node.attribute("Up_y").as_float();
		vec_up.z = next_node.attribute("Up_z").as_float();
		comp_camera->Set_Up(vec_up);

		//Front
		vec vec_front = float3::zero;
		vec_front.x = next_node.attribute("Front_x").as_float();
		vec_front.y = next_node.attribute("Front_y").as_float();
		vec_front.z = next_node.attribute("Front_z").as_float();
		comp_camera->Set_Front(vec_front);
	}

	//Checkbox deactive
    bool deactivate_checkbox = next_node.attribute("Checkbox_Deactive_Transform").as_bool();
	transform->Set_Checkbox(deactivate_checkbox);

	//--------------------MESH-----------------------
	string name;
	name = "Root_Game_Objects";
	//Check if the node is RootNode, because his mesh is NULL
	if (name.compare(Get_Name()) != 0 && Exist_Component(CAMERA) == false)
	{
		bool deactivate_checkbox = next_node.attribute("Checkbox_Deactive_Mesh").as_bool();
		mesh->Set_Checkbox(deactivate_checkbox);

		bool wireframe = next_node.attribute("Checkbox_wireframe").as_bool();
		mesh->Set_Checkbox_Wireframe(wireframe);

		//To check if the GO have a mesh
		if (mesh->Get_Mesh()->num_vertices > 0)
		{
			bool aabb = next_node.attribute("Checkbox_AABB").as_bool();
			mesh->Set_Checkbox_AABB(aabb);

			bool obb = next_node.attribute("Checkbox_OBB").as_bool();
			mesh->Set_Checkbox_OBB(obb);
		}
	}

	//--------------------MATERIAL-----------------------
	if (name.compare(Get_Name()) != 0 && Exist_Component(CAMERA) == false)
	{
		bool deactivate_checkbox_material = next_node.attribute("Checkbox_Deactive_Material").as_bool();
		material->Set_Checkbox(deactivate_checkbox_material);
	}

	//--------------------CAMERA--------------------------
	if (Exist_Component(CAMERA) == true)
	{
		//Load Check Box
		bool aabb_box = next_node.attribute("AABB_Cekcbox").as_bool();
		comp_camera->Set_Checkbox_AABB_Bounding_Boxes(aabb_box);

		bool obb_box = next_node.attribute("OBB_Cekcbox").as_bool();
		comp_camera->Set_Checkbox_OBB_Bounding_Boxes(obb_box);

		bool draw_frustum = next_node.attribute("Draw_Frustum").as_bool();
		comp_camera->Set_Draw_Frustum(draw_frustum);
	
	}



	if (children.size() > 0)
	{
		for (vector<GameObject*>::const_iterator node_go = children.begin(); node_go != children.end(); node_go++)
		{
			//if ((*node_go)->Exist_Component(CAMERA) == false)
			(*node_go)->Load(node);
		}
	}
	
	return true;
}

bool GameObject::Save(pugi::xml_node& node)const
{
	
		//First we save the parent and then the child
		Component_Transformation* transform = (Component_Transformation*)Get_Component(TRANSFORMATION);
		Component_Mesh* mesh = (Component_Mesh*)Get_Component(MESH);
		Component_Material* material = (Component_Material*)Get_Component(MATERIAL);
		Component_Camera* comp_camera = nullptr;

		//Check if the GO is the camera
		if (Exist_Component(CAMERA) == true)
			 comp_camera = (Component_Camera*)Get_Component(CAMERA);
		
		char node_go[100];
		sprintf_s(node_go, 100, "%s_%s", "GameObject", Get_Name());
		
		pugi::xml_node node_mgo = node.append_child(node_go);

		//id
		node_mgo.append_attribute("id") = id;

		//----------------TRANSFORMATION-----------------------
		//Save Position
		node_mgo.append_attribute("pos_x") = transform->Get_Position().x;
		node_mgo.append_attribute("pos_y") = transform->Get_Position().y;
		node_mgo.append_attribute("pos_z") = transform->Get_Position().z;

		if (Exist_Component(CAMERA) == false)
		{
			//Save Rotation
			node_mgo.append_attribute("rot_x") = transform->Get_Rotation().x;
			node_mgo.append_attribute("rot_y") = transform->Get_Rotation().y;
			node_mgo.append_attribute("rot_z") = transform->Get_Rotation().z;

			//Save Scale
			node_mgo.append_attribute("scale_x") = transform->Get_Scale().x;
			node_mgo.append_attribute("scale_y") = transform->Get_Scale().y;
			node_mgo.append_attribute("scale_z") = transform->Get_Scale().z;
		}
		else
		{
			//Save Rotation
			//Up
			node_mgo.append_attribute("Up_x") = comp_camera->Get_Up().x;
			node_mgo.append_attribute("Up_y") = comp_camera->Get_Up().y;
			node_mgo.append_attribute("Up_z") = comp_camera->Get_Up().z;

			//Front
			node_mgo.append_attribute("Front_x") = comp_camera->Get_Front().x;
			node_mgo.append_attribute("Front_y") = comp_camera->Get_Front().y;
			node_mgo.append_attribute("Front_z") = comp_camera->Get_Front().z;

		}

		
		//Is the Transform checkbox deactive active?
		node_mgo.append_attribute("Checkbox_Deactive_Transform") = transform->Is_Checkbox_Active();


		//--------------------MESH-----------------------
		string name = "Root_Game_Objects";
		//Check if the node is RootNode, because his mesh is NULL
		if (name.compare(Get_Name()) != 0 && Exist_Component(CAMERA) == false)
		{
			
			//Is the mesh checkbox deactive active?
			node_mgo.append_attribute("Checkbox_Deactive_Mesh") = mesh->Is_Checkbox_Active();
			//Is the mesh checkbox wireframe active?
			node_mgo.append_attribute("Checkbox_wireframe") = mesh->Is_Checkbox_Wireframe_Active();

			if (mesh->Get_Mesh()->num_vertices > 0)
			{
				//Is the mesh checkbox AABB active?
				node_mgo.append_attribute("Checkbox_AABB") = mesh->Is_Checkbox_AABB_Active();
				//Is the mesh checkbox OBB active?
				node_mgo.append_attribute("Checkbox_OBB") = mesh->Is_Checkbox_OBB_Active();
			}
		}

		//--------------------MATERIAL-----------------------
		if (name.compare(Get_Name()) != 0 && Exist_Component(CAMERA) == false)
		{
			node_mgo.append_attribute("Checkbox_Deactive_Material") = material->Is_Checkbox_Active();
		}

		//--------------------CAMERA--------------------------
		if (Exist_Component(CAMERA) == true)
		{
			//Save Check Box
			node_mgo.append_attribute("AABB_Cekcbox") = comp_camera->Get_Checkbox_AABB_Bounding_Boxes();
			node_mgo.append_attribute("OBB_Cekcbox") = comp_camera->Get_Checkbox_OBB_Bounding_Boxes();
			node_mgo.append_attribute("Draw_Frustum") = comp_camera->Get_Draw_Frustum();
		}

		if (children.size() > 0)
		{
			for (vector<GameObject*>::const_iterator node_go = children.begin(); node_go != children.end(); node_go++)
			{
				(*node_go)->Save(node);
			}
		}

	return true;
}

int GameObject::Is_Name_Repaeated(const char* name_rep)
{
	if (children.size() > 0)
	{
		for (vector<GameObject*>::const_iterator node_go = children.begin(); node_go != children.end(); node_go++)
		{
				name_repeated = (*node_go)->Is_Name_Repaeated(name_rep);
		}
	}

	string name_go = Get_Name();
	if (name_go.compare(name_rep) == 0)
	{
		name_repeated++;
	}

	int ret = name_repeated;
	name_repeated = 0;
	return ret;
}


bool GameObject::Check_Collision_With_RayCast(const LineSegment& ray, float& min_distance)
{
	if (Exist_Component(MESH))
	{
		Component_Mesh* comp_mesh = (Component_Mesh*)Get_Component(MESH);
		Component_Transformation* transform = (Component_Transformation*)Get_Component(TRANSFORMATION);
		const Mesh* mesh = comp_mesh->Get_Mesh();

		if (mesh->num_vertices > 0)
		{
			//Transform the ray to the local transform
			LineSegment ray_cast = ray;
			ray_cast.Transform(transform->Get_Tranformation_Matrix().Inverted());

			float intersection_distance = 0;
			vec intersection_point = vec::zero;

			for (int n = 0; n < mesh->num_indices / 3; n++)
			{
				int ind_1 = mesh->indices[n * 3];
				int ind_2 = mesh->indices[n * 3 + 1];
				int ind_3 = mesh->indices[n * 3 + 2];

				//Check if intersect with the triangle
				if (ray_cast.Intersects(Triangle(float3(&mesh->vertices[ind_1]), float3(&mesh->vertices[ind_2]), float3(&mesh->vertices[ind_3])), &intersection_distance, &intersection_point))
				{
					//return true;
					if (intersection_distance < min_distance)
					{
						min_distance = intersection_distance;
						return true;
						
					}
				}
			}
		}
	}
	
		return false;	
}

void GameObject::GO_Candidates_Raycast(GameObject* go, const LineSegment& ray, vector<GameObject*>& list_go)
{
	if (go->Get_Children()->size() > 0)
	{
		for (vector<GameObject*>::const_iterator node_go = go->Get_Children()->begin(); node_go != go->Get_Children()->end(); node_go++)
		{
			GO_Candidates_Raycast((*node_go), ray, list_go);
		}
	}

	//Check if the GameObject have mesh
	if (go->Exist_Component(MESH))
	{
		Component_Mesh* comp_mesh = (Component_Mesh*)go->Get_Component(MESH);

		//If the mesh of the GO have vertices it means that has a bounding box
		if (comp_mesh->Get_Mesh()->num_vertices > 0)
		{
			list_go = App->go_manager->Ray_Intersects_Quadtree_Function(ray);
		}
	}

}

void GameObject::Insert_To_Quadtree()
{
	Component_Transformation* transform = (Component_Transformation*)Get_Component(TRANSFORMATION);
		
	App->go_manager->Insert_Quadtree_Function(*this);
	
	if (children.size() > 0)
	{
		for (vector<GameObject*>::const_iterator child = children.begin(); child != children.end(); child++)
		{
			(*child)->Insert_To_Quadtree();
		}
	}
}