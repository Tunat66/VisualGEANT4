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
	wxStaticText* SetPosition = new wxStaticText(this, wxID_ANY, wxT("Set Position (mm):"), wxDefaultPosition, wxDefaultSize, 0);
	//now aligned horizontally
	wxStaticText* x = new wxStaticText(this, wxID_ANY, wxT("x:"), wxDefaultPosition, wxDefaultSize, 0);
	wxSpinCtrlDouble* XValue = nullptr;// new wxSpinCtrl(gunPage, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50,50), wxSP_ARROW_KEYS, 0, 100000, 10, "wxSpinCtrl");
	wxStaticText* y = new wxStaticText(this, wxID_ANY, wxT("y:"), wxDefaultPosition, wxDefaultSize, 0);
	wxSpinCtrlDouble* YValue = nullptr;
	wxStaticText* z = new wxStaticText(this, wxID_ANY, wxT("z:"), wxDefaultPosition, wxDefaultSize, 0);
	wxSpinCtrlDouble* ZValue = nullptr;

	
	//continue vertical
	//see visual: https://en.wikipedia.org/wiki/File:Eulerangles.svg
	//according to the visual Euler1:alpha, Euler2:beta, Euler3:gamma
	wxStaticText* SetRotation = new wxStaticText(this, wxID_ANY, wxT("Set Euler Rotation (degrees):"), wxDefaultPosition, wxDefaultSize, 0);
	//now aligned horizontally, not yet implemented
	wxStaticText* Euler1 = new wxStaticText(this, wxID_ANY, wxT("Alpha:"), wxDefaultPosition, wxDefaultSize, 0);
	wxSpinCtrlDouble* Euler1Value = nullptr;// new wxSpinCtrl(gunPage, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100000, 10, "wxSpinCtrl");
	wxStaticText* Euler2 = new wxStaticText(this, wxID_ANY, wxT("Beta:"), wxDefaultPosition, wxDefaultSize, 0);
	wxSpinCtrlDouble* Euler2Value = nullptr;
	wxStaticText* Euler3 = new wxStaticText(this, wxID_ANY, wxT("Gamma:"), wxDefaultPosition, wxDefaultSize, 0);
	wxSpinCtrlDouble* Euler3Value = nullptr;

	//continue vertical
	wxButton* RefreshViewer = nullptr;
	wxButton* DeleteBody = nullptr;
	wxStaticText* CountsFromLastRun = nullptr; //shows the counts from last run 
	//(the above static text doesn't need auto refresh outside constructor, 
	// as the user needs to change to run panel and back to have a new run)
	
	//when clicked on buttons
	void ApplyChanges(wxCommandEvent& event); //when refreshviewer is clicked
	void TranslateBodies(wxSpinDoubleEvent& event); //when the translation setting is adjusted
	void RotateBodies(wxSpinDoubleEvent& event); //when the rotation setting is adjusted
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
	wxString ParseExperimentalDataFile(); //it returns the counts as a string

	friend class ModifyCounter;

	enum {
		RefreshViewer_ID,
		CreateNew_ID,
		DeleteBody_ID,
		SelectBody_ID,
		XValue_ID,
		YValue_ID,
		ZValue_ID,
		Euler1Value_ID,
		Euler2Value_ID,
		Euler3Value_ID
	};
};

class Counter
{
public:
	Counter(double in_posX, double in_posY, double in_posZ, double in_sizeX,
		double in_sizeY,
		double in_sizeZ,
		double in_euler1 = 0,
		double in_euler2 = 0,
		double in_euler3 = 0);
	//implemented
	//BodyTypes BodyType;
	double posX;
	double posY;
	double posZ;
	double sizeX;
	double sizeY;
	double sizeZ;

	//provisional, remove initializations after implementation
	double euler1;
	double euler2;
	double euler3;

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



