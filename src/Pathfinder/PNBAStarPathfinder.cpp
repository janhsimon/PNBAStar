#include "PNBAStarPathfinder.hpp"
#include "PNBAStarPathfinderRootTask.hpp"

PNBAStarPathfinder::PNBAStarPathfinder(NavMesh *navMesh) : IPathfinder(navMesh)
{
	assert(navMesh);
}

void PNBAStarPathfinder::reset()
{
	assert(navMesh);
	navMesh->resetPathPointers();

	state = NotInitialized;
}

void PNBAStarPathfinder::calculateStep()
{
	
}

void PNBAStarPathfinder::calculatePath()
{
	assert(navMesh);

	NavMeshNode *startNode = navMesh->getStartNode();

	if (!startNode)
	{
		wxMessageBox("The nav mesh has no start node.", "Error", wxICON_ERROR);
		state = Failed;
		return;
	}

	NavMeshNode *goalNode = navMesh->getGoalNode();

	if (!goalNode)
	{
		wxMessageBox("The nav mesh has no goal node.", "Error", wxICON_ERROR);
		state = Failed;
		return;
	}

	reset();

	sharedData.setFinished(false);
	sharedData.setL(100000.0f); // infinity

	sharedData.M.clear();
	std::vector<NavMeshNode*> nodes = (*navMesh->getNodes());
	for (NavMeshNode *node : nodes)
	{
		//node->setForwardCost(navMesh->calculateDistanceBetweenNodes(node, goalNode)); // pre-process forward cost
		//node->setBackwardsCost(100000.0f); // infinity
		//node->updateTotalCost();
		
		//node->hCost1 = navMesh->calculateDistanceBetweenNodes(node, goalNode);
		//node->hCost2 = navMesh->calculateDistanceBetweenNodes(node, startNode);
		node->gCost1 = node->gCost2 = 100000.0f; // infinity
		//node->fCost1 = node->hCost1 + node->gCost1;
		//node->fCost2 = node->hCost2 + node->gCost2;

		sharedData.M.push_back(node);
	}

	//startNode->setBackwardsCost(0.0f);
	//startNode->updateTotalCost();
	//goalNode->setBackwardsCost(0.0f);
	//goalNode->updateTotalCost();

	//startNode->gCost1 = startNode->gCost2 = goalNode->gCost1 = goalNode->gCost2 = 0.0f;
	//startNode->fCost1 = startNode->gCost1 + startNode->hCost1;
	//startNode->fCost2 = startNode->gCost2 + startNode->hCost2;

	startNode->gCost1 = goalNode->gCost2 = 0.0f;

	tbb::task_scheduler_init init(tbb::task_scheduler_init::automatic);

	PNBAStarPathfinderRootTask *rootTask = new (tbb::task::allocate_root()) PNBAStarPathfinderRootTask(&sharedData, navMesh, startNode, goalNode);
	tbb::task::spawn_root_and_wait(*rootTask);
	state = Done;
}