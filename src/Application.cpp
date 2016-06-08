#include "Application.hpp"

BEGIN_EVENT_TABLE(Application, wxApp)
	EVT_IDLE(Application::idleEvent)
END_EVENT_TABLE()

bool Application::OnInit()
{
	mainFrame = new MainFrame("PNBA* -- written by Jan Simon", wxDefaultPosition, wxDefaultSize);
	
	layout = new wxBoxSizer(wxHORIZONTAL);
	mainFrame->SetSizer(layout);

	sideBar = new SideBar(mainFrame);

	assert(sideBar);
	assert(sideBar->getNavMesh());
	drawPane = new DrawPane(mainFrame, sideBar, sideBar->getNavMesh());

	layout->Add(drawPane, 1, wxEXPAND | wxALL, 10);
	layout->Add(sideBar, 0, wxEXPAND, 0);

	mainFrame->SetMinSize(wxSize(800, 600));
	mainFrame->Maximize();
	mainFrame->Show();

	return true;
}

void Application::idleEvent(wxIdleEvent &event)
{
	drawPane->render();
	event.RequestMore(); // keep rendering
}

wxIMPLEMENT_APP(Application);