#include "GameObject.h"
#include "Application.h"
#include "Components.h"
#include "Component_Mesh.h"
#include "Component_Transformation.h"
#include "Component_Material.h"
#include "Component_Camera.h"
#include "p2Defs.h"
#include "Imgui\imgui.h"

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


bool GameObject::Load(pugi::xml_node& node)
{
	Component_Transformation* transform = (Component_Transformation*)Get_Component(TRANSFORMATION);
	Component_Mesh* mesh = (Component_Mesh*)Get_Component(MESH);
	Component_Material* material = (Component_Material*)Get_Component(MATERIAL);
	pugi::xml_node next_node;
	string name = "Module_Go_Manager";

	//Check if the node is Module_Go_Manager to obtain the fist child
	if(name.compare(node.name()) == 0)
	{
		//Have access to the first child
		next_node = node.child("GameObject");
	}
	else
	{
		//Obtain the other gameobjects nodes
		next_node = node.next_sibling("GameObject");
	}

	id = next_node.attribute("id").as_int();

	//----------------TRANSFORMATION-----------------------
	//Position
	float pos_x = next_node.attribute("pos_x").as_float();
	float pos_y = next_node.attribute("pos_y").as_float();
	float pos_z = next_node.attribute("pos_z").as_float();
	transform->Set_Position(math::float3(pos_x, pos_y, pos_z));

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

	//Checkbox deactive
    bool deactivate_checkbox = next_node.attribute("Checkbox_Deactive_Transform").as_bool();
	transform->Set_Checkbox(deactivate_checkbox);

	//--------------------MESH-----------------------
	name.clear();
	name = "Root_Game_Objects";
	//Check if the node is RootNode, because his mesh is NULL
	if (name.compare(Get_Name()) != 0)
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
	if (name.compare(Get_Name()) != 0)
	{
		bool deactivate_checkbox_material = next_node.attribute("Checkbox_Deactive_Material").as_bool();
		material->Set_Checkbox(deactivate_checkbox_material);
	}



	if (children.size() > 0)
	{
		for (vector<GameObject*>::const_iterator node_go = children.begin(); node_go != children.end(); node_go++)
		{
			if ((*node_go)->Exist_Component(CAMERA) == false)
			(*node_go)->Load(next_node);
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
		
		char node_go[100];
		sprintf_s(node_go, 100, "%s_%d", "GameObject", id);
		
		pugi::xml_node node_mgo = node.append_child("GameObject");

		//id
		node_mgo.append_attribute("id") = id;

		//----------------TRANSFORMATION-----------------------
		//Save Position
		node_mgo.append_attribute("pos_x") = transform->Get_Position().x;
		node_mgo.append_attribute("pos_y") = transform->Get_Position().y;
		node_mgo.append_attribute("pos_z") = transform->Get_Position().z;

		//Save Rotation
		node_mgo.append_attribute("rot_x") = transform->Get_Rotation().x;
		node_mgo.append_attribute("rot_y") = transform->Get_Rotation().y;
		node_mgo.append_attribute("rot_z") = transform->Get_Rotation().z;

		//Save Scale
		node_mgo.append_attribute("scale_x") = transform->Get_Scale().x;
		node_mgo.append_attribute("scale_y") = transform->Get_Scale().y;
		node_mgo.append_attribute("scale_z") = transform->Get_Scale().z;

		
		//Is the Transform checkbox deactive active?
		node_mgo.append_attribute("Checkbox_Deactive_Transform") = transform->Is_Checkbox_Active();


		//--------------------MESH-----------------------
		string name = "Root_Game_Objects";
		//Check if the node is RootNode, because his mesh is NULL
		if (name.compare(Get_Name()) != 0)
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
		if (name.compare(Get_Name()) != 0)
		{
			node_mgo.append_attribute("Checkbox_Deactive_Material") = material->Is_Active();
		}


		if (children.size() > 0)
		{
			for (vector<GameObject*>::const_iterator node_go = children.begin(); node_go != children.end(); node_go++)
			{
				if((*node_go)->Exist_Component(CAMERA) == false)
				(*node_go)->Save(node);
			}
		}

	return true;
}
