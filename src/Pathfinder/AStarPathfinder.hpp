#pragma once

#include <vector>

#include "IPathfinder.hpp"

class AStarPathfinder : public IPathfinder
{
public:
	AStarPathfinder(NavMesh *navMesh);

	void findPath();

private:
	std::vector<NavMeshNode*> openList, closedList;

	NavMeshNode *getLowestTotalCostNodeOnOpenList() const;
	void closeNode(NavMeshNode *node);
	bool isNodeOnClosedList(NavMeshNode *node);
	bool isNodeOnOpenList(NavMeshNode *node);
};