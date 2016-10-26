#include "Component_Mesh.h"
#include "Application.h"
#include "GameObject.h"
#include "Components.h"
#include "Component_Transformation.h"
#include "Component_Material.h"
#include "GameObject.h"
#include "p2Defs.h"


Component_Mesh::Component_Mesh(Components_Type type, GameObject* game_object, Mesh* mesh_) : Components(type, game_object), mesh(mesh_)
{

}

Component_Mesh::~Component_Mesh()
{
	
}

void Component_Mesh::Clean_Up()
{
	if (mesh != NULL)
	{
		mesh->Clean_Up();
		RELEASE(mesh);
	}
}

void Component_Mesh::Update()
{	
	//Render the Meshes
	Component_Transformation* transformation = (Component_Transformation*)Get_Game_Object()->Get_Component(TRANSFORMATION);
	Component_Material* material = (Component_Material*)Get_Game_Object()->Get_Component(MATERIAL);
    
	//Check if the material checkbox is active
	id_image = material->Get_Id_Texture();
	if (material->Is_Checkbox_Active() == true)
	{
		id_image = 0;
	}
	
	//Check the checkbox of the parent_mesh
	Component_Mesh* mesh_parent = nullptr;
	Component_Material* material_parent = nullptr;
	const char* root = "RootNode";
    if (Get_Game_Object()->Get_Parent() != App->go_manager->Get_Root())
	{	
		if (Get_Game_Object()->Get_Parent()->Exist_Component(MESH))
		{
			mesh_parent = (Component_Mesh*)Get_Game_Object()->Get_Parent()->Get_Component(MESH);
			material_parent = (Component_Material*)Get_Game_Object()->Get_Parent()->Get_Component(MATERIAL);

			//Check the material parent
			if (material_parent->Is_Checkbox_Active() == true)
			{
				material->Set_Checkbox(true);
				id_image = 0;
			}

			if (last_active_texture == true && material_parent->Is_Checkbox_Active() == false)
			{
				material->Set_Checkbox(false);
				id_image = material->Get_Id_Texture();
			}

			//Check the mesh parent
			if (mesh_parent->Is_Checkbox_Active() == true)
				active_checkbox = true;
			
			if (last_active_mesh == true && mesh_parent->active_checkbox == false)
			{
				active_checkbox = false;
			}

			//Check the wireframe parent
			if (mesh_parent->wireframe == true)
				wireframe = true;

			if (last_active_wireframe == true && mesh_parent->wireframe == false)
			{
				wireframe = false;
			}

			last_active_wireframe = mesh_parent->wireframe;
			last_active_mesh = mesh_parent->active_checkbox;
			last_active_texture = material_parent->Is_Checkbox_Active();

		}
		
	}

	if (Is_Active())
	{
		if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (active_checkbox == false)
			{
				//--------------------------------------------------------------------
				ImGui::Text("Name : ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1, 1, 1, 1), "%s", mesh->name_node.c_str());

				//--------------------------------------------------------------------
				ImGui::Text("NumChildren : ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1, 1, 1, 1), "%i", mesh->num_children);

				//--------------------------------------------------------------------
				ImGui::Text("Parent : ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1, 1, 1, 1), "%s", mesh->parent.c_str());

				//--------------------------------------------------------------------
				ImGui::Text("NumIndices : ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1, 1, 1, 1), "%i", mesh->num_indices);

				//--------------------------------------------------------------------
				ImGui::Text("NumVertices : ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1, 1, 1, 1), "%i", mesh->num_vertices);

				//--------------------------------------------------------------------
				ImGui::Text("NumNormals : ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1, 1, 1, 1), "%i", mesh->num_normals);
			}
			else
			{
				//--------------------------------------------------------------------
				ImGui::Text("Name : ");
				//--------------------------------------------------------------------
				ImGui::Text("NumChildren : ");
				//--------------------------------------------------------------------
				ImGui::Text("Parent : ");
				//--------------------------------------------------------------------
				ImGui::Text("NumIndices : ");
				//--------------------------------------------------------------------
				ImGui::Text("NumVertices : ");
				//--------------------------------------------------------------------
				ImGui::Text("NumNormals : ");

			}

			ImGui::Checkbox("Active##foo1", &active_checkbox);
			ImGui::SameLine();
			ImGui::Checkbox("Wireframe##faa1", &wireframe);
		}
	}

	

	if (mesh_parent != nullptr)
	{
		if (active_checkbox == false && mesh_parent->active_checkbox == false)
		{
			App->renderer3D->Draw_Geometry(mesh, id_image, transformation->Get_Tranformation_Matrix().Transposed(), wireframe);
		}
	}
}


bool Component_Mesh::Is_Checkbox_Active()const
{
	return active_checkbox;
}

bool Component_Mesh::Set_Checkbox(bool on)
{
	active_checkbox = on;
	return active_checkbox;
}