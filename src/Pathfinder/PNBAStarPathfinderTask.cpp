#include <cassert>

#include "PNBAStarPathfinderTask.hpp"

PNBAStarPathfinderTask::PNBAStarPathfinderTask(PNBASharedData *sharedData, NavMesh *navMesh, NavMeshNode *startNode, NavMeshNode *goalNode)
{
	assert(sharedData);
	this->sharedData = sharedData;
	
	assert(navMesh);
	this->navMesh = navMesh;

	assert(startNode);
	this->startNode = startNode;

	assert(goalNode);
	this->goalNode = goalNode;

	otherTask = nullptr;

	openList.clear();
	F = 0.0f; // TODO: infinity?
}

void PNBAStarPathfinderTask::setOtherTask(PNBAStarPathfinderTask *otherTask)
{
	assert(otherTask);
	assert(otherTask != this);
	this->otherTask = otherTask;
}

tbb::task *PNBAStarPathfinderTask::execute()
{
	/*
	finished = shared flag
	M = shared middle list
	L = shared lowest cost

	open1/2 = local open list
	f1/2 = local f-cost
	g1/2 = local g-cost
	h1/2 = local h-cost
	d1/2 = local distance function
	F1/F2 = local lowest F-value so far

	while !finished do
		x = open1.pop()
		if x is in M then
			if f1(x) < L and g1(x) + F2 − h2(x) < L then
				for all edges (x, y) of the graph being explored do
					if y is in M and g1(y) > g1(x) + d1(x, y) then
						g1(y) = g1(x) + d1(x, y)
						f1(y) = g1(y) + h1(y)
						if y is in open1 then
							open1.remove(y)
						open1.insert(y)
						if g1(y) + g2(y) < L then
							lock
							if g1(y) + g2(y) < L then
								L = g1(y) + g2(y)
							unlock
			M.remove(x)
		if open1.size() > 0 then
			F1 = f1(open1.peek())
		else
			finished = true
	*/
	assert(otherTask);

	assert(startNode);
	assert(goalNode);
	assert(navMesh);
	openList.push_back(startNode);

	assert(sharedData);
	while (!sharedData->finished)
	{
		NavMeshNode *x = getLowestTotalCostNodeOnOpenList();

		if (!x)
		{
			sharedData->finished = true;
			break;
			//state = Failed;
		}
		
		// simulate priority queue pop operation
		removeNodeFromOpenList(x);

		if (isNodeInM(x))
		{
			float xTotalCost = x->getTotalCost();
			float L = sharedData->L;
			float xBackCost = x->getBackwardsCost();
			float otherF = otherTask->getF();
			
			if (xTotalCost < L && xBackCost + otherF - xBackCost/*h2(x)*/ < L)
			{
				std::vector<NavMeshNode*> adjacentNodes = *(x->getAdjacentNodes());
				for (std::vector<NavMeshNode*>::iterator y = adjacentNodes.begin(); y != adjacentNodes.end(); ++y)
				{
					float d = navMesh->calculateDistanceBetweenNodes(x, *y);
					if (isNodeInM(*y) && (*y)->getBackwardsCost() > x->getBackwardsCost() + d)
					{
						(*y)->setBackwardsCost(x->getBackwardsCost() + d);
						// TODO: check if this is ok and equivalent?
						(*y)->setForwardCost(navMesh->calculateDistanceBetweenNodes(*y, goalNode));
						//(*y)->setTotalCost((*y)->getBackwardsCost() + (*y)->getForwardCost());
						(*y)->updateTotalCost();

						// TODO: check if this is the correct thing to do here
						(*y)->setPathPointer(x);

						removeNodeFromOpenList(*y);
						openList.push_back(*y);

						if ((*y)->getBackwardsCost() + (*y)->getForwardCost()/*g2(y)*/ < sharedData->L)
						{
							// lock

							//if ((*y)->getBackwardsCost() + (*y)->getForwardCost()/*g2(y)*/ < sharedData->L)
								//sharedData->L = (*y)->getBackwardsCost() + (*y)->getForwardCost()/*g2(y)*/;

							// unlock
						}
					}
				}
			}

			removeNodeFromM(x);
		}

		if (openList.size() > 0)
		{
			F = getLowestTotalCostNodeOnOpenList()->getTotalCost();
		}
		else
			sharedData->finished = true;
	}

	return NULL;
}

NavMeshNode *PNBAStarPathfinderTask::getLowestTotalCostNodeOnOpenList() const
{
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

bool PNBAStarPathfinderTask::isNodeOnOpenList(NavMeshNode *node)
{
	assert(node);
	for (std::vector<NavMeshNode*>::iterator i = openList.begin(); i != openList.end(); ++i)
	{
		if (node == *i)
			return true;
	}

	return false;
}

bool PNBAStarPathfinderTask::isNodeInM(NavMeshNode *node)
{
	assert(node);
	assert(sharedData);
	for (std::vector<NavMeshNode*>::iterator i = sharedData->M.begin(); i != sharedData->M.end(); ++i)
	{
		if (node == *i)
			return true;
	}

	return false;
}

void PNBAStarPathfinderTask::removeNodeFromM(NavMeshNode *node)
{
	assert(node);
	assert(sharedData);
	for (std::vector<NavMeshNode*>::iterator i = sharedData->M.begin(); i != sharedData->M.end(); ++i)
	{
		if (node == *i)
		{
			sharedData->M.erase(i);
			return;
		}
	}
}

void PNBAStarPathfinderTask::removeNodeFromOpenList(NavMeshNode *node)
{
	assert(node);
	assert(sharedData);
	for (std::vector<NavMeshNode*>::iterator i = openList.begin(); i != openList.end(); ++i)
	{
		if (node == *i)
		{
			openList.erase(i);
			return;
		}
	}
}