#include "SideBar.hpp"
#include "Tool/ConnectNodeTool.hpp"
#include "Tool/EditNodeTool.hpp"
#include "Tool/SetStartGoalTool.hpp"

BEGIN_EVENT_TABLE(SideBar, wxPanel)
	EVT_RADIOBOX(0, SideBar::radioBoxEvent)
END_EVENT_TABLE()

SideBar::SideBar(wxWindow *parent, NavMesh *navMesh) : wxPanel(parent)
{
	SetMinSize(wxSize(150, 0));

	wxBoxSizer *layout = new wxBoxSizer(wxVERTICAL);
	SetSizer(layout);
	
	wxArrayString choices;
	choices.Add("Nodes");
	choices.Add("Connections");
	choices.Add("Set Start/Goal");
	wxRadioBox *radioBox = new wxRadioBox(this, 0, "Edit:", wxDefaultPosition, wxDefaultSize, choices, 1, wxRA_SPECIFY_COLS);
	layout->Add(radioBox, 0, wxEXPAND | wxALL, 10);

	assert(navMesh);
	this->navMesh = navMesh;

	selectedTool = new EditNodeTool(navMesh);
}

void SideBar::radioBoxEvent(wxCommandEvent &event)
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