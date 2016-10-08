#include "Panel_MathGeolib_Tools.h"
#include "Application.h"

Panel_MathGeolib_Tools::Panel_MathGeolib_Tools() : render_aabb(false), render_bounding_sphere(false)
{

}

Panel_MathGeolib_Tools::~Panel_MathGeolib_Tools()
{

}



void Panel_MathGeolib_Tools::Render()
{
	for (list<Primitive_Sphere>::iterator points = s_points.begin(); points != s_points.end(); ++points)
	{
		(*points).Render();
	}

	if (render_aabb)
		aabb_bounding_box.Render();

	if (render_bounding_sphere)
		bounding_sphere.Render();

}


void Panel_MathGeolib_Tools::Clean_Up_Variables()
{
	s_points.clear();
	aabb_bounding_box.transform.identity;
	bounding_sphere.transform.identity;
	render_aabb = false;
	render_bounding_sphere = false;
}


void  Panel_MathGeolib_Tools::Create_Points(int amount)
{
	float3 pos_vector;
	LCG rand;

	for (int num = 0; amount > num; num++)
	{
		Primitive_Sphere spheres(0.3f);
		spheres.SetPos(rand.Float(1, 5), rand.Float(1, 5), rand.Float(1, 5));
		spheres.color.g = 255.0f;
		spheres.axis = true;

		s_points.push_back(spheres);
	}
}

void Panel_MathGeolib_Tools::Create_AABB_Box()
{
	AABB aabb_box;

	float3 position(0, 0, 0);
	float3 min_position(0, 0, 0);
	float3 max_position(0, 0, 0);

	//Search the bounds of the primitive
	if (!s_points.empty())
	{
		list<Primitive_Sphere>::iterator points = s_points.begin();

		min_position = max_position = (*points).GetPos();

		while (points != s_points.end())
		{
			// X axis
			//Min x axis
			if (min_position.x > (*points).GetPos().x)
				min_position.x = (*points).GetPos().x;

			//Max x axis
			if (max_position.x < (*points).GetPos().x)
				max_position.x = (*points).GetPos().x;

			// Y axis
			//Min y axis
			if (min_position.y >(*points).GetPos().y)
				min_position.y = (*points).GetPos().y;

			//Max y axis
			if (max_position.y < (*points).GetPos().y)
				max_position.y = (*points).GetPos().y;

			// Z axis
			//Min z axis
			if (min_position.z >(*points).GetPos().z)
				min_position.z = (*points).GetPos().z;

			//Max z axis
			if (max_position.z < (*points).GetPos().z)
				max_position.z = (*points).GetPos().z;

			++points;
		}


		aabb_box.minPoint = min_position;
		aabb_box.maxPoint = max_position;


		//Size of the box
		aabb_bounding_box.size = aabb_box.Size();

		//Center of the group of points
		position.x = (max_position.x - min_position.x) / 2 + min_position.x;
		position.y = (max_position.y - min_position.y) / 2 + min_position.y;
		position.z = (max_position.z - min_position.z) / 2 + min_position.z;


		aabb_bounding_box.SetPos(position.x, position.y, position.z);
		aabb_bounding_box.wire = true;
		render_aabb = true;


	}
}


void Panel_MathGeolib_Tools::Rotate_Bounding_Box(float x, float y, float z)
{
	Quat quaternion;

	quaternion = quaternion.FromEulerXYZ(x, y, z);

	aabb_bounding_box.transform = quaternion * aabb_bounding_box.transform;
}

void Panel_MathGeolib_Tools::Sphere_Bounding_Box()
{
	if (render_aabb)
	{
		OBB oobb_box;
		oobb_box.pos = aabb_bounding_box.GetPos();
		oobb_box.r = float3(aabb_bounding_box.size.x / 2, aabb_bounding_box.size.y / 2, aabb_bounding_box.size.z / 2);


		//Axis z,y,z
		float3x3 rotation_matrix = aabb_bounding_box.transform.RotatePart();

		oobb_box.axis[0] = rotation_matrix * float3(1.0f, 0.0f, 0.0f);
		oobb_box.axis[1] = rotation_matrix * float3(0.0f, 1.0f, 0.0f);
		oobb_box.axis[2] = rotation_matrix * float3(0.0f, 0.0f, 1.0f);


		Sphere math_sphere = oobb_box.MinimalEnclosingSphere();

		bounding_sphere.SetPos(oobb_box.pos.x, oobb_box.pos.y, oobb_box.pos.z);
		bounding_sphere.radius = math_sphere.r;
		bounding_sphere.wire = true;

		render_bounding_sphere = true;

	}

}