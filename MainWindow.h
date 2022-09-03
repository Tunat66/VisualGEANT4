#pragma once
#include "wx/wx.h"
#include <string>
#include <vector>
#include "Process.h"
#include "ConfigureWindow.h"
//include any new windows here
class MainWindow : public wxFrame
{
public:
	MainWindow();
	~MainWindow();
//this is the key attribute of this class, the Kernel_args vector
//this vector is normally constructed from the command line, but what the gui does
//is to contruct this vector, then flag it as "READY" when it reaches its final form
//if flagged as "READY", Kernel_args will be passed to a Process object by cApp (main) class
private:
	std::vector<std::string> Kernel_args;
public:
	std::vector<std::string> GetArgs();
	bool Project_isOpen = false;
	std::string CurrentProjectDir;

public:
	wxTextCtrl* m_txt1 = nullptr;
	wxListBox* m_list1 = nullptr;
	wxButton* new_window_btn = nullptr;

	//event handler methods
	void OnButton1Clicked(wxCommandEvent& evt);
	virtual void NewWindowOpen(wxCommandEvent& evt);

//CONFIG SUBWINDOW DISPLAY
private:
	//this object to display the subwindow
	MainWindow* m_frame2 = nullptr;

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

	//RIGHT PANEL METHODS
	//just compiles/builds the project
	wxButton* button_ApplyChanges = nullptr;
	void ApplyChanges(wxCommandEvent& event);

	//runs the project executable
	wxButton* button_RunProject = nullptr;
	void RunProject(wxCommandEvent& event);

	wxButton* button_ConfigureProject = nullptr;
	void ConfigureProject(wxCommandEvent& event);


	//conclusion method setting the final form of Kernel_args
	void Conclude();
	
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
	ID_SaveAs = 114
};

