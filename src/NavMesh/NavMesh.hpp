#pragma once

#include <vector>

#include <wx\wx.h>

#include "NavMeshNode.hpp"

class NavMesh
{
	public:
		NavMesh();

		void addNode(NavMeshNode *node);
		void deleteNode(NavMeshNode *node);
		void deleteSelectedNode();

		inline NavMeshNode *getSelectedNode() const { return selectedNode; }
		inline void setSelectedNode(NavMeshNode *selectedNode) { this->selectedNode = selectedNode; }
		void selectNodeClosestTo(const wxPoint &point);

		inline NavMeshNode *getStartNode() const { return startNode; }
		inline void setStartNode(NavMeshNode *startNode) { this->startNode = startNode; }

		inline NavMeshNode *getGoalNode() const { return goalNode; }
		inline void setGoalNode(NavMeshNode *goalNode) { this->goalNode = goalNode; }

		void moveNode(NavMeshNode *node, const wxPoint &point);

		void connectNodes(NavMeshNode *a, NavMeshNode *b);

		void render();

	private:
		static const float SELECTION_RADIUS;

		std::vector<NavMeshNode*> nodes;
		NavMeshNode *selectedNode, *startNode, *goalNode;
};