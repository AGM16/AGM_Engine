#include "Component_Material.h"
#include "Application.h"
#include "Imgui\imgui.h"
#include "Components.h"
#include "Component_Transformation.h"
#include "Component_Mesh.h"
#include "GameObject.h"


Component_Material::Component_Material(Components_Type type, GameObject* game_object, const char* name_textu, const char* path_texture, unsigned int num_textu, unsigned int id_textu) : Components(type, game_object), name_texture(name_textu), dir_path_texture(path_texture), num_textures(num_textu), id_texture(id_textu)
{
	

}

Component_Material::~Component_Material()
{

}

void Component_Material::Clean_Up()
{
	
	
}

void Component_Material::Update()
{
	
	if (Is_Active())
	{
		 
		if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (active_checkbox == false)
			{
				ImGui::Image((ImTextureID)id_texture, ImVec2(200, 200));

				ImGui::Text("Texture name : ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1, 1, 1, 1), "%s", name_texture);

				ImGui::Text("Dir Texture : ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1, 1, 1, 1), "%s", dir_path_texture);

				ImGui::Text("Number of Textures : ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1, 1, 1, 1), "%i", num_textures);

				ImGui::Text("Id Texture : ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1, 1, 1, 1), "%i", id_texture);
			}
			else
			{
				ImGui::Image((ImTextureID)0, ImVec2(200, 200));

				ImGui::Text("Texture name : ");

				ImGui::Text("Dir Texture : ");

				ImGui::Text("Number of Textures : ");

				ImGui::Text("Id Texture : ");

			}

			ImGui::Checkbox("Active", &active_checkbox);

		}
	}

}

unsigned int Component_Material::Get_Id_Texture()const
{
	return id_texture;
}

bool Component_Material::Is_Checkbox_Active()const
{
	return active_checkbox;
}

bool Component_Material::Set_Checkbox(bool on)
{
	active_checkbox = on;
	return active_checkbox;
}