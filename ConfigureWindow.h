#pragma once
#include "wx/wx.h"
#include "wx/notebook.h"
#include <wx/spinctrl.h>
#include <string>
#include <vector>
#include "Process.h"
#include "wxChoiceVector.h"

class ConfigureWindow : public wxFrame
{
public:	
	ConfigureWindow();
	~ConfigureWindow();
	//void Test(wxCommandEvent& event);

	wxNotebook* ConfigureOptions;
	//pages and their methods
	wxPanel* visPage = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200, 100));
	wxPanel* runPage = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200, 100));
	wxPanel* gunPage = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200, 100));

	//some reusable methods
	void CreateNewConfig();
	void ModifyExistingConfig();

	//macro for event handling
	wxDECLARE_EVENT_TABLE();
};



