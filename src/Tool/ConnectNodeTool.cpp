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
		originNode->setLineEnable(false);
		originNode = nullptr;
	}
}

void ConnectNodeTool::rightMouseButtonDownEvent(wxMouseEvent &event)
{

}

void ConnectNodeTool::rightMouseButtonUpEvent(wxMouseEvent &event)
{
	if (originNode)
	{
		originNode->setLineEnable(false);
		originNode = nullptr;
	}
	else
	{
		assert(navMesh);
		NavMeshNode *mouseNode = navMesh->getSelectedNode();

		if (mouseNode)
			navMesh->deleteNodeConnections(mouseNode);
	}
}

void ConnectNodeTool::mouseMotionEvent(wxMouseEvent &event)
{
	if (originNode)
	{
		wxPoint mousePosition = event.GetPosition();
		originNode->setLinePosition(mousePosition.x, mousePosition.y);
		originNode->setLineEnable(true);
	}
	
	assert(navMesh);
	navMesh->selectNodeClosestTo(event.GetPosition());	
}