#include "Application.h"
#include "p2QuadTree.h"
#include "Components.h"
#include "GameObject.h"
#include "Module_Go_Manager.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include "Component_Mesh.h"
#include <queue>

using namespace std;

//-------------------------------QUADTREE_NODE FUNCTIONS--------------------------------------
void QuadTreeNode::Subdivide()
{
	for (int i = 0; i < 4; i++)
	{
		float2 new_center;
		float2 new_size;
		switch (i)
		{
		   case 0: //Top-Left
			   //New center
			   new_center.x = rect.centre.x - (rect.size.x / 4);
			   new_center.y = rect.centre.y - (rect.size.y / 4);
			   break;

		   case 1: //Top-Right
				   //New center
			   new_center.x = rect.centre.x + (rect.size.x / 4);
			   new_center.y = rect.centre.y - (rect.size.y / 4);
			   break;

		   case 2: //Bottom-Left
				   //New center
			   new_center.x = rect.centre.x - (rect.size.x / 4);
			   new_center.y = rect.centre.y + (rect.size.y / 4);
			   break;
		    
		   case 3: //Bottom-Right
				   //New center
			   new_center.x = rect.centre.x + (rect.size.x / 4);
			   new_center.y = rect.centre.y + (rect.size.y / 4);		     
			   break;
		}

		//New Size
		new_size.x = rect.size.x /2;
		new_size.y = rect.size.y /2;

		QuadTreeNode* new_child = new QuadTreeNode(new_center, new_size, this);
		children.push_back(new_child);
	}
}

bool QuadTreeNode::Insert(GameObject& GO, const float2 center_position)
{
	bool ret = false;

	Component_Mesh* mesh = (Component_Mesh*)GO.Get_Component(MESH);

	//Check if the mesh is not empty
	if (mesh->Get_Mesh()->num_vertices > 0)
	{
		queue<QuadTreeNode*> queue_qnode;
		queue_qnode.push(this);

		QuadTreeNode* current_node = nullptr;

		while (!queue_qnode.empty())
		{
			current_node = queue_qnode.front();
			queue_qnode.pop();

			if (current_node->Contains(center_position))
			{
				if (current_node->children.empty())
				{
					if (current_node->objects.size() < 2)//The node is available to catch this GO
					{
						current_node->objects.push_back(&GO);
						return true;
					}
					else
					{
						/*It means that we have to Subdivide the QuadtreeNode
						and reorganize the gameObjects*/
						current_node->Subdivide();

						//Insert again the old objects
						for (vector<GameObject*>::iterator i = current_node->objects.begin(); i < current_node->objects.end(); ++i)
						{
							Component_Mesh* mesh = (Component_Mesh*)(*i)->Get_Component(MESH);
							vec center = mesh->Get_AABB_Bounding_Box().CenterPoint();
							if (current_node->Insert(*(*i), float2(center.x, center.z)) != false)
							{
								(*i) = nullptr;
							}
							else
							{
								ret = false;
								break;
							}
						}

						current_node->objects.clear();

						//Insert new object
						ret = current_node->Insert(GO, center_position);
						break;
	
					}
				}
			}

			vector<QuadTreeNode*>::const_iterator children_current_node = current_node->children.begin();
			while (children_current_node != current_node->children.end())
			{
				queue_qnode.push(*children_current_node);
				children_current_node++;
			}
		}
	}

	return ret;
}

