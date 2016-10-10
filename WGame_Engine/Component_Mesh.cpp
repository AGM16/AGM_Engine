#include "Component_Mesh.h"
#include "Application.h"

using namespace std;

Component_Mesh::Component_Mesh(COMPONENT_TYPE type, GameObject* go, Mesh* mesh_) : Components(type, go), m(mesh_)
{

}

Component_Mesh::~Component_Mesh()
{

}

//Functions
void Component_Mesh::Update()
{
	if (Get_Active_Component())
	{
		if (ImGui::CollapsingHeader("Mesh"))
		{

			ImGui::Text(" NumVertices : %s", "38");
			ImGui::SameLine();
			//ImGui::TextColored(ImVec4(1.f,1.f,1.f,1.f), "%d", go_mesh->num_vertices);
			ImGui::Text(" NumNormals : ");
			ImGui::SameLine();
			//ImGui::TextColored(ImVec4(1.f, 1.f, 1.f, 1.f), "%d", go_mesh->num_normals);
		}
	}


}

