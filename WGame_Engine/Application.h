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
#include "ModuleGeometry.h"
#include "ModuleFileSystem.h"
#include "Editor.h"
#include "FPS_Info.h"
#include "Console.h"
#include "Hardware_Info.h"
#include "Module_Go_Manager.h"
#include "PugiXml\src\pugixml.hpp"
#include "PerfTimer.h"



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
	ModuleGeometry*  geometry;
	ModuleFileSystem* filesystem;
	Hardware_Info*    hardware;
	Module_Go_Manager* go_manager;

    
private:

	//FPS Info
	Timer	ms_timer;
	Timer	last_sec_frame;
	int     prev_frames_per_sec;
	int     frames_per_sec;
	float     time_per_frame;
	double     time_last_frame;
	bool       console_exists;
	int     max_frames;

	//Time Manager
	PerfTimer* time_manager;

	std::string		title;
	std::string		organization;

	bool    window_resized;

	float	dt;
	list<Module*> list_modules;

	//XML Load/Save
	mutable bool	        	want_to_save = false;
	bool				        want_to_load = false;
	std::string			        load_game;
	mutable std::string     	save_game;

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
	float Get_Delta_Time()const;

	//Window
	void Windows_Resized();
	bool Get_Windows_Resized();

	//Console
	bool Consoler_Exist();
	bool Set_Console(bool exist);
	void Log_Console(const char* text);

	//XML
	void LoadGame(const char* file);
	void SaveGame(const char* file) const;

	//Load / Save
	bool LoadGameNow();
	bool SavegameNow() const;

	//Title and Organisation
	const char* Get_Title()const;
	const char* Get_Organization()const;

	//Time Manager
	PerfTimer* Get_Time_Manager()const;



private:
	// Load config file
	pugi::xml_node LoadConfig(pugi::xml_document&) const;


	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

	
};

extern Application* App;