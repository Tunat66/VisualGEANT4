#include "ConfigureWindow.h"

//EVT_BUTTON(20001, Test)

wxBEGIN_EVENT_TABLE(ConfigureWindow, wxFrame)
	EVT_CHOICE(GunParticleEdit_ID, GunParticleEditf)
	EVT_SPINCTRLDOUBLE(GunEnergyEdit_ID, GunEnergyEditf)
	EVT_SPINCTRL(BeamOnTimesEdit_ID, BeamOnTimesEditf)
	EVT_SPINCTRLDOUBLE(GunPosX_ID, GunPositionEditf)
	EVT_SPINCTRLDOUBLE(GunPosY_ID, GunPositionEditf)
	EVT_SPINCTRLDOUBLE(GunPosZ_ID, GunPositionEditf)
	EVT_SPINCTRLDOUBLE(GunDirX_ID, GunDirectionEditf)
	EVT_SPINCTRLDOUBLE(GunDirY_ID, GunDirectionEditf)
	EVT_SPINCTRLDOUBLE(GunDirZ_ID, GunDirectionEditf)
	EVT_CHOICE(VisStyleEdit_ID, VisStyleEditf)
	EVT_CHOICE(VisFilterEdit_ID, VisFilterEditf)
	EVT_CHECKBOX(ProtonCB_ID, ProtonCBf)
	EVT_CHECKBOX(NeutronCB_ID, NeutronCBf)
	EVT_CHECKBOX(ElectronCB_ID, ElectronCBf)
	EVT_CHECKBOX(PositronCB_ID, PositronCBf)
	EVT_CHECKBOX(GammaCB_ID, GammaCBf)
wxEND_EVENT_TABLE()

void ConfigureWindow::GunDirectionEditf(wxSpinDoubleEvent& event)
{
	//for debug wxMessageBox(wxT("WorkingDir"));
	Push("source");
	Push("direction");
	Push(std::to_string(GunDirX->GetValue())); 
	Push(std::to_string(GunDirY->GetValue())); 
	Push(std::to_string(GunDirZ->GetValue())); 
	SystemManager.Conclude();
}

void ConfigureWindow::GunPositionEditf(wxSpinDoubleEvent& event)
{
	//for debug wxMessageBox(wxT("WorkingPos"));
	Push("source");
	Push("position");
	Push(std::to_string(GunPosX->GetValue()));
	Push(std::to_string(GunPosY->GetValue()));
	Push(std::to_string(GunPosZ->GetValue()));
	SystemManager.Conclude();

}

