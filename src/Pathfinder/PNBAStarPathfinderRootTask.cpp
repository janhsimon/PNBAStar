#include "PNBAStarPathfinderRootTask.hpp"
#include "PNBAStarPathfinderTask.hpp"

PNBAStarPathfinderRootTask::PNBAStarPathfinderRootTask(PNBASharedData *sharedData, NavMesh *navMesh, NavMeshNode *startNode, NavMeshNode *goalNode)
{
	assert(sharedData);
	this->sharedData = sharedData;

	assert(navMesh);
	this->navMesh = navMesh;

	assert(startNode);
	this->startNode = startNode;

	assert(goalNode);
	this->goalNode = goalNode;
}

tbb::task *PNBAStarPathfinderRootTask::execute()
{
	assert(sharedData);
	assert(navMesh);
	assert(startNode);
	assert(goalNode);
	PNBAStarPathfinderTask *t1 = new(allocate_child()) PNBAStarPathfinderTask(sharedData, navMesh, startNode, goalNode, 1);
	PNBAStarPathfinderTask *t2 = new(allocate_child()) PNBAStarPathfinderTask(sharedData, navMesh, goalNode, startNode, 2);

	t1->setOtherTask(t2);
	t2->setOtherTask(t1);

	tbb::task_list taskList;
	taskList.push_back(*t1);
	taskList.push_back(*t2);

	set_ref_count(3); // 2 (1 per child task) + 1 (for the wait) 
	
	spawn_and_wait_for_all(taskList);
	
	return NULL;
}