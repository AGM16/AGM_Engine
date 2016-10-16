#ifndef _MOODULESCENEINTRO_H_
#define _MOODULESCENEINTRO_H_


#include "Module.h"
#include "Globals.h"
#include "Primitive.h"
#include "ModuleGeometry.h"
#include <list>
#include <vector>

class GameObject;


class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Init();
	bool Start();
	update_status Update(float dt);
	bool CleanUp();


public:
	

};

#endif
