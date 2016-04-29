#pragma once

#include <wx/wx.h>

#include "DrawPane.hpp"
#include "MainFrame.hpp"
#include "NavMesh.hpp"
#include "SideBar.hpp"

class Application : public wxApp
{
public:
	bool OnInit();
	void idleEvent(wxIdleEvent &event);

private:
	NavMesh *navMesh;
	MainFrame *mainFrame;
	DrawPane *drawPane;
	SideBar *sideBar;

	DECLARE_EVENT_TABLE()
};