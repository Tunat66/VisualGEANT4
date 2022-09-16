#pragma once
#include "wx/wx.h"
#include "SystemVariables.h"
#include "wxChoiceVector.h"
#include <wx/spinctrl.h>
#include <math.h>
class GeometryPanel : public wxPanel
{
	
public:
	GeometryPanel(wxFrame* MainFrame);
private:
	//the layout (from top to bottom)
	wxStaticText* PickExisting = new wxStaticText(this, wxID_ANY, wxT("Pick Existing Body to Manipulate:"), wxDefaultPosition, wxDefaultSize, 0);
	wxChoiceVector* SelectBody = nullptr;
	wxButton* CreateNew = nullptr;
	wxStaticText* SetPosition = new wxStaticText(this, wxID_ANY, wxT("Set Position:"), wxDefaultPosition, wxDefaultSize, 0);
	//now aligned horizontally
	wxStaticText* x = new wxStaticText(this, wxID_ANY, wxT("x:"), wxDefaultPosition, wxDefaultSize, 0);
	wxSpinCtrl* XValue = nullptr;// new wxSpinCtrl(gunPage, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50,50), wxSP_ARROW_KEYS, 0, 100000, 10, "wxSpinCtrl");
	wxStaticText* y = new wxStaticText(this, wxID_ANY, wxT("y:"), wxDefaultPosition, wxDefaultSize, 0);
	wxSpinCtrl* YValue = nullptr;
	wxStaticText* z = new wxStaticText(this, wxID_ANY, wxT("z:"), wxDefaultPosition, wxDefaultSize, 0);
	wxSpinCtrl* ZValue = nullptr;
	//continue vertical
	wxStaticText* SetRotation = new wxStaticText(this, wxID_ANY, wxT("Set Euler Rotation:"), wxDefaultPosition, wxDefaultSize, 0);
	//now aligned horizontally
	wxStaticText* Euler1 = new wxStaticText(this, wxID_ANY, wxT("Euler 1:"), wxDefaultPosition, wxDefaultSize, 0);
	wxSpinCtrl* Euler1Value = nullptr;// new wxSpinCtrl(gunPage, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100000, 10, "wxSpinCtrl");
	wxStaticText* Euler2 = new wxStaticText(this, wxID_ANY, wxT("Euler 2:"), wxDefaultPosition, wxDefaultSize, 0);
	wxSpinCtrl* Euler2Value = nullptr;
	wxStaticText* Euler3 = new wxStaticText(this, wxID_ANY, wxT("Euler 3:"), wxDefaultPosition, wxDefaultSize, 0);
	wxSpinCtrl* Euler3Value = nullptr;
	//continue vertical
	wxButton* RefreshViewer = nullptr;
	

	//the sizer to manage layout:
	wxBoxSizer* VerticalLayout = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* TranslationSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* RotationSizer= new wxBoxSizer(wxHORIZONTAL);

	
	//to access backend
	SystemVariables SystemManager;

	//for events
	//wxDECLARE_EVENT_TABLE();
};

