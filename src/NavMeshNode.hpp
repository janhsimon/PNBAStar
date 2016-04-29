#pragma once

#include <vector>

class NavMeshNode
{
public:
	NavMeshNode(const wxPoint &point);

	inline float getX() const { return x; }
	inline float getY() const { return y; }
	inline void setPosition(float x, float y) { this->x = x; this->y = y; }

	inline std::vector<NavMeshNode*> getAdjacentNodes() const { return adjacentNodes; }
	inline void addAdjacentNode(NavMeshNode *node) { adjacentNodes.push_back(node); }
	
	void render(bool selected) const;

private:
	static const float ICON_SIZE;

	float x, y;
	std::vector<NavMeshNode*> adjacentNodes;
};