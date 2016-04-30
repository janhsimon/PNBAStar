#include "SetStartGoalTool.hpp"

SetStartGoalTool::SetStartGoalTool(NavMesh *navMesh) : ITool(navMesh)
{

}

void SetStartGoalTool::leftMouseButtonDownEvent(wxMouseEvent &event)
{
	
}

void SetStartGoalTool::leftMouseButtonUpEvent(wxMouseEvent &event)
{
	assert(navMesh);

	NavMeshNode *mouseNode = navMesh->getSelectedNode();

	if (mouseNode)
		navMesh->setStartNode(mouseNode);
}

void SetStartGoalTool::rightMouseButtonDownEvent(wxMouseEvent &event)
{

}

void SetStartGoalTool::rightMouseButtonUpEvent(wxMouseEvent &event)
{
	assert(navMesh);

	NavMeshNode *mouseNode = navMesh->getSelectedNode();

	if (mouseNode)
		navMesh->setGoalNode(mouseNode);
}

void SetStartGoalTool::mouseMotionEvent(wxMouseEvent &event)
{
	assert(navMesh);
	navMesh->selectNodeClosestTo(event.GetPosition());
}