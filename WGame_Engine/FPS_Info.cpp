#include "Globals.h"
#include "Application.h"
#include "FPS_Info.h"
#include "Imgui\imgui.h"


FPS_Info::FPS_Info(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}



FPS_Info::~FPS_Info()
{}

// Load assets
bool FPS_Info::Start()
{
	LOG("Loading FPS assets");
	bool ret = true;
	max_frames = App->Get_Limit_Frames();
	open_window = false;

	return ret;
}

// Load assets
bool FPS_Info::CleanUp()
{
	LOG("Unloading FPS scene");

	return true;
}

// Update
update_status FPS_Info::Update(float dt)
{
	
	Render_FPS(App->Get_Last_FPS(), frames, true);
	Render_FPS(App->Get_Last_Frame_Time(), ms, false);

	return UPDATE_CONTINUE;
}


void FPS_Info::Render_FPS(double frames, vector<float> &numb_frames, bool PlotHistogram)
{
	
	if (numb_frames.size() > 60)
	{

	       for (int i = 1; i < numb_frames.size(); i++)
	       {
	           numb_frames[i - 1] = numb_frames[i];
	       }

		   numb_frames[numb_frames.size() - 1] = frames;

	}
	else
	{
	  numb_frames.push_back(frames);
	}

	

}








