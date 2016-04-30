#include <wx/glcanvas.h>

#include "NavMeshNode.hpp"

const float NavMeshNode::ICON_SIZE = 12.0f;
const float NavMeshNode::PATH_POINTER_WIDTH = 2.5f;
const float NavMeshNode::PATH_POINTER_ARROW_LENGTH = 12.0f;

NavMeshNode::NavMeshNode(const wxPoint &point)
{
	setPosition(point.x, point.y);
	
	forwardCost = backwardsCost = totalCost = 0.0f;
	pathPointer = nullptr;
}

void NavMeshNode::renderConnections(float r, float g, float b) const
{
	glColor3f(r, g, b);

	for (NavMeshNode *adjacentNode : adjacentNodes)
	{
		glBegin(GL_LINES);
		glVertex2f(x, y);
		glVertex2f(adjacentNode->getX(), adjacentNode->getY());
		glEnd();
	}
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

void NavMeshNode::renderPathPointer(float r, float g, float b) const
{
	if (!pathPointer)
		return;

	float vecX = pathPointer->getX() - x;
	float vecY = pathPointer->getY() - y;

	float vecLength = sqrtf(vecX * vecX + vecY * vecY);
	vecX /= vecLength;
	vecY /= vecLength;

	float fromX = x + vecX * vecLength - vecX * PATH_POINTER_ARROW_LENGTH;
	float fromY = y + vecY * vecLength - vecY * PATH_POINTER_ARROW_LENGTH;

	glColor3f(r, g, b);
	glLineWidth(PATH_POINTER_WIDTH);

	glBegin(GL_LINES);

	glVertex2f(x, y);
	glVertex2f(fromX, fromY);

	glVertex2f(fromX, fromY);
	glVertex2f(fromX - vecX * PATH_POINTER_ARROW_LENGTH - vecY * PATH_POINTER_ARROW_LENGTH, fromY - vecY * PATH_POINTER_ARROW_LENGTH + vecX * PATH_POINTER_ARROW_LENGTH);

	glVertex2f(fromX, fromY);
	glVertex2f(fromX - vecX * PATH_POINTER_ARROW_LENGTH + vecY * PATH_POINTER_ARROW_LENGTH, fromY - vecY * PATH_POINTER_ARROW_LENGTH - vecX * PATH_POINTER_ARROW_LENGTH);
	
	glEnd();

	glLineWidth(1.0f);
}