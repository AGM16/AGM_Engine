#include "Smoke_Particle.h"
#include "Application.h"
#include "Random.h"

using namespace std;

Smoke_Particle::Smoke_Particle(Particles_Type type_p, float3 pos, float3 scale_, Quat rot_quat, float3 angles_, float min_width_p, float max_width_p, float min_height_p, float max_height_p, float min_depth_p, float max_depth_p, float initial_lifetime_p, float3 force_p, float initial_size_p)
: Particle(type_p, pos, scale_, rot_quat, angles_, min_width_p, max_width_p, min_height_p, max_height_p, min_depth_p, max_depth_p, initial_lifetime_p, force_p, initial_size_p)
{

}

Smoke_Particle::~Smoke_Particle()
{

}

void Smoke_Particle::Update_Particle()
{
	//Update info particles
	//Check tthe life
	if (Get_Lifetime() > Get_Age())
	{
		//Update variables
		float new_age = Get_Age() + App->Get_Delta_Time();
		Set_Age(new_age);

		float3 new_vel = Get_Velocity() + (force * App->Get_Delta_Time() * 0.5f);
		Set_Velocity(new_vel);
		float3 p_speed = Get_Velocity();
		float3 position = p_speed * App->Get_Delta_Time();
		Set_Position(Get_Position() + position);
		Set_Camera_Distance(App->camera->Get_Camera_Position());

		//Modify Size
		float new_size = ((Get_Lifetime() - Get_Age()) * initial_size_particles / Get_Lifetime());
		Set_Scale(float3(new_size, new_size, 0.f));

	}
	else
	{			
		//Creation intial movement
		Create_Initial_Movement(position_emitter);
	}	
	
}

void Smoke_Particle::Create_Initial_Movement(float3 &position)
{
	position_emitter = position;

	Random rand;
	float X = rand.RandRange(min_width, max_width);
	float Y = rand.RandRange(min_height, max_height);
	float Z = rand.RandRange(min_depth, max_depth);

	float new_lifetime = rand.RandRange(1.f, initial_lifetime);
	float speed_y = rand.Min_Max_Random(min_initial_velocity.y, max_initial_velocity.y);
	float speed_x = rand.Min_Max_Random(min_initial_velocity.x, max_initial_velocity.x);
	float speed_z = rand.Min_Max_Random(min_initial_velocity.z, max_initial_velocity.z);

	math::float3 random_vector(X, Y, Z);

	//Add values
	float3 pos = position_emitter + random_vector;
	Set_Position(pos);

	// If we want that the smoke goes to the top the velocity in the axis "y" have to be alguais positive
	Set_Velocity(float3(random_vector.Normalized().x * speed_x, random_vector.Normalized().Abs().y * speed_y, random_vector.Normalized().z * speed_z));

	Set_Lifetime(new_lifetime);
	Set_Age(0);
	Set_Scale(float3( initial_size_particles, initial_size_particles, 0));
	Reset_Camera_Distance();
}

void Smoke_Particle::Render_Particles(unsigned int &id_texture)
{
	//Render the particle
	if (Get_Lifetime() > Get_Age())
		App->renderer3D->Render_Particles(Get_Tranformation_Matrix().Transposed(), float3(Get_Scale().x, Get_Scale().y, 0.f), id_texture);
}