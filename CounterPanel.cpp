#include "CounterPanel.h"

wxBEGIN_EVENT_TABLE(CounterPanel, wxWindow)
	EVT_BUTTON(RefreshViewer_ID, ApplyChanges) //link button with ID 10001 to method OnButtonClicked
	EVT_BUTTON(CreateNew_ID, FCreateNew)
	EVT_CHOICE(SelectBody_ID, SelectBodyf)
	EVT_SPINCTRLDOUBLE(XValue_ID, TranslateBodies)
	EVT_SPINCTRLDOUBLE(YValue_ID, TranslateBodies)
	EVT_SPINCTRLDOUBLE(ZValue_ID, TranslateBodies)
	EVT_SPINCTRLDOUBLE(Euler1Value_ID, RotateBodies)
	EVT_SPINCTRLDOUBLE(Euler2Value_ID, RotateBodies)
	EVT_SPINCTRLDOUBLE(Euler3Value_ID, RotateBodies)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(ModifyCounter, wxFrame)
	EVT_BUTTON(counterbox_Create_ID, counterbox_modifyf)
wxEND_EVENT_TABLE()




CounterPanel::CounterPanel(wxFrame* MainFrame, GLGeometryViewer* GeometryViewer) : wxPanel(MainFrame, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0)
{
	ViewerAccess = GeometryViewer;

	CreateNew = new wxButton(this, CreateNew_ID, "Modify Dimensions", wxDefaultPosition, wxSize(150, 50));
	//now aligned horizontally
	XValue = new wxSpinCtrlDouble(this, XValue_ID, wxEmptyString, wxDefaultPosition, wxSize(70, 20), wxSP_ARROW_KEYS, -100000, 100000, 0, 0.0001, "wxSpinCtrl");
	YValue = new wxSpinCtrlDouble(this, YValue_ID, wxEmptyString, wxDefaultPosition, wxSize(70, 20), wxSP_ARROW_KEYS, -100000, 100000, 0, 0.0001, "wxSpinCtrl");
	ZValue = new wxSpinCtrlDouble(this, ZValue_ID, wxEmptyString, wxDefaultPosition, wxSize(70, 20), wxSP_ARROW_KEYS, -100000, 100000, 0, 0.0001, "wxSpinCtrl");
	//continue vertical
	wxButton* ApplyTranslation = new wxButton(this, RefreshViewer_ID, "Apply Translation", wxDefaultPosition, wxSize(150, 50));
	//now aligned horizontally
	Euler1Value = new wxSpinCtrlDouble(this, Euler1Value_ID, wxEmptyString, wxDefaultPosition, wxSize(50, 20), wxSP_ARROW_KEYS, -90, 90, 0, 0.0001, "wxSpinCtrl");
	Euler2Value = new wxSpinCtrlDouble(this, Euler2Value_ID, wxEmptyString, wxDefaultPosition, wxSize(50, 20), wxSP_ARROW_KEYS, -90, 90, 0, 0.0001, "wxSpinCtrl");
	Euler3Value = new wxSpinCtrlDouble(this, Euler3Value_ID, wxEmptyString, wxDefaultPosition, wxSize(50, 20), wxSP_ARROW_KEYS, -90, 90, 0, 0.0001, "wxSpinCtrl");
	wxButton* ApplyRotation = new wxButton(this, RefreshViewer_ID, "Apply Rotation", wxDefaultPosition, wxSize(150, 50));
	//continue vertical
	RefreshViewer = new wxButton(this, RefreshViewer_ID, "Refresh Viewer", wxDefaultPosition, wxSize(150, 50));
	//DeleteBody = new wxButton(this, DeleteBody_ID, "Delete Body", wxDefaultPosition, wxSize(150, 50));

	//add them to sizers
	TranslationSizer->Add(x, 0, wxALL | wxALIGN_CENTER_VERTICAL, 2);
	TranslationSizer->Add(XValue, 1, wxALL | wxALIGN_CENTER_VERTICAL, 2);
	TranslationSizer->Add(y, 0, wxALL | wxALIGN_CENTER_VERTICAL, 2);
	TranslationSizer->Add(YValue, 0, wxALL | wxALIGN_CENTER_VERTICAL, 2);
	TranslationSizer->Add(z, 0, wxALL | wxALIGN_CENTER_VERTICAL, 2);
	TranslationSizer->Add(ZValue, 0, wxALL | wxALIGN_CENTER_VERTICAL, 2);

	
	RotationSizer->Add(Euler1, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
	RotationSizer->Add(Euler1Value, 1, wxALIGN_CENTER_VERTICAL | wxALL, 2);
	RotationSizer->Add(Euler2, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
	RotationSizer->Add(Euler2Value, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
	RotationSizer->Add(Euler3, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
	RotationSizer->Add(Euler3Value, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);

	//display the counts from previous run
	wxString counts = ParseExperimentalDataFile();
	CountsFromLastRun = new wxStaticText(this, wxID_ANY, "Count from last run: " + counts, wxDefaultPosition, wxDefaultSize, 0);


	VerticalLayout->Add(PickExisting, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	VerticalLayout->Add(CreateNew, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	VerticalLayout->Add(SetPosition, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	VerticalLayout->Add(TranslationSizer, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	VerticalLayout->Add(ApplyTranslation, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	VerticalLayout->Add(SetRotation, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	VerticalLayout->Add(RotationSizer, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	VerticalLayout->Add(ApplyRotation, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	VerticalLayout->Add(RefreshViewer, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	VerticalLayout->Add(CountsFromLastRun, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	this->SetSizer(VerticalLayout);

	//finally, parse the geometry file and the run macro to initialize the settings
	if (SystemManager.Project_isOpen)
		ParseGeometryAndRunFile(); //this will fill in ObjectList and initialize settings

}

Counter::Counter(double in_posX, double in_posY, double in_posZ, double in_sizeX,
	double in_sizeY,
	double in_sizeZ,
	double in_euler1,
	double in_euler2,
	double in_euler3) {
	//BodyType = in_BodyType;
	posX = in_posX;
	posY = in_posY;
	posZ = in_posZ;
	sizeX = in_sizeX;
	sizeY = in_sizeY;
	sizeZ = in_sizeZ;
	euler1 = in_euler1;
	euler2 = in_euler2;
	euler3 = in_euler3;
}


wxString CounterPanel::ParseExperimentalDataFile() //it returns the counts as a string
{
	//we first locate the file:
	//we are going to read both files:
	//g4geom.txt
	std::ifstream ExperimentDataFile;
	std::string FileDir = SystemManager.CurrentProjectDir + "/ExperimentData/ExperimentData.txt";
	ExperimentDataFile.open(FileDir);
	if (!ExperimentDataFile.is_open())
	{
		wxMessageBox(wxT("ExperimentData file for this project cannot be opened due to an unknown reason, please consult to the developers of the project."));
		wxMessageBox(wxT("File not found: " + FileDir));
		return wxString("Not Found");
	}

	std::vector<std::vector<std::string>> DataTable;

	while (!ExperimentDataFile.eof()) //to read until the end of the file
	{
		std::string Token;
		std::getline(ExperimentDataFile, Token);

		//check if the Token is a comment line starting with #, and ignore it if that is the case
		if (Token.empty()) { continue; }
		else if (Token.at(0) == '#') { continue; }
		
		//otherwise, interpret it as a csv line:
		//the code is modified from: https://stackoverflow.com/questions/1120140/how-can-i-read-and-parse-csv-files-in-c
		std::vector<std::string> result;
		std::stringstream lineStream(Token);
		std::string cell;
		while (std::getline(lineStream, cell, ','))
		{
			result.push_back(cell);
		}
		// This checks for a trailing comma with no data after it.
		if (!lineStream && cell.empty())
		{
			// If there was a trailing comma then add an empty element.
			result.push_back("");
		}

		//and then we push back the row
		DataTable.push_back(result);

	}

	//we currently look for only this parameter (add more in the future)
	std::string TotalCounts = ""; //the last parameter of the csv line
	//it is the last column of the first row of DataTable
	std::vector <std::string> FirstRow = DataTable.at(0);
	TotalCounts = FirstRow.at(FirstRow.size() - 1);
	return wxString(TotalCounts);
}


//Parses the geometry and run files, checks for conflicts and warns user, and initializes controls
Counter CounterPanel::ParseGeometryAndRunFile()
{

	//we are going to search the run.mac and g4geom.txt files for the tokens containing 
	//counterbox, store the two as seperate "bodies" in ObjectList, and then check whether any
	//of their parameters are conflicting (and in that case raising a warning)
	//"bodies" are stored as Body objects (instances of the "Body" class)



	//we are going to read both files:
	//g4geom.txt
	std::ifstream GeometryFile;
	std::string FileDir = SystemManager.CurrentProjectDir + "/Geometry/g4geom.txt";
	GeometryFile.open(FileDir);
	if (!GeometryFile.is_open())
	{
		wxMessageBox(wxT("Geometry file for this project cannot be opened due to an unknown reason, please consult to the developers of the project."));
		wxMessageBox(wxT("File not found: " + FileDir));
		return Counter(0, 0, 0, 0, 0, 0);
	}
	//run.mac
	std::ifstream RunFile;
	FileDir = SystemManager.CurrentProjectDir + "/Macros/run.mac";
	RunFile.open(FileDir);
	if (!RunFile.is_open())
	{
		wxMessageBox(wxT("run.mac file for this project cannot be opened due to an unknown reason, please consult to the developers of the project."));
		wxMessageBox(wxT("File not found: " + FileDir));
		return Counter(0, 0, 0, 0, 0, 0);
	}

	//first delete the old object list:
	ObjectList.clear();
	//read the file line by line, and then parse the line into a vector:
	//we will be looking for the following information:
	//read according to the geant4 text format


	//now parsing g4geom.txt for counterbox
	double geom_posX;
	double geom_posY;
	double geom_posZ;
	double geom_sizeX = 0;
	double geom_sizeY = 0;
	double geom_sizeZ = 0;
	bool CounterBoxFoundFlag = false;
	while (!GeometryFile.eof() && !CounterBoxFoundFlag) //to read until the end of the file
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

		//now check if the line has the correct prefix containing "counterbox"
		
		if (!BodyInformation.empty())
		{
			if (BodyInformation.at(0) == ":VOLU" && BodyInformation.at(1) == "counterbox")
			{
				geom_sizeX = std::stod(BodyInformation.at(3));
				geom_sizeY = std::stod(BodyInformation.at(4));
				geom_sizeZ = std::stod(BodyInformation.at(5));
			}
			else if (BodyInformation.at(0) == ":PLACE" && BodyInformation.at(1) == "counterbox")
			{
				geom_posZ = std::stod(BodyInformation.at(BodyInformation.size() - 1));
				geom_posY = std::stod(BodyInformation.at(BodyInformation.size() - 2));
				geom_posX = std::stod(BodyInformation.at(BodyInformation.size() - 3));
				//now that the place volume is found record it and exit loop via flag
				CounterBoxFoundFlag = true;
				ObjectList.push_back(Counter(geom_posX, geom_posY, geom_posZ, geom_sizeX, geom_sizeY, geom_sizeZ));
			}
			else if (BodyInformation.at(0) == ":ROTM" && BodyInformation.at(1) == "R_counterbox")
			{
				//currently, parsing for the geometry file is not implemented for these parameters
			}
		}
	}
	//notifications
	if (!CounterBoxFoundFlag)
	{
		wxMessageBox(wxT("Couldn't find any volume named \'counterbox'\ in g4geom.txt"));
	}

	//now parsing run.mac
	double run_posX = 0;
	double run_posY = 0;
	double run_posZ = 0;
	double run_sizeX = 0;
	double run_sizeY = 0;
	double run_sizeZ = 0;
	double run_Euler1 = 0;
	double run_Euler2 = 0;
	double run_Euler3 = 0;
	CounterBoxFoundFlag = false;
	bool ScorerCreated = false;
	while (!RunFile.eof() /* && !CounterBoxFoundFlag*/) //to read until the end of the file
	{
		std::string Token;
		std::getline(RunFile, Token);
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

		//check if a scorer mesh is created
		if (!BodyInformation.empty() && BodyInformation.at(0) == "/score/create/boxMesh") { ScorerCreated = true; }

		//now check the appropiate prefixes if there is a scorer mesh present:
		if (!BodyInformation.empty() && ScorerCreated)
		{
			if (BodyInformation.at(0) == "/score/mesh/boxSize")
			{
				run_sizeX = std::stod(BodyInformation.at(1));
				run_sizeY = std::stod(BodyInformation.at(2));
				run_sizeZ = std::stod(BodyInformation.at(3));
				CounterBoxFoundFlag = true; //though it may lack lines about translation and rotation if the file is corrupted
				//still, no exceptions are thrown, since all parameters are initialized to zero
			}
			else if (BodyInformation.at(0) == "/score/mesh/translate/xyz")
			{
				run_posZ = std::stod(BodyInformation.at(3));
				run_posY = std::stod(BodyInformation.at(2));
				run_posX = std::stod(BodyInformation.at(1));
			
				//now that the place volume is found record it and exit loop via flag
			}
			else if (BodyInformation.at(0) == "/score/mesh/rotate/rotateY")
			{
				run_Euler1 = std::stod(BodyInformation.at(1));
			}
			else if (BodyInformation.at(0) == "/score/mesh/rotate/rotateX")
			{
				run_Euler2 = std::stod(BodyInformation.at(1));
			}
			else if (BodyInformation.at(0) == "/score/mesh/rotate/rotateZ")
			{
				run_Euler3 = std::stod(BodyInformation.at(1));
			}
		}
	}
	//finally
	if (CounterBoxFoundFlag)
	{
		ObjectList.push_back(Counter(run_posX, run_posY, run_posZ, run_sizeX, run_sizeY, run_sizeZ, run_Euler1, run_Euler2, run_Euler3));
	}

	//notifications
	if (!ScorerCreated)
	{
		wxMessageBox(wxT("Couldn't find any scorer mesh in run.mac"));
	}
	if (!CounterBoxFoundFlag)
	{
		wxMessageBox(wxT("Couldn't find any placement for scorer mesh in run.mac"));
	}
	if (ObjectList.size() != 2)
	{
		wxMessageBox(wxT("ObjectList Size is not 2, may signal a bug."));
	}

	//finally, compare the two data to check if there is a mismatch, and raise a notification
	Counter Data_g4geom = ObjectList.at(0);
	Counter Data_run = ObjectList.at(1);
	
	if (!(
		Data_g4geom.posX == Data_run.posX && 
		Data_g4geom.posY == Data_run.posY &&
		Data_g4geom.posZ == Data_run.posZ &&
		Data_g4geom.sizeX == Data_run.sizeX &&
		Data_g4geom.sizeY == Data_run.sizeY &&
		Data_g4geom.sizeZ == Data_run.sizeZ
		//currently no cross checks for euler angles between files
		))
	{
		wxMessageBox(wxT("There is a mismatch between the counterbox data in run.mac and g4geom.txt, project might be corrupt."));
		//for debugging
		//wxMessageBox(wxString(std::to_string(Data_g4geom.posX)) + " " + wxString(std::to_string(Data_run.posX)));
	}
	
	//run.mac paramters have priority, as they determine the dynamics of the simulation
	//set the position values of spin controls
	XValue->SetValue(Data_run.posX);
	YValue->SetValue(Data_run.posY);
	ZValue->SetValue(Data_run.posZ);


	Euler1Value->SetValue(Data_run.euler1);
	Euler2Value->SetValue(Data_run.euler2);
	Euler3Value->SetValue(Data_run.euler3);


	GeometryFile.close();
	RunFile.close();

	return Data_run;
}

void CounterPanel::FCreateNew(wxCommandEvent& event)
{
	//it should open a new window with choices of objects to add, and a naming bar
	Counter Data_run = ParseGeometryAndRunFile();
	ModifyCounter* Modifier = new ModifyCounter(Data_run.sizeX, Data_run.sizeY, Data_run.sizeZ);
	Modifier->Show();
}

//translation and rotation
void CounterPanel::TranslateBodies(wxSpinDoubleEvent& event)
{
	
	//gather data from input fields and manifest them onto the g4geom.txt file
	Push("counterbox");
	Push("translate");

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
void CounterPanel::RotateBodies(wxSpinDoubleEvent& event)
{

	//gather data from input fields and manifest them onto the g4geom.txt file
	Push("counterbox");
	Push("rotate");

	//dimensions
	std::string alpha = std::to_string(Euler1Value->GetValue());
	std::string beta  = std::to_string(Euler2Value->GetValue());
	std::string gamma = std::to_string(Euler3Value->GetValue());
	Push(alpha);
	Push(beta);
	Push(gamma);
	//end gathering data and manifest modification on g4geom.txt
	SystemManager.Conclude();
}


void CounterPanel::ApplyChanges(wxCommandEvent& event) //To developer: this function has a slight memory leak, fix ASAP!
{
	//Recompilation and Reset of the viewer to show changed state:
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

void CounterPanel::SelectBodyf(wxCommandEvent& event)
{
	//Body BodySelected = ObjectList.at(SelectBody->GetSelection());
	//these need to be moved to parse geometry file
	//XValue->SetValue(BodySelected.posX);
	//YValue->SetValue(BodySelected.posY);
	//ZValue->SetValue(BodySelected.posZ);
}

//now for newobject
ModifyCounter::ModifyCounter(double currentxSize, double currentySize, double currentzSize) 
	: wxFrame(nullptr, wxID_ANY, "VisualGEANT4-Modify Counter", /*setting initial position*/ wxPoint(50, 50), wxSize(600, 400))
{

	//BEGIN: counterbox Page
	//counterbox_Name = new wxStaticText(counterboxPage, wxID_ANY, wxT("Name"), wxDefaultPosition, wxDefaultSize, 0);
	//counterbox_NameEdit = new wxTextCtrl(counterboxPage, counterbox_NameEdit_ID);
	counterbox_xLength = new wxStaticText(counterboxPage, wxID_ANY, wxT("xLength"), wxDefaultPosition, wxDefaultSize, 0);
	counterbox_xLengthEdit = new wxSpinCtrl(counterboxPage, counterbox_xLengthEdit_ID, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100000, 10, "wxSpinCtrl");
	counterbox_yWidth = new wxStaticText(counterboxPage, wxID_ANY, wxT("yWidth"), wxDefaultPosition, wxDefaultSize, 0);
	counterbox_yWidthEdit = new wxSpinCtrl(counterboxPage, counterbox_yWidthEdit_ID, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100000, 10, "wxSpinCtrl");
	counterbox_zHeight = new wxStaticText(counterboxPage, wxID_ANY, wxT("zHeight"), wxDefaultPosition, wxDefaultSize, 0);
	counterbox_zHeightEdit = new wxSpinCtrl(counterboxPage, counterbox_zHeightEdit_ID, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100000, 10, "wxSpinCtrl");
	counterbox_Modify = new wxButton(counterboxPage, counterbox_Create_ID, "Apply Changes", wxDefaultPosition, wxSize(150, 50));
	//wxStaticText* WarningTextGeiger = new wxStaticText(counterboxPage, wxID_ANY, "Warning: GEANT4 allows only 16 counters to be created, beware!");

	

	wxBoxSizer* counterboxPageSizer = new wxBoxSizer(wxVERTICAL);
	//counterboxPageSizer->Add(counterbox_Name, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10); //these 0's are essential, since they allow these elements to be "stacked" without any prior scaling
	//counterboxPageSizer->Add(counterbox_NameEdit, 0, wxALIGN_LEFT | wxLEFT, 10);
	counterboxPageSizer->Add(counterbox_xLength, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10); //these 0's are essential, since they allow these elements to be "stacked" without any prior scaling
	counterboxPageSizer->Add(counterbox_xLengthEdit, 0, wxALIGN_LEFT | wxLEFT, 10);
	counterboxPageSizer->Add(counterbox_yWidth, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10);
	counterboxPageSizer->Add(counterbox_yWidthEdit, 0, wxALIGN_LEFT | wxLEFT, 10);
	counterboxPageSizer->Add(counterbox_zHeight, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10);
	counterboxPageSizer->Add(counterbox_zHeightEdit, 0, wxALIGN_LEFT | wxLEFT, 10);
	counterboxPageSizer->Add(counterbox_Modify, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10);
	counterboxPageSizer->Add(WarningText(counterboxPage), 0, wxALIGN_LEFT | wxLEFT, 10);
	//counterboxPageSizer->Add(WarningTextGeiger, 0, wxALIGN_LEFT | wxLEFT | wxTOP, 10);
	counterboxPage->SetSizer(counterboxPageSizer);
	//END: counterbox Page


	//now create the notebook/pages
	ConfigureOptions = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxSize(500, 500), wxNB_MULTILINE);
	ConfigureOptions->AddPage(counterboxPage, "Modify: Geiger Counter");
	ConfigureOptions->Layout();

	//finally add a background sizer
	wxBoxSizer* BackgroundSizer = new wxBoxSizer(wxVERTICAL); //doesn't matter, can be horizontal too
	BackgroundSizer->Add(ConfigureOptions, 1, wxEXPAND | wxALL);
	SetSizer(BackgroundSizer);


	//Now set these values to their original values
	counterbox_xLengthEdit->SetValue(currentxSize);
	counterbox_yWidthEdit->SetValue(currentySize);
	counterbox_zHeightEdit->SetValue(currentzSize);

}

void ModifyCounter::Push(std::string arg)
{
	SystemManager.Kernel_args.push_back(arg);
}

void CounterPanel::Push(std::string arg)
{
	SystemManager.Kernel_args.push_back(arg);
}

//functions bound to every single control:
//MODIFY
void ModifyCounter::counterbox_modifyf(wxCommandEvent& event) {

	//counterboxes cannot be created, they can only be modified. thus, this method only "recreates" the counterbox by resizing it

	Push("counterbox");
	Push("resize");

	//dimensions
	std::string x = std::to_string(counterbox_xLengthEdit->GetValue());
	std::string y = std::to_string(counterbox_yWidthEdit->GetValue());
	std::string z = std::to_string(counterbox_zHeightEdit->GetValue());
	Push(x);
	Push(y);
	Push(z);

	SystemManager.Conclude();

	wxMessageBox(wxT("Changes Applied. Please click \'Refresh Viewer'\ to see them."));
}