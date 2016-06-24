#pragma once

#define TBB_USE_DEBUG 1
#include <tbb/tbb.h>

#include "PNBASharedData.hpp"
#include "../NavMesh/NavMesh.hpp"
#include "../NavMesh/NavMeshNode.hpp"

class PNBAStarPathfinderTask : public tbb::task
{
public:
	PNBAStarPathfinderTask(PNBASharedData *sharedData, NavMesh *navMesh, NavMeshNode *startNode, NavMeshNode *goalNode, int taskNumber);

	void setOtherTask(PNBAStarPathfinderTask *otherTask);
	tbb::task *execute();

	inline float getF() { return F; }

	//inline float getForwardCostForNode(NavMeshNode *node) const { assert(node); return node->getForwardCost(); }
	//inline float getBackwardsCostForNode(NavMeshNode *node) const { assert(node); return node->getBackwardsCost(); }

private:
	PNBAStarPathfinderTask *otherTask;
	PNBASharedData *sharedData;
	std::vector<NavMeshNode*> openList;
	float F;
	NavMesh *navMesh;
	NavMeshNode *startNode, *goalNode;
	int taskNumber;

	NavMeshNode *getLowestTotalCostNodeOnOpenList() const;
	bool isNodeOnOpenList(NavMeshNode *node);
	bool isNodeInM(NavMeshNode *node);
	void removeNodeFromM(NavMeshNode *node);
	void removeNodeFromOpenList(NavMeshNode *node);

	void dumpOpenListToLog() const;
	void dumpMToLog() const;
};