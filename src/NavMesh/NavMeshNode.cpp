#include <sstream>

#include <wx/glcanvas.h>

#include "NavMeshNode.hpp"

const float NavMeshNode::ICON_SIZE = 12.0f;
const float NavMeshNode::CONNECTION_WIDTH = 1.5f;
const float NavMeshNode::PATH_POINTER_WIDTH = 2.5f;
const float NavMeshNode::PATH_POINTER_ARROW_LENGTH = 12.0f;

NavMeshNode::NavMeshNode(const wxPoint &point)
{
	setPosition(point.x, point.y);
	fCost1 = fCost2 = gCost1 = gCost2 = hCost1 = hCost2 = 0.0f;
	pathPointer = nullptr;
	lineEnable = false;
}

void NavMeshNode::removeAdjacentNode(NavMeshNode *node)
{
	for (std::vector<NavMeshNode*>::iterator i = adjacentNodes.begin(); i != adjacentNodes.end();)
	{
		if (*i == node)
			i = adjacentNodes.erase(i);
		else
			++i;
	}
}

void NavMeshNode::renderLineTo(float x, float y, float r, float g, float b) const
{
	glColor3f(r, g, b);
	glLineWidth(CONNECTION_WIDTH);

	glBegin(GL_LINES);
	glVertex2f(this->x, this->y);
	glVertex2f(x, y);
	glEnd();

	glLineWidth(1.0f);
}

void NavMeshNode::renderLine(float r, float g, float b) const
{
	if (!lineEnable)
		return;

	renderLineTo(lineX, lineY, r, g, b);
}

void NavMeshNode::renderConnections(float r, float g, float b) const
{
	for (NavMeshNode *adjacentNode : adjacentNodes)
		renderLineTo(adjacentNode->getX(), adjacentNode->getY(), r, g, b);
}

void NavMeshNode::renderNormalNode(float r, float g, float b) const
{
	glColor3f(r, g, b);

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x,				y - ICON_SIZE);
	glVertex2f(x - ICON_SIZE,	y);
	glVertex2f(x,				y + ICON_SIZE);
	glVertex2f(x + ICON_SIZE,	y);
	glEnd();
}

void NavMeshNode::renderStartNode(float r, float g, float b) const
{
	glColor3f(r, g, b);

	glBegin(GL_LINE_LOOP);
	glVertex2f(x,				y - ICON_SIZE);
	glVertex2f(x - ICON_SIZE,	y);
	glVertex2f(x,				y + ICON_SIZE);
	glVertex2f(x + ICON_SIZE,	y);
	glEnd();
}

void NavMeshNode::renderGoalNode(float r, float g, float b) const
{
	renderNormalNode(r, g, b);

	glColor3f(r, g, b);

	float lineSize = ICON_SIZE + ICON_SIZE / 3.0f;

	glBegin(GL_LINE_LOOP);
	glVertex2f(x,				y - lineSize);
	glVertex2f(x - lineSize,	y);
	glVertex2f(x,				y + lineSize);
	glVertex2f(x + lineSize,	y);
	glEnd();
}

void NavMeshNode::renderPathPointer(float r, float g, float b, bool invert) const
{
	if (!pathPointer)
		return;

	float vecX = pathPointer->getX() - x;
	float vecY = pathPointer->getY() - y;

	if (invert)
	{
		vecX = -vecX;
		vecY = -vecY;
	}

	float vecLength = sqrtf(vecX * vecX + vecY * vecY);
	vecX /= vecLength;
	vecY /= vecLength;

	float fromX = x + vecX * vecLength - vecX * PATH_POINTER_ARROW_LENGTH;
	float fromY = y + vecY * vecLength - vecY * PATH_POINTER_ARROW_LENGTH;

	if (invert)
	{
		fromX = pathPointer->getX() + vecX * vecLength - vecX * PATH_POINTER_ARROW_LENGTH;
		fromY = pathPointer->getY() + vecY * vecLength - vecY * PATH_POINTER_ARROW_LENGTH;
	}

	glColor3f(r, g, b);
	glLineWidth(PATH_POINTER_WIDTH);

	glBegin(GL_LINES);

	if (!invert)
		glVertex2f(x, y);
	else
		glVertex2f(pathPointer->getX(), pathPointer->getY());

	glVertex2f(fromX, fromY);

	glVertex2f(fromX, fromY);
	glVertex2f(fromX - vecX * PATH_POINTER_ARROW_LENGTH - vecY * PATH_POINTER_ARROW_LENGTH, fromY - vecY * PATH_POINTER_ARROW_LENGTH + vecX * PATH_POINTER_ARROW_LENGTH);

	glVertex2f(fromX, fromY);
	glVertex2f(fromX - vecX * PATH_POINTER_ARROW_LENGTH + vecY * PATH_POINTER_ARROW_LENGTH, fromY - vecY * PATH_POINTER_ARROW_LENGTH - vecX * PATH_POINTER_ARROW_LENGTH);
	
	glEnd();

	glLineWidth(1.0f);
}

void NavMeshNode::dumpToStringStream(std::stringstream &s) const
{
	s << "Node #" << id << " @ " << x << "/" << y << std::endl;
	s << "\tAdjacent nodes: ";

	for (NavMeshNode *adjacentNode : adjacentNodes)
		s << ((adjacentNode == adjacentNodes[0]) ? "" : ", ") << adjacentNode->getID();

	s << std::endl << "\tAssociated costs:" << std::endl;
	s << "\t\tF cost (total):\t\t" << fCost1 << "\t\t" << fCost2 << std::endl;
	s << "\t\tG cost (backwards):\t\t" << gCost1 << "\t\t" << gCost2 << std::endl;
	s << "\t\tH cost (forward, heuristic):\t" << hCost1 << "\t\t" << hCost2 << std::endl;
}