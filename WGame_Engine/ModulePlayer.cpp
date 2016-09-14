#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	



	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	

	return UPDATE_CONTINUE;
}

void ModulePlayer::Set_pos(float x, float y, float z)
{
	//vehicle->SetPos(x, y, z);
}

void ModulePlayer::Reset_Vehicle(float x, float y, float z, float orient)
{
	//vehicle->SetPos(x, y, z);
	//vehicle->Stop_Motion();
	//vehicle->Orient(orient);

}