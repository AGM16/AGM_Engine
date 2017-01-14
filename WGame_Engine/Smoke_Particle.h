#ifndef _SMOKE_PARTICLE_H_
#define _SMOKE_PARTICLE_H_

#include "Particle.h"

class Smoke_Particle : public Particle
{
public:

	Smoke_Particle(Particles_Type type_p, float3 pos, float3 scale_, Quat rot_quat, float3 angles_, float min_width_p, float max_width_p, float min_height_p, float max_height_p, float min_depth_p, float max_depth_p, float initial_lifetime_p, float3 force_p, float initial_size_p);
	~Smoke_Particle();

	void Update_Particle();

	void Create_Initial_Movement(float3 &position);
	void Smoke_Particle::Render_Particles();

private:

};

#endif
