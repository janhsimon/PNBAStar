#include <cassert>

#include "DrawPane.hpp"

BEGIN_EVENT_TABLE(DrawPane, wxGLCanvas)
	EVT_PAINT(DrawPane::paintEvent)
	EVT_LEFT_DOWN(DrawPane::leftMouseButtonDownEvent)
	EVT_LEFT_UP(DrawPane::leftMouseButtonUpEvent)
	EVT_RIGHT_DOWN(DrawPane::rightMouseButtonDownEvent)
	EVT_RIGHT_UP(DrawPane::rightMouseButtonUpEvent)
	EVT_MOTION(DrawPane::mouseMotionEvent)
END_EVENT_TABLE()

DrawPane::DrawPane(wxWindow *parent, SideBar *sideBar, NavMesh *navMesh) : wxGLCanvas(parent)
{
	glContext = new wxGLContext(this);

	assert(navMesh);
	this->navMesh = navMesh;
	
	assert(sideBar);
	this->sideBar = sideBar;
}

void DrawPane::paintEvent(wxPaintEvent&)
{
	render();
}

void DrawPane::leftMouseButtonDownEvent(wxMouseEvent &event)
{
	assert(sideBar);
	ITool *selectedTool = sideBar->getSelectedTool();
	assert(selectedTool);
	selectedTool->leftMouseButtonDownEvent(event);
}

void DrawPane::leftMouseButtonUpEvent(wxMouseEvent &event)
{
	assert(sideBar);
	ITool *selectedTool = sideBar->getSelectedTool();
	assert(selectedTool);
	selectedTool->leftMouseButtonUpEvent(event);
}

void DrawPane::rightMouseButtonDownEvent(wxMouseEvent &event)
{
	assert(sideBar);
	ITool *selectedTool = sideBar->getSelectedTool();
	assert(selectedTool);
	selectedTool->rightMouseButtonDownEvent(event);
}

void DrawPane::rightMouseButtonUpEvent(wxMouseEvent &event)
{
	assert(sideBar);
	ITool *selectedTool = sideBar->getSelectedTool();
	assert(selectedTool);
	selectedTool->rightMouseButtonUpEvent(event);
}

void DrawPane::mouseMotionEvent(wxMouseEvent &event)
{
	assert(sideBar);
	ITool *selectedTool = sideBar->getSelectedTool();
	assert(selectedTool);
	selectedTool->mouseMotionEvent(event);
}

void DrawPane::render()
{
	SetCurrent(*glContext);

	GLint w = GetSize().x;
	GLint h = GetSize().y;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, h, 0, 0, 1);
	glViewport(0, 0, w, h);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	assert(navMesh);
	navMesh->render();

	glFlush();
	SwapBuffers();
}