/*
#define TBB_USE_DEBUG 1
#include <tbb/tbb.h>

#include <iostream>

using namespace tbb;
using namespace std;

class first_task : public task
{
public:
	task *execute()
	{
		cout << "Hello World!\n";
		char anykey;
		cin >> anykey;
		return NULL;
	}
};

int main()
{
	task_scheduler_init init(task_scheduler_init::automatic);
	first_task& f1 = *new(tbb::task::allocate_root()) first_task();
	tbb::task::spawn_root_and_wait(f1);
}
*/

#include "Application.hpp"

BEGIN_EVENT_TABLE(Application, wxApp)
	EVT_IDLE(Application::idleEvent)
END_EVENT_TABLE()

bool Application::OnInit()
{
	mainFrame = new MainFrame("PNBA* -- written by Jan Simon", wxDefaultPosition, wxDefaultSize);
	
	layout = new wxBoxSizer(wxHORIZONTAL);
	mainFrame->SetSizer(layout);

	sideBar = new SideBar(mainFrame);

	assert(sideBar);
	assert(sideBar->getNavMesh());
	drawPane = new DrawPane(mainFrame, sideBar, sideBar->getNavMesh());

	layout->Add(drawPane, 1, wxEXPAND | wxALL, 10);
	layout->Add(sideBar, 0, wxEXPAND, 0);

	mainFrame->Maximize();
	mainFrame->Show();

	return true;
}

void Application::idleEvent(wxIdleEvent &event)
{
	drawPane->render();
	event.RequestMore(); // keep rendering
}

wxIMPLEMENT_APP(Application);