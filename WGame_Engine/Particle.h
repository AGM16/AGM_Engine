#ifndef _PARTICLE_
#define _PARTICLE_

#include "MathGeoLib\include\MathGeoLib.h"
#include "Component_Transformation.h"

enum Particles_Type
{
	FIREWORKS,
	SMOKE,
	NORMAL
};

class Particle
{

public:

	Particle(Particles_Type type_p, float3 pos, float3 scale_, Quat rot_quat, float3 angles_, float min_width_p, float max_width_p, float min_height_p, float max_height_p, float min_depth_p, float max_depth_p, float initial_lifetime_p, float3 force_p, float initial_size_p);
	~Particle();

	virtual void Update_Particle();
	virtual void Render_Particles();
	void Destroy_Particle();

	//Matrix transformation
	void Set_Position(const math::float3 &pos);
	void Set_Rotation(const math::float3 &rot_degrees);
	void Set_Scale(const math::float3 &scal);

	math::float3 Get_Position()const;
	math::float3 Get_Rotation()const;
	math::float3 Get_Scale()const;
	math::float4x4 Get_Tranformation_Matrix()const;

	void Update_Billboarding();

	//Particle propierties
	void Set_Velocity(const math::float3 &vel);
	void Set_Camera_Distance(const math::float3 &camera_pos);
	void Reset_Camera_Distance();
	void Set_Age(const float &new_age);
	void Set_Lifetime(const float &life);

	bool Is_Alive()const;
	math::float3 Get_Velocity()const;
	float Get_Distance()const;
	float Get_Age()const;
	float Get_Lifetime()const;
	Particles_Type Get_Type()const;

	
	//Randomize variables
	float3 max_initial_velocity;
	float3 min_initial_velocity;

	//Frustum Box
	float min_width;
	float max_width;
	float min_height;
	float max_height;
	float min_depth;
	float max_depth;
	float initial_lifetime;
	float3 force;
	float3 position_emitter = float3::zero;
	float initial_size_particles = 0;

private:

	math::float3 velocity;
	float camera_distance = 0;
	unsigned int texture_id;
	float age;
	float lifetime;
	bool alive;


	//Coordenates of the particle
	math::float3 position = math::float3::zero;
	math::float3 rotation_degrees = math::float3::zero;
	math::float3 scale = math::float3::one;
	math::Quat quat_rotation = math::Quat::identity;

	//Last Information of matrix
	math::float4x4 transformation_matrix = math::float4x4::identity;

	//Billboarding varibales
	float3 localForward = float3(0, 0, 1);
	float3 localUp = float3(0, 1, 0);

	Particles_Type type;

};


#endif // !_PARTICLE_
