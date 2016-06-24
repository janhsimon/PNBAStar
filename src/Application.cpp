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

	wxPanel *leftPanel = new wxPanel(mainFrame);
	wxBoxSizer *leftPanelLayout = new wxBoxSizer(wxVERTICAL);
	leftPanel->SetSizer(leftPanelLayout);

	assert(sideBar);
	assert(sideBar->getNavMesh());
	drawPane = new DrawPane(leftPanel, sideBar, sideBar->getNavMesh());
	leftPanelLayout->Add(drawPane, 1, wxEXPAND | wxALL, 10);

	console = new Console(leftPanel);
	leftPanelLayout->Add(console, 0, wxEXPAND, 0);

	layout->Add(leftPanel, 1, wxEXPAND | wxALL, 0);
	layout->Add(sideBar, 0, wxEXPAND, 0);

	mainFrame->SetMinSize(wxSize(800, 600));
	mainFrame->Maximize();
	mainFrame->Show();

	wxLogMessage("Console running...");

	return true;
}

void Application::idleEvent(wxIdleEvent &event)
{
	drawPane->render();
	event.RequestMore(); // keep rendering
}

wxIMPLEMENT_APP(Application);