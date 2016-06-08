#pragma once

#include <vector>

#include "../NavMesh/NavMeshNode.hpp"

struct PNBASharedData
{
	bool finished;
	std::vector<NavMeshNode*> M;
	float L;
};