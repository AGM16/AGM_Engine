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


	show_test_window = false;
	

	plane.axis = true;
	return true;
}


// Update
update_status Editor::Update(float dt)
{
	
	plane.Render();

	//AGM IF you want to change the ImGui style...
	//ImGui::ShowStyleEditor();

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

			if (ImGui::MenuItem("Assimp Documentation"))
			{
				App->RquestBrowser("http://www.assimp.org/lib_html/classes.html");
			}


			ImGui::EndMenu();
		}


		if (ImGui::BeginMenu(" MathGeoLib Tools"))
		{

			if (ImGui::MenuItem("Create Random Points"))
			{
				panels.mathgeolib_panel.Create_Points(6);
			}

			if (ImGui::MenuItem("Create AABB"))
			{
				panels.mathgeolib_panel.Create_AABB_Box();
			}

			if (ImGui::MenuItem("Create Bounding Sphere"))
			{
				panels.mathgeolib_panel.Sphere_Bounding_Box();
			}
			if (ImGui::MenuItem("Clear"))
			{
				panels.mathgeolib_panel.Clean_Up_Variables();
			}


			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Computer_Info"))
		{

			if (ImGui::MenuItem("Application"))
			{
			    panels.app_panel.Visualizate_Window();
			}

			if (ImGui::MenuItem("Console"))
			{
                App->console->Set_Open_Window(true);
			}

			ImGui::EndMenu();
		}


		ImGui::EndMainMenuBar();
	}

	//Render Panels
	panels.app_panel.Draw_Panel_Application();

    if (show_test_window)
		ImGui::ShowTestWindow(&show_test_window);


	//Render MathGeoLib Geometry
	panels.mathgeolib_panel.Render();

	return UPDATE_CONTINUE;
}


// Load assets
bool Editor::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

