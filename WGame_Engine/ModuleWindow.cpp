#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

#pragma comment (lib, "Devil/libx86/DevIL.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "Devil/libx86/ILU.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Devil/libx86/ILUT.lib") 

#include "Devil\include\il.h"
#include "Devil\include\ilu.h"
#include "Devil\include\ilut.h"

ModuleWindow::ModuleWindow(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	window = nullptr;
	screen_surface = nullptr;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		int width = SCREEN_WIDTH * SCREEN_SIZE;
		int height = SCREEN_HEIGHT * SCREEN_SIZE;
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//OpenGL Initialization
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

		//Use OpenGL 3.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(WIN_FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(WIN_RESIZABLE == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(WIN_BORDERLESS == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(WIN_FULLSCREEN_DESKTOP == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == nullptr)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	//Init IL,ILU and ULUT
	ilInit();
	iluInit();
	ilutInit();


	ilutRenderer(ILUT_OPENGL);

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != nullptr)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

void ModuleWindow::Set_Fullscreen_Window(bool active)
{
	uint flags;
	if (active)
		flags |= SDL_WINDOW_FULLSCREEN;
	else
		flags = 0;

	SDL_SetWindowFullscreen(window , flags);
}

void ModuleWindow::Set_Fullscreen_Desktop_Window(bool active)
{
	uint flags;
	if (active)
		flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
	else
		flags = 0;

	SDL_SetWindowFullscreen(window, flags);

}

void ModuleWindow::Set_Borderless_Window(bool active)
{
	SDL_bool flags;
	if (active)
		flags = SDL_TRUE;
	else
		flags = SDL_FALSE;

	SDL_SetWindowBordered(window, flags);
}


ImVec2 ModuleWindow::Get_Screen_size()
{
	int width, height;
	SDL_GetWindowSize(window, &width, &height);

	return ImVec2(width, height);
}