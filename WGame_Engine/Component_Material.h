#ifndef _COMPONENT_MATERIAL_H_
#define _COMPONENT_MATERIAL_H_

#include "Components.h"


class Component_Material : public Components
{
public:

	Component_Material(Components_Type type, GameObject* game_object, const char* name_textu, const char* path_texture, unsigned int num_textu, unsigned int id_textu);
	~Component_Material();

	void Update();
	void Clean_Up();


	//Getters
	unsigned int Get_Id_Texture()const;

	//Checkbox bool
	bool Is_Checkbox_Active()const;
	bool Set_Checkbox(bool on);


private:
    
	const char* name_texture = nullptr;
	const char* dir_path_texture = nullptr;
	unsigned int num_textures = 0;
	unsigned int id_texture = 0;
	bool active_checkbox = false;
};

#endif

