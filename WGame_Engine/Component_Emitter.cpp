#include "Component_Emitter.h"
#include "Application.h"
#include "Component_Transformation.h"
#include "Component_Material.h"
#include "Components.h"
#include "GameObject.h"
#include "Particle.h"
#include "Fireworks_Particle.h"
#include "Smoke_Particle.h"
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
	//Update the number of particules
	if (particles_container.size() > number_particles)
	{
		Resize_Particles_Vector();
	}

	//Update Billboarding
	Update_Particles_Billboarding();

	//Renders
	Render_Emmiter();
	if (fireworks_behavior == false && smoke_behavior == false)
	{
		Render_Particles();
	}
	else
	{
		vector<Particle*>::iterator tmp2 = particles_container.begin();
		while (tmp2 != particles_container.end())
		{
			(*tmp2)->Render_Particles();
			tmp2++;
		}
	}

	//Still creating Particles
	if (particles_container.size() < number_particles)
	{
		Create_Particle();
	}


	//Update info particles
	vector<Particle*>::iterator tmp = particles_container.begin();
	while (tmp != particles_container.end())
	{
		if (fireworks_behavior == false && smoke_behavior == false)
		{
			//Normal behavior particles
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
					float new_size = (((*tmp)->Get_Lifetime() - (*tmp)->Get_Age()) * size_particles / (*tmp)->Get_Lifetime());
					(*tmp)->Set_Scale(float3(new_size, new_size, 0.f));

			}
			else
			{
					//Realocate again the particles from the origin
					if (smoke_behavior == false && fireworks_behavior == false)
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
						(*tmp)->Set_Velocity(float3(random_vector.Normalized().x * speed_x, random_vector.Normalized().y * speed_y, random_vector.Normalized().z * speed_z));
						(*tmp)->Set_Lifetime(new_lifetime);
						(*tmp)->Set_Age(0);
						(*tmp)->Set_Scale(float3(size_particles, size_particles, 0));
					}
				}
		}
		else
		{

			//Update initial_vel,positionemitter,force,size... to Smoke and Fireworks Particles
			if ((*tmp)->position_emitter.Equals(position_emitter) == false)
			{
				(*tmp)->position_emitter = position_emitter;
			}

			if ((*tmp)->force.Equals(force) == false)
			{
				(*tmp)->force = force;
			}

			if ((*tmp)->min_initial_velocity.Equals(min_initial_velocity) == false)
			{
				(*tmp)->min_initial_velocity = min_initial_velocity;
			}

			if((*tmp)->max_initial_velocity.Equals(max_initial_velocity) == false)
			{
				(*tmp)->max_initial_velocity = max_initial_velocity;
			}

			if ((*tmp)->initial_size_particles != size_particles)
			{
				(*tmp)->initial_size_particles = size_particles;
			}

			if ((*tmp)->initial_lifetime != lifetime)
			{
				(*tmp)->initial_lifetime = lifetime;
			}

			if ((*tmp)->Get_Type() == FIREWORKS)
			{
				//Update initial force, initial vel... of the children
				Fireworks_Particle* p = (Fireworks_Particle*)(*tmp);

				if (p->Get_Child_Max_Initial_Velocity().Equals(max_fireowks_children_initial_velocity) == false)
				{
					p->Set_Max_Initial_Child_Velocity(max_fireowks_children_initial_velocity);
				}

				if (p->Get_Child_Min_Initial_Velocity().Equals(min_fireowks_children_initial_velocity) == false)
				{
					p->Set_Min_Initial_Child_Velocity(min_fireowks_children_initial_velocity);
				}

				if (p->Get_Child_Force().Equals(force_firework_children) == false)
				{
					p->Set_Force_Child(force_firework_children);
				}

				if (p->Get_Child_Size() != size_fireowks_children_particles)
				{
					p->Set_Size_Child(size_fireowks_children_particles);
				}

				if (p->Get_Child_Lifetime() != lifetimefireowks_children)
				{
					p->Set_Lifetime_Child(lifetimefireowks_children);
				}
			}

			(*tmp)->Update_Particle();

		}

		tmp++;
	}
		
	//Render panel with information
	Render_Panel();

}

