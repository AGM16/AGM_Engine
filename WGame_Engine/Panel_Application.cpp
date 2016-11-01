#include "Panel_Application.h"
#include "Application.h"
#include "Imgui/imgui.h"
#include "MathGeoLib\include\MathGeoLib.h"

using namespace std;

Panel_Application::Panel_Application() : active_window(true), fullscreen(false), fullscreen_desktop(false), borderless(true), resizable(false)
{

}

Panel_Application::~Panel_Application()
{

}

void Panel_Application::Visualizate_Window()
{
	active_window = !active_window;
}

bool Panel_Application::Is_Active()
{
	return active_window;
}

bool Panel_Application::Draw_Panel_Application()
{
	if (Is_Active())
	{
		ImVec2 size_w;
		float2 pos;
		if (App->Get_Windows_Resized() == false)
		{
			size_w = ImVec2(290.f, 300.f);
			pos = float2(291, 332);
		}
		else
		{
			size_w = ImVec2(380.f, 375.f);
			pos = float2(382, 639);
		}
		ImGui::SetNextWindowPos(ImVec2(App->window->Get_Screen_size().x - pos.x, pos.y));
		ImGui::SetNextWindowSize(size_w);
		ImGui::Begin("Computer");

		//--------------------------------------_FPS_-------------------------------------------
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

		//------------------------------------_HARDWARE_------------------------------------------
		if (ImGui::CollapsingHeader("Hardware"))
		{
			App->hardware->Render_Hardware_Info();
		}

		//------------------------------------_WINDOW_------------------------------------------
		if (ImGui::CollapsingHeader("Window"))
		{
			if (ImGui::Checkbox("FULLSCREEN", &fullscreen))
			{
				if (fullscreen_desktop == true || borderless == true)
				{
					fullscreen_desktop = false;
					App->window->Set_Fullscreen_Desktop_Window(fullscreen_desktop);

				}

				App->window->Set_Fullscreen_Window(fullscreen);
			}

			if (ImGui::Checkbox("FULLSCREEN DESKTOP", &fullscreen_desktop))
			{
				if (fullscreen == true || borderless == true)
				{
					fullscreen = false;
					App->window->Set_Fullscreen_Window(fullscreen);
				}

				App->window->Set_Fullscreen_Desktop_Window(fullscreen_desktop);
			}

			if (ImGui::Checkbox("BORDERLESS", &borderless))
			{
				if (fullscreen == true || fullscreen_desktop == true)
				{
					fullscreen = false;
					App->window->Set_Fullscreen_Window(fullscreen);
					//fullscreen_desktop = false;	
					//App->window->Set_Fullscreen_Desktop_Window(fullscreen_desktop);
				}

				App->window->Set_Borderless_Window(borderless);
			}

		}


		if (ImGui::Button("Close"))
		{
			Visualizate_Window();
		}

		ImGui::End();

		
	}

	return Is_Active();

}

