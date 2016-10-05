#include "Globals.h"
#include "Application.h"
#include "Editor.h"
#include "MathGeoLib\include\Algorithm\Random\LCG.h"
#include "Imgui\imgui.h"

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

	open_application = false;



	render_aabb = false;
	render_bounding_sphere = false;
	show_test_window = false;
	

	plane.axis = true;
	Create_AABB_Box();
	Sphere_Bounding_Box();
	return true;
}


// Update
update_status Editor::Update(float dt)
{
	
	plane.Render();

	//Menu Bar ---------------------------
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Quit"))
			{
				return UPDATE_STOP;
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{

			if (ImGui::MenuItem("Test Window"))
			{
				show_test_window = !show_test_window;
			}

			if (ImGui::MenuItem("MathGeoLib Documentation"))
			{
				App->RquestBrowser("http://clb.demon.fi/MathGeoLib/");
			}


			ImGui::EndMenu();
		}


		if (ImGui::BeginMenu(" MathGeoLib Tools"))
		{

			if (ImGui::MenuItem("Create Random Points"))
			{
				Create_Points(6);
			}

			if (ImGui::MenuItem("Create AABB"))
			{
				Create_AABB_Box();
			}

			if (ImGui::MenuItem("Create Bounding Sphere"))
			{
				Sphere_Bounding_Box();
			}
			if (ImGui::MenuItem("Clear"))
			{
				s_points.clear();
				aabb_bounding_box.transform.identity;
				bounding_sphere.transform.identity;
				render_aabb = false;
				render_bounding_sphere = false;
			}


			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Computer_Info"))
		{

			if (ImGui::MenuItem("Application"))
			{
			
			  open_application = !open_application;
	
			}

			if (ImGui::MenuItem("Console"))
			{
                App->console->Set_Open_Window(true);
			}

			ImGui::EndMenu();
		}


		ImGui::EndMainMenuBar();
	}

	

	if (open_application)
	{
		Application_Window();
	}


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

void Editor::Render()
{
	for (list<Primitive_Sphere>::iterator points = s_points.begin(); points != s_points.end(); ++points)
	{
		(*points).Render();
	}

	if (render_aabb)
		aabb_bounding_box.Render();

	if (render_bounding_sphere)
		bounding_sphere.Render();

	if (show_test_window)
		ImGui::ShowTestWindow();

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


void Editor::Rotate_Bounding_Box(float x, float y, float z)
{
	Quat quaternion;

	quaternion = quaternion.FromEulerXYZ(x, y, z);

	aabb_bounding_box.transform = quaternion * aabb_bounding_box.transform;
}

void Editor::Sphere_Bounding_Box()
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


void Editor::Application_Window()
{
	ImGui::Begin("Computer");

	if (ImGui::CollapsingHeader("FPS"))
	{
		char title[25];
		sprintf_s(title, 25, "Framerate %.1f", App->fps_info->frames[App->fps_info->frames.size() - 1]);
		ImGui::PlotHistogram("##framerate", &App->fps_info->frames[0], App->fps_info->frames.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));

		char title2[25];
		sprintf_s(title2, 25, "Milliseconds %0.1f", App->fps_info->ms[App->fps_info->ms.size() - 1]);
		ImGui::PlotHistogram("##milliseconds", &App->fps_info->ms[0], App->fps_info->ms.size(), 0, title2, 0.0f, 40.0f, ImVec2(310, 100));

		

		if (ImGui::SliderInt("Max FPS", &App->fps_info->max_frames, 0, 100))
		{
			App->Set_Limit_Frames(App->fps_info->max_frames);
		}
	}

	if (ImGui::CollapsingHeader("Hardware"))
	{
		App->hardware->Render_Hardware_Info();
	}	

	if (ImGui::Button("Close"))
	{
		open_application = false;
	}

	ImGui::End();

}