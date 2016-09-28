#pragma once

#include <list>
#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"
#include "Editor.h"
#include "FPS_Info.h"
#include "Console.h"

using namespace std; 

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleSceneIntro* scene_intro;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModulePhysics3D* physics;
	Editor*     editor;
	FPS_Info*        fps_info;
	Console*        console;

private:

	//FPS Info
	Timer	ms_timer;
	Timer	last_sec_frame;
	int     prev_frames_per_sec;
	int     frames_per_sec;
	int     max_frames;
	double     time_per_frame;
	double     time_last_frame;
	bool       console_exists;

	


	float	dt;
	list<Module*> list_modules;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	void RquestBrowser(const char* url);
	
	//FPS 
	bool Sec_Counter(double time);
	void Delay(double ms_time, double time_per_frame);
	int Get_Last_FPS()const;
	double Get_Last_Frame_Time()const;
	int Get_Limit_Frames()const;
	void Set_Limit_Frames(int max_fps);
	bool Consoler_Exist();
	bool Set_Console(bool exist);

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

	
};

extern Application* App;