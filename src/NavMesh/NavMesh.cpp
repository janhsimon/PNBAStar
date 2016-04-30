#include <wx/glcanvas.h>

#include "NavMesh.hpp"

const float NavMesh::SELECTION_RADIUS = 25.0f;

NavMesh::NavMesh()
{
	selectedNode = startNode = goalNode = nullptr;
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
			std::vector<NavMeshNode*> *adjacentNodes = (*i)->getAdjacentNodes();
			assert(adjacentNodes);

			for (std::vector<NavMeshNode*>::iterator j = adjacentNodes->begin(); j != adjacentNodes->end();)
			{
				if (*j == node)
					j = adjacentNodes->erase(j);
				else
					++j;
			}

			++i;
		}
	}
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
	{
		if (node == selectedNode)
			node->render(1.0f, 0.0f, 1.0f);
		else if (node == startNode)
			node->render(0.0f, 1.0f, 0.0f);
		else if (node == goalNode)
			node->render(1.0f, 0.0f, 0.0f);
		else
			node->render(0.0f, 0.0, 1.0f);
	}
}