void Component_Emitter::Create_Particle()
{
	if (fireworks_behavior == false && smoke_behavior == false)
	{
		Particle* p = new Particle(NORMAL, position_emitter, float3::one, Quat::identity, float3::zero, min_width, max_width, min_height, max_height, min_depth, max_depth, lifetime, force, size_particles);

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

	if (fireworks_behavior)
	{
		Fireworks_Particle* p = new Fireworks_Particle(FIREWORKS, position_emitter, float3::one, Quat::identity, float3::zero, min_width, max_width, min_height, max_height, min_depth, max_depth, lifetime, force, size_particles);

		p->min_initial_velocity = min_initial_velocity;
		p->max_initial_velocity = max_initial_velocity;

		p->Set_Max_Initial_Child_Velocity(max_fireowks_children_initial_velocity);
		p->Set_Min_Initial_Child_Velocity(min_fireowks_children_initial_velocity);
		p->Set_Size_Child(size_fireowks_children_particles);
		p->Set_Lifetime_Child(lifetimefireowks_children);
		p->Set_Force_Child(force_firework_children);

		p->Creation_Particle_Explosion(position_emitter);
		particles_container.push_back((Particle*)p);
	}


	if (smoke_behavior)
	{
		Smoke_Particle* p = new Smoke_Particle(SMOKE, position_emitter, float3::one, Quat::identity, float3::zero, min_width, max_width, min_height, max_height, min_depth, max_depth, lifetime, force, size_particles);
		p->min_initial_velocity = min_initial_velocity;
		p->max_initial_velocity = max_initial_velocity;
		p->Create_Initial_Movement(position_emitter);
		particles_container.push_back((Particle*)p);
	}

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
		number_particles = 0;
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
				if (max_initial_velocity.x < min_initial_velocity.x)
				{
					max_initial_velocity.x = min_initial_velocity.x;
				}

				ImGui::Text("Max Initial Velocity Y : ");
				ImGui::SameLine();
				ImGui::DragFloat("##Drag_max2", &max_initial_velocity.y, 1.0f, min_initial_velocity.y, 200.f);
				if (max_initial_velocity.y < min_initial_velocity.y)
				{
					max_initial_velocity.y = min_initial_velocity.y;
				}

				ImGui::Text("Max Initial Velocity Z : ");
				ImGui::SameLine();
				ImGui::DragFloat("##Drag_max3", &max_initial_velocity.z, 1.0f, min_initial_velocity.z, 200.f);
				if (max_initial_velocity.z < min_initial_velocity.z)
				{
					max_initial_velocity.z = min_initial_velocity.z;
				}


				ImGui::Text("                             Min Initial Velocity ");
				ImGui::Text("Min Initial Velocity X : ");
				ImGui::SameLine();
				ImGui::DragFloat("##Drag_min1", &min_initial_velocity.x, 1.0f, -200.f, max_initial_velocity.x);
				if (min_initial_velocity.x > max_initial_velocity.x)
				{
					min_initial_velocity.x = max_initial_velocity.x;
				}

				ImGui::Text("Min Initial Velocity Y : ");
				ImGui::SameLine();
				ImGui::DragFloat("##Drag_min2", &min_initial_velocity.y, 1.0f, -200.f, max_initial_velocity.y);
				if (min_initial_velocity.y > max_initial_velocity.y)
				{
					min_initial_velocity.y = max_initial_velocity.y;
				}

				ImGui::Text("Min Initial Velocity Z : ");
				ImGui::SameLine();
				ImGui::DragFloat("##Drag_min3", &min_initial_velocity.z, 1.0f, -200.f, max_initial_velocity.z);
				if (min_initial_velocity.z > max_initial_velocity.z)
				{
					min_initial_velocity.z = max_initial_velocity.z;
				}

				ImGui::Text("Lifetime Particle : ");
				ImGui::DragFloat("##Drag_life", &lifetime, 1.0f, 1.0f, 50.f);

				ImGui::Text("Size Particle : ");
				ImGui::DragFloat("##Drag_size", &size_particles, 1.0f, 1.0f, 10.f);

				ImGui::Text("Force Particle : ");
				ImGui::DragFloat3("##Drag_force", force.ptr());

				ImGui::Text("Number Particle : ");
				ImGui::DragInt("##Drag_num_p", &number_particles, 1.0f, 1.0f, 400.f);

			}
			else
			{
				ImGui::Text("                Initial Particle Velocity                  ");
				ImGui::Text("                             Max Initial Velocity ");
				ImGui::Text("Max Initial Velocity X : ");

				ImGui::Text("Max Initial Velocity Y : ");
			

				ImGui::Text("Max Initial Velocity Z : ");


				ImGui::Text("                             Min Initial Velocity ");
				ImGui::Text("Min Initial Velocity X : ");
	

				ImGui::Text("Min Initial Velocity Y : ");
		

				ImGui::Text("Min Initial Velocity Z : ");


				ImGui::Text("Lifetime Particle : ");

				ImGui::Text("Size Particle : ");

				ImGui::Text("Force Particle : ");

				ImGui::Text("Number Particle : ");				

			}

			if (fireworks_behavior)
			{
				//--------------------------------------------------------------------
				ImGui::Text("                Fireworks Children Particles                  ");
				ImGui::Text("                             Max Initial Velocity ");
				ImGui::Text("Max Initial Velocity X : ");
				ImGui::SameLine();
				ImGui::DragFloat("##Drag_child_max1", &max_fireowks_children_initial_velocity.x, 1.0f, min_fireowks_children_initial_velocity.x, 200.f);
				if (max_fireowks_children_initial_velocity.x < min_fireowks_children_initial_velocity.x)
				{
					max_fireowks_children_initial_velocity.x = min_fireowks_children_initial_velocity.x;
				}

				ImGui::Text("Max Initial Velocity Y : ");
				ImGui::SameLine();
				ImGui::DragFloat("##Drag_child_max2", &max_fireowks_children_initial_velocity.y, 1.0f, min_fireowks_children_initial_velocity.y, 200.f);
				if (max_fireowks_children_initial_velocity.y < min_fireowks_children_initial_velocity.y)
				{
					max_fireowks_children_initial_velocity.y = min_fireowks_children_initial_velocity.y;
				}

				ImGui::Text("Max Initial Velocity Z : ");
				ImGui::SameLine();
				ImGui::DragFloat("##Drag_child_max3", &max_fireowks_children_initial_velocity.z, 1.0f, min_fireowks_children_initial_velocity.z, 200.f);
				if (max_fireowks_children_initial_velocity.z < min_fireowks_children_initial_velocity.z)
				{
					max_fireowks_children_initial_velocity.z = min_fireowks_children_initial_velocity.z;
				}

				ImGui::Text("                             Min Initial Velocity ");
				ImGui::Text("Min Initial Velocity X : ");
				ImGui::SameLine();
				ImGui::DragFloat("##Drag_child_min1", &min_fireowks_children_initial_velocity.x, 1.0f, -200.f, max_fireowks_children_initial_velocity.x);
				if (min_fireowks_children_initial_velocity.x > max_fireowks_children_initial_velocity.x)
				{
					min_fireowks_children_initial_velocity.x = max_fireowks_children_initial_velocity.x;
				}

				ImGui::Text("Min Initial Velocity Y : ");
				ImGui::SameLine();
				ImGui::DragFloat("##Drag_child_min2", &min_fireowks_children_initial_velocity.y, 1.0f, -200.f, max_fireowks_children_initial_velocity.y);
				if (min_fireowks_children_initial_velocity.y > max_fireowks_children_initial_velocity.y)
				{
					min_fireowks_children_initial_velocity.y = max_fireowks_children_initial_velocity.y;
				}

				ImGui::Text("Min Initial Velocity Z : ");
				ImGui::SameLine();
				ImGui::DragFloat("##Drag_child_min3", &min_fireowks_children_initial_velocity.z, 1.0f, -200.f, max_fireowks_children_initial_velocity.z);
				if (min_fireowks_children_initial_velocity.z > max_fireowks_children_initial_velocity.z)
				{
					min_fireowks_children_initial_velocity.z = max_fireowks_children_initial_velocity.z;
				}


				ImGui::Text("Lifetime Particle : ");
				ImGui::DragFloat("##Drag_child_life", &lifetimefireowks_children, 1.0f, 1.0f, 50.f);

				ImGui::Text("Size Particle : ");
				ImGui::DragFloat("##Drag_child_size", &size_fireowks_children_particles, 1.0f, 1.0f, 10.f);

				ImGui::Text("Force Particle : ");
				ImGui::DragFloat3("##Drag_child_force", force_firework_children.ptr());
			}

			ImGui::Checkbox("Deactivate##fccemiter", &active_checkbox);

			//----------------------Emulate smoke--------------------
			if (fireworks_behavior == false)
			{
				if (ImGui::Button("Smoke"))
				{
					smoke_behavior = !smoke_behavior;

					if (smoke_behavior)
					{
						Clean_Up();
						//New values in the initial velocity to emulate smoke
						min_initial_velocity = float3(-1.f, 0.f, -1.f);
						max_initial_velocity = float3(1.f, 20.f, 1.f);
						force = float3(0.f, 20.f, 0.f);
						Create_Particle();
						number_particles = 80;
						
					}
					else
					{
						//Random Values
						Random rand;
						min_initial_velocity = rand.Random_Float_Vector(-20.f, 0.f);
						max_initial_velocity = rand.Random_Float_Vector(1.f, 20.f);
						force = rand.Random_Float_Vector(-20.f, 20.f);
					}
				}
			}
			
			

			//----------------------Emulate Fireworks--------------------
			ImGui::SameLine();
			if (smoke_behavior == false)
			{
				if (ImGui::Button("Fireworks"))
				{
					fireworks_behavior = !fireworks_behavior;

					if (fireworks_behavior)
					{	
						Clean_Up();

						min_initial_velocity = float3(-2.f, 30.f, -2.f);
						max_initial_velocity = float3(2.f, 50.f, 2.f);
						force = float3(0.f, 120.f, 0.f);
						lifetime = 1.f;
						size_particles = 2.f;

						//Children Firework particles
						max_fireowks_children_initial_velocity = float3(10.f, 15.f, 10.f);
						min_fireowks_children_initial_velocity = float3(-10.f, 10.f, -10.f);
						size_fireowks_children_particles = 1.5f;
						lifetimefireowks_children = 3.f;
						force_firework_children = float3(0.f, -15.f, 0.f);

						Create_Particle();
						number_particles++;
					}
					else
					{
						//Random Values
						Random rand;
						min_initial_velocity = rand.Random_Float_Vector(-20.f, 0.f);
						max_initial_velocity = rand.Random_Float_Vector(1.f, 20.f);
						force = rand.Random_Float_Vector(-20.f, 20.f);
						lifetime = 15.f;
						number_particles = rand.Random_int(1, 150);
					}
				}
			}


			if (fireworks_behavior)
			{
				ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), "Fireworks Activated");
			}
			
			if (smoke_behavior)
			{
				ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), "Smoke Activated");
			}			
		}
	}
}

