// ----------------------------------------------------
// Quadtree implementation --
// ----------------------------------------------------

#ifndef __P2QUADTREE_H__
#define __P2QUADTREE_H__

#include "GameObject.h"
#include "Components.h"
#include "Component_Transformation.h"
#include "Component_Mesh.h"
#include "Module_Go_Manager.h"
#include "SDL\include\SDL.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include  <vector>

struct Mesh;

#define QUADTREE_MAX_ITEMS 2

struct AABB_Box
{
	SDL_Rect rect;
	math::float2 centre;
	math::float2 size;

	AABB_Box(math::float2 centre_value, math::float2 Size_value ) : centre(centre_value), size(Size_value)
	{
		rect.x = centre_value.x - (size.x / 2);
		rect.y = centre_value.y - (size.y / 2);
	};

	bool contains(const math::float2 pos_go) const
	{
		if (pos_go.x < centre.x + size.x && pos_go.x > centre.x - size.x)
		{
			if (pos_go.y < centre.y + size.y && pos_go.y > centre.y - size.y)
			{
				return true;
			}
		}
		return false;
	}

	math::float2 Get_Center()const
	{
		return centre;
	}

	math::float2 Get_Size()const
	{
		return size;
	}

	void Clear()
	{
		centre = float2::zero;
		size = float2::zero;
		rect.h = 0;
		rect.w = 0;
		rect.x = 0;
		rect.y = 0;
	}
};


// Tree node -------------------------------------------------------
class QuadTreeNode
{

public:

	AABB_Box				     rect;
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
	

	/*int CollectCandidates(p2DynArray<Collider*>& nodes, const SDL_Rect& r) const
	{
		// TODO: Omplir el array "nodes" amb tots els colliders candidats
		// de fer intersecció amb el rectangle r
		// retornar el número de intersección calculades en el procés
		// Nota: és una funció recursiva
		return 0;
	}

	void CollectRects(p2DynArray<p2QuadTreeNode*>& nodes) 
	{
		nodes.PushBack(this);

		for(int i = 0; i < 4; ++i)
			if(childs[i] != NULL) childs[i]->CollectRects(nodes);
	}*/

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


	/*int CollectCandidates(p2DynArray<GameObject*>& nodes, const SDL_Rect& r) const
	{
		int tests = 1;
		if(root != NULL && Intersects(root->rect, r))
			tests = root->CollectCandidates(nodes, r);
		return tests;
	}

	void CollectRects(p2DynArray<QuadTreeNode*>& nodes) const
	{
		if(root != NULL)
			root->CollectRects(nodes);
	}*/

public:

	QuadTreeNode*	root;

};

#endif // __p2QuadTree_H__*/