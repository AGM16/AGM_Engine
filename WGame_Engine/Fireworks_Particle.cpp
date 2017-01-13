#include "Fireworks_Particle.h"
#include "Application.h"
#include "Particle.h"
#include "Random.h"

using namespace std;

Fireworks_Particle::Fireworks_Particle(float3 pos, float3 scale_, Quat rot_quat, float3 angles_, float min_width_p, float max_width_p, float min_height_p, float max_height_p, float min_depth_p, float max_depth_p, float initial_lifetime_p, float3 force_p) 
: Particle(pos, scale_, rot_quat, angles_,  min_width_p,  max_width_p,  min_height_p,  max_height_p,  min_depth_p,  max_depth_p,  initial_lifetime_p,  force_p)
{
	min_initial_velocity = float3(-2.f, 30.f, -2.f);
	max_initial_velocity = float3(2.f, 50.f, 2.f);
}

Fireworks_Particle::~Fireworks_Particle()
{

}

void Fireworks_Particle::Update_Particle()
{
	if (Has_To_Explode() == true)
	{
		//Have not explode yet
		if (Get_Lifetime() > Get_Age())
		{
			//Update variables
			float new_age = Get_Age() + App->Get_Delta_Time();
			Set_Age(new_age);

			float3 new_vel = Get_Velocity() + (force * App->Get_Delta_Time());
			Set_Velocity(new_vel);
			float3 p_speed = Get_Velocity();
			float3 new_position = p_speed * App->Get_Delta_Time();
			Set_Position(Get_Position() + new_position);

			//Modify Size
			float new_size = ((Get_Lifetime() + 5) - Get_Age()) * 2 / (Get_Lifetime() + 5);
			Set_Scale(float3(new_size, new_size, 0.f));
		}
		else
		{
			//Create Children
			Create_Children_Explosion();
			Set_Explode(false);
		}
	}
	
	//Update particles children
	if (explosion_particles.size() > 0)
	{
		//Update Children Billboarding
		vector<Particle*>::iterator tmp2 = explosion_particles.begin();
		while (tmp2 != explosion_particles.end())
		{
			(*tmp2)->Update_Billboarding();
			tmp2++;
		}

		//Update variables
	    tmp2 = explosion_particles.begin();
		while (tmp2 != explosion_particles.end())
		{
			if ((*tmp2)->Get_Lifetime() > (*tmp2)->Get_Age())
			{
				float new_age = (*tmp2)->Get_Age() + App->Get_Delta_Time();
				(*tmp2)->Set_Age(new_age);

				float3 new_vel = (*tmp2)->Get_Velocity() + (float3(1, -15, 1) * App->Get_Delta_Time());
				(*tmp2)->Set_Velocity(new_vel);
				float3 p_speed = (*tmp2)->Get_Velocity();
				float3 position = p_speed * App->Get_Delta_Time();
				(*tmp2)->Set_Position((*tmp2)->Get_Position() + position);

				//Modify Size
				float new_size = (((*tmp2)->Get_Lifetime() - (*tmp2)->Get_Age()) * 1.5f / (*tmp2)->Get_Lifetime());
				(*tmp2)->Set_Scale(float3(new_size, new_size, 0.f));
			}

			tmp2++;
		}
	}

	//Check if the children are Alive
	if (explosion_particles.size() > 0 && Are_Children_Dead() && Has_To_Explode() == false)
	{
		Delete_children();
		Creation_Particle_Explosion(position_emitter);
	}
}


void Fireworks_Particle::Creation_Particle_Explosion(float3 position)
{
	Random rand;
	float X = rand.RandRange(min_width, max_width);
	float Y = rand.RandRange(min_height, max_height);
	float Z = rand.RandRange(min_depth, max_depth);

	float new_lifetime = rand.RandRange(0.1f, initial_lifetime);
	float time_to_explode = rand.RandRange(1.f,initial_lifetime);
	float speed_y = rand.Min_Max_Random(min_initial_velocity.y, max_initial_velocity.y);
	float speed_x = rand.Min_Max_Random(min_initial_velocity.x, max_initial_velocity.x);
	float speed_z = rand.Min_Max_Random(min_initial_velocity.z, max_initial_velocity.z);

	math::float3 random_vector(X, Y, Z);

	//Add values
	position_emitter = position;
	float3 pos = position_emitter + random_vector;
	Set_Position(pos);

	// If we want that the smoke goes to the top the velocity in the axis "y" have to be alguais positive
	Set_Velocity(float3(random_vector.Normalized().x * speed_x, random_vector.Normalized().Abs().y * speed_y, random_vector.Normalized().z * speed_z));

	//Lifetime to explode
	Set_Lifetime(0);
	Set_Lifetime(time_to_explode);
	Set_Age(0);
	Set_Scale(float3(2, 2, 0));

	//Firework variables
	Set_Explode(true);
	unsigned int amount = 30;
    Set_NumParticles_Fireworks(amount);
}


