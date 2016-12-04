// ----------------------------------------------------
// Quadtree implementation --
// ----------------------------------------------------

#ifndef __P2QUADTREE_H__
#define __P2QUADTREE_H__

#include "GameObject.h"
#include "Components.h"
#include "Component_Transformation.h"
#include "Component_Mesh.h"
#include "Component_Camera.h"
#include "SDL\include\SDL.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include  <vector>
#include "Box.h"


// Tree node -------------------------------------------------------
class QuadTreeNode
{

public:

	Box				             rect;
	std::vector<GameObject*>     objects;
	QuadTreeNode*			     parent;
	std::vector<QuadTreeNode*>   children;

public:

	QuadTreeNode(float2 center_rect, float2 size, QuadTreeNode* parent_n) : rect(center_rect, size), parent(parent_n)
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

	
		for (vector<GameObject*>::iterator i = objects.begin(); i < objects.end(); ++i)
		{
			if ((*i) != nullptr)
			{
				delete (*i);
				(*i) = nullptr;
			}
		}

		objects.clear();
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

	//Camera Culling
	void Intersect_Node(Component_Camera& geo);

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

	//Camera Culling
	void Intersects_Quadtree_Nodes(Component_Camera& geo)const;

public:

	QuadTreeNode*	root;

};

#endif // __p2QuadTree_H__*/