#ifndef _COMPONENT_MATERIAL_H_
#define _COMPONENT_MATERIAL_H_

#include "Components.h"


class Component_Material : public Components
{
public:

	Component_Material(Components_Type type, GameObject* game_object, const char* name_textu, const char* path_texture, unsigned int num_textu, unsigned int id_textu, bool is_a_particle_texture);
	~Component_Material();

	void Update();
	void Clean_Up();


	//Getters
	unsigned int Get_Id_Texture()const;

	//Checkbox bool
	bool Is_Checkbox_Active()const;
	bool Set_Checkbox(bool on);

	void Set_Dir_Firework_Texture(const char* new_dir);
	void Set_Id_Firework_Texture(unsigned int id);
	const char* Get_Dir_Firework_Texture()const;
	unsigned int Get_id_Firework_Texture();

private:
    
	const char* name_texture = nullptr;
	const char* dir_path_texture = nullptr;
	unsigned int num_textures = 0;
	unsigned int id_texture = 0;
	bool active_checkbox = false;
	bool is_a_particle_material = false;

	//Fireworks Texture
	const char* dir_path_firework_texture = nullptr;
	unsigned int id_firework_texture = 0;

};

#endif

