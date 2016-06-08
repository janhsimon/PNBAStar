#pragma once

#define TBB_USE_DEBUG 1
#include <tbb/tbb.h>

#include "PNBASharedData.hpp"
#include "../NavMesh/NavMesh.hpp"
#include "../NavMesh/NavMeshNode.hpp"

class PNBAStarPathfinderTask : public tbb::task
{
public:
	PNBAStarPathfinderTask(PNBASharedData *sharedData, NavMesh *navMesh, NavMeshNode *startNode, NavMeshNode *goalNode);

	void setOtherTask(PNBAStarPathfinderTask *otherTask);
	tbb::task *execute();

	inline float getF() { return F; }

private:
	PNBAStarPathfinderTask *otherTask;
	PNBASharedData *sharedData;
	std::vector<NavMeshNode*> openList;
	float F;
	NavMesh *navMesh;
	NavMeshNode *startNode, *goalNode;

	NavMeshNode *getLowestTotalCostNodeOnOpenList() const;
	bool isNodeOnOpenList(NavMeshNode *node);
	bool isNodeInM(NavMeshNode *node);
	void removeNodeFromM(NavMeshNode *node);
	void removeNodeFromOpenList(NavMeshNode *node);
};