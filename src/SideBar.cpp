#include "SideBar.hpp"
#include "Pathfinder/AStarPathfinder.hpp"
#include "Tool/ConnectNodeTool.hpp"
#include "Tool/EditNodeTool.hpp"
#include "Tool/SetStartGoalTool.hpp"

BEGIN_EVENT_TABLE(SideBar, wxPanel)
	EVT_RADIOBOX(0, SideBar::toolRadioBoxEvent)
	EVT_RADIOBOX(1, SideBar::pathfinderRadioBoxEvent)
	EVT_RADIOBOX(2, SideBar::pathDisplayRadioBoxEvent)
	EVT_BUTTON(0, SideBar::findPathButtonEvent)
	EVT_BUTTON(1, SideBar::resetPathButtonEvent)
END_EVENT_TABLE()

SideBar::SideBar(wxWindow *parent) : wxPanel(parent)
{
	SetMinSize(wxSize(150, 0));

	wxBoxSizer *layout = new wxBoxSizer(wxVERTICAL);
	SetSizer(layout);
	
	wxArrayString toolNames;
	toolNames.Add("Nodes");
	toolNames.Add("Connections");
	toolNames.Add("Start/Goal");
	wxRadioBox *toolRadioBox = new wxRadioBox(this, 0, "Edit:", wxDefaultPosition, wxDefaultSize, toolNames, 1, wxRA_SPECIFY_COLS);
	layout->Add(toolRadioBox, 0, wxEXPAND | wxALL, 10);

	wxArrayString pathfinderNames;
	pathfinderNames.Add("A* (serial)");
	pathfinderNames.Add("PNBA* (parallel)");
	wxRadioBox *pathfinderRadioBox = new wxRadioBox(this, 1, "Pathfinder:", wxDefaultPosition, wxDefaultSize, pathfinderNames, 1, wxRA_SPECIFY_COLS);
	layout->Add(pathfinderRadioBox, 0, wxEXPAND | wxALL, 10);

	wxArrayString pathDisplayNames;
	pathDisplayNames.Add("Final path only");
	pathDisplayNames.Add("All path pointers");
	wxRadioBox *pathDisplayRadioBox = new wxRadioBox(this, 2, "Show:", wxDefaultPosition, wxDefaultSize, pathDisplayNames, 1, wxRA_SPECIFY_COLS);
	layout->Add(pathDisplayRadioBox, 0, wxEXPAND | wxALL, 10);

	wxPanel *buttonPanel = new wxPanel(this);
	wxBoxSizer *buttonPanelLayout = new wxBoxSizer(wxVERTICAL);
	buttonPanel->SetSizer(buttonPanelLayout);
	layout->Add(buttonPanel, 0, wxEXPAND | wxALL, 10);

	wxButton *findPathButton = new wxButton(buttonPanel, 0, "Find Path");
	buttonPanelLayout->Add(findPathButton, 0, wxEXPAND);

	wxButton *resetPathButton = new wxButton(buttonPanel, 1, "Reset Path");
	buttonPanelLayout->Add(resetPathButton, 0, wxEXPAND);

	navMesh = new NavMesh();

	assert(navMesh);
	selectedTool = new EditNodeTool(navMesh);
	selectedPathfinder = new AStarPathfinder(navMesh);
}

void SideBar::toolRadioBoxEvent(wxCommandEvent &event)
{
	int selection = event.GetSelection();

	if (selection == 0)
	{
		if (selectedTool)
			delete selectedTool;

		assert(navMesh);
		selectedTool = new EditNodeTool(navMesh);
	}
	else if (selection == 1)
	{
		if (selectedTool)
			delete selectedTool;

		assert(navMesh);
		selectedTool = new ConnectNodeTool(navMesh);
	}
	else if (selection == 2)
	{
		if (selectedTool)
			delete selectedTool;

		assert(navMesh);
		selectedTool = new SetStartGoalTool(navMesh);
	}
}

void SideBar::pathfinderRadioBoxEvent(wxCommandEvent &event)
{
	int selection = event.GetSelection();

	if (selection == 0)
	{
		if (selectedPathfinder)
			delete selectedPathfinder;

		assert(navMesh);
		selectedPathfinder = new AStarPathfinder(navMesh);
	}
}

void SideBar::pathDisplayRadioBoxEvent(wxCommandEvent &event)
{
	assert(navMesh);
	navMesh->setShowFinalPathOnly(event.GetSelection() == 0);
}

void SideBar::findPathButtonEvent(wxCommandEvent&)
{
	assert(selectedPathfinder);
	selectedPathfinder->findPath();
}

void SideBar::resetPathButtonEvent(wxCommandEvent&)
{
	assert(navMesh);
	navMesh->resetPathPointers();
}