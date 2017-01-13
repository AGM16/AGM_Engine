#ifndef _FIREWORKS_PARTICLE_H_
#define _FIREWORKS_PARTICLE_H_

#include "Particle.h"
#include <vector>

class Fireworks_Particle : public Particle
{
public:

	Fireworks_Particle(float3 pos, float3 scale_, Quat rot_quat, float3 angles_, float min_width_p, float max_width_p, float min_height_p, float max_height_p, float min_depth_p, float max_depth_p, float initial_lifetime_p, float3 force_p);
	~Fireworks_Particle();

	void Update_Particle();

	//Getters
	bool Has_To_Explode()const;
	unsigned int Get_Number_Child_Particles()const;
	bool Is_Child()const;
	bool Are_Children_Dead();

	//Setters
	void Set_NumParticles_Fireworks(unsigned int &amount);
	void Set_Explode(bool on);
	void Set_Child(bool on);


	bool Delete_children();
	void Creation_Particle_Explosion(float3 position);
	void Create_Children_Explosion();
	void Render_Particles();

private:

	unsigned int number_child_particles;
	bool explode;
	bool is_child;
	std::vector<Particle*> explosion_particles;
	
};
#endif // !_FIREWORKS_PARTICLE_H_
