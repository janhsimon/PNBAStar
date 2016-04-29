#include "ConnectNodeTool.hpp"

ConnectNodeTool::ConnectNodeTool(NavMesh *navMesh) : ITool(navMesh)
{

}

void ConnectNodeTool::leftMouseButtonDownEvent(wxMouseEvent &event)
{

}

void ConnectNodeTool::leftMouseButtonUpEvent(wxMouseEvent &event)
{
	assert(navMesh);
	NavMeshNode *mouseNode = navMesh->getSelectedNode();

	if (!mouseNode)
		return;

	if (!originNode)
		originNode = mouseNode;
	else if (originNode != mouseNode)
	{
		navMesh->connectNodes(originNode, mouseNode);
		originNode = nullptr;
	}
}

void ConnectNodeTool::rightMouseButtonDownEvent(wxMouseEvent &event)
{

}

void ConnectNodeTool::rightMouseButtonUpEvent(wxMouseEvent &event)
{
	// TODO: delete all adjacencies of selected node
	//assert(navMesh);
	//navMesh->deleteSelectedNode();
}

void ConnectNodeTool::mouseMotionEvent(wxMouseEvent &event)
{
	assert(navMesh);
	navMesh->selectNodeClosestTo(event.GetPosition());
}