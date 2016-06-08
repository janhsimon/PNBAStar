#include "SideBar.hpp"
#include "Pathfinder/AStarPathfinder.hpp"
#include "Pathfinder/PNBAStarPathfinder.hpp"
#include "Tool/ConnectNodeTool.hpp"
#include "Tool/EditNodeTool.hpp"
#include "Tool/SetStartGoalTool.hpp"

BEGIN_EVENT_TABLE(SideBar, wxPanel)
	EVT_RADIOBOX(0, SideBar::toolRadioBoxEvent)
	EVT_RADIOBOX(1, SideBar::pathfinderRadioBoxEvent)
	EVT_RADIOBOX(2, SideBar::pathDisplayRadioBoxEvent)
	EVT_BUTTON(0, SideBar::calculateStepButtonEvent)
	EVT_BUTTON(1, SideBar::calculatePathButtonEvent)
	EVT_BUTTON(2, SideBar::resetPathButtonEvent)
END_EVENT_TABLE()

SideBar::SideBar(wxWindow *parent) : wxPanel(parent)
{
	SetMinSize(wxSize(150, 0));

	layout = new wxBoxSizer(wxVERTICAL);
	SetSizer(layout);
	
	wxArrayString toolNames;
	toolNames.Add("Nodes");
	toolNames.Add("Connections");
	toolNames.Add("Start/Goal");
	toolRadioBox = new wxRadioBox(this, 0, "Edit:", wxDefaultPosition, wxDefaultSize, toolNames, 1, wxRA_SPECIFY_COLS);
	layout->Add(toolRadioBox, 0, wxEXPAND | wxALL, 10);

	wxArrayString pathfinderNames;
	pathfinderNames.Add("A* (serial)");
	pathfinderNames.Add("PNBA* (parallel)");
	pathfinderRadioBox = new wxRadioBox(this, 1, "Pathfinder:", wxDefaultPosition, wxDefaultSize, pathfinderNames, 1, wxRA_SPECIFY_COLS);
	layout->Add(pathfinderRadioBox, 0, wxEXPAND | wxALL, 10);

	wxArrayString pathDisplayNames;
	pathDisplayNames.Add("All path pointers");
	pathDisplayNames.Add("Final path only");
	pathDisplayRadioBox = new wxRadioBox(this, 2, "Show:", wxDefaultPosition, wxDefaultSize, pathDisplayNames, 1, wxRA_SPECIFY_COLS);
	layout->Add(pathDisplayRadioBox, 0, wxEXPAND | wxALL, 10);

	buttonPanel = new wxPanel(this);
	buttonPanelLayout = new wxBoxSizer(wxVERTICAL);
	buttonPanel->SetSizer(buttonPanelLayout);
	layout->Add(buttonPanel, 0, wxEXPAND | wxALL, 10);

	pathfinderStateLabel = new wxStaticText(buttonPanel, 0, "");
	buttonPanelLayout->Add(pathfinderStateLabel, 0, wxEXPAND);

	calculateStepButton = new wxButton(buttonPanel, 0, ">");
	buttonPanelLayout->Add(calculateStepButton, 0, wxEXPAND);

	calculatePathButton = new wxButton(buttonPanel, 1, ">>");
	buttonPanelLayout->Add(calculatePathButton, 0, wxEXPAND);

	resetPathButton = new wxButton(buttonPanel, 2, "<<");
	buttonPanelLayout->Add(resetPathButton, 0, wxEXPAND);

	navMesh = new NavMesh;

	assert(navMesh);
	selectedTool = new EditNodeTool(navMesh);
	selectedPathfinder = new AStarPathfinder(navMesh);

	updatePathfinderControls();
}

SideBar::~SideBar()
{
	delete navMesh;
	delete selectedTool;
	delete selectedPathfinder;
}

void SideBar::updatePathfinderControls()
{
	assert(selectedPathfinder);
	assert(pathfinderStateLabel);
	pathfinderStateLabel->SetLabelText("State: " + selectedPathfinder->getStateString());

	assert(calculateStepButton);
	assert(calculatePathButton);
	assert(resetPathButton);
	if (selectedPathfinder->getState() == NotInitialized)
	{
		calculateStepButton->Enable(true);
		calculatePathButton->Enable(true);
		resetPathButton->Enable(false);
	}
	else if ( selectedPathfinder->getState() == Running)
	{
		calculateStepButton->Enable(true);
		calculatePathButton->Enable(true);
		resetPathButton->Enable(true);
	}
	else if (selectedPathfinder->getState() == Done || selectedPathfinder->getState() == Failed)
	{
		calculateStepButton->Enable(false);
		calculatePathButton->Enable(false);
		resetPathButton->Enable(true);
	}
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
	else if (selection == 1)
	{
		if (selectedPathfinder)
			delete selectedPathfinder;

		assert(navMesh);
		selectedPathfinder = new PNBAStarPathfinder(navMesh);
	}
}

void SideBar::pathDisplayRadioBoxEvent(wxCommandEvent &event)
{
	assert(navMesh);
	navMesh->setShowFinalPathOnly(event.GetSelection() == 1);
}

void SideBar::calculateStepButtonEvent(wxCommandEvent&)
{
	assert(selectedPathfinder);
	selectedPathfinder->calculateStep();
	updatePathfinderControls();
}

void SideBar::calculatePathButtonEvent(wxCommandEvent&)
{
	assert(selectedPathfinder);
	selectedPathfinder->calculatePath();
	updatePathfinderControls();
}

void SideBar::resetPathButtonEvent(wxCommandEvent&)
{
	assert(selectedPathfinder);
	selectedPathfinder->reset();
	updatePathfinderControls();
}