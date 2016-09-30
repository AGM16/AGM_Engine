#include"Application.h"
#include "Console.h"
#include "Globals.h"


Console::Console(Application* app, bool start_enabled) : Module(app, start_enabled), open_window(true)
{

}

Console::~Console()
{
   
}

bool Console::Start()
{
	
	return true;
}

update_status Console::Update(float dt)
{
	Render_Console();

	return UPDATE_CONTINUE;
}

bool Console::CleanUp()
{
	
	text_console.clear();
	return true;
}

void Console::Render_Console()
{
	ImGui::Begin("Console", &open_window);
	
		ImGui::TextUnformatted(text_console.begin());
		ImGui::SetScrollHere(1.0f);
	

	ImGui::End();

}

void Console::Save_Log(const char* text)
{
	text_console.append(text);
}