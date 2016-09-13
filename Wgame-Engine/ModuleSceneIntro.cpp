#include "Globals.h"
#include "Module.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "ModuleWindow.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "Imgui\imgui.h"



ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	graphics = NULL;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update(float dt)
{
	ImGuiWindowFlags window_flags = 0;
	ImGui::ShowTestWindow();
	if (ImGui::BeginMenu("Menu"))
	{
		ImGui::Text("Menu Clicked!");
		ImGui::EndMenu();
	}
	ImGui::BulletText("This is a bullet text");
	ImGui::Button("Button", ImVec2(50, 30));
	if (ImGui::BeginPopup("popup"))
	{
		ImGui::Text("PopUp clicked!");
		ImGui::EndPopup();
	}
	ImGui::Text("Hello");
	ImGui::LabelText("LABEL", "text");


	Plane(0, 1, 0, 0).Render();
	


	

	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	int a = 0;
}
