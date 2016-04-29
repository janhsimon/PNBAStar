#include "AddNodeTool.hpp"
#include "ConnectNodeTool.hpp"
#include "MoveNodeTool.hpp"
#include "SideBar.hpp"

BEGIN_EVENT_TABLE(SideBar, wxPanel)
	EVT_RADIOBOX(0, SideBar::radioBoxEvent)
END_EVENT_TABLE()

SideBar::SideBar(wxWindow *parent, NavMesh *navMesh) : wxPanel(parent)
{
	SetMinSize(wxSize(250, 0));

	wxBoxSizer *layout = new wxBoxSizer(wxVERTICAL);
	SetSizer(layout);
	
	wxArrayString choices;
	choices.Add("Add Nodes");
	choices.Add("Move Nodes");
	choices.Add("Connect Nodes");
	wxRadioBox *radioBox = new wxRadioBox(this, 0, "Tools:", wxDefaultPosition, wxDefaultSize, choices, 1, wxRA_SPECIFY_COLS);
	layout->Add(radioBox, 0, wxEXPAND | wxALL, 10);

	assert(navMesh);
	this->navMesh = navMesh;

	selectedTool = new AddNodeTool(navMesh);
}

void SideBar::radioBoxEvent(wxCommandEvent &event)
{
	int selection = event.GetSelection();

	if (selection == 0)
	{
		if (selectedTool)
			delete selectedTool;

		assert(navMesh);
		selectedTool = new AddNodeTool(navMesh);
	}
	else if (selection == 1)
	{
		if (selectedTool)
			delete selectedTool;

		assert(navMesh);
		selectedTool = new MoveNodeTool(navMesh);
	}
	else if (selection == 2)
	{
		if (selectedTool)
			delete selectedTool;

		assert(navMesh);
		selectedTool = new ConnectNodeTool(navMesh);
	}
}