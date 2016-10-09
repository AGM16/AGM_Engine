#ifndef _HARDWARE_INFO_H_
#define _HARDWARE_INFO_H_


#include "Module.h"


class Hardware_Info : public Module
{
public:

	Hardware_Info(Application* app, bool start_enabled = true);
	~Hardware_Info();

	bool Init();
	bool CleanUp();

	void Render_Hardware_Info();
	void Open_Window(bool active);

private:

	bool window_active;
	ImVec4 color;
	int cpu;
	int cpu_cache;
	int ram;
	string caps;
	

};



#endif