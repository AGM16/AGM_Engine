#include "Application.h"
#include "Module_Go_Manager.h"
#include "GameObject.h"
#include "Components.h"
#include "Component_Mesh.h"
#include "Component_Transformation.h"
#include "Component_Material.h"
#include "Random.h"
#include "p2Defs.h"
#include "p2QuadTree.h"

using namespace std;

Module_Go_Manager::Module_Go_Manager( bool start_enabled) : Module(start_enabled)
{
	Set_Name_Module("Module_Go_Manager");
	Random rand;
	root_game_object = new GameObject(nullptr, "Root_Game_Objects", rand.Random_int(0, 2147483647));
	root_game_object->Add_Component_Transformation(float3::zero, float3::one, Quat::identity, float3::zero, false);
	root_game_object->Add_Component_Material("", "", 0, 0, false,false);
	
	quadtree_go.Create(float2(100.f, 100.f), float2(0.f, 0.f));
}

Module_Go_Manager::~Module_Go_Manager()
{
	if (root_game_object)
		RELEASE(root_game_object);

	game_object_selected = nullptr;
	last_game_object_selected = nullptr;
	dir_particle_texture.clear();
	
}


GameObject* Module_Go_Manager::Create_Game_Object( Mesh* m, GameObject* Parent)
{
	if (Parent == nullptr)
	{
		Parent = root_game_object;
    }
	Random rand;
		GameObject* new_game_object = new GameObject(Parent, m->name_node.c_str(), rand.Random_int(0, 2147483647));

		//Add Child to the parent
		Parent->Add_Child(new_game_object);
		LOG("The GameObject %s has a new child : %s ", new_game_object->Get_Parent()->Get_Name(), new_game_object->Get_Name());


			//Add Components Transformation
			new_game_object->Add_Component_Transformation(m->translation,m->scaling,m->rotation, RadToDeg(m->rotation.ToEulerXYZ()), false);
			LOG("The GameObject %s has a new component : %s ", new_game_object->Get_Name(), "TRANSFORMATION");
			
			//Add Components Mesh
			new_game_object->Add_Component_Mesh(m, false);
			LOG("The GameObject %s has a new component : %s ", new_game_object->Get_Name(), "MESH");

			//Add Component Material
			new_game_object->Add_Component_Material(m->name_texture.c_str(), m->dir_texture.c_str(), m->num_image_textures, m->id_image_texture, false, false);
			LOG("The GameObject %s has a new component : %s ", new_game_object->Get_Name(), "MaATERIAL");


	return new_game_object;


}

GameObject* Module_Go_Manager::Create_Camera_Game_Object(GameObject* Parent, const char* name_camera)
{
	if (Parent == nullptr)
	{
		Parent = root_game_object;
	}
	Random rand;
	GameObject* new_game_object = new GameObject(Parent, name_camera, rand.Random_int(0, 2147483647));

	//Add Child to the parent
	Parent->Add_Child(new_game_object);
	LOG("The GameObject %s has a new child : %s ", new_game_object->Get_Parent()->Get_Name(), new_game_object->Get_Name());


	//Add Component Transformation
	new_game_object->Add_Component_Transformation(float3::zero, float3::one, Quat::identity, float3::zero, false);
	LOG("The GameObject %s has a new component : %s ", new_game_object->Get_Name(), "TRANSFORMATION");

	//Add Component Camera
	new_game_object->Add_Component_Camera(name_camera);
	LOG("The GameObject %s has a new component : %s ", new_game_object->Get_Name(), "CAMERA");


	return new_game_object;
}


GameObject* Module_Go_Manager::Create_Empty_Game_Object(const char* name_go, GameObject* Parent)
{
	if (Parent == nullptr)
	{
		Parent = root_game_object;
	}
	Random rand;
	GameObject* new_game_object = new GameObject(Parent, name_go, rand.Random_int(0, 2147483647));

	//Add Child to the parent
	Parent->Add_Child(new_game_object);
	LOG("The GameObject %s has a new child : %s ", new_game_object->Get_Parent()->Get_Name(), new_game_object->Get_Name());

	//Add Component Transformation
	new_game_object->Add_Component_Transformation(float3::zero, float3::one, Quat::identity, float3::zero, false);
	LOG("The GameObject %s has a new component : %s ", new_game_object->Get_Name(), "TRANSFORMATION");

	return new_game_object;
}

