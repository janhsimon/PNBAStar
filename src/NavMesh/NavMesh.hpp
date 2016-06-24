#pragma once

#include <vector>

#include <wx/wx.h>

#include "NavMeshNode.hpp"

class NavMesh
{
	public:
		NavMesh();
		~NavMesh();

		inline std::vector<NavMeshNode*> *getNodes() { return &nodes; }

		void addNode(NavMeshNode *node);
		void deleteNode(NavMeshNode *node);
		void deleteSelectedNode();
		void deleteNodeConnections(NavMeshNode *node);

		inline NavMeshNode *getSelectedNode() const { return selectedNode; }
		inline void setSelectedNode(NavMeshNode *selectedNode) { this->selectedNode = selectedNode; }
		void selectNodeClosestTo(const wxPoint &point);

		inline NavMeshNode *getStartNode() const { return startNode; }
		inline void setStartNode(NavMeshNode *startNode) { this->startNode = startNode; }

		inline NavMeshNode *getGoalNode() const { return goalNode; }
		inline void setGoalNode(NavMeshNode *goalNode) { this->goalNode = goalNode; }

		inline void setShowFinalPathOnly(bool showFinalPathOnly) { this->showFinalPathOnly = showFinalPathOnly; }

		void moveNode(NavMeshNode *node, const wxPoint &point);

		void connectNodes(NavMeshNode *a, NavMeshNode *b);

		float calculateDistanceBetweenNodes(NavMeshNode *a, NavMeshNode *b) const;

		void resetPathPointers();

		void render();

		void dumpToLog() const;

	private:
		static const float SELECTION_RADIUS;

		std::vector<NavMeshNode*> nodes;
		NavMeshNode *selectedNode, *startNode, *goalNode;
		bool showFinalPathOnly;

		void renderFinalPath(NavMeshNode *node);
};