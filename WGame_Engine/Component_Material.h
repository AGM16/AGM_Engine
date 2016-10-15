#ifndef _COMPONENT_MATERIAL_H_
#define _COMPONENT_MATERIAL_H_

#include "Components.h"

struct Mesh;


class Component_Material : public Components
{
public:

	Component_Material(Components_Type type, GameObject* game_object, std::string name_textu, std::string path_texture, unsigned int num_textu, unsigned int id_textu);
	~Component_Material();

	void Update();

public:
    
	std::string name_texture;
	std::string dir_path_texture;
	unsigned int num_textures;
	unsigned int id_texture;

};

#endif

