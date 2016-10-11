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
	console = new Console(this);
	geometry = new ModuleGeometry(this);
	filesystem = new ModuleFileSystem(this);
	hardware = new Hardware_Info(this);
	go_manager = new Module_Go_Manager(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);

	//FPS
	AddModule(filesystem);
	AddModule(console);
	AddModule(hardware);
	AddModule(fps_info);
	AddModule(go_manager);
	AddModule(geometry);
	AddModule(camera);
	AddModule(input);
	AddModule(audio);
	AddModule(physics);	


	// Scenes
	
	AddModule(editor);
	AddModule(scene_intro);
	


	// Renderer last!
	AddModule(renderer3D);


	//FPS Info
	prev_frames_per_sec = 0; 
	frames_per_sec = 0;
	max_frames = 60; 
	

	//Console
	console_exists = false;
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

    console_exists = true;

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

	const char* dir = filesystem->Get_Base_Dir();
	
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
	time_per_frame = 1000 / max_frames;
}

// ---------------------------------------------
void Application::FinishUpdate()
{	

	Delay(ms_timer.Read(), time_per_frame);
	
	time_last_frame = ms_timer.Read();
	
	//Check if have passed 1sec
	if (Sec_Counter(last_sec_frame.Read()))
	{
		LOG("FPS: %d", prev_frames_per_sec);
	}

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
	//Console
	console_exists = false;

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
	      max_frames = max_fps;
	else
		  max_frames = 60;
}

bool  Application::Consoler_Exist()
{
	return console_exists;
}

bool Application::Set_Console(bool exist)
{
	return (console_exists = exist);
}

void Application::Log_Console(const char* text)
{
	if (console_exists == true)
	{
		if (console != NULL)
			console->Save_Log(text);
	}

}