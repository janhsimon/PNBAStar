#pragma once

#include <wx/wx.h>

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size);
	~MainFrame();

private:
	wxMenu *menuFile, *menuHelp;
	wxMenuBar *menuBar;

	void OnHello(wxCommandEvent &event);
	void OnExit(wxCommandEvent &event);
	void OnAbout(wxCommandEvent &event);

	wxDECLARE_EVENT_TABLE();
};