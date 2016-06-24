#include <cassert>

#include "tbb\mutex.h"

#include "Console.hpp"

Console::Console(wxWindow *parent) : wxPanel(parent)
{
	SetMinSize(wxSize(200, 200));

	layout = new wxBoxSizer(wxVERTICAL);
	SetSizer(layout);

	textControl = new wxTextCtrl(this, 0, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxHSCROLL | wxTE_READONLY);
	layout->Add(textControl, 1, wxEXPAND | wxALL, 10);

	logTextControl = new wxLogTextCtrl(textControl);
	wxLog::SetActiveTarget(logTextControl);
}

Console::~Console()
{
	
}