#include "GeometryPanel.h"

wxBEGIN_EVENT_TABLE(GeometryPanel, wxWindow)
	EVT_BUTTON(RefreshViewer_ID, ApplyChanges) //link button with ID 10001 to method OnButtonClicked
	EVT_BUTTON(CreateNew_ID, FCreateNew)
	EVT_CHOICE(SelectBody_ID, SelectBodyf)
	EVT_BUTTON(DeleteBody_ID, DeleteBodyf)
	EVT_SPIN(XValue_ID, TranslateBodies)
	EVT_SPIN(YValue_ID, TranslateBodies)
	EVT_SPIN(ZValue_ID, TranslateBodies)
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
	SelectBody = new wxChoiceVector(this, SelectBody_ID, ObjectNameList);
	//Found an essential bug: if this field is left empty, it causes all object names to be deleted in the g4geom.txt file
	//solution: initialize this field
	SelectBody->SetSelection(0);

	CreateNew = new wxButton(this, CreateNew_ID, "Create New Body", wxDefaultPosition, wxSize(150, 50));
	//now aligned horizontally
	XValue = new wxSpinCtrl(this, XValue_ID, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -100000, 100000, 0, "wxSpinCtrl");
	YValue = new wxSpinCtrl(this, YValue_ID, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -100000, 100000, 0, "wxSpinCtrl");
	ZValue = new wxSpinCtrl(this, ZValue_ID, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -100000, 100000, 0, "wxSpinCtrl");
	//continue vertical
	//now aligned horizontally
	//Euler1Value = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -6.28, 6.28, 0, "wxSpinCtrl");
	//Euler2Value = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -6.28, 6.28, 0, "wxSpinCtrl");
	//Euler3Value = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -6.28, 6.28, 0, "wxSpinCtrl");
	//continue vertical
	RefreshViewer = new wxButton(this, RefreshViewer_ID, "Refresh Viewer", wxDefaultPosition, wxSize(150, 50));
	DeleteBody = new wxButton(this, DeleteBody_ID, "Delete Body", wxDefaultPosition, wxSize(150, 50));

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
	VerticalLayout->Add(DeleteBody, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
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

void GeometryPanel::TranslateBodies(wxSpinEvent& event)
{
	//gather data from input fields and manifest them onto the g4geom.txt file
	Push("geom");
	Push("translate");

	//name
	int TranslatedBodyIndex = SelectBody->GetSelection();
	wxString TranslatedBodyName = SelectBody->GetString(TranslatedBodyIndex);
	std::string TranslatedBodyName_arg = std::string(TranslatedBodyName.mb_str());
	Push(TranslatedBodyName_arg);

	//dimensions
	std::string x = std::to_string(XValue->GetValue());
	std::string y = std::to_string(YValue->GetValue());
	std::string z = std::to_string(ZValue->GetValue());
	Push(x);
	Push(y);
	Push(z);
	//end gathering data and manifest modification on g4geom.txt
	SystemManager.Conclude();
}


void GeometryPanel::ApplyChanges(wxCommandEvent& event) //To developer: this function has a slight memory leak, fix ASAP!
{

	//now reallocate the ObjectList, thus updating it
	if (SystemManager.Project_isOpen)
		ParseGeometryFile(); //this will fill in ObjectList
	//now just extract the names from ObjectList and put them into ObjectNameList
	for (int i = 0; i < ObjectList.size(); i++)
	{
		Body AccessedBody = ObjectList.at(i);
		wxString wxBodyName(AccessedBody.BodyName);
		ObjectNameList.push_back(wxBodyName);
	}
	
	//AFTER HERE: Recompilationd and Reset of the viewer to show changed state:
	if (SystemManager.Project_isOpen)
	{
		//OLD VERSION: body addition required program to be recompiled, hence this bit of code was included:
		//SetStatusText("Compiling Project...");
		/*
		SystemManager.Kernel_args.push_back("build");

		//read the G4DIR and push back to kernel args
		std::string G4DIR;
		std::ifstream Reader("G4DIR.txt");
		Reader >> G4DIR;
		SystemManager.Kernel_args.push_back(G4DIR);

		//after that concludes, instruct GLGeometryViewer to load the new object:

		//finally
		SystemManager.Conclude();*/
		//SetStatusText("Compile Complete.");
		//END OLD VERSION

		std::string ProjectDir = SystemManager.CurrentProjectDir;
		std::filesystem::path Existing(ProjectDir + "/g4_00.wrl");
		if (std::filesystem::exists(Existing))
			std::filesystem::remove(Existing);

		//creates a wavefront file setup.obj to view the experiment geometry, using the file g4_00.wrl created by the compiled Geant4 executables
		std::string MainPath = ProjectDir + "/main.exe"; //former LastArg
		std::string MainDir = ProjectDir; //former ChildDir
		char cmdArgs[] = "main.exe vis2.mac";

		//all app executables are saved as main.exe, otherwise an error will be raised (this issue may be solved in future versions)
		//allocation to C type string and then casting to wchar_t (the type used by CreateProcess)
		char* AppDirectory = new char[MainPath.length() + 1];
		strcpy(AppDirectory, MainPath.c_str());
		//system(AppDirectory);
		wchar_t* wtext = new wchar_t[100];
		mbstowcs(wtext, AppDirectory, strlen(AppDirectory) + 1);//Plus null
		LPWSTR ptr = wtext;
		delete[] wtext;
		char* ChildDirectory = new char[MainPath.length() + 1];
		strcpy(ChildDirectory, MainDir.c_str());
		wchar_t* wtext2 = new wchar_t[100];
		mbstowcs(wtext2, ChildDirectory, strlen(ChildDirectory) + 1);//Plus null
		LPWSTR dir = wtext2;
		delete[] wtext2;
		wchar_t wtext3[100];
		mbstowcs(wtext3, cmdArgs, strlen(cmdArgs) + 1);//Plus null
		LPWSTR cmd = wtext3;


		// additional information
		STARTUPINFO si_wrlcreate;
		PROCESS_INFORMATION pi_wrlcreate;

		// set the size of the structures
		ZeroMemory(&si_wrlcreate, sizeof(si_wrlcreate));
		si_wrlcreate.cb = sizeof(si_wrlcreate);
		ZeroMemory(&pi_wrlcreate, sizeof(pi_wrlcreate));

		// start the program to create wrl files up
		CreateProcess(ptr,   // the path
			cmd,        // Command line
			NULL,           // Process handle not inheritable
			NULL,           // Thread handle not inheritable
			FALSE,          // Set handle inheritance to FALSE
			0,              // No creation flags
			NULL,           // Use parent's environment block
			dir,           // Use project directory as starting directory 
			&si_wrlcreate,            // Pointer to STARTUPINFO structure
			&pi_wrlcreate             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
		);
		

		//wxLogMessage("wrl creation complete!");
		delete[] AppDirectory;
		delete[] ChildDirectory;
		//wait for the wrl file to be created before running meshconv on it:
		WaitForSingleObject(pi_wrlcreate.hProcess, INFINITE);

		//NOW: update the setup.obj file in accordance to the contents of the wrl file
		//first remove the old file (so that meshconv creates a new one, does not append to the old one!)
		//std::filesystem::path SetupObjExisting(ProjectDir + "/setup.obj");
		//if (std::filesystem::exists(SetupObjExisting))
			//std::filesystem::remove(SetupObjExisting);

		//creates a wavefront file setup.obj to view the experiment geometry, using 
		//std::string LastArg = ProjectDir + "\\testopener.exe";
		std::string OpenerPath = ProjectDir + "\\meshconv.exe"; //former LastArg
		std::string OpenerDir = ProjectDir; //former ChildDir
		wxString dir2(OpenerPath);
		wxLogMessage(dir2);

		//char cmdArgs[] = "meshconv.exe -c obj g4_00.wrl";
		//all app executables are saved as main.exe, otherwise an error will be raised (this issue may be solved in future versions)
		//allocation to C type string and then casting to wchar_t (the type used by CreateProcess)
		AppDirectory = new char[OpenerPath.length() + 1];
		strcpy(AppDirectory, OpenerPath.c_str());
		//system(AppDirectory);
		wtext = new wchar_t[100];
		mbstowcs(wtext, AppDirectory, strlen(AppDirectory) + 1);//Plus null
		ptr = wtext;
		delete[] wtext;
		ChildDirectory = new char[OpenerPath.length() + 1];
		strcpy(ChildDirectory, OpenerDir.c_str());
		wtext2 = new wchar_t[100];
		mbstowcs(wtext2, ChildDirectory, strlen(ChildDirectory) + 1);//Plus null
		LPWSTR Openerdir = wtext2;
		delete[] wtext2;

		char cmdArgs2[] = "meshconv.exe -c obj -o setup g4_00.wrl";
		wchar_t wtext4[100];
		mbstowcs(wtext4, cmdArgs2, strlen(cmdArgs2) + 1);//Plus null
		cmd = wtext4;

		// additional information
		STARTUPINFO si_meshconvopener;
		PROCESS_INFORMATION pi_meshconvopener;

		// set the size of the structures
		ZeroMemory(&si_meshconvopener, sizeof(si_meshconvopener));
		si_meshconvopener.cb = sizeof(si_meshconvopener);
		ZeroMemory(&pi_meshconvopener, sizeof(pi_meshconvopener));

		// start the program up, test 

		if (CreateProcess(ptr,   // the path
			cmd,        // Command line
			NULL,           // Process handle not inheritable
			NULL,           // Thread handle not inheritable
			FALSE,          // Set handle inheritance to FALSE
			0,              // No creation flags
			NULL,           // Use parent's environment block
			Openerdir,            // Use local directory as starting directory 
			&si_meshconvopener,            // Pointer to STARTUPINFO structure
			&pi_meshconvopener             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
		)) {
			wxLogMessage("Proc. successful!");
		}

		//try to open the object only when the process is completed:
		//to explain this weird syntax, refer to: 
		//https://stackoverflow.com/questions/11725958/getexitcodeprocess-return-1-when-process-is-not-yet-finished
		/*
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

		
		//wait until the creation of setup.obj completes, and meshconvopener.exe shuts down
		WaitForSingleObject(pi_meshconvopener.hProcess, INFINITE);
		

		//now validate if setup.obj file exists
		std::filesystem::path SetupFilePath(SystemManager.CurrentProjectDir + "\\setup.obj");
		
		//this caused program to froze
		/*while (!SetupFileExists)
		{
			if (std::filesystem::exists(SetupFilePath))
				SetupFileExists = true;
		}*/
		//instead: use this
		if (!std::filesystem::exists(SetupFilePath))
		{
			wxMessageBox(wxT("setup.obj file not found or deleted, VisualGEANT4 may crash!"));
		}

		//wxMessageBox(wxT("Changes Saved. (Potential bug, do not turn off immediately)"));
		ViewerAccess->refresh_view();
		ViewerAccess->load_obj();

		delete[] AppDirectory;
		delete[] ChildDirectory;


		//finally: close process and thread handles of the two processes created within this method
		CloseHandle(pi_wrlcreate.hProcess);
		CloseHandle(pi_wrlcreate.hThread);
		CloseHandle(pi_meshconvopener.hProcess);
		CloseHandle(pi_meshconvopener.hThread);

	}

}

void GeometryPanel::SelectBodyf(wxCommandEvent& event)
{
	Body BodySelected = ObjectList.at(SelectBody->GetSelection());
	XValue->SetValue(BodySelected.posX);
	YValue->SetValue(BodySelected.posY);
	ZValue->SetValue(BodySelected.posZ);
}

void GeometryPanel::DeleteBodyf(wxCommandEvent& event)
{
	//here is a guard: projects with no bodies are not allowed, thus if such an attempt it raised, it is stopped here
	int NumberOfBodies = ObjectList.size();
	if (NumberOfBodies == 1)
	{
		wxMessageBox(wxT("Projects with no bodies are not allowed, you cannot delete the last remaining body!"));
		return;
	}
	
	Push("geom");
	SystemManager.Kernel_args.push_back("delete");

	//the order of appearence of bodies in g4geom.txt MATTERS! they are referenced by their order index, not their name!
	int DeletedBodyIndex = SelectBody->GetSelection();
	wxString DeletedBodyName = SelectBody->GetString(DeletedBodyIndex);

	//first delete the body from the lists
	ObjectList.erase(ObjectList.begin() + DeletedBodyIndex);
	ObjectNameList.erase(ObjectNameList.begin() + DeletedBodyIndex);
	SelectBody->Delete(DeletedBodyIndex);

	//then, pass the command to delete the body
	std::string DeletedBodyName_arg = std::string(DeletedBodyName.mb_str());
	SystemManager.Kernel_args.push_back(DeletedBodyName_arg);
	SystemManager.Conclude();
	wxMessageBox(wxT("Deleted Body: " + DeletedBodyName_arg + " Click \'Refresh Viewer\' to see the effect."));


}


//now for newobject
NewObject::NewObject() : wxFrame(nullptr, wxID_ANY, "VisualGEANT4-New Object", /*setting initial position*/ wxPoint(50, 50), wxSize(600, 400))
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
	std::vector<wxString> Materials{ "Air", "Water" , "Iron", "Aluminum", "Copper", "Polystyrene", "Plexiglass", "Polyethylene", "Gold" };

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
	CubePageSizer->Add(WarningText(cubePage), 0, wxALIGN_LEFT | wxLEFT, 10);
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
	//CubePageSizer->Add(WarningText, 0, wxALIGN_LEFT | wxLEFT, 10);
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

void GeometryPanel::Push(std::string arg)
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
