#pragma once

#include <wx/wx.h>

class Console : public wxPanel
{
public:
	Console(wxWindow *parent);
	~Console();

private:
	wxBoxSizer *layout;
	wxTextCtrl *textControl;
	wxLogTextCtrl *logTextControl;
};