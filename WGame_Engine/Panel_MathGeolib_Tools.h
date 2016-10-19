#ifndef _PANEL_MATHGEOLIB_TOOLS_H_
#define _PANEL_MATHGEOLIB_TOOLS_H_


#include "Primitive.h"
#include  <list>



class Panel_MathGeolib_Tools
{

public:
	
	Panel_MathGeolib_Tools();
	~Panel_MathGeolib_Tools();

	void Create_Points(int amount);
	void Create_AABB_Box();
	void Rotate_Bounding_Box(float x, float y, float z);
	void Sphere_Bounding_Box();

	void Render();
	void Clean_Up_Variables();

private:

	std::list<Primitive_Sphere> s_points;

	Primitive_Cube aabb_bounding_box;
	Primitive_Sphere bounding_sphere;

	bool render_aabb;
	bool render_bounding_sphere;

	AABB aabb_box;
};

#endif // !_PANEL_MATHGEOLIB_TOOLS_H_

