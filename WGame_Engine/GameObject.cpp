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

	//Position
	int pos_x = node.attribute("pos_x").as_float();
	int pos_y = node.attribute("pos_y").as_float();
	int pos_z = node.attribute("pos_z").as_float();
	transform->Set_Position(math::float3(pos_x, pos_y, pos_z));

	//Rotation
	int rot_x = node.attribute("rot_x").as_float();
	int rot_y = node.attribute("rot_y").as_float();
	int rot_z = node.attribute("rot_z").as_float();
	transform->Set_Rotation(math::float3(rot_x, rot_y, rot_z));

	//Scale
	int scale_x = node.attribute("scale_x").as_float();
	int scale_y = node.attribute("scale_y").as_float();
	int scale_z = node.attribute("scale_z").as_float();
	transform->Set_Rotation(math::float3(scale_x, scale_y, scale_z));

	if (children.size() > 0)
	{
		for (vector<GameObject*>::const_iterator node_go = children.begin(); node_go != children.end(); node_go++)
		{
			Load(node);
		}
	}
	
	return true;
}

bool GameObject::Save(pugi::xml_node& node)const
{
	
		//First we save the parent and then the child
		Component_Transformation* transform = (Component_Transformation*)Get_Component(TRANSFORMATION);
		pugi::xml_node node_mgo = node.append_child("Module_Go_Manager");

		char node_go[100];
		sprintf_s(node_go, 100, "%s_%d", "GameObject", id);

		node_mgo.append_child(node_go);

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

		if (children.size() > 0)
		{
			for (vector<GameObject*>::const_iterator node_go = children.begin(); node_go != children.end(); node_go++)
			{
				Save(node);
			}
		}

	return true;
}
