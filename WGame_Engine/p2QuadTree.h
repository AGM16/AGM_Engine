// ----------------------------------------------------
// Quadtree implementation --
// ----------------------------------------------------

#ifndef __P2QUADTREE_H__
#define __P2QUADTREE_H__

#include "GameObject.h"
#include "Components.h"
#include "Component_Transformation.h"
#include "Component_Mesh.h"
#include "SDL\include\SDL.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include  <vector>
#include "Box.h"


// Tree node -------------------------------------------------------
class QuadTreeNode
{

public:

	Box				             rect;
	GameObject*             	 objects;
	QuadTreeNode*			     parent;
	std::vector<QuadTreeNode*>   children;

public:

	QuadTreeNode(float2 center_rect, float2 size, QuadTreeNode* parent_n) : rect(center_rect, size), parent(parent_n), objects(nullptr)
	{

	}

	~QuadTreeNode()
	{
		for (vector<QuadTreeNode*>::iterator i = children.begin(); i < children.end(); ++i)
		{
			if ((*i) != nullptr)
			{
				delete (*i);
				(*i) = nullptr;
			}
		}

		children.clear();

		parent = nullptr;
		objects = nullptr;
	}

	void Subdivide();

	bool Contains(const float2 position) const
	{
		return rect.contains(position);
	}
	 
	bool Insert(GameObject* GO, float2 center_position);
	bool Remove(GameObject* GO, float2 center_position);
	bool Clear_Nodes();

	void Draw_Node();


};

// Tree class -------------------------------------------------------
class p2QuadTree
{
public:

	// Constructor
	p2QuadTree() : root(NULL)
	{}

	// Destructor
	virtual ~p2QuadTree()
	{
		Clear();
	}

	void Create(float2 size_rect, float2 center);

	bool Insert(GameObject* GO);

	bool Remove(GameObject* GO);
	
	bool Clear();

	void Draw();


public:

	QuadTreeNode*	root;

};

#endif // __p2QuadTree_H__*/