bool QuadTreeNode::Remove(GameObject& GO, const float2 center_position)
{
	bool ret = false;
	queue<QuadTreeNode*> queue_qnode;
	queue_qnode.push(this);

	QuadTreeNode* current_node = nullptr;

	while (!queue_qnode.empty())
	{
		current_node = queue_qnode.front();
		queue_qnode.pop();

		if (current_node->Contains(center_position))
		{
			//Check if this one is the QuadtreeNode that have this GO
			//Insert again the old objects
			for (vector<GameObject*>::iterator i = current_node->objects.begin(); i < current_node->objects.end(); ++i)
			{
				if ((*i) == &GO && current_node->rect.centre.Equals(center_position))
				{
					(*i) = nullptr;
					ret = true;
					break;
				}
			}
		}

		vector<QuadTreeNode*>::const_iterator children_current_node = current_node->children.begin();
		while (children_current_node != current_node->children.end())
		{
			queue_qnode.push(*children_current_node);
			children_current_node++;
		}
	}

	return ret;
}

bool QuadTreeNode::Clear_Nodes()
{
	if (this->children.size() > 0)
	{
		for (vector<QuadTreeNode*>::iterator children_node = this->children.begin(); children_node != this->children.end(); children_node++)
		{
			(*children_node)->Clear_Nodes();
			//Now delete the child
			delete (*children_node);
			(*children_node) = nullptr;
		}

		this->children.clear();
	}

	rect.Clear();
	parent = nullptr;

	if (this->objects.size() > 0)
	{
		for (vector<GameObject*>::iterator i = objects.begin(); i < objects.end(); ++i)
		{
			(*i) = nullptr;	
		}

		this->objects.clear();
	}

	return true;
}

void QuadTreeNode::Draw_Node()
{
	if (this->children.size() > 0)
	{
		for (vector<QuadTreeNode*>::const_iterator children_node = this->children.begin(); children_node != this->children.end(); children_node++)
		{
			(*children_node)->Draw_Node();
		}
	}

	App->renderer3D->Render_AABB_Cube(this->rect.rect);
}

//For Camera Culling
void QuadTreeNode::Intersect_Node(Component_Camera& geo)
{

		queue<QuadTreeNode*> queue_qnode;
		queue_qnode.push(this);

		QuadTreeNode* current_node = nullptr;

		while (!queue_qnode.empty())
		{
			current_node = queue_qnode.front();
			queue_qnode.pop();
			if (current_node->objects.empty() == false)
			{
				//Check each GO of the objects vector
				for (vector<GameObject*>::iterator i = current_node->objects.begin(); i < current_node->objects.end(); ++i)
				{
					Component_Mesh* comp_mesh = (Component_Mesh*)(*i)->Get_Component(MESH);

					if (geo.Intersect_Frustum_AABB(current_node->rect.rect))
					{

						if (current_node->children.size() == 0)
						{		
							    //Activate varibale to draw the GO
								comp_mesh->Set_Rendered(true);
						}
					}
					else
					{
						//Deactivate the variable to not draw the GO
						if (comp_mesh->Is_Rendered())
							comp_mesh->Set_Rendered(false);
					}
				}
			}


			vector<QuadTreeNode*>::const_iterator children_current_node = current_node->children.begin();
			while (children_current_node != current_node->children.end())
			{
				queue_qnode.push(*children_current_node);
				children_current_node++;
			}
		}
}

//Optimize mouse picking
vector<GameObject*> QuadTreeNode::Ray_Intersects_Node(const LineSegment& ray)
{
	vector<GameObject*> ret;

	queue<QuadTreeNode*> queue_qnode;
	queue_qnode.push(this);

	QuadTreeNode* current_node = nullptr;

	while (!queue_qnode.empty())
	{
		current_node = queue_qnode.front();
		queue_qnode.pop();
		if (current_node->objects.empty() == false)
		{
			//Check each GO of the objects vector
			for (vector<GameObject*>::iterator i = current_node->objects.begin(); i < current_node->objects.end(); ++i)
			{
				Component_Mesh* comp_mesh = (Component_Mesh*)(*i)->Get_Component(MESH);

				if (comp_mesh->Is_Rendered())
				{
					if (ray.Intersects(current_node->rect.rect))
					{
						if (current_node->children.size() == 0)
						{
							ret.push_back((*i));
							//Calculate the distance to the ray
							(*i)->Set_Distance_To_Ray((App->camera->Get_Camera_Position() - comp_mesh->Get_AABB_Bounding_Box().CenterPoint()));
						}
					}
				}
			}
		}

		vector<QuadTreeNode*>::const_iterator children_current_node = current_node->children.begin();
		while (children_current_node != current_node->children.end())
		{
			queue_qnode.push(*children_current_node);
			children_current_node++;
		}
	}

	return ret;
}