GameObject* Module_Go_Manager::Create_Particle_Game_Object(const char* name_go, GameObject* Parent)
{
	if (Parent == nullptr)
	{
		Parent = root_game_object;
	}
	Random rand;
	GameObject* new_game_object = new GameObject(Parent, name_go, rand.Random_int(0, 2147483647));

	//Add Child to the parent
	Parent->Add_Child(new_game_object);
	LOG("The GameObject %s has a new child : %s ", new_game_object->Get_Parent()->Get_Name(), new_game_object->Get_Name());

	//Add Component Transformation
	new_game_object->Add_Component_Transformation(float3::zero, float3::one, Quat::identity, float3::zero, false);
	LOG("The GameObject %s has a new component : %s ", new_game_object->Get_Name(), "TRANSFORMATION");

	//Add Component Material
	vector<string>::iterator tmp = dir_particle_texture.begin();
	new_game_object->Add_Component_Material("Particle Texture", (*tmp).c_str(),1, 0, true, false);
	LOG("The GameObject %s has a new component : %s ", new_game_object->Get_Name(), "MATERIAL");

	if (dir_particle_texture.size() > 1)
	{
		tmp++;
		//Now we pass Firework Texture and load it
		Component_Material* material = (Component_Material*)new_game_object->Get_Component(MATERIAL);
		material->Set_Dir_Firework_Texture((*tmp).c_str());
		unsigned int id_texture = 0;
		App->geometry->Load_Texture(material->Get_Dir_Firework_Texture(), id_texture);
		material->Set_Id_Firework_Texture(id_texture);
	}

	//Add Component Emmitter
	new_game_object->Add_Component_Emitter(false);
	LOG("The GameObject %s has a new component : %s ", new_game_object->Get_Name(), "EMITTER");

	return new_game_object;
}

update_status Module_Go_Manager::Update(float dt)
{
	//Render and Update Components
	if (root_game_object->Get_Children()->size() > 0)
	{
		vector<GameObject*>::const_iterator node_game_obj = root_game_object->Get_Children()->begin();

		while (node_game_obj != root_game_object->Get_Children()->end())
		{
			
			(*node_game_obj)->Update_Go_Components();
			node_game_obj++;
		}

	}

	//Render Hierarchy Panel
	ImGui::SetNextWindowPos(ImVec2(2, 20));
	ImVec2 size_w;
	if (App->Get_Windows_Resized() == false)
	{
		size_w = ImVec2(170.f, 340.f);
	}
	else
	{
		size_w = ImVec2(350.f, 740.f);
	}
	ImGui::SetNextWindowSize(size_w);
	ImGui::Begin("Hierarchy");

	if (root_game_object->Get_Children()->size() > 0)
	{	
		Window_Hierarchy(root_game_object);
	}

	ImGui::End();

	//----------------------------------MOUSE PICKING-------------------------------------
	//Select a GO with the mouse
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && !ImGui::IsMouseHoveringAnyWindow())
	{
		//Creation Ray
		LineSegment ray = App->camera->Create_RayCast();
		
		if (game_object_selected != nullptr)
		{
			//Deactive the components of the las gameobject selected
			Search_GameObject_To_Deactive(game_object_selected);
		}

		//Obtain the GO selected
		game_object_selected = Obtain_GO_By_Raycast(ray, Collect_GO_Candidates(ray));

		if (game_object_selected != nullptr)
		{
			Search_GameObject_To_Active(game_object_selected);
		}

		last_game_object_selected = game_object_selected;
	}

	if (App->camera->camera_component_test->Get_Deactive_Qaudtree_Draw() == false)
	{
		//Draw Quadtree
		quadtree_go.Draw();
	}

	//Update Camera Culling optimizated
	App->go_manager->Intersect_Camera_Culling_Quadtree_Function(*App->camera->camera_component_test);

	return UPDATE_CONTINUE;
}

