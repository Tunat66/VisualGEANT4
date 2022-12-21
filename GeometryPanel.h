#pragma once
#include "wx/wx.h"
#include "SystemVariables.h"
#include "wxChoiceVector.h"
#include <wx/spinctrl.h>
#include <math.h>
#include "GLGeometryViewer.h"
class GeometryPanel : public wxPanel
{
	
public:
	GeometryPanel(wxFrame* MainFrame, GLGeometryViewer* GeometryViewer);
	GLGeometryViewer* ViewerAccess;

	//to access the backend, and some aliases for clean code made with address matching, possible bugs with assignment
	SystemVariables SystemManager;

private:
	//the layout (from top to bottom)
	wxStaticText* PickExisting = new wxStaticText(this, wxID_ANY, wxT("Pick Existing Body to Manipulate:"), wxDefaultPosition, wxDefaultSize, 0);
	wxChoiceVector* SelectBody = nullptr;
	wxButton* CreateNew = nullptr;
	void FCreateNew(wxCommandEvent& event);
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
	//now aligned horizontally, not yet implemented
	wxStaticText* Euler1 = new wxStaticText(this, wxID_ANY, wxT("Euler 1:"), wxDefaultPosition, wxDefaultSize, 0);
	wxSpinCtrl* Euler1Value = nullptr;// new wxSpinCtrl(gunPage, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100000, 10, "wxSpinCtrl");
	wxStaticText* Euler2 = new wxStaticText(this, wxID_ANY, wxT("Euler 2:"), wxDefaultPosition, wxDefaultSize, 0);
	wxSpinCtrl* Euler2Value = nullptr;
	wxStaticText* Euler3 = new wxStaticText(this, wxID_ANY, wxT("Euler 3:"), wxDefaultPosition, wxDefaultSize, 0);
	wxSpinCtrl* Euler3Value = nullptr;
	
	//continue vertical
	wxButton* RefreshViewer = nullptr;
	void ApplyChanges(wxCommandEvent& event);

	//the sizer to manage layout:
	wxBoxSizer* VerticalLayout = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* TranslationSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* RotationSizer= new wxBoxSizer(wxHORIZONTAL);

	//for events
	wxDECLARE_EVENT_TABLE();

	enum {
		RefreshViewer_ID = 1,
		CreateNew_ID = 2
	};
};

