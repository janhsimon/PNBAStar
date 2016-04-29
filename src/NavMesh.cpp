#include "NavMesh.hpp"

#include <wx/glcanvas.h>

const float NavMesh::SELECTION_RADIUS = 25.0f;

NavMesh::NavMesh()
{
	selectedNode = nullptr;
}

void NavMesh::addNode(NavMeshNode *node)
{
	assert(node);
	nodes.push_back(node);
};

void NavMesh::deleteNode(NavMeshNode *node)
{
	assert(node);

	for (NavMeshNode *n : nodes)
	{
		// TODO: fix, removal during iteration
		std::vector<NavMeshNode*> adjacentNodes = n->getAdjacentNodes();
		adjacentNodes.erase(std::remove(adjacentNodes.begin(), adjacentNodes.end(), node));
	}

	nodes.erase(std::remove(nodes.begin(), nodes.end(), node));
}

void NavMesh::deleteSelectedNode()
{
	if (selectedNode)
		deleteNode(selectedNode);
}

void NavMesh::selectNodeClosestTo(const wxPoint &point)
{
	NavMeshNode *closestNode = nullptr;
	float closestDistance;
	for (NavMeshNode *node : nodes)
	{
		wxPoint pointToNode = wxPoint(node->getX(), node->getY()) - point;
		float thisDistance = pointToNode.x * pointToNode.x + pointToNode.y * pointToNode.y;

		if ((!closestNode || thisDistance < closestDistance) && thisDistance < SELECTION_RADIUS * SELECTION_RADIUS)
		{
			closestNode = node;
			closestDistance = thisDistance;
		}
	}

	setSelectedNode(closestNode);
}

void NavMesh::moveNode(NavMeshNode *node, const wxPoint &point)
{
	assert(node);
	node->setPosition(point.x, point.y);
}

void NavMesh::connectNodes(NavMeshNode *a, NavMeshNode *b)
{
	assert(a);
	assert(b);

	a->addAdjacentNode(b);
	b->addAdjacentNode(a);
}

void NavMesh::render()
{
	for (NavMeshNode *node : nodes)
		node->render(node == selectedNode);
}