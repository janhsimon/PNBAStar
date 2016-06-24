#pragma once

#include <wx/wx.h>

#include "Console.hpp"
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
	Console *console;

	wxBoxSizer *layout;

	DECLARE_EVENT_TABLE()
};