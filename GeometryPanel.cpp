#include "GeometryPanel.h"

wxBEGIN_EVENT_TABLE(GeometryPanel, wxWindow)
	EVT_BUTTON(RefreshViewer_ID, ApplyChanges) //link button with ID 10001 to method OnButtonClicked
	EVT_BUTTON(CreateNew_ID, FCreateNew)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(NewObject, wxFrame)
	EVT_BUTTON(box_Create_ID, box_createf)
	EVT_BUTTON(sphere_Create_ID, sphere_createf)
wxEND_EVENT_TABLE()


GeometryPanel::GeometryPanel(wxFrame* MainFrame, GLGeometryViewer* GeometryViewer) : wxPanel(MainFrame, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0)
{
	ViewerAccess = GeometryViewer;
	
	if(SystemManager.Project_isOpen)
		ParseGeometryFile(); //this will fill in ObjectList
	//now just extract the names from ObjectList and put them into ObjectNameList
	for (int i = 0; i < ObjectList.size(); i++)
	{
		Body AccessedBody = ObjectList.at(i);
		wxString wxBodyName(AccessedBody.BodyName);
		ObjectNameList.push_back(wxBodyName);
	}

	//the layout (from top to bottom)
	//std::vector<wxString> DummyChoices = {"ahcjbhknklml", "b"};
	SelectBody = new wxChoiceVector(this, wxID_ANY, ObjectNameList);
	CreateNew = new wxButton(this, CreateNew_ID, "Create New Body", wxDefaultPosition, wxSize(150, 50));
	//now aligned horizontally
	XValue = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -100000, 100000, 0, "wxSpinCtrl");
	YValue = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -100000, 100000, 0, "wxSpinCtrl");
	ZValue = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -100000, 100000, 0, "wxSpinCtrl");
	//continue vertical
	//now aligned horizontally
	//Euler1Value = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -6.28, 6.28, 0, "wxSpinCtrl");
	//Euler2Value = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -6.28, 6.28, 0, "wxSpinCtrl");
	//Euler3Value = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -6.28, 6.28, 0, "wxSpinCtrl");
	//continue vertical
	wxButton* RefreshViewer = new wxButton(this, RefreshViewer_ID, "Refresh Viewer", wxDefaultPosition, wxSize(150, 50));

	//add them to sizers
	TranslationSizer->Add(x, 0, wxALL | wxALIGN_CENTER_VERTICAL, 2);
	TranslationSizer->Add(XValue, 1, wxALL | wxALIGN_CENTER_VERTICAL, 2);
	TranslationSizer->Add(y, 0, wxALL | wxALIGN_CENTER_VERTICAL, 2);
	TranslationSizer->Add(YValue, 0, wxALL | wxALIGN_CENTER_VERTICAL, 2);
	TranslationSizer->Add(z, 0, wxALL | wxALIGN_CENTER_VERTICAL, 2);
	TranslationSizer->Add(ZValue, 0, wxALL | wxALIGN_CENTER_VERTICAL, 2);

	/*
	RotationSizer->Add(Euler1, 1, wxALIGN_CENTER_VERTICAL | wxALL, 2);
	RotationSizer->Add(Euler1Value, 1, wxALIGN_CENTER_VERTICAL | wxALL, 2);
	RotationSizer->Add(Euler2, 1, wxALIGN_CENTER_VERTICAL | wxALL, 2);
	RotationSizer->Add(Euler2Value, 1, wxALIGN_CENTER_VERTICAL | wxALL, 2);
	RotationSizer->Add(Euler3, 1, wxALIGN_CENTER_VERTICAL | wxALL, 2);
	RotationSizer->Add(Euler3Value, 1, wxALIGN_CENTER_VERTICAL | wxALL, 2);*/

	VerticalLayout->Add(PickExisting, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	VerticalLayout->Add(SelectBody, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	VerticalLayout->Add(CreateNew, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	VerticalLayout->Add(SetPosition, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	VerticalLayout->Add(TranslationSizer, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	//VerticalLayout->Add(SetRotation, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	//VerticalLayout->Add(RotationSizer, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	VerticalLayout->Add(RefreshViewer, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	this->SetSizer(VerticalLayout);

}

void GeometryPanel::ParseGeometryFile() 
{
	/*
	std::string line;
	std::ifstream Geo
	while (std::getline(, line))
	{
		std::istringstream iss(line);
		int a, b;
		if (!(iss >> a >> b)) { break; } // error

		// process pair (a,b)
	}*/
	
	//we are going to read file word by word, and add to the list of existing "bodies"
	//"bodies" are stored as Body objects (instances of the "Body" class)

	std::ifstream GeometryFile;
	std::string FileDir = SystemManager.CurrentProjectDir + "/Geometry/g4geom.txt";
	GeometryFile.open(FileDir);
	if (!GeometryFile.is_open())
	{
		wxMessageBox(wxT("Geometry file for this project cannot be opened due to an unknown reason, please consult to the developers of the project."));
		wxMessageBox(wxT("File not found: " + FileDir));
		return;
	}

	//read the file line by line, and then parse the line into a vector:
	//we will be looking for the following information:
	//read according to the geant4 text format
	std::string BodyName;
	BodyTypes BodyType;
	double posX;
	double posY;
	double posZ;
	std::string BodyMaterial;
	//std::string NewVolumeToken; //this will alert the parser that a new volume is encountered when it turns equal to ":VOLU"
	while (!GeometryFile.eof()) //to read until the end of the file
	{
		std::string Token;
		std::getline(GeometryFile, Token);
		//now parse this line into words according to whitespaces
		//note that line elements with whitespaces, but in quotation marks are not
		//supported by this parser
		std::istringstream StringParser(Token);
		std::string Element;
		std::vector<std::string> BodyInformation;
		while (StringParser >> Element)
		{
			BodyInformation.push_back(Element);
		}

		//now check if the line is a :VOLU or :PLACE line
		bool PlacedBodyFoundFlag = false;
		if (!BodyInformation.empty())
		{
			if (BodyInformation.at(0) == ":VOLU")
			{
				BodyMaterial = BodyInformation.at(BodyInformation.size() - 1);
			}
			else if (BodyInformation.at(0) == ":PLACE")
			{
				BodyName = BodyInformation.at(1);
				posZ = std::stod(BodyInformation.at(BodyInformation.size() - 1));
				posY = std::stod(BodyInformation.at(BodyInformation.size() - 2));
				posX = std::stod(BodyInformation.at(BodyInformation.size() - 3));
				PlacedBodyFoundFlag = true;
			}

			//if found, add the body to ObjectList
			if (PlacedBodyFoundFlag)
			{
				ObjectList.push_back(Body(BodyName, posX, posY, posZ)); //add material and body type in later versions
			}
		}
	}
	GeometryFile.close();
}

void GeometryPanel::FCreateNew(wxCommandEvent& event)
{
	//it should open a new window with choices of objects to add, and a naming bar
	NewObject* NewObjectAdder = new NewObject();
	NewObjectAdder->Show();
}


void GeometryPanel::ApplyChanges(wxCommandEvent& event) //To developer: this function has a slight memory leak, fix ASAP!
{
	//gather data from input fields and manifest them onto the g4geom.txt file
	
	
	//AFTER HERE: Recompilationd and Reset of the viewer to show changed state:
	if (SystemManager.Project_isOpen)
	{
		//OLD VERSION: body addition required program to be recompiled, hence this bit of code was included:
		//SetStatusText("Compiling Project...");
		SystemManager.Kernel_args.push_back("build");

		//read the G4DIR and push back to kernel args
		std::string G4DIR;
		std::ifstream Reader("G4DIR.txt");
		Reader >> G4DIR;
		SystemManager.Kernel_args.push_back(G4DIR);

		//after that concludes, instruct GLGeometryViewer to load the new object:

		//finally
		SystemManager.Conclude();
		//SetStatusText("Compile Complete.");
		//END OLD VERSION

		//wait for the setup.obj file to be created (it takes time) before the following is executed
		//there should be a proper winAPI way of doing this but I'd rather do it the villager way: (dear winAPI: wtf is a LPCSTR I mean)
		//I don't want to deal with LPCSTR types!

		std::string ProjectDir = SystemManager.CurrentProjectDir;

		//first remove the old file (so that meshconv creates a new one, does not append to the old one!)
		std::filesystem::path Existing(ProjectDir + "/setup.obj");
		if (std::filesystem::exists(Existing))
			std::filesystem::remove(Existing);

		//creates a wavefront file setup.obj to view the experiment geometry, using 
		//the file g4_00.wrl created by the compiled Geant4 executables
		//std::string LastArg = ProjectDir + "\\testopener.exe";
		std::string LastArg = ProjectDir + "\\meshconvOpener.exe";
		std::string ChildDir = ProjectDir;
		wxString dir2(LastArg);
		wxLogMessage(dir2);

		//char cmdArgs[] = "meshconv.exe -c obj g4_00.wrl";
		//all app executables are saved as main.exe, otherwise an error will be raised (this issue may be solved in future versions)
		//allocation to C type string and then casting to wchar_t (the type used by CreateProcess)
		char* AppDirectory = new char[LastArg.length() + 1];
		strcpy(AppDirectory, LastArg.c_str());
		//system(AppDirectory);
		wchar_t* wtext = new wchar_t[100];
		mbstowcs(wtext, AppDirectory, strlen(AppDirectory) + 1);//Plus null
		LPWSTR ptr = wtext;
		delete[] wtext;
		char* ChildDirectory = new char[LastArg.length() + 1];
		strcpy(ChildDirectory, ChildDir.c_str());
		wchar_t* wtext2 = new wchar_t[100];
		mbstowcs(wtext2, ChildDirectory, strlen(ChildDirectory) + 1);//Plus null
		LPWSTR dir = wtext2;
		delete[] wtext2;

		// additional information
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		// set the size of the structures
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		// start the program up, test 

		if (CreateProcess(ptr,   // the path
			NULL,        // Command line
			NULL,           // Process handle not inheritable
			NULL,           // Thread handle not inheritable
			FALSE,          // Set handle inheritance to FALSE
			0,              // No creation flags
			NULL,           // Use parent's environment block
			dir,            // Use local directory as starting directory 
			&si,            // Pointer to STARTUPINFO structure
			&pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
		)) {
			wxLogMessage("Proc. successful!");
		}

		//try to open the object only when the process is completed:
		//to explain this weird syntax, refer to: 
		//https://stackoverflow.com/questions/11725958/getexitcodeprocess-return-1-when-process-is-not-yet-finished
		DWORD exitCode = 0; //this allocates memory, whose address is passed as:
		if (GetExitCodeProcess(pi.hProcess, &exitCode) == FALSE)
		{
			// Handle GetExitCodeProcess failure
			wxLogMessage("GetExitCodeProcess failed for meshconvOpener.exe");
		}

		//another wait
		bool flag = false;
		while (!flag) {
			if (exitCode != STILL_ACTIVE) { flag = true; }
		}

		//another wait
		/*
		bool SetupFileExists = false;
		std::filesystem::path SetupFilePath(SystemManager.CurrentProjectDir + "\\setup.obj");
		while (!SetupFileExists)
		{
			if (std::filesystem::exists(SetupFilePath))
				SetupFileExists = true;
		}*/
		//Aha! it needs not only for the file to be created, but the content to be written on it!
		//sorry, gotta use winAPI

		//ViewerAccess->refresh_view_dbg();
		//ViewerAccess->load_obj();

		// Close process and thread handles. 
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);


		bool SetupFileExists = false;
		std::filesystem::path SetupFilePath(SystemManager.CurrentProjectDir + "\\setup.obj");
		while (!SetupFileExists)
		{
			if (std::filesystem::exists(SetupFilePath))
				SetupFileExists = true;
		}


		wxMessageBox(wxT("Changes Saved. (Potential bug, do not turn off immediately)"));
		ViewerAccess->refresh_view();
		ViewerAccess->load_obj();

		delete[] AppDirectory;
		delete[] ChildDirectory;

	}

}


//now for newobject
NewObject::NewObject() : wxFrame(nullptr, wxID_ANY, "VisualGEANT4-New Object", /*setting initial position*/ wxPoint(50, 50), wxSize(600, 350))
{
	/*OLD CODE
	
	std::vector<wxString> ObjectTypes{ "Box", "Sphere" };
	Name = new wxStaticText(Backdrop, wxID_ANY, wxT("Name (no spaces allowed):"), wxDefaultPosition, wxDefaultSize, 0);
	NameEdit = new wxTextCtrl();
	Type = Name = new wxStaticText(Backdrop, wxID_ANY, wxT("Type: "), wxDefaultPosition, wxDefaultSize, 0);
	TypeEdit = new wxChoiceVector(Backdrop, wxID_ANY, ObjectTypes);
	

	wxBoxSizer* EmbedBackdrop = new wxBoxSizer(wxVERTICAL);
	EmbedBackdrop->Add(Backdrop, 1, wxALL | wxEXPAND);
	this->SetSizer(EmbedBackdrop);

	wxBoxSizer* StackingSizer = new wxBoxSizer(wxVERTICAL);
	StackingSizer->Add(Name, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10);
	StackingSizer->Add(NameEdit, 0, wxALIGN_LEFT | wxLEFT, 10);
	StackingSizer->Add(Type, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10);
	StackingSizer->Add(TypeEdit, 0, wxALIGN_LEFT | wxLEFT, 10);
	Backdrop->SetSizer(StackingSizer);
	

	*/
	//wxButton* Test = new wxButton(this, 20001);

	//add a tabbed window for the user to access different options, add this to a panel
	//wxPanel* Background = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200, 100));
	//Background->SetTransparent(1);
	

	//BEGIN: Cube Page
	std::vector<wxString> Materials{ "Air", "Water" , "Iron", "Aluminum", "Copper", "Polystyrene", "Plexiglass", "Polyethylene", "Gold"};

	box_Name = new wxStaticText(cubePage, wxID_ANY, wxT("Name"), wxDefaultPosition, wxDefaultSize, 0);
	box_NameEdit = new wxTextCtrl(cubePage, box_NameEdit_ID);
	box_Material = new wxStaticText(cubePage, wxID_ANY, wxT("Material"), wxDefaultPosition, wxDefaultSize, 0);
	box_MaterialEdit = new wxChoiceVector(cubePage, box_MaterialEdit_ID, Materials);
	box_xLength = new wxStaticText(cubePage, wxID_ANY, wxT("xLength"), wxDefaultPosition, wxDefaultSize, 0);
	box_xLengthEdit = new wxSpinCtrl(cubePage, box_xLengthEdit_ID, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100000, 10, "wxSpinCtrl");
	box_yWidth = new wxStaticText(cubePage, wxID_ANY, wxT("yWidth"), wxDefaultPosition, wxDefaultSize, 0);
	box_yWidthEdit = new wxSpinCtrl(cubePage, box_yWidthEdit_ID, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100000, 10, "wxSpinCtrl");
	box_zHeight = new wxStaticText(cubePage, wxID_ANY, wxT("zHeight"), wxDefaultPosition, wxDefaultSize, 0);
	box_zHeightEdit = new wxSpinCtrl(cubePage, box_zHeightEdit_ID, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100000, 10, "wxSpinCtrl");
	box_Create = new wxButton(cubePage, box_Create_ID, "Create New Cube", wxDefaultPosition, wxSize(150, 50));


	wxBoxSizer* CubePageSizer = new wxBoxSizer(wxVERTICAL);
	CubePageSizer->Add(box_Name, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10); //these 0's are essential, since they allow these elements to be "stacked" without any prior scaling
	CubePageSizer->Add(box_NameEdit, 0, wxALIGN_LEFT | wxLEFT, 10);
	CubePageSizer->Add(box_Material, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10);
	CubePageSizer->Add(box_MaterialEdit, 0, wxALIGN_LEFT | wxLEFT, 10);
	CubePageSizer->Add(box_xLength, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10); //these 0's are essential, since they allow these elements to be "stacked" without any prior scaling
	CubePageSizer->Add(box_xLengthEdit, 0, wxALIGN_LEFT | wxLEFT, 10);
	CubePageSizer->Add(box_yWidth, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10);
	CubePageSizer->Add(box_yWidthEdit, 0, wxALIGN_LEFT | wxLEFT, 10);
	CubePageSizer->Add(box_zHeight, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10);
	CubePageSizer->Add(box_zHeightEdit, 0, wxALIGN_LEFT | wxLEFT, 10);
	CubePageSizer->Add(box_Create, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10);
	cubePage->SetSizer(CubePageSizer);
	//END: Cube Page

	
	//BEGIN: Sphere Page
	sphere_Name = new wxStaticText(spherePage, wxID_ANY, wxT("Name"), wxDefaultPosition, wxDefaultSize, 0);
	sphere_NameEdit = new wxTextCtrl(spherePage, sphere_NameEdit_ID);
	sphere_Material = new wxStaticText(spherePage, wxID_ANY, wxT("Material"), wxDefaultPosition, wxDefaultSize, 0);
	sphere_MaterialEdit = new wxChoiceVector(spherePage, sphere_MaterialEdit_ID, Materials);
	sphere_Radius = new wxStaticText(spherePage, wxID_ANY, wxT("Radius"), wxDefaultPosition, wxDefaultSize, 0);
	sphere_RadiusEdit = new wxSpinCtrl(spherePage, sphere_RadiusEdit_ID, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100000, 10, "wxSpinCtrl");
	sphere_Create = new wxButton(spherePage, sphere_Create_ID, "Create New Sphere", wxDefaultPosition, wxSize(150, 50));


	wxBoxSizer* SpherePageSizer = new wxBoxSizer(wxVERTICAL);
	SpherePageSizer->Add(sphere_Name, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10); //these 0's are essential, since they allow these elements to be "stacked" without any prior scaling
	SpherePageSizer->Add(sphere_NameEdit, 0, wxALIGN_LEFT | wxLEFT, 10);
	SpherePageSizer->Add(sphere_Material, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10);
	SpherePageSizer->Add(sphere_MaterialEdit, 0, wxALIGN_LEFT | wxLEFT, 10);
	SpherePageSizer->Add(sphere_Radius, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10); 
	SpherePageSizer->Add(sphere_RadiusEdit, 0, wxALIGN_LEFT | wxLEFT, 10);
	SpherePageSizer->Add(sphere_Create, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10);
	spherePage->SetSizer(SpherePageSizer);

	//END: Sphere Page

	//now create the notebook/pages
	ConfigureOptions = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxSize(500, 500), wxNB_MULTILINE);
	ConfigureOptions->AddPage(cubePage, "Add: Cube");
	ConfigureOptions->AddPage(spherePage, "Add: Sphere");
	ConfigureOptions->Layout();

	//finally add a background sizer
	wxBoxSizer* BackgroundSizer = new wxBoxSizer(wxVERTICAL); //doesn't matter, can be horizontal too
	BackgroundSizer->Add(ConfigureOptions, 1, wxEXPAND | wxALL);
	SetSizer(BackgroundSizer);


}

void NewObject::Push(std::string arg)
{
	SystemManager.Kernel_args.push_back(arg);
}

//functions bound to every single control:
void NewObject::box_createf(wxCommandEvent& event) {
	
	Push("geom");
	Push("add");
	Push("box");

	//now get the data
	//name
	wxString NewBodyName(box_NameEdit->GetValue());
	std::string NewBodyName_arg = std::string(NewBodyName.mb_str());
	Push(NewBodyName_arg);

	//location: DEFAULT AT ORIGIN
	Push("0");
	Push("0");
	Push("0");

	//dimensions
	std::string x = std::to_string(box_xLengthEdit->GetValue());
	std::string y = std::to_string(box_yWidthEdit->GetValue());
	std::string z = std::to_string(box_zHeightEdit->GetValue());
	Push(x);
	Push(y);
	Push(z);

	//material
	int material_index = box_MaterialEdit->GetSelection();
	//remember
	//std::vector<wxString> Materials{ "Air", "Water" , "Iron", "Aluminum", "Copper", "Polystyrene", "Plexiglass", "Polyethylene", "Gold" };

	switch (material_index) 
	{
	case 0:
		Push("G4_AIR");
		break;
	case 1:
		Push("G4_WATER");
		break;
	case 2:
		Push("G4_Fe");
		break;
	case 3:
		Push("G4_Al");
		break;
	case 4:
		Push("G4_Cu");
		break;
	case 5:
		Push("G4_POLYSTYRENE");
		break;
	case 6:
		Push("G4_PLEXIGLASS");
		break;
	case 7:
		Push("G4_POLYETHYLENE");
		break;
	case 8:
		Push("G4_Au");
		break;
	//add later!!!!!!! BURADA KALDIN!!
	}

	//add a parser to the g4geom.txt file to check upon it!!!!
	wxMessageBox(wxT("Box: \'" + NewBodyName_arg + "\' is succesfully created! Please click \'Refresh Viewer'\ to see it."));

	SystemManager.Conclude();
}

void NewObject::sphere_createf(wxCommandEvent& event) {

	Push("geom");
	Push("add");
	Push("sphere");

	//now get the data
	//name
	wxString NewBodyName(sphere_NameEdit->GetValue());
	std::string NewBodyName_arg = std::string(NewBodyName.mb_str());
	Push(NewBodyName_arg);

	//location: DEFAULT AT ORIGIN
	Push("0");
	Push("0");
	Push("0");

	//dimensions
	std::string radius = std::to_string(sphere_RadiusEdit->GetValue());
	Push(radius);

	//material
	int material_index = sphere_MaterialEdit->GetSelection();
	//remember
	//std::vector<wxString> Materials{ "Air", "Water" , "Iron", "Aluminum", "Copper", "Polystyrene", "Plexiglass", "Polyethylene", "Gold" };

	switch (material_index)
	{
	case 0:
		Push("G4_AIR");
		break;
	case 1:
		Push("G4_WATER");
		break;
	case 2:
		Push("G4_Fe");
		break;
	case 3:
		Push("G4_Al");
		break;
	case 4:
		Push("G4_Cu");
		break;
	case 5:
		Push("G4_POLYSTYRENE");
		break;
	case 6:
		Push("G4_PLEXIGLASS");
		break;
	case 7:
		Push("G4_POLYETHYLENE");
		break;
	case 8:
		Push("G4_Au");
		break;
		//add later!!!!!!! BURADA KALDIN!!
	}

	//add a parser to the g4geom.txt file to check upon it!!!!
	wxMessageBox(wxT("Sphere: \'" + NewBodyName_arg + "\' is succesfully created! Please click \'Refresh Viewer'\ to see it."));

	SystemManager.Conclude();
}
