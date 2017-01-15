#include "Component_Material.h"
#include "Application.h"
#include "Imgui\imgui.h"
#include "Components.h"
#include "Component_Transformation.h"
#include "Component_Mesh.h"
#include "Component_Emitter.h"
#include "GameObject.h"


Component_Material::Component_Material(Components_Type type, GameObject* game_object, const char* name_textu, const char* path_texture, unsigned int num_textu, unsigned int id_textu, bool is_a_particle_texture) : Components(type, game_object), name_texture(name_textu), dir_path_texture(path_texture), num_textures(num_textu), id_texture(id_textu), is_a_particle_material(is_a_particle_texture)
{
	if (is_a_particle_material)
	{
		App->geometry->Load_Texture(dir_path_texture, id_texture);
	}
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
				if (Get_Game_Object()->Exist_Component(EMITTER))
				{
					Component_Emitter* emitter = (Component_Emitter*)Get_Game_Object()->Get_Component(EMITTER);

					if (emitter->Is_Firework_Behavior_Active() && emitter->Is_Smoke_Behavior_Active() == false)
					{
						ImGui::Image((ImTextureID)id_firework_texture, ImVec2(200, 200));
					}
					else
					{
						ImGui::Image((ImTextureID)id_texture, ImVec2(200, 200));
					}

				}
				else
				{
					ImGui::Image((ImTextureID)id_texture, ImVec2(200, 200));
				}

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

			ImGui::Checkbox("Deactivate##fcc2", &active_checkbox);

		}
	}

}

unsigned int Component_Material::Get_Id_Texture()const
{
	if (active_checkbox == false)
	{
		if (Get_Game_Object()->Exist_Component(EMITTER))
		{
			Component_Emitter* emitter = (Component_Emitter*)Get_Game_Object()->Get_Component(EMITTER);

			if (emitter->Is_Firework_Behavior_Active() && emitter->Is_Smoke_Behavior_Active() == false)
				return id_firework_texture;
		}

		return id_texture;
	}
	
	return 0;
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

void Component_Material::Set_Dir_Firework_Texture(const char* new_dir)
{
	dir_path_firework_texture = new_dir;
}

const char* Component_Material::Get_Dir_Firework_Texture()const
{
	return dir_path_firework_texture;
}

void Component_Material::Set_Id_Firework_Texture(unsigned int id)
{
	id_firework_texture = id;
}

unsigned int Component_Material::Get_id_Firework_Texture()
{
	return id_firework_texture;
}