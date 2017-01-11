#ifndef __RANDOM_H__
#define __RANDOM_H__

#include <stdint.h>
#include "MathGeoLib\include\MathGeoLib.h"

class Random
{
public:

	float F_Random();
	float Min_Max_Random(float min_a, float max_b);
	float3 Random_Float_Vector(float min_a, float max_b);
	int Random_int(int min_a, int max_b);

	 float RandRange(float fMin, float fMax);
	 math::float3 RandUnitVec();

};
#endif // !__RANDOM_H__