void Module_Go_Manager::Window_Hierarchy(GameObject* Root_node)
{

	for (vector<GameObject*>::const_iterator node_go = Root_node->Get_Children()->begin(); node_go != Root_node->Get_Children()->end(); node_go++)
	{
		uint flags = 0;
		if ((*node_go) == game_object_selected)
			flags = ImGuiTreeNodeFlags_Selected;

		if ((*node_go)->Get_Children()->size() > 0)
		{
			if (ImGui::TreeNodeEx((*node_go)->Get_Name(), flags | ImGuiTreeNodeFlags_DefaultOpen))
			{
				if (ImGui::IsItemClicked(0))
				{   
					if (game_object_selected != nullptr)
					{
						Search_GameObject_To_Deactive(game_object_selected);
					}

					game_object_selected = (*node_go);

					Search_GameObject_To_Active( game_object_selected);

					last_game_object_selected = game_object_selected;
				}

				Window_Hierarchy((*node_go));
				ImGui::TreePop();
			}
		}
		else
		{
			if (ImGui::TreeNodeEx((*node_go)->Get_Name(), flags | ImGuiTreeNodeFlags_Leaf))
			{
				if (ImGui::IsItemClicked(0))
				{
					if (game_object_selected != nullptr)
					{
						Search_GameObject_To_Deactive(game_object_selected);
					}

					game_object_selected = (*node_go);
					Search_GameObject_To_Active( game_object_selected);

					last_game_object_selected = game_object_selected;
				}

				ImGui::TreePop();
			}
		}
	}

}

void Module_Go_Manager::Search_GameObject_To_Active( GameObject* root_go)const
{
	vector<Components*>::const_iterator node_comp = root_go->Get_List_Components()->begin();
	while (node_comp != root_go->Get_List_Components()->end())
	{
		(*node_comp)->Enable();
		node_comp++;
	}
}

void Module_Go_Manager::Search_GameObject_To_Deactive( GameObject* root_go)const
{
	if (last_game_object_selected != nullptr)
	{
		vector<Components*>::const_iterator node_comp = last_game_object_selected->Get_List_Components()->begin();
		while (node_comp != last_game_object_selected->Get_List_Components()->end())
		{
			(*node_comp)->Disable();
			node_comp++;
		}
	}
	
}

GameObject* Module_Go_Manager::Get_Root()const
{
	return root_game_object;
}

bool  Module_Go_Manager::Load(pugi::xml_node& node)
{
	//First we load the parent and then the child
	root_game_object->Load(node);
	return true;
}


bool  Module_Go_Manager::Save(pugi::xml_node& node)const
{
		
    //First we save the parent and then the child
	root_game_object->Save(node);

	return true;
}

bool Compare_Bounding_Boxes(const GameObject* go_1, const GameObject* go_2)
{
	return go_1->Get_Distance_To_Ray().Length() < go_2->Get_Distance_To_Ray().Length();
}

GameObject* Module_Go_Manager::Obtain_GO_By_Raycast(const LineSegment& r, const vector<GameObject*> list_go)const
{
	GameObject* go_selected = nullptr;

    float min_distance_to_ray = App->camera->Get_Camera_Component()->Get_Far_Plane();

	for (vector<GameObject*>::const_iterator go = list_go.begin(); go != list_go.end(); go++)
	{

		if ((*go)->Exist_Component(MESH))
		{
			Component_Mesh* comp_mesh = (Component_Mesh*)(*go)->Get_Component(MESH);
			Component_Transformation* transform = (Component_Transformation*)(*go)->Get_Component(TRANSFORMATION);
			const Mesh* mesh = comp_mesh->Get_Mesh();

			if (mesh->num_vertices > 0)
			{
				//Transform raycast to Local Transformation
				LineSegment ray_cast = r;
				ray_cast.Transform(transform->Get_Tranformation_Matrix().Inverted());
				
				float intersection_distance = 0;
				vec intersection_point = vec::zero;

				for (int n = 0; n < mesh->num_indices / 3; n++)
				{
					int ind_1 = mesh->indices[n * 3];
					int ind_2 = mesh->indices[n * 3 + 1];
					int ind_3 = mesh->indices[n * 3 + 2];

					//Check if intersect with the triangle
					if (ray_cast.Intersects(Triangle(float3(&mesh->vertices[ind_1]), float3(&mesh->vertices[ind_2]), float3(&mesh->vertices[ind_3])),&intersection_distance, &intersection_point))
					{
						//Check if it is the nearest to the ray
						if (intersection_distance < min_distance_to_ray)
						{
							min_distance_to_ray = intersection_distance;
							go_selected = (*go);
						}
					}
				}
			}
		}
	}
	
	return go_selected;
}

