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
#include <algorithm>

class Body;
class NewObject;

class GeometryPanel : public wxPanel
{

public:
	GeometryPanel(wxFrame* MainFrame, GLGeometryViewer* GeometryViewer);
	GLGeometryViewer* ViewerAccess;
	friend class NewObject;

	//to access the backend, and some aliases for clean code made with address matching, possible bugs with assignment
	SystemVariables SystemManager;
	void Push(std::string arg);

public:
	//the layout (from top to bottom)
	wxStaticText* PickExisting = new wxStaticText(this, wxID_ANY, wxT("Pick Existing Body to Manipulate:"), wxDefaultPosition, wxDefaultSize, 0);
	wxChoiceVector* SelectBody = nullptr;
	wxButton* CreateNew = nullptr;
	void FCreateNew(wxCommandEvent& event);
	wxStaticText* SetPosition = new wxStaticText(this, wxID_ANY, wxT("Set Position(mm):"), wxDefaultPosition, wxDefaultSize, 0);
	//now aligned horizontally
	wxStaticText* x = new wxStaticText(this, wxID_ANY, wxT("x:"), wxDefaultPosition, wxDefaultSize, 0);
	wxSpinCtrlDouble* XValue = nullptr;// new wxSpinCtrl(gunPage, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50,50), wxSP_ARROW_KEYS, 0, 100000, 10, "wxSpinCtrl");
	wxStaticText* y = new wxStaticText(this, wxID_ANY, wxT("y:"), wxDefaultPosition, wxDefaultSize, 0);
	wxSpinCtrlDouble* YValue = nullptr;
	wxStaticText* z = new wxStaticText(this, wxID_ANY, wxT("z:"), wxDefaultPosition, wxDefaultSize, 0);
	wxSpinCtrlDouble* ZValue = nullptr;
	
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
	wxStaticText* SelectedBodyMaterial = nullptr; //shows the material of the selected body
	//when clicked
	void ApplyChanges(wxCommandEvent& event); //when refreshviewer is clicked
	void TranslateBodies(wxSpinDoubleEvent& event); //when the translation setting is adjusted
	void DeleteBodyf(wxCommandEvent& event);
	void SelectBodyf(wxCommandEvent& event);
	void SelectBodyf_auto(int index); //for the program (not the user) to select the body

	//the sizer to manage layout:
	wxBoxSizer* VerticalLayout = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* TranslationSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* RotationSizer= new wxBoxSizer(wxHORIZONTAL);

	//for events
	wxDECLARE_EVENT_TABLE();

	//to parse geometry files
	std::vector<Body> ObjectList;
	std::vector<wxString> ObjectNameList;
	void ParseGeometryFile();
	void RefreshSelectBody(); //to refresh the body selector;

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

class Body 
{
public:
	Body(std::string in_BodyName , double in_posX, double in_posY, double in_posZ, std::string in_BodyMaterial, double in_eulerTheta = 0,
		double in_eulerPhi = 0,
		double in_eulerPsi = 0) {
		BodyName = in_BodyName;
		//BodyType = in_BodyType;
		posX = in_posX;
		posY = in_posY;
		posZ = in_posZ;
		BodyMaterial = in_BodyMaterial;
	}
	//implemented
	std::string BodyName;
	//BodyTypes BodyType;
	double posX;
	double posY;
	double posZ;
	std::string BodyMaterial; //I plan in the future not to store the material not as a string, this is only provisional

	//provisional, remove initializations after implementation
	double eulerTheta = 0;
	double eulerPhi = 0;
	double eulerPsi = 0;

};

enum class BodyTypes
{
	NA,
	box,
	sphere
	//add further bodies here
};



//a small wxFrame for adding new objects
class NewObject : public wxFrame
{

public:
	bool IsOpen = true;
	NewObject(GeometryPanel* Parent);
	GeometryPanel* CurrentParent = nullptr;
	//serves as a destructor
	void DestroyNewObjectPanel();
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

	//for box:
	wxStaticText* box_Name;
	wxTextCtrl* box_NameEdit;
	wxStaticText* box_Material;
	wxChoiceVector* box_MaterialEdit;
	wxStaticText* box_xLength;
	wxSpinCtrlDouble* box_xLengthEdit;
	wxStaticText* box_yWidth;
	wxSpinCtrlDouble* box_yWidthEdit;
	wxStaticText* box_zHeight;
	wxSpinCtrlDouble* box_zHeightEdit;
	wxButton* box_Create;
	

	//for sphere:
	wxStaticText* sphere_Name;
	wxTextCtrl* sphere_NameEdit;
	wxStaticText* sphere_Material;
	wxChoiceVector* sphere_MaterialEdit;
	wxStaticText* sphere_Radius;
	wxSpinCtrlDouble* sphere_RadiusEdit;
	wxButton* sphere_Create;

	//wxStaticText** WarningTextBoxArray = new wxStaticText*[BodyImplementedNumber];
		//new wxStaticText(this, wxID_ANY, wxT("WARNING: Body names with whitespaces are not allowed!"));

	
	wxStaticText* WarningText(wxPanel* ParentPage)
	{
		wxStaticText* WarningTextInstance = new wxStaticText(ParentPage, wxID_ANY, wxT("WARNING: Body names with whitespaces are not allowed!"));
		return WarningTextInstance;
	}

	wxNotebook* ConfigureOptions;
	//pages and their methods
	wxPanel* spherePage = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200, 100));
	wxPanel* cubePage = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200, 100));

	//some reusable methods
	void Push(std::string arg);

	//create methods
	void box_createf(wxCommandEvent& event);
	void sphere_createf(wxCommandEvent& event);
	void counterbox_createf(wxCommandEvent& event);

	//related to materials: (inline is needed otherwise compiler is angry)
	inline static std::vector<wxString> Materials { "Air", "Water" , "Iron", "Aluminum", "Copper", "Polystyrene", "Plexiglass", "Polyethylene", "Gold" };
	inline static std::vector<std::string> G4_Materials{ "G4_AIR", "G4_WATER", "G4_Fe", "G4_Al", "G4_Cu", "G4_POLYSTYRENE", "G4_PLEXIGLASS", "G4_POLYETHYLENE", "G4_Au" }; //their names as they appear on the g4geom.txt file



	//std::unordered_map<std::string, int> age;
	// Insert
	//age["Michael"] = 16;
	//age.insert(std::pair<std::string, int>{"Bill", 25});
	//age.insert({ "Chris", 30 });



	enum {
		box_NameEdit_ID = 100,
		box_MaterialEdit_ID = 150,
		box_xLengthEdit_ID = 200,
		box_yWidthEdit_ID = 300,
		box_zHeightEdit_ID = 400,
		box_Create_ID = 500,
		sphere_RadiusEdit_ID = 600,
		sphere_Create_ID = 700,
		sphere_NameEdit_ID = 800,
		sphere_MaterialEdit_ID = 900,
	};
	wxDECLARE_EVENT_TABLE();
};