ConfigureWindow::ConfigureWindow() : wxFrame(nullptr, wxID_ANY, "VisualGEANT4-Configure Project", /*setting initial position*/ wxPoint(50, 50), wxSize(800, 350))
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
	GunEnergyText = new wxStaticText(gunPage, wxID_ANY, wxT("Energy (KeV)"), wxDefaultPosition, wxDefaultSize, 0);
	GunEnergyEdit = new wxSpinCtrlDouble(gunPage, GunEnergyEdit_ID, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0., 100000., 0, 0.1, "wxSpinCtrl");
	//GunUnitText = new wxStaticText(gunPage, wxID_ANY, wxT("Energy Unit"), wxDefaultPosition, wxDefaultSize, 0);
	//wxChoice* GunUnitEdit = new wxChoice(gunPage, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0, wxDefaultValidator, "Particle");
	//GunUnitEdit = new wxChoiceVector(gunPage, GunUnitEdit_ID, EnergyUnits);
	GunPositionText = new wxStaticText(gunPage, wxID_ANY, wxT("Set Gun Position (units in cm, -250 to 250 allowed, two decimal digits allowed):"), wxDefaultPosition, wxDefaultSize, 0);
	GunPosX = new wxSpinCtrlDouble(gunPage, GunPosX_ID, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -250, 250, 0, 0.01, "wxSpinCtrl");
	GunPosY = new wxSpinCtrlDouble(gunPage, GunPosY_ID, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -250, 250, 0, 0.01, "wxSpinCtrl");
	GunPosZ = new wxSpinCtrlDouble(gunPage, GunPosZ_ID, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -250, 250, 0, 0.01, "wxSpinCtrl");
	GunDirectionText = new wxStaticText(gunPage, wxID_ANY, wxT("Set Gun Direction (Enter the components of the direction vector, it needn't be a unit vector) (two decimal digits allowed):"), wxDefaultPosition, wxDefaultSize, 0);
	GunDirX = new wxSpinCtrlDouble(gunPage, GunDirX_ID, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -250, 250, 0, 0.01, "wxSpinCtrl");
	GunDirY = new wxSpinCtrlDouble(gunPage, GunDirY_ID, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -250, 250, 0, 0.01, "wxSpinCtrl");
	GunDirZ = new wxSpinCtrlDouble(gunPage, GunDirZ_ID, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -250, 250, 0, 0.01, "wxSpinCtrl");
	wxStaticText* GunInstructionText = new wxStaticText(gunPage, wxID_ANY, wxT("Changes are saved automatically, close this window after performing the changes."), wxDefaultPosition, wxDefaultSize, 0);
	

	wxBoxSizer* GunPageSizer = new wxBoxSizer(wxVERTICAL);
	GunPageSizer->Add(GunParticleText, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10); //these 0's are essential, since they allow these elements to be "stacked" without any prior scaling
	GunPageSizer->Add(GunParticleEdit, 0, wxALIGN_LEFT | wxLEFT, 10);
	GunPageSizer->Add(GunEnergyText, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10);
	GunPageSizer->Add(GunEnergyEdit, 0, wxALIGN_LEFT | wxLEFT, 10);
	//GunPageSizer->Add(GunUnitText, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10);
	//GunPageSizer->Add(GunUnitEdit, 0, wxALIGN_LEFT | wxLEFT, 10);
	GunPageSizer->Add(GunPositionText, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10);
	wxBoxSizer* GunPosSizer = new wxBoxSizer(wxHORIZONTAL);
	GunPosSizer->Add(GunPosX, 0, wxLEFT | wxRIGHT, 3);
	GunPosSizer->Add(GunPosY, 0, wxRIGHT, 3);
	GunPosSizer->Add(GunPosZ, 0, wxRIGHT, 3);
	GunPageSizer->Add(GunPosSizer, 0, wxALIGN_LEFT | wxLEFT, 10);
	GunPageSizer->Add(GunDirectionText, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10);
	wxBoxSizer* GunDirSizer = new wxBoxSizer(wxHORIZONTAL);
	GunDirSizer->Add(GunDirX, 0, wxLEFT | wxRIGHT, 3);
	GunDirSizer->Add(GunDirY, 0, wxRIGHT, 3);
	GunDirSizer->Add(GunDirZ, 0, wxRIGHT, 3);
	GunPageSizer->Add(GunDirSizer, 0, wxALIGN_LEFT | wxLEFT, 10);
	GunPageSizer->Add(GunInstructionText, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10);
	gunPage->SetSizer(GunPageSizer);
	ParseGunFile();
	//END: Particle Gun

	//BEGIN: Run Options
	BeamOnTimesText = new wxStaticText(runPage, wxID_ANY, wxT("Number of Particles to be shot"), wxDefaultPosition, wxDefaultSize, 0);
	BeamOnTimesEdit = new wxSpinCtrl(runPage, BeamOnTimesEdit_ID, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10000000, 0, "wxSpinCtrl");
	wxStaticText* RunInstructionText = new wxStaticText(runPage, wxID_ANY, wxT("Changes are saved automatically, close this window after performing the changes."), wxDefaultPosition, wxDefaultSize, 0);

	wxBoxSizer* RunPageSizer = new wxBoxSizer(wxVERTICAL);
	RunPageSizer->Add(BeamOnTimesText, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10); //these 0's are essential, since they allow these elements to be "stacked" without any prior scaling
	RunPageSizer->Add(BeamOnTimesEdit, 0, wxALIGN_LEFT | wxLEFT, 10);
	RunPageSizer->Add(RunInstructionText, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10);
	runPage->SetSizer(RunPageSizer);
	ParseRunFile();
	//END: Run Options

	//BEGIN: Vis Options (shit this is a bit more complex),
	std::vector<wxString> Styles{ "Wireframe", "Surface" };
	std::vector<wxString> FilterBy{ "Particle", "Charge" };

	VisStyleText = new wxStaticText(visPage, wxID_ANY, wxT("Style"), wxDefaultPosition, wxDefaultSize, 0);
	VisStyleEdit = new wxChoiceVector(visPage, VisStyleEdit_ID, Styles);
	VisFilterText = new wxStaticText(visPage, wxID_ANY, wxT("Color Trajectories By:"), wxDefaultPosition, wxDefaultSize, 0);
	VisFilterEdit = new wxChoiceVector(visPage, VisFilterEdit_ID, FilterBy);
	//the particles for particle filter (make it dissapearable/unclickable), with their horizontal sizer:
	VisSelectParticleText = new wxStaticText(visPage, wxID_ANY, wxT("Select Particles to be shown:"), wxDefaultPosition, wxDefaultSize, 0);
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
	wxStaticText* VisInstructionText = new wxStaticText(visPage, wxID_ANY, wxT("Changes are saved automatically, close this window after performing the changes."), wxDefaultPosition, wxDefaultSize, 0);


	wxBoxSizer* VisPageSizer = new wxBoxSizer(wxVERTICAL);
	VisPageSizer->Add(VisStyleText, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10); //these 0's are essential, since they allow these elements to be "stacked" without any prior scaling
	VisPageSizer->Add(VisStyleEdit, 0, wxALIGN_LEFT | wxLEFT, 10);
	VisPageSizer->Add(VisFilterText, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10);
	VisPageSizer->Add(VisFilterEdit, 0, wxALIGN_LEFT | wxLEFT, 10);
	VisPageSizer->Add(VisSelectParticleText, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10);
	//VisPageSizer->Add(CBHolder, 0, wxALIGN_LEFT | wxLEFT, 10);
	VisPageSizer->Add(ParticleCBSizer, 0, wxALIGN_LEFT | wxLEFT, 10);
	VisPageSizer->Add(VisInstructionText, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10);
	visPage->SetSizer(VisPageSizer);
	//PARSE THE FILE AND INITIALIZE CONTROLS USING:
	ParseVisFile();
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

