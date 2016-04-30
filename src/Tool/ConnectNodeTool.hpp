#pragma once

#include "ITool.hpp"

class ConnectNodeTool : public ITool
{
public:
	ConnectNodeTool(NavMesh *navMesh);

	void leftMouseButtonDownEvent(wxMouseEvent &event);
	void leftMouseButtonUpEvent(wxMouseEvent &event);
	void rightMouseButtonDownEvent(wxMouseEvent &event);
	void rightMouseButtonUpEvent(wxMouseEvent &event);
	void mouseMotionEvent(wxMouseEvent &event);

private:
	NavMeshNode *originNode;
};