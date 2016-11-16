#include "Application.h"
#include "PugiXml\src\pugixml.hpp"


Application::Application()
{
	window = new ModuleWindow();
	input = new ModuleInput();
	audio = new ModuleAudio();
	scene_intro = new ModuleSceneIntro();
	renderer3D = new ModuleRenderer3D();
	camera = new ModuleCamera3D();
	physics = new ModulePhysics3D();
	editor = new Editor();
	fps_info = new FPS_Info();
	console = new Console();
	geometry = new ModuleGeometry();
	filesystem = new ModuleFileSystem();
	hardware = new Hardware_Info();
	go_manager = new Module_Go_Manager();

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
	AddModule(audio);
	AddModule(physics);
	AddModule(input);


	// Scenes
	AddModule(editor);
	AddModule(scene_intro);
	


	// Renderer last!
	AddModule(renderer3D);

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

	window_resized = false;


	pugi::xml_document config_file;
	pugi::xml_node root_node_config;

	//Load the config document
	root_node_config = LoadConfig(config_file);

	//Check if there is a root node
	if (root_node_config.empty() == true)
	{
		LOG("ERROR: There is no rootnode in the document Config_File.xml");
		ret = false;
	}
	else
	{
		//We enter into the node App to load the values
		pugi::xml_node app_node = root_node_config.child("Application");

		//Title and organization
		title = app_node.attribute("title").as_string();
		organization = app_node.attribute("organization").as_string();


		//FPS Variables
		prev_frames_per_sec = app_node.attribute("prev_frames_per_sec").as_int();
		frames_per_sec = app_node.attribute("frames_per_sec").as_int();
		max_frames = app_node.attribute("max_frames").as_int();

		//Console variable
		console_exists = app_node.attribute("console_exists ").as_bool();
	}



	// Call Init() in all modules
	list<Module*>::iterator i = list_modules.begin();

	//Activate the console
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
	
	last_sec_frame.Start();
	ms_timer.Start();
	return ret;
}

pugi::xml_node Application::LoadConfig(pugi::xml_document& config_file) const
{
	pugi::xml_node ret;

	char* buffer = nullptr;
	//Load the file xml
	int size = App->filesystem->Load("Config_File.xml", &buffer);
	pugi::xml_parse_result result = config_file.load_buffer(buffer, size);
	

	//Check if the document have been loaded correctly
	
		//Now we enter into the rootnode
		ret = config_file.child("config");
	

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
	if (want_to_save == true)
		SavegameNow();

	if (want_to_load == true)
		LoadGameNow();

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
	ShellExecuteA(nullptr, "open", url, nullptr, nullptr, SW_SHOWNORMAL);
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
		if (console != nullptr)
			console->Save_Log(text);
	}

}

void Application::Windows_Resized()
{
	window_resized = !window_resized;
}

bool Application::Get_Windows_Resized()
{
	return window_resized;
}

const char* Application::Get_Title()const
{
	return title.c_str();
}

const char* Application::Get_Organization()const
{
	return organization.c_str();
}


// Load / Save XML
void Application::LoadGame(const char* file)
{
	// we should be checking if that file actually exist
	want_to_load = true;
	load_game.assign(file);
}

// ---------------------------------------
void Application::SaveGame(const char* file) const
{
	// we should be checking if that file actually exist
	want_to_save = true;
	save_game.assign(file);
}

bool Application::LoadGameNow()
{
	bool ret = false;

	char* buffer;
	uint size = filesystem->Load(load_game.c_str(), &buffer);

	if (size > 0)
	{
		pugi::xml_document data;
		pugi::xml_node root;

		pugi::xml_parse_result result = data.load_buffer(buffer, size);
		RELEASE(buffer);

		if (result != NULL)
		{
			LOG("Loading new Game State from %s...", load_game.c_str());

			root = data.child("game_state");

			list<Module*>::iterator item = list_modules.begin();
			ret = true;

			while (item != list_modules.end() && ret == true)
			{
				ret = (*item)->Load(root.child((*item)->Get_Name_Module()));
				++item;
			}

			data.reset();
			if (ret == true)
			{
				LOG("...finished loading");
			}
			else
			{
				LOG("...loading process interrupted with error on module %s", ((*item) != NULL) ? (*item)->Get_Name_Module() : "unknown");
			}
		}
		else
		{
			LOG("Could not parse game state xml file %s. pugi error: %s", load_game.c_str(), result.description());
		}
	}
	else
		LOG("Could not load game state xml file %s", load_game.c_str());

	want_to_load = false;
	return ret;
}

bool Application::SavegameNow() const
{
	bool ret = true;

	LOG("Saving Game State to %s...", save_game.c_str());

	// xml object were we will store all data
	pugi::xml_document data;
	pugi::xml_node root;

	root = data.append_child("game_state");

	list<Module*>::const_iterator item = list_modules.begin();

	while (item != list_modules.end() && ret == true)
	{
		ret = (*item)->Save(root.append_child((*item)->Get_Name_Module()));
		++item;
	}

	if (ret == true)
	{
		std::stringstream stream;
		data.save(stream);

		// we are done, so write data to disk
		filesystem->Save(save_game.c_str(), stream.str().c_str(), stream.str().length());
		LOG("... finished saving", save_game.c_str());
	}
	else
		LOG("Save process halted from an error in module %s", ((*item) != NULL) ? (*item)->Get_Name_Module() : "unknown");

	data.reset();
	want_to_save = false;
	return ret;
}