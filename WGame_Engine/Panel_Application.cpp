#include "Panel_Application.h"
#include "Application.h"
#include "Imgui\imgui.h"


Panel_Application::Panel_Application() : active_window(false)
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
			Visualizate_Window();
		}

		ImGui::End();

		
	}

	return Is_Active();

}