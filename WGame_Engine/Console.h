#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include "Module.h"
#include "Globals.h"
#include "Imgui\imgui.h"
#include "Editor.h"

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
	void Set_Open_Window(bool on);

    ImGuiTextBuffer text_console;

private:

	bool open_window;
};

#endif




