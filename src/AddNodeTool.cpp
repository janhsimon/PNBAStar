#include "AddNodeTool.hpp"

AddNodeTool::AddNodeTool(NavMesh *navMesh) : ITool(navMesh)
{
	
}

void AddNodeTool::leftMouseButtonDownEvent(wxMouseEvent &event)
{
	
}

void AddNodeTool::leftMouseButtonUpEvent(wxMouseEvent &event)
{
	assert(navMesh);
	navMesh->addNode(new NavMeshNode(event.GetPosition()));
	mouseMotionEvent(event);
}

void AddNodeTool::rightMouseButtonDownEvent(wxMouseEvent &event)
{

}

void AddNodeTool::rightMouseButtonUpEvent(wxMouseEvent &event)
{
	assert(navMesh);
	navMesh->deleteSelectedNode();
}

void AddNodeTool::mouseMotionEvent(wxMouseEvent &event)
{
	assert(navMesh);
	navMesh->selectNodeClosestTo(event.GetPosition());
}