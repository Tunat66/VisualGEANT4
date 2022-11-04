#include "RunPanel.h"

wxBEGIN_EVENT_TABLE(RunPanel, wxWindow)
	EVT_BUTTON(10001, ApplyChanges) //link button with ID 10001 to method OnButtonClicked
	EVT_BUTTON(10003, RunProject)
	EVT_BUTTON(10004, ConfigureProject)
wxEND_EVENT_TABLE()


RunPanel::RunPanel(wxFrame* MainFrame) : wxPanel(MainFrame, wxID_ANY, wxDefaultPosition,  wxDefaultSize, 0)
{
	RightPanelSizer = new wxBoxSizer(wxVERTICAL);
	button_RunProject = new wxButton(this, 10003, "Run Project", wxDefaultPosition, wxSize(150, 50));
	button_ApplyChanges = new wxButton(this, 10001, "Apply Changes", wxDefaultPosition, wxSize(150, 50));
	button_ConfigureProject = new wxButton(this, 10004, "Configure Project", wxDefaultPosition, wxSize(150, 50));
	RightPanelSizer->Add(button_RunProject, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 20);
	RightPanelSizer->Add(button_ApplyChanges, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 20);
	RightPanelSizer->Add(button_ConfigureProject, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 20);
	this->SetSizer(RightPanelSizer);
	
	//now attach itself to the right panel
	//wxBoxSizer* Attach = new wxBoxSizer(wxHORIZONTAL);
	//Attach->Add(RightPanel, 0, wxEXPAND | wxALL, 0);
}



void RunPanel::RunProject(wxCommandEvent& event)
{
	if (SystemManager.Project_isOpen)
	{
		SystemManager.Kernel_args.push_back("open");
		SystemManager.Conclude();
		//SetStatusText(SystemManager.Kernel_args.at(1)); possible bug source
		//SetStatusText("Project Running...");
	}
	else
		SetStatusText("Please Select a Project before running.");
}


void RunPanel::ApplyChanges(wxCommandEvent& event)
{
	SetStatusText("Compiling Project...");
	//do stuff
}

void RunPanel::ConfigureProject(wxCommandEvent& event)
{
	ConfigureWindow* NewInstance = new ConfigureWindow();
	NewInstance->Show();
}

void RunPanel::SetStatusText(std::string Value)
{
	StatusText = Value;
}

std::string RunPanel::GetStatusText()
{
	return StatusText;
}

RunPanel::~RunPanel()
{
	this->SetSizer(NULL);
	button_RunProject->Destroy();
	button_ApplyChanges->Destroy();
	button_ConfigureProject->Destroy();
}