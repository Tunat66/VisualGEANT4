#include "GeometryPanel.h"

wxBEGIN_EVENT_TABLE(GeometryPanel, wxWindow)
	EVT_BUTTON(RefreshViewer_ID, ApplyChanges) //link button with ID 10001 to method OnButtonClicked
	EVT_BUTTON(CreateNew_ID, FCreateNew)
wxEND_EVENT_TABLE()


GeometryPanel::GeometryPanel(wxFrame* MainFrame, GLGeometryViewer* GeometryViewer) : wxPanel(MainFrame, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0)
{
	ViewerAccess = GeometryViewer;
	
	//the layout (from top to bottom)
	std::vector<wxString> DummyChoices = {"ahcjbhknklml", "b"};
	SelectBody = new wxChoiceVector(this, wxID_ANY, DummyChoices);;
	CreateNew = new wxButton(this, 10001, "Create New Body", wxDefaultPosition, wxSize(150, 50));
	//now aligned horizontally
	XValue = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -100000, 100000, 0, "wxSpinCtrl");
	YValue = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -100000, 100000, 0, "wxSpinCtrl");
	ZValue = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -100000, 100000, 0, "wxSpinCtrl");
	//continue vertical
	//now aligned horizontally
	Euler1Value = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -6.28, 6.28, 0, "wxSpinCtrl");
	Euler2Value = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -6.28, 6.28, 0, "wxSpinCtrl");
	Euler3Value = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -6.28, 6.28, 0, "wxSpinCtrl");
	//continue vertical
	wxButton* RefreshViewer = new wxButton(this, RefreshViewer_ID, "Refresh Viewer", wxDefaultPosition, wxSize(150, 50));

	//add them to sizers
	TranslationSizer->Add(x, 0, wxALL | wxALIGN_CENTER_VERTICAL, 2);
	TranslationSizer->Add(XValue, 1, wxALL | wxALIGN_CENTER_VERTICAL, 2);
	TranslationSizer->Add(y, 0, wxALL | wxALIGN_CENTER_VERTICAL, 2);
	TranslationSizer->Add(YValue, 0, wxALL | wxALIGN_CENTER_VERTICAL, 2);
	TranslationSizer->Add(z, 0, wxALL | wxALIGN_CENTER_VERTICAL, 2);
	TranslationSizer->Add(ZValue, 0, wxALL | wxALIGN_CENTER_VERTICAL, 2);

	RotationSizer->Add(Euler1, 1, wxALIGN_CENTER_VERTICAL | wxALL, 2);
	RotationSizer->Add(Euler1Value, 1, wxALIGN_CENTER_VERTICAL | wxALL, 2);
	RotationSizer->Add(Euler2, 1, wxALIGN_CENTER_VERTICAL | wxALL, 2);
	RotationSizer->Add(Euler2Value, 1, wxALIGN_CENTER_VERTICAL | wxALL, 2);
	RotationSizer->Add(Euler3, 1, wxALIGN_CENTER_VERTICAL | wxALL, 2);
	RotationSizer->Add(Euler3Value, 1, wxALIGN_CENTER_VERTICAL | wxALL, 2);

	VerticalLayout->Add(PickExisting, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	VerticalLayout->Add(SelectBody, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	VerticalLayout->Add(CreateNew, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	VerticalLayout->Add(SetPosition, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	VerticalLayout->Add(TranslationSizer, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	VerticalLayout->Add(SetRotation, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	VerticalLayout->Add(RotationSizer, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	VerticalLayout->Add(RefreshViewer, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	this->SetSizer(VerticalLayout);

}

void GeometryPanel::FCreateNew(wxCommandEvent& event)
{
	//it should open a new window with choices of objects to add, and a naming bar

}


void GeometryPanel::ApplyChanges(wxCommandEvent& event) //To developer: this function has a slight memory leak, fix ASAP!
{
	//gather data from entry fields

	
	
	//AFTER HERE: Recompilationd and Reset of the viewer to show changed state:
	if (SystemManager.Project_isOpen)
	{
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