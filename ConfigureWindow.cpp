#include "ConfigureWindow.h"

//EVT_BUTTON(20001, Test)

wxBEGIN_EVENT_TABLE(ConfigureWindow, wxFrame)
	EVT_CHOICE(GunParticleEdit_ID, GunParticleEditf)
	EVT_SPINCTRL(GunEnergyEdit_ID, GunEnergyEditf)
	EVT_CHOICE(GunParticleEdit_ID, GunUnitEditf)
	EVT_SPINCTRL(BeamOnTimesEdit_ID, BeamOnTimesEditf)
	EVT_CHOICE(VisStyleEdit_ID, VisStyleEditf)
	EVT_CHOICE(VisFilterEdit_ID, VisFilterEditf)
	EVT_CHECKBOX(ProtonCB_ID, ProtonCBf)
	EVT_CHECKBOX(NeutronCB_ID, NeutronCBf)
	EVT_CHECKBOX(ElectronCB_ID, ElectronCBf)
	EVT_CHECKBOX(PositronCB_ID, PositronCBf)
	EVT_CHECKBOX(GammaCB_ID, GammaCBf)
wxEND_EVENT_TABLE()



ConfigureWindow::ConfigureWindow() : wxFrame(nullptr, wxID_ANY, "VisualGEANT4-Configure Project", /*setting initial position*/ wxPoint(50, 50), wxSize(600, 350))
{
	//wxButton* Test = new wxButton(this, 20001);
	
	//add a tabbed window for the user to access different options, add this to a panel
	//wxPanel* Background = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200, 100));
	//Background->SetTransparent(1);

	
	

	//BEGIN: Particle Gun Notebook Page (these are to be set dynamically later on, according to the physics list selected)
	std::vector<wxString> Particles{"e-", "e+" , "proton", "neutron", "gamma"};
	std::vector<wxString> EnergyUnits{"eV", "keV", "MeV", "GeV"};

	GunParticleText = new wxStaticText(gunPage, wxID_ANY, wxT("Particle"), wxDefaultPosition, wxDefaultSize, 0);
	//wxChoice* GunParticleEdit = new wxChoice(gunPage, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0, wxDefaultValidator, "Particle");
	GunParticleEdit = new wxChoiceVector(gunPage, GunParticleEdit_ID, Particles);
	GunEnergyText = new wxStaticText(gunPage, wxID_ANY, wxT("Energy"), wxDefaultPosition, wxDefaultSize, 0);
	GunEnergyEdit = new wxSpinCtrl(gunPage, GunEnergyEdit_ID, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100000, 10, "wxSpinCtrl");
	GunUnitText = new wxStaticText(gunPage, wxID_ANY, wxT("Energy Unit"), wxDefaultPosition, wxDefaultSize, 0);
	//wxChoice* GunUnitEdit = new wxChoice(gunPage, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0, wxDefaultValidator, "Particle");
	GunUnitEdit = new wxChoiceVector(gunPage, GunUnitEdit_ID, EnergyUnits);

	wxBoxSizer* GunPageSizer = new wxBoxSizer(wxVERTICAL);
	GunPageSizer->Add(GunParticleText, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10); //these 0's are essential, since they allow these elements to be "stacked" without any prior scaling
	GunPageSizer->Add(GunParticleEdit, 0, wxALIGN_LEFT | wxLEFT, 10);
	GunPageSizer->Add(GunEnergyText, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10);
	GunPageSizer->Add(GunEnergyEdit, 0, wxALIGN_LEFT | wxLEFT, 10);
	GunPageSizer->Add(GunUnitText, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10);
	GunPageSizer->Add(GunUnitEdit, 0, wxALIGN_LEFT | wxLEFT, 10);
	gunPage->SetSizer(GunPageSizer);
	//END: Particle Gun

	//BEGIN: Run Options
	BeamOnTimesText = new wxStaticText(runPage, wxID_ANY, wxT("Number of Particles to be shot"), wxDefaultPosition, wxDefaultSize, 0);
	BeamOnTimesEdit = new wxSpinCtrl(runPage, BeamOnTimesEdit_ID, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10000000, 0, "wxSpinCtrl");

	wxBoxSizer* RunPageSizer = new wxBoxSizer(wxVERTICAL);
	RunPageSizer->Add(BeamOnTimesText, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10); //these 0's are essential, since they allow these elements to be "stacked" without any prior scaling
	RunPageSizer->Add(BeamOnTimesEdit, 0, wxALIGN_LEFT | wxLEFT, 10);
	runPage->SetSizer(RunPageSizer);
	//END: Run Options

	//BEGIN: Vis Options (shit this is a bit more complex),
	std::vector<wxString> Styles{ "Wireframe", "Surface" };
	std::vector<wxString> FilterBy{ "Particle", "Charge" };

	VisStyleText = new wxStaticText(visPage, wxID_ANY, wxT("Style"), wxDefaultPosition, wxDefaultSize, 0);
	VisStyleEdit = new wxChoiceVector(visPage, VisStyleEdit_ID, Styles);
	VisFilterText = new wxStaticText(visPage, wxID_ANY, wxT("Filter By:"), wxDefaultPosition, wxDefaultSize, 0);
	VisFilterEdit = new wxChoiceVector(visPage, VisFilterEdit_ID, FilterBy);
	//the particles for particle filter (make it dissapearable/unclickable), with their horizontal sizer:
	VisSelectParticleText = new wxStaticText(visPage, wxID_ANY, wxT("(If filtering by particle) Select Particles to be shown:"), wxDefaultPosition, wxDefaultSize, 0);
	ProtonCB = new wxCheckBox(visPage, ProtonCB_ID, wxT("Proton"), wxDefaultPosition, wxDefaultSize, 0);
	NeutronCB = new wxCheckBox(visPage, NeutronCB_ID, wxT("Neutron"), wxDefaultPosition, wxDefaultSize, 0);
	PositronCB = new wxCheckBox(visPage, PositronCB_ID, wxT("e+"), wxDefaultPosition, wxDefaultSize, 0);
	ElectronCB = new wxCheckBox(visPage, ElectronCB_ID, wxT("e-"), wxDefaultPosition, wxDefaultSize, 0);
	GammaCB = new wxCheckBox(visPage, GammaCB_ID, wxT("gamma"), wxDefaultPosition, wxDefaultSize, 0);
	wxBoxSizer* ParticleCBSizer = new wxBoxSizer(wxHORIZONTAL);
	ParticleCBSizer->Add(ProtonCB, 0, wxLEFT | wxRIGHT, 3);
	ParticleCBSizer->Add(NeutronCB, 0, wxRIGHT, 3);
	ParticleCBSizer->Add(PositronCB, 0, wxRIGHT, 3);
	ParticleCBSizer->Add(ElectronCB, 0, wxRIGHT, 3);
	ParticleCBSizer->Add(GammaCB, 0, wxRIGHT, 3);
	//implement pions and muons later
	//to be added:
	//color picker for trajectories (not yet in backend)
	

	wxBoxSizer* VisPageSizer = new wxBoxSizer(wxVERTICAL);
	VisPageSizer->Add(VisStyleText, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10); //these 0's are essential, since they allow these elements to be "stacked" without any prior scaling
	VisPageSizer->Add(VisStyleEdit, 0, wxALIGN_LEFT | wxLEFT, 10);
	VisPageSizer->Add(VisFilterText, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10);
	VisPageSizer->Add(VisFilterEdit, 0, wxALIGN_LEFT | wxLEFT, 10);
	VisPageSizer->Add(VisSelectParticleText, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10);
	//VisPageSizer->Add(CBHolder, 0, wxALIGN_LEFT | wxLEFT, 10);
	VisPageSizer->Add(ParticleCBSizer, 0, wxALIGN_LEFT | wxLEFT, 10);
	visPage->SetSizer(VisPageSizer);
	//END: Vis Options

	//now create the notebook/pages
	ConfigureOptions = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxSize(500, 500), wxNB_MULTILINE);
	ConfigureOptions->AddPage(visPage, "Visualization");
	ConfigureOptions->AddPage(runPage, "Run");
	ConfigureOptions->AddPage(gunPage, "Particle Gun");
	ConfigureOptions->Layout();

	//finally add a background sizer
	wxBoxSizer* BackgroundSizer = new wxBoxSizer(wxVERTICAL); //doesn't matter, can be horizontal too
	BackgroundSizer->Add(ConfigureOptions, 1, wxEXPAND | wxALL);
	SetSizer(BackgroundSizer);

}

