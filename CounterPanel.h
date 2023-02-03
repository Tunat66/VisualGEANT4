#pragma once
#pragma once
#include "wx/wx.h"
#include "SystemVariables.h"
#include "wxChoiceVector.h"
#include "wx/notebook.h"
#include <wx/spinctrl.h>
#include <math.h>
#include "GLGeometryViewer.h"
#include <fstream>
#include <sstream>
#include <string>


class Counter;

class CounterPanel : public wxPanel
{

public:
	
	CounterPanel(wxFrame* MainFrame, GLGeometryViewer* GeometryViewer);
	GLGeometryViewer* ViewerAccess;

	//to access the backend, and some aliases for clean code made with address matching, possible bugs with assignment
	SystemVariables SystemManager;
	void Push(std::string arg);


public:
	//the layout (from top to bottom)
	wxStaticText* PickExisting = new wxStaticText(this, wxID_ANY, wxT("Manipulate the Geiger Counter"), wxDefaultPosition, wxDefaultSize, 0);
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

	//PROVISIONAL, will be implemented in later releases
	/*
	//continue vertical
	wxStaticText* SetRotation = new wxStaticText(this, wxID_ANY, wxT("Set Euler Rotation:"), wxDefaultPosition, wxDefaultSize, 0);
	//now aligned horizontally, not yet implemented
	wxStaticText* Euler1 = new wxStaticText(this, wxID_ANY, wxT("Euler 1:"), wxDefaultPosition, wxDefaultSize, 0);
	wxSpinCtrl* Euler1Value = nullptr;// new wxSpinCtrl(gunPage, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100000, 10, "wxSpinCtrl");
	wxStaticText* Euler2 = new wxStaticText(this, wxID_ANY, wxT("Euler 2:"), wxDefaultPosition, wxDefaultSize, 0);
	wxSpinCtrl* Euler2Value = nullptr;
	wxStaticText* Euler3 = new wxStaticText(this, wxID_ANY, wxT("Euler 3:"), wxDefaultPosition, wxDefaultSize, 0);
	wxSpinCtrl* Euler3Value = nullptr;*/

	//continue vertical
	wxButton* RefreshViewer = nullptr;
	wxButton* DeleteBody = nullptr;
	//when clicked
	void ApplyChanges(wxCommandEvent& event); //when refreshviewer is clicked
	void TranslateBodies(wxSpinEvent& event); //when the translation setting is adjusted
	void DeleteBodyf(wxCommandEvent& event);
	void SelectBodyf(wxCommandEvent& event);

	//the sizer to manage layout:
	wxBoxSizer* VerticalLayout = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* TranslationSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* RotationSizer = new wxBoxSizer(wxHORIZONTAL);

	//for events
	wxDECLARE_EVENT_TABLE();

	//to parse geometry files
	std::vector<Counter> ObjectList;
	std::vector<wxString> ObjectNameList;
	Counter ParseGeometryAndRunFile();

	friend class ModifyCounter;

	enum {
		RefreshViewer_ID,
		CreateNew_ID,
		DeleteBody_ID,
		SelectBody_ID,
		XValue_ID,
		YValue_ID,
		ZValue_ID
	};
};

class Counter
{
public:
	Counter(double in_posX, double in_posY, double in_posZ, double in_sizeX,
		double in_sizeY,
		double in_sizeZ,
		double in_eulerTheta = 0,
		double in_eulerPhi = 0,
		double in_eulerPsi = 0);
	//implemented
	//BodyTypes BodyType;
	double posX;
	double posY;
	double posZ;
	double sizeX;
	double sizeY;
	double sizeZ;

	//provisional, remove initializations after implementation
	double eulerTheta = 0;
	double eulerPhi = 0;
	double eulerPsi = 0;

};


/*
enum class BodyTypes
{
	NA,
	box
	//add further bodies here
};*/



//a small wxFrame for adding new objects
class ModifyCounter : public wxFrame
{
public:
	ModifyCounter(double currentxSize, double currentySize, double currentzSize);
	//~NewObject(); //{ delete[] WarningTextBoxArray; }
	//update this number when you add elements to the enum: it is the number of elements in enum BodyTypes
	int BodyImplementedNumber = 3;
	//~NewObject();
	SystemVariables SystemManager;
	//wxPanel* Backdrop = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200, 100));
	//wxStaticText* Name;
	//wxTextCtrl* NameEdit;
	//wxStaticText* Type;
	//wxChoiceVector* TypeEdit;

	

	//for Geiger Counter box:
	//these will be implemented when multiple counterboxes will be supported
	//wxStaticText* counterbox_Name;
	//wxTextCtrl* counterbox_NameEdit;

	wxStaticText* counterbox_xLength;
	wxSpinCtrl* counterbox_xLengthEdit;
	wxStaticText* counterbox_yWidth;
	wxSpinCtrl* counterbox_yWidthEdit;
	wxStaticText* counterbox_zHeight;
	wxSpinCtrl* counterbox_zHeightEdit;
	wxButton* counterbox_Modify;

	//wxStaticText** WarningTextBoxArray = new wxStaticText*[BodyImplementedNumber];
		//new wxStaticText(this, wxID_ANY, wxT("WARNING: Body names with whitespaces are not allowed!"));


	wxStaticText* WarningText(wxPanel* ParentPage)
	{
		wxStaticText* WarningTextInstance = new wxStaticText(ParentPage, wxID_ANY, wxT("WARNING: Body names with whitespaces are not allowed!"));
		return WarningTextInstance;
	}

	wxNotebook* ConfigureOptions;
	//pages and their methods
	wxPanel* counterboxPage = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200, 100));

	//some reusable methods
	void Push(std::string arg);

	//create methods
	void counterbox_modifyf(wxCommandEvent& event);

	//parse methods
	friend Counter CounterPanel::ParseGeometryAndRunFile();

	enum {
		counterbox_NameEdit_ID = 100,
		counterbox_MaterialEdit_ID = 150,
		counterbox_xLengthEdit_ID = 200,
		counterbox_yWidthEdit_ID = 300,
		counterbox_zHeightEdit_ID = 400,
		counterbox_Create_ID = 500,
		sphere_RadiusEdit_ID = 600
	};
	wxDECLARE_EVENT_TABLE();
};



