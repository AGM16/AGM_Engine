#include "Application.h"
#include "Hardware_Info.h"
#include "SDL\include\SDL.h"

Hardware_Info::Hardware_Info(Application* app, bool start_enabled) : Module(app, start_enabled), open_window_hardware(true), color()
{

}

Hardware_Info::~Hardware_Info()
{

}

bool Hardware_Info::Init()
{
	//Initilize variables
	cpu = SDL_GetCPUCount();
	cpu_cache = SDL_GetCPUCacheLineSize();
	ram = SDL_GetSystemRAM();

	//Check if exist
	caps = " ";

	if (SDL_HasRDTSC())
		caps.append( "RDTSC  ");

	if (SDL_HasMMX())
		caps.append("MMX  ");

	if (SDL_HasSSE())
		caps.append("SSE  ");

	if (SDL_HasSSE2())
		caps += "SSE2  ";

	if (SDL_HasSSE3())
		caps.append("SSE3  ");

	if (SDL_HasSSE41())
		caps.append("SSE41  ");

	if (SDL_HasSSE42())
		caps.append("SSE42  ");

	if (SDL_HasAVX())
		caps.append("AVX  ");

	return true;
}

update_status Hardware_Info::Update(float dt)
{
	Render_Hardware_Info();

	return UPDATE_CONTINUE;
}

bool Hardware_Info::CleanUp()
{
	return true;
}

void Hardware_Info::Render_Hardware_Info()
{
	if (open_window_hardware)
	{
		ImGui::CollapsingHeader("Hardware_Info");

		ImGui::Text("CPUs: ");
		ImGui::SameLine();
		ImGui::TextColored(color, "%d", cpu);

		ImGui::Text("CPUs cache: ");
		ImGui::SameLine();
		ImGui::TextColored(color, "%d  KB", cpu_cache);

		ImGui::Text("RAM: ");
		ImGui::SameLine();
		ImGui::TextColored(color, "%.f  GB", ram);


		ImGui::Text("Cap: ");
		ImGui::SameLine();
		ImGui::TextColored(color, "%s", caps);


	}
}