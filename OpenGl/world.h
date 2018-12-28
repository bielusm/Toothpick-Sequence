#pragma once
#include "Toothpick.hpp"
#include <vector>
class World
{
public:
	World(float pickWidth, float pickOffset, int maxGen);
	void update();

	std::vector<Toothpick> picks;
	std::vector<Toothpick> deadPicks;
private:
	float pickWidth;
	float pickOffset;

	int N;
	int maxGen;

	void setSides(Toothpick * pick);
};

