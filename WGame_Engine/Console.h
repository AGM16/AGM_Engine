#pragma once
#include "Module.h"
#include "Globals.h"
#include "Imgui\imgui.h"

class Console : public Module
{

public:
	Console(Application* app, bool start_enabled = true);
	~Console();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Render_Console();
	void Save_Log(const char* text);

    ImGuiTextBuffer text_console;

private:

	bool open_window;
};




