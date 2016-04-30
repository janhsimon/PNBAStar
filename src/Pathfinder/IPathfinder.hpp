#pragma once

#include <wx/wx.h>

#include "../NavMesh/NavMesh.hpp"

class IPathfinder
{
protected:
	NavMesh *navMesh;

	IPathfinder(NavMesh *navMesh) { assert(navMesh); this->navMesh = navMesh; }

public:
	virtual void findPath() = 0;
};