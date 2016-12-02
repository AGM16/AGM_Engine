#ifndef _MOODULESCENEINTRO_H_
#define _MOODULESCENEINTRO_H_


#include "Module.h"
#include "Globals.h"
#include "Primitive.h"
#include "ModuleGeometry.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include "p2QuadTree.h"
#include <list>
#include <vector>

class GameObject;


class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(bool start_enabled = true);
	~ModuleSceneIntro();

	bool Init();
	bool Start();
	update_status Update(float dt);
	bool CleanUp();


public:
	
	p2QuadTree scene;

};

#endif
