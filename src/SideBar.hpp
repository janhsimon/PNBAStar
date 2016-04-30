#pragma once

#include <wx/wx.h>

#include "NavMesh/NavMesh.hpp"
#include "Tool/ITool.hpp"

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