#pragma once
#include "wx/wx.h"
#include "wx/filepicker.h"
#include "wx/imagpng.h"
#include <string>
#include <vector>
#include "SystemVariables.h"
#include "GetCommand.h"

//right panels
#include "RunPanel.h"
#include "GeometryPanel.h"
#include "SourcePanel.h"

//left panels
#include "GLGeometryViewer.h"
//#include "G4VGraphicsSystem.hh"
//maybe later
//#include "G4OpenGLImmediateXViewer.hh"

//include any new windows here

//Misc:
#include "NewDirPicker.h"
class MainWindow : public wxFrame
{
public:
	MainWindow();
	~MainWindow();
	wxBoxSizer* MainWindowSizer = new wxBoxSizer(wxHORIZONTAL);// = new wxBoxSizer(wxHORIZONTAL);

//to access the backend, and some aliases for clean code made with address matching, possible bugs with assignment
	SystemVariables SystemManager;
	//std::vector<std::string>& Kernel_args = SystemManager.Kernel_args;
	//bool& Project_isOpen = SystemManager.Project_isOpen;
	//std::string& CurrentProjectDir = SystemManager.CurrentProjectDir;

//CONFIG SUBWINDOW DISPLAY
private:
	//this object to display the subwindow
	//MainWindow* m_frame2 = nullptr;
	
//MENU BAR METHODS	
private:
	//File Menu (in order)
	void OnNewProject(wxCommandEvent& event);
	void OnOpenProject(wxCommandEvent& event);
	void OnSave(wxCommandEvent& event);
	void OnSaveAs(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);

	//help menu
	void OnAbout(wxCommandEvent& event);

	//command line menu:
	void OnCommandLine(wxCommandEvent& event);

//PANELS
public:
	wxPanel* LeftPanel;
	wxBoxSizer* ViewerSizer = new wxBoxSizer(wxVERTICAL);
	wxPanel* RightPanel;
	
	//FOR RIGHT PANEL
	//use a template to modify the right panel
	//void ReallocateRightPanel(wxPanel* New);
	//the reallocation happens so fast that the user does not notice it happened
	template <typename RightPanelType>
	void ReallocatePanel()
	{
		RightPanel->Destroy();
		RightPanel = new RightPanelType(this);
		RightPanel->SetBackgroundColour(wxColor(205, 205, 205));
		MainWindowSizer->Add(RightPanel, 1, wxEXPAND | wxRIGHT | wxBOTTOM | wxTOP, 5);
		MainWindowSizer->Layout();
	}

	//FOR LEFT PANEL (Geometry Viewers)
	GLGeometryViewer* GeometryViewer;
	//G4OpenGLImmediateXViewer* SetupViewer;

//TOOLBAR METHODS
	void RunPanelShow(wxCommandEvent& event);
	void GeometryPanelShow(wxCommandEvent& event);
	void SourcePanelShow(wxCommandEvent& event);


	//conclusion method setting the final form of Kernel_args
	//void Conclude();
	
	//finally, a macro for event handling
	wxDECLARE_EVENT_TABLE();

};

enum { //assign ids to click events
	//assigning rules are as follows:
	//first digit: 1(menu bar) 2(side bar)
	//second digit: element of the bar
	//third digit: subelement of the bar
	ID_NewProject = 111,
	ID_OpenProject = 112,
	ID_Save = 113,
	ID_SaveAs = 114,
	ID_CommandLine = 311
};

