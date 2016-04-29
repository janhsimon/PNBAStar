#pragma once

#include <wx/wx.h>

#include "ITool.hpp"
#include "NavMesh.hpp"

class SideBar : public wxPanel
{
public:
	SideBar(wxWindow *parent, NavMesh *navMesh);

	inline ITool *getSelectedTool() const { return selectedTool; }

private:
	NavMesh *navMesh;
	ITool *selectedTool;

	void radioBoxEvent(wxCommandEvent &event);

	DECLARE_EVENT_TABLE()
};