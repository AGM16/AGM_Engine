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

bool QuadTreeNode::Insert(GameObject* GO, float2 center_position)
{
	bool ret = false;

	Component_Mesh* mesh = (Component_Mesh*)GO->Get_Component(MESH);

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
					if (current_node->objects == nullptr)//The node is available to catch this GO
					{
						current_node->objects = GO;
						return true;
					}
					else
					{
						/*It means that we have to Subdivide the QuadtreeNode
						and reorganize the gameObjects*/
						current_node->Subdivide();

						//Insert again the old objects
						Component_Mesh* mesh = (Component_Mesh*)current_node->objects->Get_Component(MESH);
						vec center = mesh->Get_AABB_Bounding_Box().CenterPoint();
						
						if (current_node->Insert(current_node->objects, float2(center.x, center.y)) != false)
						{
							current_node->objects = nullptr;
							//Insert new object
							ret = current_node->Insert(GO, center_position);
							break;
						}
						else
						{
							ret = false;
							break;
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
	}

	return ret;
}

bool QuadTreeNode::Remove(GameObject* GO, float2 center_position)
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
			if (current_node->objects == GO && current_node->rect.centre.Equals(center_position))
			{
				current_node->objects = nullptr;
				ret = true;
				break;
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
	objects = nullptr;

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


//-------------------------------QUADTREE FUNCTIONS--------------------------------------
void p2QuadTree::Create(float2 size_rect, float2 center)
{
	if (root != nullptr)
		delete root;

	//Create the new root node
	root = new QuadTreeNode(center, size_rect, nullptr);
}

bool p2QuadTree::Insert(GameObject* GO)
{
	bool ret = false;
	Component_Transformation* transform = (Component_Transformation*)GO->Get_Component(TRANSFORMATION);
	Component_Mesh* mesh = (Component_Mesh*)GO->Get_Component(MESH);
	if (mesh != nullptr && mesh->Get_Mesh()->num_vertices > 0)
	{
		float2 center_point_go = float2(transform->Get_Position().x, transform->Get_Position().y);
		vec center = mesh->Get_AABB_Bounding_Box().CenterPoint();
		if (root != nullptr && root->Contains(float2(center.x, center.z)))
		{
			ret = root->Insert(GO, float2(center.x, center.z));
		}
	}

	return ret;
}

bool p2QuadTree::Remove(GameObject* GO)
{
	bool ret = false;
	Component_Mesh* mesh = (Component_Mesh*)GO->Get_Component(MESH);
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

	if (root != nullptr && root->children.size() > 0)
	{
		root->Clear_Nodes();
	}

	return ret;

}

void p2QuadTree::Draw()
{
	if (root != nullptr && root->children.size() > 0)
		root->Draw_Node();
}