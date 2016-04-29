#include "MoveNodeTool.hpp"

MoveNodeTool::MoveNodeTool(NavMesh *navMesh) : ITool(navMesh)
{
	
}

void MoveNodeTool::leftMouseButtonDownEvent(wxMouseEvent &event)
{
	assert(navMesh);
	draggingNode = navMesh->getSelectedNode();
	mouseMotionEvent(event);
}

void MoveNodeTool::leftMouseButtonUpEvent(wxMouseEvent &event)
{
	draggingNode = nullptr;
}

void MoveNodeTool::rightMouseButtonDownEvent(wxMouseEvent &event)
{

}

void MoveNodeTool::rightMouseButtonUpEvent(wxMouseEvent &event)
{
	assert(navMesh);

	if (!event.LeftIsDown())
		navMesh->deleteSelectedNode();
	else if (draggingNode)
		navMesh->deleteNode(draggingNode);
}

void MoveNodeTool::mouseMotionEvent(wxMouseEvent &event)
{
	assert(navMesh);

	if (!event.LeftIsDown())
		navMesh->selectNodeClosestTo(event.GetPosition());
	else if (draggingNode)
		navMesh->moveNode(draggingNode, event.GetPosition());
}