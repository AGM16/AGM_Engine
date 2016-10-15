#include "Component_Material.h"
#include "Imgui\imgui.h"


Component_Material::Component_Material(Components_Type type, GameObject* game_object, std::string name_textu, std::string path_texture, unsigned int num_textu, unsigned int id_textu) : Components(type, game_object), name_texture(name_textu), dir_path_texture(path_texture), num_textures(num_textu), id_texture(id_textu)
{

}

Component_Material::~Component_Material()
{

}

void Component_Material::Update()
{
	if (Is_Active())
	{
		if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
		{
			
			ImGui::Image((ImTextureID)id_texture, ImVec2(200, 200));

			ImGui::Text("Texture name : ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 1, 1), "%s",name_texture.c_str());

			ImGui::Text("Dir Texture : ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 1, 1), "%s", dir_path_texture.c_str());

			ImGui::Text("Number of Textures : ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 1, 1), "%i", num_textures);

			ImGui::Text("Id Texture : ");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1, 1, 1, 1), "%i", id_texture);

		}
	}


}