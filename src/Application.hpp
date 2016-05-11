#pragma once

#include <wx/wx.h>

#include "DrawPane.hpp"
#include "MainFrame.hpp"
#include "SideBar.hpp"

class Application : public wxApp
{
public:
	bool OnInit();
	void idleEvent(wxIdleEvent &event);

private:
	MainFrame *mainFrame;
	SideBar *sideBar;
	DrawPane *drawPane;

	wxBoxSizer *layout;

	DECLARE_EVENT_TABLE()
};