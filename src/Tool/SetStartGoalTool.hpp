#pragma once

#include "ITool.hpp"

class SetStartGoalTool : public ITool
{
public:
	SetStartGoalTool(NavMesh *navMesh);

	void leftMouseButtonDownEvent(wxMouseEvent &event);
	void leftMouseButtonUpEvent(wxMouseEvent &event);
	void rightMouseButtonDownEvent(wxMouseEvent &event);
	void rightMouseButtonUpEvent(wxMouseEvent &event);
	void mouseMotionEvent(wxMouseEvent &event);
};