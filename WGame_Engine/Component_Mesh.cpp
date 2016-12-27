#include "Component_Mesh.h"
#include "Application.h"
#include "GameObject.h"
#include "Components.h"
#include "Component_Transformation.h"
#include "Component_Material.h"
#include "GameObject.h"
#include "p2Defs.h"


Component_Mesh::Component_Mesh(Components_Type type, GameObject* game_object, Mesh* mesh_) : Components(type, game_object), mesh(mesh_)
{

	//To build only the bounding box to the GO that have mesh
	if (mesh != nullptr  && mesh->num_vertices > 0)
	{
		//Generate the initial AABB
		bounding_box.SetNegativeInfinity();
		bounding_box.Enclose((float3*)mesh->vertices, mesh->num_vertices);

		Component_Transformation* transformation = (Component_Transformation*)Get_Game_Object()->Get_Component(TRANSFORMATION);
		Recalculate_Properties_Bounding_Box(transformation);
	}

}

Component_Mesh::~Component_Mesh()
{
	
}

void Component_Mesh::Clean_Up()
{
	if (mesh != NULL)
	{
		mesh->Clean_Up();
		RELEASE(mesh);
	}
}

void Component_Mesh::Update()
{	
	//To Render the Meshes
	Component_Transformation* transformation = (Component_Transformation*)Get_Game_Object()->Get_Component(TRANSFORMATION);
	Component_Material* material = nullptr;
	if (Get_Game_Object()->Exist_Component(MATERIAL))
	{
		material = (Component_Material*)Get_Game_Object()->Get_Component(MATERIAL);

		//Check if the material checkbox is active
		id_image = material->Get_Id_Texture();
		if (material->Is_Checkbox_Active() == true)
		{
			id_image = 0;
		}
	}
	else
	{
		id_image = 0;
	}


	//Check if the global matrix has changed to recalculate the bounding box
	Recalculate_Properties_Bounding_Box(transformation);

	//Check the checkbox of the parent components
	Component_Mesh* mesh_parent = nullptr;
	Component_Material* material_parent = nullptr;
	Check_Parent_Checkboxes(mesh_parent, material_parent, material);

	//Render Info Mesh
	Render_Mesh_Panel();

	//Render Mesh and bounding boxes
	if (draw != false)
	{
		if (mesh_parent != nullptr)
		{
			if (active_checkbox == false && mesh_parent->active_checkbox == false)
			{
				App->renderer3D->Draw_Geometry(mesh, id_image, transformation->Get_Tranformation_Matrix().Transposed(), wireframe);

				if (aabb_checkbox == true || App->camera->Get_Camera_Component()->Get_Checkbox_AABB_Bounding_Boxes())
					App->renderer3D->Render_AABB_Cube(new_bounding_box);

				if (obb_checkbox == true || App->camera->Get_Camera_Component()->Get_Checkbox_OBB_Bounding_Boxes())
					App->renderer3D->Render_OBB_Cube(obb_box);

			}
		}
		else
		{
			if (mesh != nullptr)
			{
				if (active_checkbox == false)
				{
					App->renderer3D->Draw_Geometry(mesh, id_image, transformation->Get_Tranformation_Matrix().Transposed(), wireframe);

					if (aabb_checkbox == true || App->camera->Get_Camera_Component()->Get_Checkbox_AABB_Bounding_Boxes())
						App->renderer3D->Render_AABB_Cube(new_bounding_box);

					if (obb_checkbox == true || App->camera->Get_Camera_Component()->Get_Checkbox_OBB_Bounding_Boxes())
						App->renderer3D->Render_OBB_Cube(obb_box);
				}

			}
		}
	}
}


bool Component_Mesh::Is_Checkbox_Active()const
{
	return active_checkbox;
}

void Component_Mesh::Set_Checkbox(bool on)
{
	active_checkbox = on;
}

