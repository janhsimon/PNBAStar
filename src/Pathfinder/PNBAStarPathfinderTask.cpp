#include <cassert>
#include <sstream>

#include "tbb\spin_mutex.h"

#include "PNBAStarPathfinderTask.hpp"

PNBAStarPathfinderTask::PNBAStarPathfinderTask(PNBASharedData *sharedData, NavMesh *navMesh, NavMeshNode *startNode, NavMeshNode *goalNode, int taskNumber)
{
	assert(sharedData);
	this->sharedData = sharedData;
	
	assert(navMesh);
	this->navMesh = navMesh;

	assert(startNode);
	this->startNode = startNode;

	assert(goalNode);
	this->goalNode = goalNode;

	this->taskNumber = taskNumber;

	otherTask = nullptr;

	openList.clear();
	F = 0.0f; // TODO: infinity? seems like 0, relatively sure
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

	std::stringstream s;
	s << "Task " << taskNumber << " starting...";
	wxLogMessage(s.str().c_str());

	navMesh->dumpToLog();

	std::stringstream ss;
	ss << "Start node: " << startNode->getID() << " Goal node: " << goalNode->getID();
	wxLogMessage(ss.str().c_str());

	assert(sharedData);
	while (!sharedData->getFinished())
	{
		std::stringstream s;
		s << std::endl << "--------------------------------------------------------------------------------" << std::endl;
		s << "New iteration for task " << taskNumber;
		wxLogMessage(s.str().c_str());

		dumpOpenListToLog();

		NavMeshNode *x = getLowestTotalCostNodeOnOpenList();

		std::stringstream ss;
		ss << "Node #" << x->getID() << " identified as lowest f-cost node on the open list.";
		wxLogMessage(ss.str().c_str());

		if (!x)
		// should never happen!
		{
			wxLogMessage("ERROR: There was no lowest f-cost node on the open list, something went wrong.");
			sharedData->setFinished(true);
			//state = Failed;
			return NULL;
		}
		
		// simulate priority queue pop operation
		removeNodeFromOpenList(x);

		wxLogMessage("Beginning of iteration...");
		dumpOpenListToLog();
		dumpMToLog();

		if (isNodeInM(x))
		{
			wxLogMessage("Lowest f-cost node found in M.");

			float L = sharedData->getL();
			//float h2_x = otherTask->getForwardCostForNode(x);
			float F2 = otherTask->getF();

			float f1_x = (taskNumber == 1) ? x->fCost1 : x->fCost2;
			float g1_x = (taskNumber == 1) ? x->gCost1 : x->gCost2;
			float h2_x = (taskNumber == 1) ? x->hCost2 : x->hCost1;

			std::stringstream s;
			s << std::endl << "f1(x): " << f1_x << std::endl;
			s << "g1(x) + F2 − h2(x): " << g1_x + F2 - h2_x << std::endl;
			s << "L: " << L;
			wxLogMessage(s.str().c_str());

			// if f1(x) < L and g1(x) + F2 − h2(x) < L then
			if (f1_x < L && g1_x + F2 - h2_x < L)
			{
				std::vector<NavMeshNode*> *adjacentNodes = (x->getAdjacentNodes());
				for (std::vector<NavMeshNode*>::iterator y = adjacentNodes->begin(); y != adjacentNodes->end(); ++y)
				{
					std::stringstream s;
					s << "Checking adjacent node #" << (*y)->getID() << ": ";
					
					float d = navMesh->calculateDistanceBetweenNodes(x, *y);
					
					float g1_y = (taskNumber == 1) ? (*y)->gCost1 : (*y)->gCost2;

					// if y is in M and g1(y) > g1(x) + d1(x, y) then
					if (isNodeInM(*y) && g1_y > g1_x + d)
					{
						s << "Candidate found!";

						// g1(y) = g1(x) + d1(x, y)
						// f1(y) = g1(y) + h1(y)
						if (taskNumber == 1)
						{
							(*y)->gCost1 = x->gCost1 + d;
							(*y)->fCost1 = (*y)->gCost1 + (*y)->hCost1;
						}
						else
						{
							(*y)->gCost2 = x->gCost2 + d;
							(*y)->fCost2 = (*y)->gCost2 + (*y)->hCost2;
						}


						// if y is in open1 then
						// open1.remove(y)
						removeNodeFromOpenList(*y);
						
						// open1.insert(y)
						openList.push_back(*y);

						// TODO: check if this is the correct thing to do here or below, or both
						(*y)->setPathPointer(x);

						//float g2_y = otherTask->getBackwardsCostForNode(*y);
						float g2_y = (taskNumber == 1) ? (*y)->gCost2 : (*y)->gCost1;

						// if g1(y) + g2(y) < L then
						if (g1_y + g2_y < sharedData->getL())
						{
							// lock
							tbb::spin_mutex::scoped_lock myLock(tbb::spin_mutex);

							// if g1(y) + g2(y) < L then
							if (g1_y + g2_y < sharedData->getL())
							{
								// L = g1(y) + g2(y)
								sharedData->setL(g1_y + g2_y);

								// TODO: check if this is the correct thing to do here or above, or both
								(*y)->setPathPointer(x);
							}

							// unlock
						}
					}
					else
						s << "Candidate not found!";

					wxLogMessage(s.str().c_str());
				}
			}
			else
			{
				wxLogMessage("Some cost comparison failed!");
			}

			removeNodeFromM(x);
		}
		else
			wxLogMessage("Lowest f-cost node not found in M.");

		wxLogMessage("End of iteration...");
		dumpOpenListToLog();
		dumpMToLog();

		if (openList.size() > 0)
		{
			NavMeshNode *lowestTotalCostNode = getLowestTotalCostNodeOnOpenList();
			assert(lowestTotalCostNode);
			F = (taskNumber == 1) ? lowestTotalCostNode->fCost1 : lowestTotalCostNode->fCost2;
		}
		else
		{
			//goalNode->setPathPointer(x);
			sharedData->setFinished(true);
			std::stringstream s;
			s << "Task number " << taskNumber << " set the finish flag to true...";
			wxLogMessage(s.str().c_str());
		}
	}

	return NULL;
}

