
#pragma once
#include "MathGeoLib\include\MathGeoLib.h"
#include "Color.h"

using namespace math;

enum PrimitiveTypes
{
	Primitive_Point_Type,
	Primitive_Line_Type,
	Primitive_Plane_Type,
	Primitive_Cube_Type,
	Primitive_Sphere_Type,
	Primitive_Cylinder_Type
};

class Primitive
{
public:

	Primitive();

	virtual void	Render() const;
	virtual void	InnerRender() const;
	void			SetPos(float x, float y, float z);
	void			SetRotation(float angle, const vec &u);
	void			Scale(float x, float y, float z);
	PrimitiveTypes	GetType() const;
	float3				GetPos()const;
public:
	
	Color color;
	float4x4 transform;
	bool axis,wire;

protected:
	PrimitiveTypes type;
};

// ============================================
class Primitive_Cube : public Primitive
{
public :
	Primitive_Cube();
	Primitive_Cube(float sizeX, float sizeY, float sizeZ);
	void InnerRender() const;
public:
	float3 size;
};

// ============================================
class Primitive_Sphere : public Primitive
{
public:
	Primitive_Sphere();
	Primitive_Sphere(float radius);
	void InnerRender() const;
public:
	float radius;
};

// ============================================
class Primitive_Cylinder : public Primitive
{
public:
	Primitive_Cylinder();
	Primitive_Cylinder(float radius, float height);
	void InnerRender() const;
public:
	float radius;
	float height;
};

// ============================================
class Primitive_Line : public Primitive
{
public:
	Primitive_Line();
	Primitive_Line(float x, float y, float z);
	void InnerRender() const;
public:
	float3 origin;
	float3 destination;
};

// ============================================
class Primitive_Plane : public Primitive
{
public:
	Primitive_Plane();
	Primitive_Plane(float x, float y, float z, float d);
	void InnerRender() const;
public:
	float3 normal;
	float constant;
};