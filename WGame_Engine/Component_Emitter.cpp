#include "Component_Emitter.h"
#include "Application.h"
#include "Component_Transformation.h"
#include "Component_Material.h"
#include "Components.h"
#include "GameObject.h"
#include "Particle.h"
#include "Random.h"

using namespace std;

Component_Emitter::Component_Emitter(Components_Type type, GameObject* game_object, unsigned int amount_particles) : Components(type, game_object)
{
	//Initialize emitter position
	Component_Transformation* transformation = (Component_Transformation*)Get_Game_Object()->Get_Component(TRANSFORMATION);
	position_emitter = transformation->Get_Position();
	number_particles = amount_particles;

	//Establish emitter dimensions
	min_width = -2;
	max_width = 2;
	min_height = -2;
	max_height = 2;
	min_depth = -2;
	max_depth = 2;

	//Particle Variables
	max_initial_velocity.y = 20.f;
	max_initial_velocity.x = 2.f;
	max_initial_velocity.z = 2.f;

	min_initial_velocity.x = -2.f;
	min_initial_velocity.y =  0.f;
	min_initial_velocity.z = -2.f;

	lifetime = 15;
	force = float3(0.f, 10.f, 0.f);
	alive = true;
	size_particles = 1.5f;


	//Creation of the first Particle
	Create_Particle();

}

Component_Emitter::~Component_Emitter()
{}

void Component_Emitter::Update()
{
	if (particles_container.size() > number_particles)
	{
		while (particles_container.size() > number_particles)
		{
			particles_container.pop_back();
		}
	}

	vector<Particle*>::iterator tmp3 = particles_container.begin();
	while (tmp3 != particles_container.end())
	{
     	//Update Billboarding Particles
		(*tmp3)->Update_Billboarding();

		tmp3++;
	}

	//Debug Emitter
	Component_Transformation* transformation = (Component_Transformation*)Get_Game_Object()->Get_Component(TRANSFORMATION);
	position_emitter = transformation->Get_Position();
	App->renderer3D->Debug_Emitter(transformation->Get_Tranformation_Matrix().Transposed(), max_width);


	//Render Particles
	vector<Particle*>::iterator tmp2 = particles_container.begin();
	while (tmp2 != particles_container.end())
	{

		App->renderer3D->Render_Particles((*tmp2)->Get_Tranformation_Matrix().Transposed(), float3((*tmp2)->Get_Scale().x, (*tmp2)->Get_Scale().y, 0.f), 0);

		tmp2++;

	}

	//Still creating Particles
	if (particles_container.size() < number_particles)
	{
		Create_Particle();
	}

	vector<Particle*>::iterator tmp = particles_container.begin();
	while (tmp != particles_container.end())
	{
		//Check the life
		if ((*tmp)->Get_Lifetime() > (*tmp)->Get_Age())
		{
			//Update variables
			float new_age = (*tmp)->Get_Age() + App->Get_Delta_Time();
			(*tmp)->Set_Age(new_age);

			float3 new_vel = (*tmp)->Get_Velocity() + (force * App->Get_Delta_Time() * 0.5f);
			(*tmp)->Set_Velocity(new_vel);
			float3 p_speed = (*tmp)->Get_Velocity();
			float3 position = p_speed * App->Get_Delta_Time();
			(*tmp)->Set_Position((*tmp)->Get_Position() + position);

			//Modify Size
			(*tmp)->Set_Age((*tmp)->Get_Age() + App->Get_Delta_Time());
			float new_size = (((*tmp)->Get_Lifetime() - (*tmp)->Get_Age()) * size_particles / (*tmp)->Get_Lifetime());
			(*tmp)->Set_Scale(float3(new_size, new_size, 0.f));
		}
		else
		{
			
			//Generate the particle from the origin of the emitter
			Random rand;
			float X = rand.RandRange(min_width, max_width);
			float Y = rand.RandRange(min_height, max_height);
			float Z = rand.RandRange(min_depth, max_depth);

			float new_lifetime = rand.RandRange(1.f, lifetime);
			float speed_y = rand.Min_Max_Random(min_initial_velocity.y, max_initial_velocity.y);
			float speed_x = rand.Min_Max_Random(min_initial_velocity.x, max_initial_velocity.x);
			float speed_z = rand.Min_Max_Random(min_initial_velocity.z, max_initial_velocity.z);

			math::float3 random_vector(X, Y, Z);

			float3 pos = position_emitter + random_vector;
			(*tmp)->Set_Position(pos);
			(*tmp)->Set_Velocity(float3(random_vector.Normalized().x * speed_x, random_vector.Normalized().Abs().y * speed_y, random_vector.Normalized().z * speed_z));
			(*tmp)->Set_Lifetime(new_lifetime);
			(*tmp)->Set_Age(0);
			(*tmp)->Set_Scale(float3(size_particles, size_particles, 0));

		}

		tmp++;
	}

	//Render panel with information
	Render_Panel();

}

