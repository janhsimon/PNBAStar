#pragma once

#include <wx/wx.h>

#include "NavMesh.hpp"

class ITool
{
protected:
	NavMesh *navMesh;

	ITool(NavMesh *navMesh) { assert(navMesh); this->navMesh = navMesh; }

public:
	virtual void leftMouseButtonDownEvent(wxMouseEvent &event) = 0;
	virtual void leftMouseButtonUpEvent(wxMouseEvent &event) = 0;
	virtual void rightMouseButtonDownEvent(wxMouseEvent &event) = 0;
	virtual void rightMouseButtonUpEvent(wxMouseEvent &event) = 0;
	virtual void mouseMotionEvent(wxMouseEvent &event) = 0;
};