//Checkbox Wireframe bool
bool Component_Mesh::Is_Checkbox_Wireframe_Active()const
{
	return wireframe;
}

void Component_Mesh::Set_Checkbox_Wireframe(bool on)
{
	wireframe = on;
}

//Checkbox AABB bool
bool Component_Mesh::Is_Checkbox_AABB_Active()const
{
	return aabb_checkbox;
}

void Component_Mesh::Set_Checkbox_AABB(bool on)
{
	aabb_checkbox = on;
}

//Checkbox OBB bool
bool Component_Mesh::Is_Checkbox_OBB_Active()const
{
	return obb_checkbox;
}

void Component_Mesh::Set_Checkbox_OBB(bool on)
{
	obb_checkbox = on;
}

//Check Render bool
bool Component_Mesh::Is_Rendered()const
{
	return draw;
}

void Component_Mesh::Set_Rendered(bool new_render)
{
	draw = new_render;
}

void Component_Mesh::Check_Parent_Checkboxes(Component_Mesh* mesh_parent, Component_Material* material_parent, Component_Material* material)
{
	if (Get_Game_Object()->Get_Parent() != App->go_manager->Get_Root())
	{
		if (Get_Game_Object()->Get_Parent()->Exist_Component(MESH) && Get_Game_Object()->Get_Parent()->Exist_Component(MATERIAL))
		{
			mesh_parent = (Component_Mesh*)Get_Game_Object()->Get_Parent()->Get_Component(MESH);
			material_parent = (Component_Material*)Get_Game_Object()->Get_Parent()->Get_Component(MATERIAL);

			//Check the material parent
			if (material_parent->Is_Checkbox_Active() == true)
			{
				material->Set_Checkbox(true);
				id_image = 0;
			}

			if (last_active_texture == true && material_parent->Is_Checkbox_Active() == false)
			{
				material->Set_Checkbox(false);
				id_image = material->Get_Id_Texture();
			}

			//Check the mesh parent
			if (mesh_parent->Is_Checkbox_Active() == true)
				active_checkbox = true;

			if (last_active_mesh == true && mesh_parent->active_checkbox == false)
			{
				active_checkbox = false;
			}

			//Check the wireframe parent
			if (mesh_parent->wireframe == true)
				wireframe = true;

			if (last_active_wireframe == true && mesh_parent->wireframe == false)
			{
				wireframe = false;
			}

			//Modify the last results
			last_active_wireframe = mesh_parent->wireframe;
			last_active_mesh = mesh_parent->active_checkbox;
			last_active_texture = material_parent->Is_Checkbox_Active();

		}
	}
}

void Component_Mesh::Recalculate_Properties_Bounding_Box(Component_Transformation* transformation)
{
	//Compare both matrixs
	if (last_transformation_matrix.Equals(transformation->Get_Tranformation_Matrix()) == false)
	{
		//Build the new OBB
		obb_box = bounding_box.ToOBB();
		obb_box.Transform(transformation->Get_Tranformation_Matrix());

		//Build the new AABB
		new_bounding_box.SetNegativeInfinity();
		new_bounding_box.Enclose(obb_box);

		last_transformation_matrix = transformation->Get_Tranformation_Matrix();
	}
}

