#include "Particle.h"
#include "Application.h"
#include "Component_Transformation.h"
#include "MathGeoLib\include\MathGeoLib.h"

using namespace std;

Particle::Particle(Particles_Type type_p, float3 pos, float3 scale_, Quat rot_quat, float3 angles_, float min_width_p, float max_width_p, float min_height_p, float max_height_p, float min_depth_p, float max_depth_p, float initial_lifetime_p, float3 force_p, float initial_size_p) : position(pos), scale(scale_), quat_rotation(rot_quat), rotation_degrees(angles_)
{
	max_initial_velocity = float3::zero;
	min_initial_velocity = float3::zero;

	//Frustum Box
	min_width = min_width_p;
	max_width = max_width_p;
	min_height = min_height_p;
	max_height = max_height_p;
    min_depth = min_depth_p;
	max_depth = max_depth_p;
	initial_lifetime = initial_lifetime_p;
	force = force_p;

	transformation_matrix = transformation_matrix.FromTRS(position, quat_rotation, scale);
	type = type_p;
	initial_size_particles = initial_size_p;
}

Particle::~Particle()
{

}

void Particle::Update_Particle()
{

}

void Particle::Render_Particles()
{

}


void Particle::Update_Billboarding()
{
	if (App->camera->Get_Camera_Component()->Get_Up().IsZero() == false)
	{

		float3 forward = App->camera->Get_Camera_Component()->Get_Position() - transformation_matrix.TranslatePart().Normalized();

		float4x4 tmp = float4x4::LookAt(localForward, forward.Normalized(), localUp, App->camera->Get_Camera_Component()->Get_Up().Normalized());

		Set_Rotation(RadToDeg(tmp.ToEulerXYZ()));
	}
}

void Particle::Destroy_Particle()
{
	alive = false;
	velocity = float3::zero;
	texture_id = 0;
	age = 0.f;
	lifetime = 0.f;


}

math::float4x4 Particle::Get_Tranformation_Matrix()const
{
	return transformation_matrix;
}

math::float3 Particle::Get_Position()const
{
	return position;
}

math::float3 Particle::Get_Rotation()const
{
	return rotation_degrees;
}

math::float3 Particle::Get_Scale()const
{
	return scale;
}


void Particle::Set_Position(const math::float3 &pos)
{
	position = pos;
	transformation_matrix = transformation_matrix.FromTRS(position, quat_rotation, scale);
}

void Particle::Set_Rotation(const math::float3 &rot_degrees)
{
	rotation_degrees = rot_degrees;
	float3 rotation_radians = DegToRad(rotation_degrees);

	quat_rotation = quat_rotation.FromEulerXYZ(rotation_radians.x, rotation_radians.y, rotation_radians.z);

	transformation_matrix = transformation_matrix.FromTRS(position, quat_rotation, scale);

}

void Particle::Set_Scale(const math::float3 &scal)
{
	scale = scal;
	transformation_matrix = transformation_matrix.FromTRS(position, quat_rotation, scale);
}

void Particle::Set_Velocity(const math::float3 &vel)
{
	velocity = vel;
}

void Particle::Set_Camera_Distance(const math::float3 &camera_pos)
{
	float3 distance = camera_pos - position;
	camera_distance = distance.Length();
}

void Particle::Set_Age(const float &new_age)
{
	age = new_age;
}

void Particle::Set_Lifetime(const float &life)
{
	lifetime = life;
}

bool Particle::Is_Alive()const
{
	return alive;
}


math::float3 Particle::Get_Velocity()const
{
	return velocity;
}

float Particle::Get_Distance()const
{
	return camera_distance;
}

float Particle::Get_Age()const
{
	return age;
}

float Particle::Get_Lifetime()const
{
	return lifetime;
}

Particles_Type Particle::Get_Type()const
{
	return type;
}
