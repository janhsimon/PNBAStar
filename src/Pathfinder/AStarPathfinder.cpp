#include <sstream>

#include "AStarPathfinder.hpp"

AStarPathfinder::AStarPathfinder(NavMesh *navMesh) : IPathfinder(navMesh)
{
	currentNode = startNode = goalNode = nullptr;
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

void AStarPathfinder::reset()
{
	openList.clear();
	closedList.clear();

	assert(navMesh);
	navMesh->resetPathPointers();

	state = NotInitialized;
}

void AStarPathfinder::calculateStep()
{
	if (state == NotInitialized)
	{
		reset();

		startNode = navMesh->getStartNode();

		if (!startNode)
		{
			wxMessageBox("The nav mesh has no start node.", "Error", wxICON_ERROR);
			state = Failed;
			return;
		}

		goalNode = navMesh->getGoalNode();

		if (!goalNode)
		{
			wxMessageBox("The nav mesh has no goal node.", "Error", wxICON_ERROR);
			state = Failed;
			return;
		}

		openList.push_back(startNode);

		currentNode = startNode;

		state = Running;
	}
	else if (state == Running)
	{
		assert(currentNode);
		assert(startNode);
		assert(goalNode);

		if (currentNode == goalNode)
		{
			state = Done;
			return;
		}

		closeNode(currentNode);

		std::vector<NavMeshNode*> adjacentNodes = *(currentNode->getAdjacentNodes());
		for (std::vector<NavMeshNode*>::iterator j = adjacentNodes.begin(); j != adjacentNodes.end(); ++j)
		{
			if (isNodeOnClosedList(*j))
				continue;

			if (!isNodeOnOpenList(*j))
			{
				openList.push_back(*j);
				(*j)->setForwardCost(navMesh->calculateDistanceBetweenNodes(*j, goalNode));
				(*j)->setBackwardsCost(currentNode->getBackwardsCost() + navMesh->calculateDistanceBetweenNodes(currentNode, *j));
				(*j)->updateTotalCost();
				(*j)->setPathPointer(currentNode);
			}
			else
			{
				float currentBackwardsCost = (*j)->getBackwardsCost();
				float newBackwardsCost = currentNode->getBackwardsCost() + navMesh->calculateDistanceBetweenNodes(currentNode, *j);

				if (newBackwardsCost < currentBackwardsCost)
				{
					(*j)->setPathPointer(currentNode);
					(*j)->setBackwardsCost(newBackwardsCost);
					(*j)->updateTotalCost();
				}
			}
		}

		currentNode = getLowestTotalCostNodeOnOpenList();

		if (!currentNode)
			state = Failed;
	}
}

void AStarPathfinder::calculatePath()
{
	while (state != Done && state != Failed)
		calculateStep();
}