//file parsers to initialize controls (their code is largely derived/modified from the file parser of GeometryPanel)
// parser is again a slight misnomer, these methods also initialize the controls
//for vis.mac
void ConfigureWindow::ParseVisFile()
{
	std::ifstream VisFile;
	std::string FileDir = SystemManager.CurrentProjectDir + "/Macros/vis.mac";
	VisFile.open(FileDir);
	if (!VisFile.is_open())
	{
		wxMessageBox(wxT("vis.mac file for this project cannot be opened due to an unknown reason, please consult to the developers of the project."));
		wxMessageBox(wxT("File not found: " + FileDir));
		return;
	}

	//now read the file line by line and perform checks in each line
	while (!VisFile.eof()) //to read until the end of the file
	{
		std::string Token;
		std::getline(VisFile, Token);
		//now parse this line into words according to whitespaces
		//note that line elements with whitespaces, but in quotation marks are not
		//supported by this parser
		std::istringstream StringParser(Token);
		std::string Element;
		std::vector<std::string> VisInformation;
		while (StringParser >> Element)
		{
			VisInformation.push_back(Element);
		}

		//now check if the line has a certain value or prefix
		if (!VisInformation.empty())
		{
			std::string Prefix = VisInformation.at(0); //the first element seperated by whitespace of a line is taken as prefix
			//style
			if (Prefix == "/vis/viewer/set/style")
			{
				std::string Style = VisInformation.at(1);
				int StyleCode = 0; //0 is there just to initialize
				if (Style == "surface")
				{
					StyleCode = 1;
				}
				else if (Style == "wireframe")
				{
					StyleCode = 0;
				}
				VisStyleEdit->SetSelection(StyleCode);
			}
			//filtering
			else if (Prefix == "/vis/modeling/trajectories/create/drawByCharge")
			{
				//this approach is slightly prone to bugs, consider an approach
				// which searches through the wxChoiceVector vector argument to find the position of "Charge"
				
				//the code for byCharge is 1
				VisFilterEdit->SetSelection(1);
			}
			else if (Prefix == "/vis/modeling/trajectories/create/drawByParticleID")
			{
				//this approach is slightly prone to bugs, consider an approach
				// which searches through the wxChoiceVector vector argument to find the position of "Particle"

				//the code for byParticleID is 0
				VisFilterEdit->SetSelection(0);
			}
			//particle checkboxes
			else if (Prefix == "/vis/filtering/trajectories/particleFilter-0/add")
			{
				std::string ParticleName = VisInformation.at(1);
				if (ParticleName == "proton") { ProtonCB->SetValue(true); }
				if (ParticleName == "neutron") { NeutronCB->SetValue(true); }
				if (ParticleName == "e+") { PositronCB->SetValue(true); }
				if (ParticleName == "e-") { ElectronCB->SetValue(true); }
				if (ParticleName == "gamma") { GammaCB->SetValue(true); }
			}
		}
	}
	VisFile.close();

}
//for run.mac
void ConfigureWindow::ParseRunFile()
{
	std::ifstream RunFile;
	std::string FileDir = SystemManager.CurrentProjectDir + "/Macros/run.mac";
	RunFile.open(FileDir);
	if (!RunFile.is_open())
	{
		wxMessageBox(wxT("run.mac file for this project cannot be opened due to an unknown reason, please consult to the developers of the project."));
		wxMessageBox(wxT("File not found: " + FileDir));
		return;
	}

	//now read the file line by line and perform checks in each line
	while (!RunFile.eof()) //to read until the end of the file
	{
		std::string Token;
		std::getline(RunFile, Token);
		//now parse this line into words according to whitespaces
		//note that line elements with whitespaces, but in quotation marks are not
		//supported by this parser
		std::istringstream StringParser(Token);
		std::string Element;
		std::vector<std::string> RunInformation;
		while (StringParser >> Element)
		{
			RunInformation.push_back(Element);
		}

		//now check if the line has a certain value or prefix
		if (!RunInformation.empty())
		{
			std::string Prefix = RunInformation.at(0); //the first element seperated by whitespace of a line is taken as prefix
			//style
			if (Prefix == "/run/beamOn")
			{
				std::string NofParticlesToBeShot = RunInformation.at(1);
				BeamOnTimesEdit->SetValue(std::stoi(NofParticlesToBeShot));
			}
		}
	}
	RunFile.close();
}
//for gun.mac
void ConfigureWindow::ParseGunFile()
{
	std::ifstream GunFile;
	std::string FileDir = SystemManager.CurrentProjectDir + "/Macros/gun.mac";
	GunFile.open(FileDir);
	if (!GunFile.is_open())
	{
		wxMessageBox(wxT("gun.mac file for this project cannot be opened due to an unknown reason, please consult to the developers of the project."));
		wxMessageBox(wxT("File not found: " + FileDir));
		return;
	}

	//now read the file line by line and perform checks in each line
	while (!GunFile.eof()) //to read until the end of the file
	{
		std::string Token;
		std::getline(GunFile, Token);
		//now parse this line into words according to whitespaces
		//note that line elements with whitespaces, but in quotation marks are not
		//supported by this parser
		std::istringstream StringParser(Token);
		std::string Element;
		std::vector<std::string> GunInformation;
		while (StringParser >> Element)
		{
			GunInformation.push_back(Element);
		}

		//now check if the line has a certain value or prefix
		if (!GunInformation.empty())
		{
			std::string Prefix = GunInformation.at(0); //the first element seperated by whitespace of a line is taken as prefix
			//style
			if (Prefix == "/gun/particle")
			{
				std::string GunParticle = GunInformation.at(1);
				//select according to the order defined in wxchoicevector
				if (GunParticle == "e-") { GunParticleEdit->SetSelection(0); }
				if (GunParticle == "e+") { GunParticleEdit->SetSelection(1); }
				if (GunParticle == "proton") { GunParticleEdit->SetSelection(2); }
				if (GunParticle == "neutron") { GunParticleEdit->SetSelection(3); }
				if (GunParticle == "gamma") { GunParticleEdit->SetSelection(4); }
			}
			else if (Prefix == "/gun/energy")
			{
				double EnergyInKev = std::stod(GunInformation.at(1));
				GunEnergyEdit->SetValue(EnergyInKev);
			}
			else if (Prefix == "/gun/direction")
			{
				double XDir = std::stod(GunInformation.at(1));
				double YDir = std::stod(GunInformation.at(2));
				double ZDir = std::stod(GunInformation.at(3));
				GunDirX->SetValue(XDir);
				GunDirY->SetValue(YDir);
				GunDirZ->SetValue(ZDir);

			}
			else if (Prefix == "/gun/position")
			{
				double XPos = std::stod(GunInformation.at(1));
				double YPos = std::stod(GunInformation.at(2));
				double ZPos = std::stod(GunInformation.at(3));
				GunPosX->SetValue(XPos);
				GunPosY->SetValue(YPos);
				GunPosZ->SetValue(ZPos);
			}
		}
	}
	GunFile.close();
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


void ConfigureWindow::GunEnergyEditf(wxSpinDoubleEvent& event) {
	Push("config");
	Push("gun");
	Push("setEnergy");
	Push(std::to_string(GunEnergyEdit->GetValue()) + " keV"); //capitalization be carful
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
		Push("wireframe");
		break;
	case 1:
		Push("surface");
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
	int index = VisFilterEdit->GetSelection();

	switch (index) {
	case 0:
		Push("drawByParticleID");
		//wxLogMessage(wxT("done!"));
		break;
	case 1:
		Push("drawByCharge");
		break;
	}
	Push("vis.mac");
	SystemManager.Conclude();
}
void ConfigureWindow::ProtonCBf(wxCommandEvent& event) {
	bool IsChecked = ProtonCB->GetValue();
	if (IsChecked)
	{
		Push("config");
		Push("vis");
		Push("filter");
		Push("addparticle");
		//spec
		Push("proton");
		Push("vis.mac");
		SystemManager.Conclude();
	}
	else //unchecked
	{
		Push("config");
		Push("vis");
		Push("filter");
		Push("removeparticle");
		//spec
		Push("proton");
		Push("vis.mac");
		SystemManager.Conclude();
	}
}
void ConfigureWindow::NeutronCBf(wxCommandEvent& event) {
	bool IsChecked = NeutronCB->GetValue();
	if (IsChecked)
	{
		Push("config");
		Push("vis");
		Push("filter");
		Push("addparticle");
		//spec
		Push("neutron");
		Push("vis.mac");
		SystemManager.Conclude();
	}
	else //unchecked
	{
		Push("config");
		Push("vis");
		Push("filter");
		Push("removeparticle");
		//spec
		Push("neutron");
		Push("vis.mac");
		SystemManager.Conclude();
	}
}
void ConfigureWindow::PositronCBf(wxCommandEvent& event) {
	bool IsChecked = PositronCB->GetValue();
	if (IsChecked)
	{
		Push("config");
		Push("vis");
		Push("filter");
		Push("addparticle");
		//spec
		Push("e+");
		Push("vis.mac");
		SystemManager.Conclude();
	}
	else //unchecked
	{
		Push("config");
		Push("vis");
		Push("filter");
		Push("removeparticle");
		//spec
		Push("e+");
		Push("vis.mac");
		SystemManager.Conclude();
	}
}
void ConfigureWindow::ElectronCBf(wxCommandEvent& event) {
	bool IsChecked = ElectronCB->GetValue();
	if (IsChecked)
	{
		Push("config");
		Push("vis");
		Push("filter");
		Push("addparticle");
		//spec
		Push("e-");
		Push("vis.mac");
		SystemManager.Conclude();
	}
	else //unchecked
	{
		Push("config");
		Push("vis");
		Push("filter");
		Push("removeparticle");
		//spec
		Push("e-");
		Push("vis.mac");
		SystemManager.Conclude();
	}
}
void ConfigureWindow::GammaCBf(wxCommandEvent& event) {
	bool IsChecked = GammaCB->GetValue();
	if (IsChecked)
	{
		Push("config");
		Push("vis");
		Push("filter");
		Push("addparticle");
		//spec
		Push("gamma");
		Push("vis.mac");
		SystemManager.Conclude();
	}
	else //unchecked
	{
		Push("config");
		Push("vis");
		Push("filter");
		Push("removeparticle");
		//spec
		Push("gamma");
		Push("vis.mac");
		SystemManager.Conclude();
	}
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



