#pragma once

#include "ITool.hpp"

class MoveNodeTool : public ITool
{
public:
	MoveNodeTool(NavMesh *navMesh);

	void leftMouseButtonDownEvent(wxMouseEvent &event);
	void leftMouseButtonUpEvent(wxMouseEvent &event);
	void rightMouseButtonDownEvent(wxMouseEvent &event);
	void rightMouseButtonUpEvent(wxMouseEvent &event);
	void mouseMotionEvent(wxMouseEvent &event);

private:
	NavMeshNode *draggingNode;
};