void Fireworks_Particle::Create_Children_Explosion()
{
	for (int i = 0; i < number_child_particles; i++)
	{

		float3 force_children = float3(0.f, -15.f, 0.f);
		

		Particle* p = new Particle(Get_Position(), float3::one, Quat::identity, float3::zero, min_width, max_width, min_height, max_height, min_depth, max_depth, 3.f, force_children);
        p->min_initial_velocity = float3(-10.f, 10.f, -10.f);
		p->max_initial_velocity = float3(10.f, 15.f, 10.f);
		//Generate the particle from the origin of the emitter
		Random rand;
		float X = rand.RandRange(min_width, max_width);
		float Y = rand.RandRange(min_height, max_height);
		float Z = rand.RandRange(min_depth, max_depth);

		float new_lifetime = rand.RandRange(1.f, p->initial_lifetime);
		float speed_y = rand.Min_Max_Random(p->min_initial_velocity.y, p->max_initial_velocity.y);
		float speed_x = rand.Min_Max_Random(p->min_initial_velocity.x, p->max_initial_velocity.x);
		float speed_z = rand.Min_Max_Random(p->min_initial_velocity.z, p->max_initial_velocity.z);

		math::float3 random_vector(X, Y, Z);

		float3 pos = Get_Position() + random_vector;
		p->Set_Position(pos);
		p->Set_Velocity(float3(random_vector.Normalized().x * speed_x, random_vector.Normalized().y * speed_y, random_vector.Normalized().z * speed_z));
		p->Set_Lifetime(new_lifetime);
		p->Set_Age(0);
		p->Set_Scale(float3(1.5f, 1.5f, 0));

		explosion_particles.push_back(p);
	}
}


bool Fireworks_Particle::Are_Children_Dead()
{
	if (explosion_particles.size() > 0)
	{
		int dead = 0;
		vector<Particle*>::iterator tmp = explosion_particles.begin();
		while (tmp != explosion_particles.end())
		{
			if ((*tmp)->Get_Lifetime() < (*tmp)->Get_Age())
			{
				dead++;
			}

			tmp++;
		}

		if (dead >= number_child_particles)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	return true;
}

void Fireworks_Particle::Render_Particles()
{
	//Render the particle
	if(Has_To_Explode())
		App->renderer3D->Render_Particles(Get_Tranformation_Matrix().Transposed(), float3(Get_Scale().x, Get_Scale().y, 0.f), 0);

	//Render the children
	if (explosion_particles.size() > 0)
	{
		vector<Particle*>::iterator tmp = explosion_particles.begin();
		while (tmp != explosion_particles.end())
		{
			App->renderer3D->Render_Particles((*tmp)->Get_Tranformation_Matrix().Transposed(), float3((*tmp)->Get_Scale().x, (*tmp)->Get_Scale().y, 0.f), 0);
			tmp++;
		}
	}

}

bool Fireworks_Particle::Has_To_Explode()const
{
	return explode;
}

unsigned int  Fireworks_Particle::Get_Number_Child_Particles()const
{
	return number_child_particles;
}

bool  Fireworks_Particle::Is_Child()const
{
	return is_child;
}

void  Fireworks_Particle::Set_NumParticles_Fireworks(unsigned int &amount)
{
	number_child_particles = amount;
}

void  Fireworks_Particle::Set_Explode(bool on)
{
	explode = on;
}

void  Fireworks_Particle::Set_Child(bool on)
{
	is_child = on;
}

bool  Fireworks_Particle::Delete_children()
{
	if (explosion_particles.size() > 0)
	{
		vector<Particle*>::iterator tmp2 = explosion_particles.begin();
		while (tmp2 != explosion_particles.end())
		{
			delete (*tmp2);
			tmp2++;
		}

		explosion_particles.clear();

		return true;
	}

	return false;
}