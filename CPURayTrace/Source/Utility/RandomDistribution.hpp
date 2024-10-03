#pragma once

#include <random>

template<typename Tx>
class RandomDistribution
{
public:
	RandomDistribution(Tx InMin, Tx InMax);
	RandomDistribution(int Seed, Tx InMin, Tx InMax);

	Tx GetRandom();
	void SetNewSeed(int InSeed);
private:
	std::mt19937 RandomSeed;
	std::uniform_real_distribution<Tx> Uniform;
};

template<typename Tx>
void RandomDistribution<Tx>::SetNewSeed(int InSeed)
{
	RandomSeed.seed(InSeed);
}

template<typename Tx>
RandomDistribution<Tx>::RandomDistribution(Tx InMin, Tx InMax) :
	RandomSeed(0), Uniform(InMin, InMax)
{

}

template<typename Tx>
RandomDistribution<Tx>::RandomDistribution(int Seed, Tx InMin, Tx InMax):
	RandomSeed(Seed), Uniform(InMin, InMax)
{

}

template<typename Tx>
Tx RandomDistribution<Tx>::GetRandom()
{
	return Uniform(RandomSeed);
}