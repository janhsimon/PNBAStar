#pragma once

#include <vector>

class NavMeshNode
{
public:
	NavMeshNode(const wxPoint &point);

	inline float getX() const { return x; }
	inline float getY() const { return y; }
	inline void setPosition(float x, float y) { this->x = x; this->y = y; }

	inline std::vector<NavMeshNode*> *getAdjacentNodes() { return &adjacentNodes; }
	inline void addAdjacentNode(NavMeshNode *node) { adjacentNodes.push_back(node); }
	
	void renderConnections(float r, float g, float b) const;
	void renderNormalNode(float r, float g, float b) const;
	void renderStartNode(float r, float g, float b) const;
	void renderGoalNode(float r, float g, float b) const;
	void renderPathPointer(float r, float g, float b) const;

	inline float getForwardCost() const { return forwardCost; }
	inline float getBackwardsCost() const { return backwardsCost; }
	inline float getTotalCost() const { return totalCost; }
	inline void setForwardCost(float forwardCost) { this->forwardCost = forwardCost; }
	inline void setBackwardsCost(float backwardsCost) { this->backwardsCost = backwardsCost; }
	inline void updateTotalCost() { totalCost = forwardCost + backwardsCost; }

	inline NavMeshNode *getPathPointer() const { return pathPointer; }
	inline void setPathPointer(NavMeshNode *pathPointer) { this->pathPointer = pathPointer; }
	
private:
	static const float ICON_SIZE;
	static const float PATH_POINTER_WIDTH;
	static const float PATH_POINTER_ARROW_LENGTH;

	float x, y;
	std::vector<NavMeshNode*> adjacentNodes;

	float forwardCost, backwardsCost, totalCost;
	NavMeshNode *pathPointer;
};