std::vector<GameObject*> Module_Go_Manager::Collect_GO_Candidates(const math::LineSegment& r)const
{
	vector<GameObject*> list_candidates;

	//Check which GO that intersects with the ray
	root_game_object->GO_Candidates_Raycast(root_game_object, r, list_candidates);

	//Now that we have the candidates, we have to sort them by distance
	sort(list_candidates.begin(), list_candidates.end(), Compare_Bounding_Boxes);

	return list_candidates;
}



//----------------------------------------QUADTREE FUNCTIONS ---------------------------------------------

void Module_Go_Manager::Create_Quadtree_Root_Function(const float2 size_rect, const float2 center)
{	
	quadtree_go.Create(size_rect, center);
}

void Module_Go_Manager::Insert_GO_To_Quadtree()
{
	if (root_game_object->Get_Children()->size() > 0)
	{
		root_game_object->Insert_To_Quadtree();
	}
}

bool Module_Go_Manager::Insert_Quadtree_Function(GameObject& go)
{
	bool ret = false;

	/*To insert go only once. If we want to insert again we have
	to clean the gameobject and create another quadtreenode root*/
	if (game_obj_Inserted == false)
	{
		ret = quadtree_go.Insert(go);
	}

	return ret;
}

bool Module_Go_Manager::Clear_Quadtree_Function()
{
	if (quadtree_go.root != nullptr)
	{
		game_obj_Inserted = false;
		return quadtree_go.Clear();
	}
}

void Module_Go_Manager::Intersect_Camera_Culling_Quadtree_Function(Component_Camera& camera)
{
	if(quadtree_go.root != nullptr)
		quadtree_go.Intersects_Quadtree_Nodes(camera);
}

vector<GameObject*> Module_Go_Manager::Ray_Intersects_Quadtree_Function(const LineSegment& ray)
{
	if (quadtree_go.root != nullptr)
		return quadtree_go.Ray_Intersects_Quadtree_Nodes(ray);
}

void Module_Go_Manager::Set_Boundaries_Quadtree_Root_Function(const float2 size_rect)
{
	//Only modify the boundaries if there is no insertion yet
	if (quadtree_go.root != nullptr && game_obj_Inserted == false)
		quadtree_go.Set_Boundaries_Root(size_rect);
}

float2 Module_Go_Manager::Get_Boundaries_Quadtree_Root_Function()const
{
	if (quadtree_go.root != nullptr)
		return quadtree_go.Get_Boundaries();
	else
		return float2::zero;
}


void  Module_Go_Manager::Module_Go_Manager::Deactivate_GO_Render()
{
	if (quadtree_go.root != nullptr)
		quadtree_go.Deactivate_All_GO_Renders();
}

void  Module_Go_Manager::Set_Game_Obj_Inserted(bool on)
{
	game_obj_Inserted = on;
}

GameObject*  Module_Go_Manager::Get_Selected_GO()const
{
	return game_object_selected;
}


vector<string> Module_Go_Manager::Get_Dir_Particle_Texture()const
{
	return dir_particle_texture;
}

void Module_Go_Manager::Set_Dir_Particle_Texture(const char* dir)
{
	string tmp = dir;
	//The first string of the vector will always be the somke texture dir
	dir_particle_texture.push_back(dir);
}
