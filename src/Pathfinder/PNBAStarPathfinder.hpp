#pragma once

#include "IPathfinder.hpp"
#include "PNBASharedData.hpp"

class PNBAStarPathfinder : public IPathfinder
{
public:
	PNBAStarPathfinder(NavMesh *navMesh);

	void reset();
	void calculateStep();
	void calculatePath();

private:
	PNBASharedData sharedData;
};