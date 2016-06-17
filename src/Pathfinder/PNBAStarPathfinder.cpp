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

	sharedData.finished = false;
	sharedData.L = 1000000000.0f; // TODO: infinity?

	sharedData.M.clear();
	std::vector<NavMeshNode*> nodes = (*navMesh->getNodes());
	for (NavMeshNode *node : nodes)
	{
		node->setBackwardsCost(1000000000.0f); // infinity
		sharedData.M.push_back(node);
	}

	startNode->setBackwardsCost(0.0f);
	goalNode->setBackwardsCost(0.0f);

	tbb::task_scheduler_init init(1); // tbb::task_scheduler_init::automatic TODO: uncomment

	PNBAStarPathfinderRootTask *rootTask = new (tbb::task::allocate_root()) PNBAStarPathfinderRootTask(&sharedData, navMesh, startNode, goalNode);
	tbb::task::spawn_root_and_wait(*rootTask);
	state = Done;
}