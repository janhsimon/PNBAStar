#pragma once

#include "ITool.hpp"

class EditNodeTool : public ITool
{
public:
	EditNodeTool(NavMesh *navMesh);

	void leftMouseButtonDownEvent(wxMouseEvent &event);
	void leftMouseButtonUpEvent(wxMouseEvent &event);
	void rightMouseButtonDownEvent(wxMouseEvent &event);
	void rightMouseButtonUpEvent(wxMouseEvent &event);
	void mouseMotionEvent(wxMouseEvent &event);

private:
	NavMeshNode *draggingNode;
};