#ifndef _COMPONENT_MESH_H_
#define _COMPONENT_MESH_H_

#include "Components.h"
#include "Application.h"



class Component_Mesh : public Components
{

public:

	Component_Mesh(COMPONENT_TYPE type, GameObject* go, Mesh* mesh_);
	~Component_Mesh();

	//Functions
	void Update();

private:

	Mesh* m;

};

#endif // !_COMPONENT_MESH_H_
