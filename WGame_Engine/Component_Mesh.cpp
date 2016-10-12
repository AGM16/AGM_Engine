#include "Component_Mesh.h"
#include "Application.h"

Component_Mesh::Component_Mesh(Components_Type type, Mesh* mesh_) : Components(type), mesh(mesh_)
{

}

Component_Mesh::~Component_Mesh()
{

}

void Component_Mesh::Update()
{
	App->renderer3D->Draw_Geometry(mesh, App->scene_intro->lena_image, mesh->translation);
}