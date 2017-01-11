#include "Random.h"
#include "MathGeoLib\include\Algorithm\Random\LCG.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include <random>
#include <time.h>

using namespace std;


float Random::F_Random()
{
	//Use time(NULL) to generate a diferent random number every time that we call the function
	mt19937 generator(time(0));
	uniform_real_distribution<float> distributor(0.0f, 1.0f);

	//Generate Float Random Number
	return distributor(generator);

}


float Random::Min_Max_Random(float min_a, float max_b)
{
	//Generator
	mt19937 generator(time(0));
	uniform_real_distribution<float> distributor(min_a, max_b);
	
	//Generate Int Random Number
	return distributor(generator);
}

int Random::Random_int(int min_a, int max_b)
{
	int rand = 0;
	LCG random;

	rand = random.Float(min_a, max_b);
	return rand;
}

float3 Random::Random_Float_Vector(float min_a, float max_b)
{
	//Generate a random with opengl LCG
	float3 vector;
	LCG random;

	vector.x = random.Float(min_a, max_b);
	vector.y = random.Float(min_a, max_b);
	vector.z = random.Float(min_a, max_b);

	return vector;

}

 float Random::RandRange(float fMin, float fMax)
{
	if (fMin > fMax) std::swap(fMin, fMax);
	return ((rand() / (float)RAND_MAX) * (fMax - fMin)) + fMin;
}

 math::float3 Random::RandUnitVec()
{
	float x = ((rand() / (float)RAND_MAX) * 2.0f) - 1.0f;
	float y = ((rand() / (float)RAND_MAX) * 2.0f) - 1.0f;
	float z = ((rand() / (float)RAND_MAX) * 2.0f) - 1.0f;

	math::float3 vec = math::float3(x, y, z);
	return vec.Normalized();
}