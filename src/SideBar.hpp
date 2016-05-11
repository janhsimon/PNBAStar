#pragma once

#include <wx/wx.h>

#include "NavMesh/NavMesh.hpp"
#include "Pathfinder/IPathfinder.hpp"
#include "Tool/ITool.hpp"

class SideBar : public wxPanel
{
public:
	SideBar(wxWindow *parent);
	~SideBar();

	inline NavMesh *getNavMesh() const { return navMesh; }
	inline ITool *getSelectedTool() const { return selectedTool; }
	inline IPathfinder *getSelectedPathfinder() const { return selectedPathfinder; }

private:
	NavMesh *navMesh;
	ITool *selectedTool;
	IPathfinder *selectedPathfinder;

	wxBoxSizer *layout;
	wxRadioBox *toolRadioBox, *pathfinderRadioBox, *pathDisplayRadioBox;
	wxPanel *buttonPanel;
	wxBoxSizer *buttonPanelLayout;
	wxButton *findPathButton, *resetPathButton;

	void toolRadioBoxEvent(wxCommandEvent &event);
	void pathfinderRadioBoxEvent(wxCommandEvent &event);
	void pathDisplayRadioBoxEvent(wxCommandEvent &event);
	void findPathButtonEvent(wxCommandEvent&);
	void resetPathButtonEvent(wxCommandEvent&);

	DECLARE_EVENT_TABLE()
};