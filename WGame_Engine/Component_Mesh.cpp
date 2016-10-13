#include "Component_Mesh.h"
#include "Application.h"
#include "GameObject.h"
#include "Components.h"
#include "Component_Transformation.h"


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
			//--------------------------------------------------------------------
			ImGui::Text("Name : ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 1, 1), "%s", mesh->name_node);

			ImGui::SameLine();
			ImGui::Text("                NumVertices : ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 1, 1), "%i", mesh->num_vertices);

			//--------------------------------------------------------------------
			ImGui::Text("NumChildren : ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 1, 1), "%i", mesh->num_children);

			ImGui::SameLine();
			ImGui::Text("         NumIndices : ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 1, 1), "%i", mesh->num_indices);

			//--------------------------------------------------------------------
			ImGui::Text("Parent : ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 1, 1), "%s", mesh->parent);

			ImGui::SameLine();
			ImGui::Text("            NumNormals : ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 1, 1), "%i", mesh->num_normals);


		}
	}
	
	Component_Transformation* transformation =  (Component_Transformation*)Get_Game_Object()->Get_Component(TRANSFORMATION);

	App->renderer3D->Draw_Geometry(mesh, App->scene_intro->lena_image, transformation->Get_Tranformation_Matrix().Transposed());

}