#include "Globals.h"
#include "Application.h"
#include "Editor.h"
#include "MathGeoLib\include\Algorithm\Random\LCG.h"
#include "Imgui\imgui.h"

using namespace std;

Editor::Editor( bool start_enabled) : Module(start_enabled), plane(0,1,0,0)
{
	Set_Name_Module("Editor");
}

Editor::~Editor()
{}

// Load assets
bool Editor::Start()
{
	LOG("Loading Intro assets");



	show_test_window = false;
	

	plane.axis = true;
	return true;
}


// Update
update_status Editor::Update(float dt)
{
	
	plane.Render();

	
	//ImGui::ShowStyleEditor();


	//Menu Bar ---------------------------
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Load"))
			{
				load = true;
			}

			/*This item will save the information in the last xml save_file document that 
			we have create*/
			if (ImGui::MenuItem("Save"))
			{
				string name_last_file = last_name_file_saved;
				if (name_last_file.size() > 0)
				{
					//Save automatically in the last xml file that we have loaded or saved
					//Get the dir of the scene where we contain the xml files
					string name_file = "/Library/";
					string scene = App->geometry->Get_Scene();
					scene.append("/");
					name_file.append(scene.c_str());
					name_file.append(last_name_file_saved);
					App->SaveGame(name_file.c_str());
				}
				else
				{
					LOG("There is no xml document to save the information");
				}

				
			}

			if (ImGui::MenuItem("Save As"))
			{
				save = true;
			}

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


	//Render Windows Save As
	Render_Panel_Save_As();

	//Render Windows Load
	Render_Panel_Load();

	//Render MathGeoLib Geometry
	panels.mathgeolib_panel.Render();

	//Render panel Time manager
	Render_Panel_Time_Manager();

	return UPDATE_CONTINUE;
}

void Editor::Render_Panel_Save_As()
{
	if (save)
	{
		ImVec2 size_w;
		float2 pos;
		if (App->Get_Windows_Resized() == false)
		{
			size_w = ImVec2(290.f, 100.f);
			pos = float2(550, 320);
		}
		else
		{
			size_w = ImVec2(290.f, 100.f);
			pos = float2(1150, 320);
		}

		ImGui::SetNextWindowPos(ImVec2(App->window->Get_Screen_size().x - pos.x, pos.y));
		ImGui::SetNextWindowSize(size_w);
		ImGui::Begin("Save");

		//Clear buffer
		static int clear = 0;
		if (clear == 0)
		{
			sprintf_s(last_name_file_saved, 100, "\0");
			clear++;
		}

		ImGui::InputText(".xml", last_name_file_saved, sizeof(last_name_file_saved));

		if (ImGui::Button("Close", ImVec2(50, 20)))
		{
			//Check it the name of the file is empty
			string name = last_name_file_saved;
			if (name.size() > 0)
			{
				string name_file = "/Library/";
				string scene = App->geometry->Get_Scene();
				scene.append("/");
				name_file.append(scene.c_str());
				sprintf_s(last_name_file_saved, 100, "%s.%s", last_name_file_saved, "xml");
				name_file.append(last_name_file_saved);

				if (App->filesystem->Exists(name_file.c_str()) == false)
					App->SaveGame(name_file.c_str());
				else
					LOG("The information have not been saved. The xml file %s already exists", last_name_file_saved);
			}

			clear = 0;
			save = false;
		}

		ImGui::End();
	}
}

void Editor::Render_Panel_Load()
{
	if (load)
	{
		ImVec2 size_w;
		float2 pos;
		if (App->Get_Windows_Resized() == false)
		{
			size_w = ImVec2(290.f, 100.f);
			pos = float2(550, 320);
		}
		else
		{
			size_w = ImVec2(290.f, 100.f);
			pos = float2(1150, 320);
		}

		ImGui::SetNextWindowPos(ImVec2(App->window->Get_Screen_size().x - pos.x, pos.y));
		ImGui::SetNextWindowSize(size_w);
		ImGui::Begin("Load");
		ImGui::Text("Files :");

		//I put the if to don't repeate every update this process		
		if (!get_xml_Files)
		{
			//Obtain the save_files documents that there is in this scene
			string dir_scene = "/Library/";
			dir_scene.append(App->geometry->Get_Scene());

			xml_files = App->filesystem->Get_Documents_XML_From_Path(dir_scene.c_str());
			get_xml_Files = true;
		}

		for (vector<string>::const_iterator i = xml_files.begin(); i != xml_files.end(); i++)
		{
			ImGui::Selectable((*i).c_str());
			if (ImGui::IsItemClicked(0))
			{
				//Change the last_name_file_saved to save automatically when we clic Save instead Save As
				sprintf_s(last_name_file_saved, 100, (*i).data());

				//Get the dir of the scene where we contain the cml files
				string name_file = "/Library/";
				string scene = App->geometry->Get_Scene();
				scene.append("/");
				name_file.append(scene.c_str());
				name_file.append((*i).c_str());

				App->LoadGame(name_file.c_str());

				get_xml_Files = false;
				load = false;

				break;
			}
		}

		if (ImGui::Button("Close", ImVec2(50, 20)))
		{
			load = false;
			get_xml_Files = false;
		}

		ImGui::End();
	}
}


// Load assets
bool Editor::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

bool Editor::Is_Saving()const
{
	return save;
}

bool Editor::Is_Loading()const
{
	return load;
}

void Editor::Render_Panel_Time_Manager()
{
	ImVec2 size_w;
	float2 pos;
	if (App->Get_Windows_Resized() == false)
	{
		size_w = ImVec2(290.f, 100.f);
		pos = float2(550, 320);
	}
	else
	{
		size_w = ImVec2(290.f, 100.f);
		pos = float2(1150, 320);
	}

	ImGui::SetNextWindowPos(ImVec2(App->window->Get_Screen_size().x - pos.x, pos.y));
	ImGui::SetNextWindowSize(size_w);
	bool open_window = true;
	ImGui::Begin("Time_Manager",&open_window);


	if (ImGui::Button("Play", ImVec2(50, 20)))
	{
	   //Save information

       //Call to the Play function
		App->Get_Time_Manager()->Play();
	}

	ImGui::SameLine();
	if (ImGui::Button("Stop", ImVec2(50, 20)))
	{
		//Load Information

		//Call to the Stop function
		App->Get_Time_Manager()->Stop();
	}

	ImGui::SameLine();
	if (ImGui::Button("Pause", ImVec2(50, 20)))
	{
		//Call to the Pause function
		App->Get_Time_Manager()->Pause();
	}

	ImGui::SameLine();
	ImGui::Text("Gameclock: %f", (float)App->Get_Time_Manager()->ReadSeconds_GameClock());


	ImGui::End();
}


