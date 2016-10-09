#ifndef _COMPONENT_MESH_H_
#define _COMPONENT_MESH_H_

#include "Components.h"
#include "Application.h"



class Component_Mesh : public Components
{

public:

	Component_Mesh(COMPONENT_TYPE type, Mesh* mesh_);
	~Component_Mesh();

	//Functions
	void Update();
	bool Assign_Mesh(Mesh *mesh);

private:


};

#endif // !_COMPONENT_MESH_H_
