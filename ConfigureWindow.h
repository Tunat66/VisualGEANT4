#pragma once
#include "wx/wx.h"
#include "wx/notebook.h"
#include <wx/spinctrl.h>
#include <string>
#include <vector>
#include "wxChoiceVector.h"
#include "SystemVariables.h"

class ConfigureWindow : public wxFrame
{
public:	
	ConfigureWindow();
	~ConfigureWindow();
	//void Test(wxCommandEvent& event);

	SystemVariables SystemManager;

	wxNotebook* ConfigureOptions;
	//pages and their methods
	wxPanel* visPage = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200, 100));
	wxPanel* runPage = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200, 100));
	wxPanel* gunPage = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200, 100));

	//some reusable methods
	void CreateNewConfig();
	void ModifyExistingConfig();
	void Push(std::string arg);

	//input elements
	wxStaticText* GunParticleText;
	wxChoiceVector* GunParticleEdit;
	wxStaticText* GunEnergyText; 
	wxSpinCtrl* GunEnergyEdit;
	wxStaticText* GunUnitText;
	//wxChoice* GunUnitEdit = new wxChoice(gunPage, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0, wxDefaultValidator, "Particle");
	wxChoiceVector* GunUnitEdit;

	wxStaticText* BeamOnTimesText;// = new wxStaticText(runPage, wxID_ANY, wxT("Number of Particles to be shot"), wxDefaultPosition, wxDefaultSize, 0);
	wxSpinCtrl* BeamOnTimesEdit;// = new wxSpinCtrl(runPage, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10000000, 0, "wxSpinCtrl");

	wxStaticText* VisStyleText;// = new wxStaticText(visPage, wxID_ANY, wxT("Style"), wxDefaultPosition, wxDefaultSize, 0);
	wxChoiceVector* VisStyleEdit;// = new wxChoiceVector(visPage, wxID_ANY, Styles);
	wxStaticText* VisFilterText;// = new wxStaticText(visPage, wxID_ANY, wxT("Filter By:"), wxDefaultPosition, wxDefaultSize, 0);
	wxChoiceVector* VisFilterEdit;// = new wxChoiceVector(visPage, wxID_ANY, FilterBy);
	//the particles for particle filter (make it dissapearable/unclickable), with their horizontal sizer:
	wxStaticText* VisSelectParticleText;// = new wxStaticText(visPage, wxID_ANY, wxT("(If filtering by particle) Select Particles to be shown:"), wxDefaultPosition, wxDefaultSize, 0);
	wxCheckBox* ProtonCB;// = new wxCheckBox(visPage, wxID_ANY, wxT("Proton"), wxDefaultPosition, wxDefaultSize, 0);
	wxCheckBox* NeutronCB;// = new wxCheckBox(visPage, wxID_ANY, wxT("Neutron"), wxDefaultPosition, wxDefaultSize, 0);
	wxCheckBox* PositronCB;// = new wxCheckBox(visPage, wxID_ANY, wxT("e+"), wxDefaultPosition, wxDefaultSize, 0);
	wxCheckBox* ElectronCB;// = new wxCheckBox(visPage, wxID_ANY, wxT("e-"), wxDefaultPosition, wxDefaultSize, 0);
	wxCheckBox* GammaCB;// = new wxCheckBox(visPage, wxID_ANY, wxT("gamma"), wxDefaultPosition, wxDefaultSize, 0);

	//functions bound to every single control:
	void GunParticleEditf(wxCommandEvent& event);
	void GunEnergyEditf(wxCommandEvent& event);
	void GunUnitEditf(wxCommandEvent& event);

	void BeamOnTimesEditf(wxCommandEvent& event);

	void VisStyleEditf(wxCommandEvent& event);
	void VisFilterEditf(wxCommandEvent& event);
	void ProtonCBf(wxCommandEvent& event);
	void NeutronCBf(wxCommandEvent& event);
	void PositronCBf(wxCommandEvent& event);
	void ElectronCBf(wxCommandEvent& event);
	void GammaCBf(wxCommandEvent& event);



	//macro for event handling
	wxDECLARE_EVENT_TABLE();
};



