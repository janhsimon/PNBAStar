#include <wx/glcanvas.h>

#include "NavMeshNode.hpp"

const float NavMeshNode::ICON_SIZE = 10.0f;

NavMeshNode::NavMeshNode(const wxPoint &point)
{
	setPosition(point.x, point.y);
}

void NavMeshNode::render(float r, float g, float b) const
{
	glColor3f(r, g, b);

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x,				y - ICON_SIZE);
	glVertex2f(x - ICON_SIZE,	y);
	glVertex2f(x,				y + ICON_SIZE);
	glVertex2f(x + ICON_SIZE,	y);
	glEnd();

	glColor3f(0.0f, 0.0f, 1.0f);

	for (NavMeshNode *adjacentNode : adjacentNodes)
	{
		glBegin(GL_LINES);
		glVertex2f(x, y);
		glVertex2f(adjacentNode->getX(), adjacentNode->getY());
		glEnd();
	}
}