#pragma once

#include <vector>

#include "IPathfinder.hpp"

class AStarPathfinder : public IPathfinder
{
public:
	AStarPathfinder(NavMesh *navMesh);

	void reset();
	void calculateStep();
	void calculatePath();

private:
	std::vector<NavMeshNode*> openList, closedList;
	NavMeshNode *currentNode, *startNode, *goalNode;

	NavMeshNode *getLowestTotalCostNodeOnOpenList() const;
	void closeNode(NavMeshNode *node);
	bool isNodeOnClosedList(NavMeshNode *node);
	bool isNodeOnOpenList(NavMeshNode *node);
};