#include "EditNodeTool.hpp"

EditNodeTool::EditNodeTool(NavMesh *navMesh) : ITool(navMesh)
{

}

void EditNodeTool::leftMouseButtonDownEvent(wxMouseEvent &event)
{
	assert(navMesh);

	NavMeshNode *mouseNode = navMesh->getSelectedNode();

	if (mouseNode)
		draggingNode = mouseNode;
	else
	{
		NavMeshNode *newNode = new NavMeshNode(event.GetPosition());
		draggingNode = newNode;
		navMesh->addNode(newNode);
		mouseMotionEvent(event);
	}
}

void EditNodeTool::leftMouseButtonUpEvent(wxMouseEvent &event)
{
	draggingNode = nullptr;
}

void EditNodeTool::rightMouseButtonDownEvent(wxMouseEvent &event)
{

}

void EditNodeTool::rightMouseButtonUpEvent(wxMouseEvent &event)
{
	assert(navMesh);
	
	if (!event.LeftIsDown())
		navMesh->deleteSelectedNode();
	else if (draggingNode)
		navMesh->deleteNode(draggingNode);
}

void EditNodeTool::mouseMotionEvent(wxMouseEvent &event)
{
	assert(navMesh);

	navMesh->selectNodeClosestTo(event.GetPosition());
	
	if (draggingNode)
		navMesh->moveNode(draggingNode, event.GetPosition());
}