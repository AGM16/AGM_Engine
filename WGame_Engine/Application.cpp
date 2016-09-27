#include "Application.h"


Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	audio = new ModuleAudio(this, true);
	scene_intro = new ModuleSceneIntro(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	physics = new ModulePhysics3D(this);
	editor = new Editor(this);
	fps_info = new FPS_Info(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);

	//FPS
	AddModule(fps_info);

	AddModule(camera);
	AddModule(input);
	AddModule(audio);
	AddModule(physics);


	// Scenes
	AddModule(scene_intro);
	AddModule(editor);
	

	// Renderer last!
	AddModule(renderer3D);


	//FPS Info
	prev_frames_per_sec = 0; 
	frames_per_sec = 0;
	max_frames = 60; 
	time_per_frame = 1000 / 60;
}

Application::~Application()
{
	list<Module*>::reverse_iterator i = list_modules.rbegin();

	while (i != list_modules.rend())
	{
		delete (*i);
		++i;
	}
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules
	list<Module*>::iterator i = list_modules.begin();

	while (i != list_modules.end() && ret == true)
	{
		ret = (*i)->Init();
		++i;
	}


	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	i = list_modules.begin();

	while(i != list_modules.end() && ret == true)
	{
		ret = (*i)->Start();
		++i;
	}
	
	last_sec_frame.Start();
	ms_timer.Start();
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	frames_per_sec++;
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	if (Sec_Counter(last_sec_frame.Read()))
	{
		LOG("FPS: %d", prev_frames_per_sec);
	}

	double ms_time = ms_timer.Read();

	time_last_frame = ms_time;

	Delay(ms_time, time_per_frame);

}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	list<Module*>::iterator i = list_modules.begin();

	while (i != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*i)->PreUpdate(dt);
		++i;
	}

	i = list_modules.begin();

	while(i != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*i)->Update(dt);
		++i;
	}

	i = list_modules.begin();

	while (i != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*i)->PostUpdate(dt);
		i++;
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	list<Module*>::reverse_iterator i = list_modules.rbegin();

	while (i != list_modules.rend() && ret == true)
	{
		ret = (*i)->CleanUp();
		++i;
	}


	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

void Application::RquestBrowser(const char* url)
{
	ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
}

bool Application::Sec_Counter(double time)
{
	if (time > 1000)
	{
		last_sec_frame.Start();
		prev_frames_per_sec = frames_per_sec;
		frames_per_sec = 0;

		return true;
	}

	return false;
}

void Application::Delay(double ms_time, double time_per_frame)
{

	if (time_per_frame > 0 && ms_time < time_per_frame)
	{
		SDL_Delay(time_per_frame - ms_time);
	}
}

int Application::Get_Last_FPS()const
{
	return prev_frames_per_sec;
}

double Application::Get_Last_Frame_Time()const
{
	return time_last_frame;
}

int Application::Get_Limit_Frames()const
{
	return max_frames;
}

void Application::Set_Limit_Frames(int max_fps)
{
	if(max_fps != 0)
	      time_per_frame = 1000 / max_fps;
	else
		  time_per_frame = 1000 / 60;
}