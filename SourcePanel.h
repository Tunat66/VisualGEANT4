#pragma once
#include "wx/wx.h"
#include "SystemVariables.h"
#include "wxChoiceVector.h"
#include <wx/spinctrl.h>
#include <math.h>
#include "GLGeometryViewer.h"

class SourcePanel : public wxPanel
{
public:
	SourcePanel(wxFrame* MainFrame,GLGeometryViewer* GeometryViewer);
	GLGeometryViewer* ViewerAccess;
private:
	//the layout (from top to bottom)
	wxStaticText* PickExisting = new wxStaticText(this, wxID_ANY, wxT("Pick Beam to manipulate:"), wxDefaultPosition, wxDefaultSize, 0);
	wxChoiceVector* SelectBody = nullptr;
	wxButton* CreateNew = nullptr;
	wxStaticText* SetPosition = new wxStaticText(this, wxID_ANY, wxT("Set Position of the Beam:"), wxDefaultPosition, wxDefaultSize, 0);
	//now aligned horizontally
	wxStaticText* x = new wxStaticText(this, wxID_ANY, wxT("x:"), wxDefaultPosition, wxDefaultSize, 0);
	wxSpinCtrl* XValue = nullptr;// new wxSpinCtrl(gunPage, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50,50), wxSP_ARROW_KEYS, 0, 100000, 10, "wxSpinCtrl");
	wxStaticText* y = new wxStaticText(this, wxID_ANY, wxT("y:"), wxDefaultPosition, wxDefaultSize, 0);
	wxSpinCtrl* YValue = nullptr;
	wxStaticText* z = new wxStaticText(this, wxID_ANY, wxT("z:"), wxDefaultPosition, wxDefaultSize, 0);
	wxSpinCtrl* ZValue = nullptr;
	//continue vertical
	wxStaticText* SetRotation = new wxStaticText(this, wxID_ANY, wxT("Set Direction of the Beam:"), wxDefaultPosition, wxDefaultSize, 0);
	//now aligned horizontally
	wxStaticText* xDirection = new wxStaticText(this, wxID_ANY, wxT("x Dir:"), wxDefaultPosition, wxDefaultSize, 0);
	wxSpinCtrl* xDirectionValue = nullptr;// new wxSpinCtrl(gunPage, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100000, 10, "wxSpinCtrl");
	wxStaticText* yDirection = new wxStaticText(this, wxID_ANY, wxT("y Dir:"), wxDefaultPosition, wxDefaultSize, 0);
	wxSpinCtrl* yDirectionValue = nullptr;
	wxStaticText* zDirection = new wxStaticText(this, wxID_ANY, wxT("z Dir:"), wxDefaultPosition, wxDefaultSize, 0);
	wxSpinCtrl* zDirectionValue = nullptr;
	//continue vertical
	wxButton* RefreshViewer = nullptr;


	//the sizer to manage layout:
	wxBoxSizer* VerticalLayout = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* TranslationSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* RotationSizer = new wxBoxSizer(wxHORIZONTAL);


	//to access backend
	SystemVariables SystemManager;

	//for events
	//wxDECLARE_EVENT_TABLE();
};