void Component_Emitter::Particle_Smoke_Behavior( Particle &p)
{
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
	p.Set_Position(pos);

	// If we want that the smoke goes to the top the velocity in the axis "y" have to be alguais positive
	p.Set_Velocity(float3(random_vector.Normalized().x * speed_x, random_vector.Normalized().Abs().y * speed_y, random_vector.Normalized().z * speed_z));

	p.Set_Lifetime(new_lifetime);
	p.Set_Age(0);
	p.Set_Scale(float3(size_particles, size_particles, 0));

}


void Component_Emitter::Resize_Particles_Vector()
{
	while (particles_container.size() > number_particles)
	{
		particles_container.pop_back();
	}
}

void Component_Emitter::Update_Particles_Billboarding()
{
	if (particles_container.size() > 0)
	{
		vector<Particle*>::iterator tmp3 = particles_container.begin();
		while (tmp3 != particles_container.end())
		{
			//Update Billboarding Particles
			(*tmp3)->Update_Billboarding();

			tmp3++;
		}
	}
}

void  Component_Emitter::Render_Emmiter()
{
	if (Get_Game_Object()->Exist_Component(TRANSFORMATION))
	{
		//Render Emitter
		Component_Transformation* transformation = (Component_Transformation*)Get_Game_Object()->Get_Component(TRANSFORMATION);
		position_emitter = transformation->Get_Position();
		App->renderer3D->Debug_Emitter(transformation->Get_Tranformation_Matrix().Transposed(), max_width);
	}
}

void  Component_Emitter::Render_Particles()
{
	if (particles_container.size() > 0)
	{
		//Render Particles
		vector<Particle*>::iterator tmp2 = particles_container.begin();
		while (tmp2 != particles_container.end())
		{

			App->renderer3D->Render_Particles((*tmp2)->Get_Tranformation_Matrix().Transposed(), float3((*tmp2)->Get_Scale().x, (*tmp2)->Get_Scale().y, 0.f), 0);

			tmp2++;

		}
	}
}


//Checkbox bool
bool Component_Emitter::Is_Checkbox_Active()const
{
	return active_checkbox;
}

void Component_Emitter::Set_Checkbox(bool on)
{
	active_checkbox = on;
}





