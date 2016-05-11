#include "IPathfinder.hpp"

std::string IPathfinder::getStateString()
{
	if (state == NotInitialized)
		return "Initial";
	else if (state == Running)
		return "Running";
	else if (state == Done)
		return "Done";
	else
		return "Failed";
}