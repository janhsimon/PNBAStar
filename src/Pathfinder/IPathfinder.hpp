#pragma once

#include <wx/wx.h>

#include "../NavMesh/NavMesh.hpp"

enum PathfinderState
{
	NotInitialized,
	Running,
	Done,
	Failed
};

class IPathfinder
{
protected:
	NavMesh *navMesh;
	PathfinderState state;

	IPathfinder(NavMesh *navMesh) { assert(navMesh); this->navMesh = navMesh; state = NotInitialized; }

public:
	virtual void reset() = 0;
	virtual void calculateStep() = 0;
	virtual void calculatePath() = 0;
	
	inline PathfinderState getState() { return state; }

	std::string getStateString();
};