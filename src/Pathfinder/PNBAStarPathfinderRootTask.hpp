#pragma once

#define TBB_USE_DEBUG 1
#include <tbb/tbb.h>

#include "PNBASharedData.hpp"
#include "../NavMesh/NavMesh.hpp"

class PNBAStarPathfinderRootTask : public tbb::task
{
public:
	PNBAStarPathfinderRootTask(PNBASharedData *sharedData, NavMesh *navMesh, NavMeshNode *startNode, NavMeshNode *goalNode);

	tbb::task *execute();

private:
	PNBASharedData *sharedData;
	NavMesh *navMesh;
	NavMeshNode *startNode, *goalNode;
};