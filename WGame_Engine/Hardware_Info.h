#ifndef _HARDWARE_INFO_
#define _HARDWARE_INFO_


#include "Module.h"
#include "Globals.h"
#include <vector>


class Hardware_Info : public Module
{
public:

	Hardware_Info(Application* app, bool start_enabled = true);
	~Hardware_Info();

	bool Init();
	update_status Update(float dt);
	bool CleanUp();

	void Render_Hardware_Info();

private:

	bool open_window_hardware;
	ImVec4 color;
	int cpu;
	int cpu_cache;
	int ram;

	string caps;
	

};



#endif