//for utility
void ConfigureWindow::Push(std::string arg)
{
	SystemManager.Kernel_args.push_back(arg);
}

//functions bound to every single control:
void ConfigureWindow::GunParticleEditf(wxCommandEvent& event) {
	SystemManager.Kernel_args.push_back("config");
	SystemManager.Kernel_args.push_back("gun");
	SystemManager.Kernel_args.push_back("setParticle");
	int index = GunParticleEdit->GetSelection();
	//remember
	/*std::vector<wxString> Particles{"e-", "e+" , "proton", "neutron", "gamma"};
	std::vector<wxString> EnergyUnits{"eV", "keV", "MeV", "GeV"};*/
	
	switch (index) {
	case 0:
		SystemManager.Kernel_args.push_back("e-");
		break;
	case 1:
		SystemManager.Kernel_args.push_back("e+");
		break;
	case 2:
		SystemManager.Kernel_args.push_back("proton");
		break;
	case 3:
		SystemManager.Kernel_args.push_back("neutron");
		break;
	case 4:
		SystemManager.Kernel_args.push_back("gamma");
		break;
	}

	SystemManager.Kernel_args.push_back("gun.mac");
	SystemManager.Conclude();
}


void ConfigureWindow::GunEnergyEditf(wxSpinEvent& event) {
	Push("config");
	Push("gun");
	Push("setEnergy");
	Push(std::to_string(GunEnergyEdit->GetValue()) + "KeV"); //capitalization be carful
	Push("gun.mac");
	SystemManager.Conclude();
}
void ConfigureWindow::GunUnitEditf(wxCommandEvent& event) {
	//currently defunct. due to syntax problems with GEANT4, the only available/default energy unit is KeV
}