void Component_Emitter::Create_Particle()
{
	Particle* p = new Particle(position_emitter, float3::one, Quat::identity, float3::zero);

	//Random Values
	Random rand;
	float X = rand.RandRange(min_width, max_width);
	float Y = rand.RandRange(min_height, max_height);
	float Z = rand.RandRange(min_depth, max_depth);

	float new_lifetime = rand.RandRange(1.f, lifetime);
	float speed_y = rand.Min_Max_Random(min_initial_velocity.y, max_initial_velocity.y);
	float speed_x = rand.Min_Max_Random(min_initial_velocity.x, max_initial_velocity.x);
	float speed_z = rand.Min_Max_Random(min_initial_velocity.z, max_initial_velocity.z);

	math::float3 random_vector(X, Y, Z);

	//Add values
	float3 pos = position_emitter + random_vector;
	p->Set_Position(pos);
	p->Set_Velocity(float3(random_vector.Normalized().x * speed_x, random_vector.Normalized().y * speed_y, random_vector.Normalized().z * speed_z));
	p->Set_Lifetime(new_lifetime);
	p->Set_Age(0);
	p->Set_Scale(float3(size_particles, size_particles, 0));
	particles_container.push_back(p);

}

void Component_Emitter::Clean_Up()
{
	if (particles_container.size() > 0)
	{
		vector<Particle*>::iterator tmp2 = particles_container.begin();
		while (tmp2 != particles_container.end())
		{
			delete (*tmp2);
			tmp2++;
		}

		particles_container.clear();
	}
}

void Component_Emitter::Render_Panel()
{
	if (Is_Active())
	{
		if (ImGui::CollapsingHeader("Emitter", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (active_checkbox == false)
			{
				//--------------------------------------------------------------------
				ImGui::Text("                Initial Particle Velocity                  ");
				ImGui::Text("                             Max Initial Velocity ");
				ImGui::Text("Max Initial Velocity X : ");
				ImGui::SameLine();
				ImGui::DragFloat("##Drag_max1", &max_initial_velocity.x, 1.0f, min_initial_velocity.x, 200.f);

				ImGui::Text("Max Initial Velocity Y : ");
				ImGui::SameLine();
				ImGui::DragFloat("##Drag_max2", &max_initial_velocity.y, 1.0f, min_initial_velocity.y, 200.f);

				ImGui::Text("Max Initial Velocity Z : ");
				ImGui::SameLine();
				ImGui::DragFloat("##Drag_max3", &max_initial_velocity.z, 1.0f, min_initial_velocity.z, 200.f);


				ImGui::Text("                             Min Initial Velocity ");
				ImGui::Text("Min Initial Velocity X : ");
				ImGui::SameLine();
				ImGui::DragFloat("##Drag_min1", &min_initial_velocity.x, 1.0f, -200.f, max_initial_velocity.x);

				ImGui::Text("Min Initial Velocity Y : ");
				ImGui::SameLine();
				ImGui::DragFloat("##Drag_min2", &min_initial_velocity.y, 1.0f, -200.f, max_initial_velocity.y);

				ImGui::Text("Min Initial Velocity Z : ");
				ImGui::SameLine();
				ImGui::DragFloat("##Drag_min3", &min_initial_velocity.z, 1.0f, -200.f, max_initial_velocity.z);
				

				ImGui::Text("Lifetime Particle : ");
				ImGui::DragFloat("##Drag_life", &lifetime, 1.0f, 00.f, 50.f);

				ImGui::Text("Size Particle : ");
				ImGui::DragFloat("##Drag_size", &size_particles, 1.0f, 1.f, 10.f);

				ImGui::Text("Force Particle : ");
				ImGui::DragFloat3("##Drag_force", force.ptr());

				ImGui::Text("Number Particle : ");
				ImGui::DragInt("##Drag_num_p", &number_particles, 1.0f, 1.0f, 400.f);

			}
			else
			{
				//--------------------------------------------------------------------
				ImGui::Text("Name : ");
				//--------------------------------------------------------------------
				ImGui::Text("NumChildren : ");
				//--------------------------------------------------------------------
				ImGui::Text("Parent : ");
				//--------------------------------------------------------------------
				ImGui::Text("NumIndices : ");
				//--------------------------------------------------------------------
				ImGui::Text("NumVertices : ");
				//--------------------------------------------------------------------
				ImGui::Text("NumNormals : ");
				//--------------------------------------------------------------------
				ImGui::Text("NumUvs : ");

			}
		}
	}
}


//Checkbox bool
bool Component_Emitter::Is_Checkbox_Active()const
{
	return true;
}

bool Component_Emitter::Set_Checkbox(bool on)
{
	return true;
}





