#pragma once
#include "wx/wx.h"
#include "SystemVariables.h"
#include "ConfigureWindow.h"
#include "GLGeometryViewer.h"
#include <windows.h>
//#include "MainWindow.h"
//this object is panel which attaches itself to the right panel

class MainWindow;

class RunPanel : public wxPanel
{
public:
	RunPanel(wxFrame* MainFrame, GLGeometryViewer* GeometryViewer);
	~RunPanel();

	GLGeometryViewer* ViewerAccess;

	//to access the backend, and some aliases for clean code made with address matching, possible bugs with assignment
	SystemVariables SystemManager;
	//std::vector<std::string>& Kernel_args = SystemManager.Kernel_args;
	//bool& Project_isOpen = SystemManager.Project_isOpen;
	//std::string& CurrentProjectDir = SystemManager.CurrentProjectDir;

	//just compiles/builds the project
	wxButton* button_ApplyChanges = nullptr;
	void ApplyChanges(wxCommandEvent& event);
	//runs the project executable
	wxButton* button_RunProject = nullptr;
	void RunProject(wxCommandEvent& event);
	//configures the project
	wxButton* button_ConfigureProject = nullptr;
	void ConfigureProject(wxCommandEvent& event);

	//sizer stuff
	wxBoxSizer* RightPanelSizer = new wxBoxSizer(wxVERTICAL);


	//for the status text, I haven't figured out a way to connect it to the status bar
private:
	std::string StatusText = "";
	void SetStatusText(std::string Value);
	std::string GetStatusText();


	//for events
	wxDECLARE_EVENT_TABLE();

};