NavMeshNode *PNBAStarPathfinderTask::getLowestTotalCostNodeOnOpenList() const
{
	NavMeshNode *lowestCostNode = nullptr;
	float lowestTotalCost;
	for (NavMeshNode *node : openList)
	{
		float thisCost = (taskNumber == 1) ? node->fCost1 : node->fCost2;
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

	// lock
	tbb::spin_mutex::scoped_lock myLock(tbb::spin_mutex);

	for (std::vector<NavMeshNode*>::iterator i = sharedData->M.begin(); i != sharedData->M.end(); ++i)
	{
		if (node == *i)
			return true;
	}

	// unlock

	return false;
}

void PNBAStarPathfinderTask::removeNodeFromM(NavMeshNode *node)
{
	assert(node);
	assert(sharedData);

	// lock
	tbb::spin_mutex::scoped_lock myLock(tbb::spin_mutex);

	for (std::vector<NavMeshNode*>::iterator i = sharedData->M.begin(); i != sharedData->M.end(); ++i)
	{
		if (node == *i)
		{
			sharedData->M.erase(i);
			return;
		}
	}

	// unlock
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

void PNBAStarPathfinderTask::dumpOpenListToLog() const
{
	std::stringstream s;
	s << "Open list: ";
	for (NavMeshNode *node : openList)
	{
		if (node == openList[0])
			s << node->getID();
		else
			s << ", " << node->getID();
	}

	wxLogMessage(s.str().c_str());
}

void PNBAStarPathfinderTask::dumpMToLog() const
{
	std::stringstream s;
	s << "M list: ";
	assert(sharedData);
	for (NavMeshNode *node : sharedData->M)
	{
		if (node == sharedData->M[0])
			s << node->getID();
		else
			s << ", " << node->getID();
	}

	wxLogMessage(s.str().c_str());
}