void ConfigureWindow::BeamOnTimesEditf(wxSpinEvent& event) {
	Push("config");
	Push("run");
	Push("beamOnTimes");
	Push(std::to_string(BeamOnTimesEdit->GetValue())); 
	Push("run.mac");
	SystemManager.Conclude();
}


void ConfigureWindow::VisStyleEditf(wxCommandEvent& event) {
	//remember
	/*std::vector<wxString> Styles{"Wireframe", "Surface"};
	std::vector<wxString> FilterBy{ "Particle", "Charge" };*/
	Push("config");
	Push("vis");
	Push("style");
	int index = VisStyleEdit->GetSelection();

	switch (index) {
	case 0:
		Push("Wireframe");
		break;
	case 1:
		Push("Surface");
		break;
	}
	Push("vis.mac");
	SystemManager.Conclude();
}
void ConfigureWindow::VisFilterEditf(wxCommandEvent& event) {
	//Note to self: Filter by is kind of a misnomer, consider "color by"
/*std::vector<wxString> Styles{"Wireframe", "Surface"};
std::vector<wxString> FilterBy{ "Particle", "Charge" };*/
	Push("config");
	Push("vis");
	Push("trajectories");
	int index = VisStyleEdit->GetSelection();

	switch (index) {
	case 0:
		Push("drawByParticleID");
		break;
	case 1:
		Push("drawByCharge");
		break;
	}
	Push("vis.mac");
	SystemManager.Conclude();
}
void ConfigureWindow::ProtonCBf(wxCommandEvent& event) {
	Push("config");
	Push("vis");
	Push("filter");
	Push("addparticle");
	//spec
	Push("proton");
	Push("vis.mac");
	SystemManager.Conclude();
}
void ConfigureWindow::NeutronCBf(wxCommandEvent& event) {
	Push("config");
	Push("vis");
	Push("filter");
	Push("addparticle");
	//spec
	Push("neutron");
	Push("vis.mac");
	SystemManager.Conclude();
}
void ConfigureWindow::PositronCBf(wxCommandEvent& event) {
	Push("config");
	Push("vis");
	Push("filter");
	Push("addparticle");
	//spec
	Push("e+");
	Push("vis.mac");
	SystemManager.Conclude();
}
void ConfigureWindow::ElectronCBf(wxCommandEvent& event) {
	Push("config");
	Push("vis");
	Push("filter");
	Push("addparticle");
	//spec
	Push("e-");
	Push("vis.mac");
	SystemManager.Conclude();
}
void ConfigureWindow::GammaCBf(wxCommandEvent& event) {
	Push("config");
	Push("vis");
	Push("filter");
	Push("addparticle");
	//spec
	Push("gamma");
	Push("vis.mac");
	SystemManager.Conclude();
}

//ISSUE: there are no remove-particle methods! Add them ASAP!

ConfigureWindow::~ConfigureWindow()
{
}

void ConfigureWindow::CreateNewConfig()
{

}

void ConfigureWindow::ModifyExistingConfig()
{

}

/*void ConfigureWindow::Test(wxCommandEvent& event)
{
	int i = 0;
}*/



