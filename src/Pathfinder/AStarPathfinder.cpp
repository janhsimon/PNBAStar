#include <sstream>

#include "AStarPathfinder.hpp"

AStarPathfinder::AStarPathfinder(NavMesh *navMesh) : IPathfinder(navMesh)
{

}

NavMeshNode *AStarPathfinder::getLowestTotalCostNodeOnOpenList() const
{
	assert(navMesh);

	NavMeshNode *lowestCostNode = nullptr;
	float lowestTotalCost;
	for (NavMeshNode *node : openList)
	{
		float thisCost = node->getTotalCost();
		if (!lowestCostNode || thisCost < lowestTotalCost)
		{
			lowestCostNode = node;
			lowestTotalCost = thisCost;
		}
	}

	return lowestCostNode;
}

void AStarPathfinder::closeNode(NavMeshNode *node)
{
	assert(node);
	for (std::vector<NavMeshNode*>::iterator i = openList.begin(); i != openList.end(); ++i)
	{
		if (node == *i)
		{
			openList.erase(i);
			closedList.push_back(node);
			return;
		}
	}
}

bool AStarPathfinder::isNodeOnClosedList(NavMeshNode *node)
{
	assert(node);
	for (std::vector<NavMeshNode*>::iterator i = closedList.begin(); i != closedList.end(); ++i)
	{
		if (node == *i)
			return true;
	}

	return false;
}

bool AStarPathfinder::isNodeOnOpenList(NavMeshNode *node)
{
	assert(node);
	for (std::vector<NavMeshNode*>::iterator i = openList.begin(); i != openList.end(); ++i)
	{
		if (node == *i)
			return true;
	}

	return false;
}

void AStarPathfinder::findPath()
{
	assert(navMesh);

	NavMeshNode *startNode = navMesh->getStartNode();

	if (!startNode)
	{
		wxMessageBox("The nav mesh has no start node.", "Error", wxICON_ERROR);
		return;
	}

	NavMeshNode *goalNode = navMesh->getGoalNode();

	if (!goalNode)
	{
		wxMessageBox("The nav mesh has no goal node.", "Error", wxICON_ERROR);
		return;
	}

	openList.clear();
	closedList.clear();

	navMesh->resetPathPointers();

	
	// step 0
	
	openList.push_back(startNode);


	// step 1
	
	NavMeshNode *i = getLowestTotalCostNodeOnOpenList();

	while (i)
	{
		// step 2

		if (i == goalNode)
			return;


		// step 3
	
		closeNode(i);


		// step 4

		std::vector<NavMeshNode*> *adjacentNodes = i->getAdjacentNodes();
		assert(adjacentNodes);
		for (std::vector<NavMeshNode*>::iterator j = adjacentNodes->begin(); j != adjacentNodes->end(); ++j)
		{
			if (isNodeOnClosedList(*j))
				continue;

			if (!isNodeOnOpenList(*j))
			{
				openList.push_back(*j);
				(*j)->setForwardCost(navMesh->calculateDistanceBetweenNodes(*j, goalNode));
				(*j)->setBackwardsCost(i->getBackwardsCost() + navMesh->calculateDistanceBetweenNodes(i, *j));
				(*j)->updateTotalCost();
				(*j)->setPathPointer(i);
			}
			else
			{
				float currentBackwardsCost = (*j)->getBackwardsCost();
				float newBackwardsCost = i->getBackwardsCost() + navMesh->calculateDistanceBetweenNodes(i, *j);

				if (newBackwardsCost < currentBackwardsCost)
				{
					(*j)->setPathPointer(i);
					(*j)->setBackwardsCost(newBackwardsCost);
					(*j)->updateTotalCost();
				}
			}
		}

		i = getLowestTotalCostNodeOnOpenList();
	}

	wxMessageBox("No path between start and goal node found.", "Info", wxICON_INFORMATION);
}