void  QuadTreeNode::Deactivate_All_GO_Renders()
{
	if (this->children.size() > 0)
	{
		for (vector<QuadTreeNode*>::const_iterator children_node = this->children.begin(); children_node != this->children.end(); children_node++)
		{
			(*children_node)->Deactivate_All_GO_Renders();
		}
	}

	if (this->objects.size() > 0)
	{
		for (vector<GameObject*>::iterator i = objects.begin(); i < objects.end(); ++i)
		{
			if ((*i)->Exist_Component(MESH))
			{
				Component_Mesh* comp_mesh = (Component_Mesh*)(*i)->Get_Component(MESH);
				comp_mesh->Set_Rendered(false);
			}
		}
	}

	
}



//-------------------------------QUADTREE FUNCTIONS--------------------------------------
void p2QuadTree::Create(const float2 size_rect, const float2 center)
{
	if (root != nullptr)
	{
		LOG("There is already quadtreenode created");
	}
	else
	{
		//Create the new root node
		root = new QuadTreeNode(center, size_rect, nullptr);
	}
}

void p2QuadTree::Set_Boundaries_Root(const float2 size_rect)
{
	root->rect.Set_Boundaries_rect(size_rect);
}

math::float2 p2QuadTree::Get_Boundaries()const
{
	float2 size = float2::zero;
	size.x = root->rect.rect.Size().x;
	size.y = root->rect.rect.Size().z;

	return size;
}

bool p2QuadTree::Insert(GameObject& GO)
{
	bool ret = false;
	Component_Mesh* mesh = (Component_Mesh*)GO.Get_Component(MESH);
	if (mesh != nullptr && mesh->Get_Mesh()->num_vertices > 0)
	{
		vec center = mesh->Get_AABB_Bounding_Box().CenterPoint();
		if (root != nullptr && root->Contains(float2(center.x, center.z)))
		{
			ret = root->Insert(GO, float2(center.x, center.z));
		}
	}
	return ret;
}

bool p2QuadTree::Remove(GameObject& GO)
{
	bool ret = false;
	Component_Mesh* mesh = (Component_Mesh*)GO.Get_Component(MESH);
	float2 center_point_go = float2(mesh->Get_AABB_Bounding_Box().CenterPoint().x, mesh->Get_AABB_Bounding_Box().CenterPoint().z);
	if (root != nullptr && root->Contains(center_point_go))
	{
		ret = root->Remove(GO, center_point_go);
	}
	
	return ret;
}

bool p2QuadTree::Clear()
{
	bool ret = false;

	if (root != nullptr)
	{
		root->Clear_Nodes();
		root = nullptr;
	}

	return ret;

}

void p2QuadTree::Draw()
{
	if (root != nullptr)
		root->Draw_Node();
}

//For Camera Culling
void p2QuadTree::Intersects_Quadtree_Nodes(Component_Camera& geo)const
{
	if (root->children.size() > 0)
	{
		root->Intersect_Node(geo);	
	}
}

//Optimize mouse picking
vector<GameObject*>  p2QuadTree::Ray_Intersects_Quadtree_Nodes(const LineSegment& ray)const
{
	vector<GameObject*> ret;

	if (root != nullptr && root->children.size() > 0)
	{
		ret = root->Ray_Intersects_Node(ray);
	}

	return ret;
}

void p2QuadTree::Deactivate_All_GO_Renders()
{
		root->Deactivate_All_GO_Renders();
}
