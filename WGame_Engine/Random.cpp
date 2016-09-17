#include "Random.h"
#include <random>
#include <ctime>

using namespace std;

float Random::F_Random()
{
	//Use time(NULL) to generate a diferent random number every time that we call the function
	mt19937 generator(time(NULL));
	uniform_real_distribution<float> distributor(0.0f, 1.0f);

	//Generate Float Random Number
	return distributor(generator);

}


int Random::Min_Max_Random(int min_a, int max_b)
{
	//Generator
	mt19937 generator(time(NULL));
	uniform_int_distribution<int> distributor(min_a, max_b);
	
	//Generate Int Random Number
	return distributor(generator);
}