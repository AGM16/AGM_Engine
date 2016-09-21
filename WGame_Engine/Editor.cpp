#include "Globals.h"
#include "Application.h"
#include "Editor.h"
#include "MathGeoLib\include\Algorithm\Random\LCG.h"

using namespace std;

Editor::Editor(Application* app, bool start_enabled) : Module(app, start_enabled), plane(0,1,0,0)
{
}

Editor::~Editor()
{}

// Load assets
bool Editor::Start()
{
	LOG("Loading Intro assets");

	App->camera->Move(float3(5.0f, 6.0f, 15.0f));
	App->camera->LookAt(float3(0, 0, 0));
	render_aabb = false;

	plane.axis = true;
    Create_Points(6);
	Create_AABB_Box();
	return true;
}


// Update
update_status Editor::Update(float dt)
{
	
	plane.Render();

	Render();

	return UPDATE_CONTINUE;
}


// Load assets
bool Editor::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

//Functions
void  Editor::Create_Points(int amount)
{
	float3 pos_vector;
	Random rand;

	for (int num = 0; amount > num; num++)
	{
		pos_vector = rand.Random_Float_Vector(1, 5);

		Primitive_Sphere spheres(0.3f);
		spheres.SetPos(pos_vector.x, pos_vector.y, pos_vector.z);
		spheres.GetPos();
		spheres.color.g = 255.0f;
	    spheres.axis = true;

		s_points.push_back(spheres);
	}
}

void Editor::Render()
{
	for (list<Primitive_Sphere>::iterator points = s_points.begin(); points != s_points.end(); ++points)
	{
		(*points).Render();
	}

	if (render_aabb)
		aabb_bounding_box.Render();
}

void Editor::Create_AABB_Box()
{
	AABB aabb_box;

	float3 position(0,0,0);
	float3 min_position(0,0,0);
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

