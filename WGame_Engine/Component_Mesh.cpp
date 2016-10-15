#include "Component_Mesh.h"
#include "Application.h"
#include "GameObject.h"
#include "Components.h"
#include "Component_Transformation.h"
#include "Component_Material.h"


Component_Mesh::Component_Mesh(Components_Type type, GameObject* game_object, Mesh* mesh_) : Components(type, game_object), mesh(mesh_)
{

}

Component_Mesh::~Component_Mesh()
{

}

void Component_Mesh::Update()
{
	if (Is_Active())
	{
		if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (active == false)
			{
				//--------------------------------------------------------------------
				ImGui::Text("Name : ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1, 1, 1, 1), "%s", mesh->name_node);

				//--------------------------------------------------------------------
				ImGui::Text("NumChildren : ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1, 1, 1, 1), "%i", mesh->num_children);

				//--------------------------------------------------------------------
				ImGui::Text("Parent : ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1, 1, 1, 1), "%s", mesh->parent);

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

			ImGui::Checkbox("Active##foo1", &active);
		}
	}

	Component_Transformation* transformation = (Component_Transformation*)Get_Game_Object()->Get_Component(TRANSFORMATION);

	Component_Material* material = (Component_Material*)Get_Game_Object()->Get_Component(MATERIAL);
    
	id_image = material->id_texture;
	if (material->active == true)
	{
		id_image = 0;
	}

	if (active == false)
	{

		App->renderer3D->Draw_Geometry(mesh, id_image, transformation->Get_Tranformation_Matrix().Transposed());
	}
	

}