#ifndef _FPS_INFO_
#define _FPS_INFO_

#include "Module.h"
#include "Globals.h"
#include <list>


class FPS_Info : public Module
{
public:
	FPS_Info(Application* app, bool start_enabled = true);
	~FPS_Info();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();


	void Render_FPS(double frames, vector<float> &numb_frames, bool PlotHistogram);

public:
	
	vector<float> frames;
	vector<float> ms;
	int max_frames;
	int frames_per_second;
	bool open_window;

};

#endif