void Component_Mesh::Render_Mesh_Panel()
{

	if (Is_Active())
	{
		if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (active_checkbox == false && mesh != nullptr)
			{
				//--------------------------------------------------------------------
				ImGui::Text("Name : ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1, 1, 1, 1), "%s", mesh->name_node.c_str());

				//--------------------------------------------------------------------
				ImGui::Text("NumChildren : ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1, 1, 1, 1), "%i", mesh->num_children);

				//--------------------------------------------------------------------
				ImGui::Text("Parent : ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1, 1, 1, 1), "%s", mesh->parent.c_str());

				//--------------------------------------------------------------------
				ImGui::Text("NumIndices : ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1, 1, 1, 1), "%i", mesh->num_indices);

				//--------------------------------------------------------------------
				ImGui::Text("NumVertices : ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1, 1, 1, 1), "%i", mesh->num_vertices);

				//--------------------------------------------------------------------
				ImGui::Text("NumNormals : ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1, 1, 1, 1), "%i", mesh->num_normals);

				//--------------------------------------------------------------------
				ImGui::Text("NumUvs : ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(1, 1, 1, 1), "%i", mesh->num_uvs_texture_coords);
			}
			else
			{
				//--------------------------------------------------------------------
				ImGui::Text("Name : ");
				//--------------------------------------------------------------------
				ImGui::Text("NumChildren : ");
				//--------------------------------------------------------------------
				ImGui::Text("Parent : ");
				//--------------------------------------------------------------------
				ImGui::Text("NumIndices : ");
				//--------------------------------------------------------------------
				ImGui::Text("NumVertices : ");
				//--------------------------------------------------------------------
				ImGui::Text("NumNormals : ");
				//--------------------------------------------------------------------
				ImGui::Text("NumUvs : ");

			}

			ImGui::Checkbox("Deactivate##foo1", &active_checkbox);
			ImGui::SameLine();
			ImGui::Checkbox("Wireframe##faa1", &wireframe);

			//Render Bounding Boxes
			Render_Bounding_Box_Panel();
		}
	}

}



void Component_Mesh::Render_Bounding_Box_Panel()
{
	if (mesh != nullptr)
	{
		//Check if the GO have a mesh
		if (mesh->num_vertices != 0)
		{
			if (ImGui::CollapsingHeader("Bounding Box", ImGuiTreeNodeFlags_DefaultOpen))
			{
				if (ImGui::CollapsingHeader("AABB Box", ImGuiTreeNodeFlags_DefaultOpen))
				{
					if (aabb_checkbox == true)
					{
						ImGui::Text("MinPoint : ");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1, 1, 1, 1), "%f , %f, %f", new_bounding_box.minPoint.x, new_bounding_box.minPoint.y, new_bounding_box.minPoint.z);

						ImGui::Text("MaxPoint : ");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1, 1, 1, 1), "%f , %f, %f", new_bounding_box.maxPoint.x, new_bounding_box.maxPoint.y, new_bounding_box.maxPoint.z);
					}
					else
					{
						ImGui::Text("MinPoint : ");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1, 1, 1, 1), "%f , %f, %f", 0.f, 0.f, 0.f);

						ImGui::Text("MaxPoint : ");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1, 1, 1, 1), "%f , %f, %f", 0.f, 0.f, 0.f);
					}

					ImGui::Checkbox("Active AABB##foo1", &aabb_checkbox);

				}

				if (ImGui::CollapsingHeader("OBB Box", ImGuiTreeNodeFlags_DefaultOpen))
				{
					if (obb_checkbox == true)
					{
						ImGui::Text("Center Position : ");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1, 1, 1, 1), "%f , %f, %f", obb_box.pos.x, obb_box.pos.y, obb_box.pos.z);

						ImGui::Text("Axis : ");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1, 1, 1, 1), "%f , %f, %f", obb_box.axis->x, obb_box.axis->y, obb_box.axis->z);
					}
					else
					{
						ImGui::Text("Center Position : ");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1, 1, 1, 1), "%f , %f, %f", 0.f, 0.f, 0.f);

						ImGui::Text("Axis : ");
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1, 1, 1, 1), "%f , %f, %f", 0.f, 0.f, 0.f);
					}

					ImGui::Checkbox("Active OBB##foo1", &obb_checkbox);
				}


			}
		}
	}
	
}

Mesh* Component_Mesh::Get_Mesh()const
{
	return mesh;
}

AABB  Component_Mesh::Get_AABB_Bounding_Box()const
{
	return new_bounding_box;
}

OBB Component_Mesh::Get_OBB_Bounding_Box()const
{
	return obb_box;
}