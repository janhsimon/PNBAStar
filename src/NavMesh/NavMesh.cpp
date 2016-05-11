#include <wx/glcanvas.h>

#include "NavMesh.hpp"

const float NavMesh::SELECTION_RADIUS = 20.0f;

NavMesh::NavMesh()
{
	selectedNode = startNode = goalNode = nullptr;
	showFinalPathOnly = false;
}

NavMesh::~NavMesh()
{
	for (NavMeshNode *node : nodes)
		delete node;
	
	nodes.clear();
}

void NavMesh::addNode(NavMeshNode *node)
{
	assert(node);
	nodes.push_back(node);
};

void NavMesh::deleteNode(NavMeshNode *node)
{
	assert(node);

	for (std::vector<NavMeshNode*>::iterator i = nodes.begin(); i != nodes.end();)
	{
		if (*i == node)
			i = nodes.erase(i);
		else
		{
			std::vector<NavMeshNode*> adjacentNodes = *((*i)->getAdjacentNodes());
			for (std::vector<NavMeshNode*>::iterator j = adjacentNodes.begin(); j != adjacentNodes.end();)
			{
				if (*j == node)
					j = adjacentNodes.erase(j);
				else
					++j;
			}

			++i;
		}
	}

	if (node == selectedNode)
		selectedNode = nullptr;

	if (node == startNode)
		startNode = nullptr;

	if (node == goalNode)
		goalNode = nullptr;
}

void NavMesh::deleteSelectedNode()
{
	if (selectedNode)
		deleteNode(selectedNode);
}

void NavMesh::deleteNodeConnections(NavMeshNode *node)
{
	assert(node);
	std::vector<NavMeshNode*> adjacentNodes = (*node->getAdjacentNodes());
	for (NavMeshNode *adjacentNode : adjacentNodes)
	{
		adjacentNode->removeAdjacentNode(node);
		node->removeAdjacentNode(adjacentNode);
	}
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

float NavMesh::calculateDistanceBetweenNodes(NavMeshNode *a, NavMeshNode *b) const
{
	float x = b->getX() - a->getX();
	float y = b->getY() - a->getY();
	return x * x + y * y;
}

void NavMesh::resetPathPointers()
{
	for (NavMeshNode *node : nodes)
		node->setPathPointer(nullptr);
}

void NavMesh::render()
{
	for (NavMeshNode *node : nodes)
	{
		node->renderConnections(0.0f, 0.0f, 0.0f);
		node->renderLine(0.0f, 0.0f, 1.0f);
	}

	for (NavMeshNode *node : nodes)
	{
		float r = 0.0f;
		float g = 0.0f;
		float b = 0.0f;

		if (node == selectedNode)
			b = 1.0f;

		if (node == startNode)
			node->renderStartNode(r, g, b);
		else if (node == goalNode)
			node->renderGoalNode(r, g, b);
		else
			node->renderNormalNode(r, g, b);

		if (!showFinalPathOnly)
			node->renderPathPointer(1.0f, 0.0f, 0.0f, false);
		else
			renderFinalPath(goalNode);
	}
}

void NavMesh::renderFinalPath(NavMeshNode *node)
{
	if (!node)
		return;

	node->renderPathPointer(0.0f, 1.0f, 0.0f, true);

	// recursion
	renderFinalPath(node->getPathPointer());
}