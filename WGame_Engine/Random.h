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
};
#endif // !__RANDOM_H__
