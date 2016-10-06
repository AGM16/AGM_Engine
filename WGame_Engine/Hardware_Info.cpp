#include "Application.h"
#include "Hardware_Info.h"
#include "SDL\include\SDL.h"

Hardware_Info::Hardware_Info(Application* app, bool start_enabled) : Module(app, start_enabled), window_active(true), color(1.f,1.f,0.f,1.f)
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
	caps.assign("");

	if (SDL_HasRDTSC())
		caps.append( "RDTSC ");

	if (SDL_HasMMX())
		caps.append("MMX ");

	if (SDL_HasSSE())
		caps.append("SSE ");

	if (SDL_HasSSE2())
		caps += "SSE2 ";

	if (SDL_HasSSE3())
		caps.append("SSE3 ");

	if (SDL_HasSSE41())
		caps.append("SSE41 ");

	if (SDL_HasSSE42())
		caps.append("SSE42 ");

	if (SDL_HasAVX())
		caps.append("AVX ");

	return true;
}

update_status Hardware_Info::Update(float dt)
{
	caps.clear();
	return UPDATE_CONTINUE;
}

bool Hardware_Info::CleanUp()
{
	return true;
}

void Hardware_Info::Render_Hardware_Info()
{
	if (window_active)
	{

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
		ImGui::TextColored(color, "%s", caps.c_str());


	}
}

void Hardware_Info::Open_Window(bool active)
{
	window_active = active;
}