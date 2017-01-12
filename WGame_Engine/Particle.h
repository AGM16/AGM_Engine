#ifndef _PARTICLE_
#define _PARTICLE_

#include "MathGeoLib\include\MathGeoLib.h"
#include "Component_Transformation.h"

class Particle
{

public:

	Particle(float3 pos, float3 scale_, Quat rot_quat, float3 angles_);
	~Particle();
	void Update_Particle(math::float3 force);
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
	void Set_Age(const float &new_age);
	void Set_Lifetime(const float &life);

	bool Is_Alive()const;
	math::float3 Get_Velocity()const;
	float Get_Distance()const;
	float Get_Age()const;
	float Get_Lifetime()const;


private:

	math::float3 velocity;
	float camera_distance;
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

};


#endif // !_PARTICLE_
