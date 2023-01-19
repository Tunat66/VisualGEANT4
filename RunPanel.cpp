#include "RunPanel.h"

wxBEGIN_EVENT_TABLE(RunPanel, wxWindow)
	EVT_BUTTON(10001, ApplyChanges) //link button with ID 10001 to method OnButtonClicked
	EVT_BUTTON(10003, RunProject)
	EVT_BUTTON(10004, ConfigureProject)
wxEND_EVENT_TABLE()


RunPanel::RunPanel(wxFrame* MainFrame, GLGeometryViewer* GeometryViewer) : wxPanel(MainFrame, wxID_ANY, wxDefaultPosition,  wxDefaultSize, 0)
{
	RightPanelSizer = new wxBoxSizer(wxVERTICAL);
	button_RunProject = new wxButton(this, 10003, "Run Project", wxDefaultPosition, wxSize(150, 50));
	button_ApplyChanges = new wxButton(this, 10001, "Apply Changes", wxDefaultPosition, wxSize(150, 50));
	button_ConfigureProject = new wxButton(this, 10004, "Configure Project", wxDefaultPosition, wxSize(150, 50));
	RightPanelSizer->Add(button_RunProject, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 20);
	RightPanelSizer->Add(button_ApplyChanges, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 20);
	RightPanelSizer->Add(button_ConfigureProject, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 20);
	this->SetSizer(RightPanelSizer);

	ViewerAccess = GeometryViewer;
	
	//now attach itself to the right panel
	//wxBoxSizer* Attach = new wxBoxSizer(wxHORIZONTAL);
	//Attach->Add(RightPanel, 0, wxEXPAND | wxALL, 0);
}



void RunPanel::RunProject(wxCommandEvent& event)
{
	if (SystemManager.Project_isOpen)
	{
		SystemManager.Kernel_args.push_back("open");
		SystemManager.Conclude();
		//SetStatusText(SystemManager.Kernel_args.at(1)); possible bug source
		//SetStatusText("Project Running...");
	}
	else
		SetStatusText("Please Select a Project before running.");
}


void RunPanel::ApplyChanges(wxCommandEvent& event)
{
	if (SystemManager.Project_isOpen)
	{
		SetStatusText("Compiling Project...");
		SystemManager.Kernel_args.push_back("build");

		//read the G4DIR and push back to kernel args
		std::string G4DIR;
		std::ifstream Reader("G4DIR.txt");
		Reader >> G4DIR;
		SystemManager.Kernel_args.push_back(G4DIR);

		//after that concludes, instruct GLGeometryViewer to load the new object:

		//finally
		SystemManager.Conclude();
		SetStatusText("Compile Complete.");

		//wait for the setup.obj file to be created (it takes time) before the following is executed
		//there should be a proper winAPI way of doing this but I'd rather do it the villager way: (dear winAPI: wtf is a LPCSTR I mean)
		//I don't want to deal with LPCSTR types!
		
		std::string ProjectDir = SystemManager.CurrentProjectDir;

		//first remove the old file (so that meshconv creates a new one, does not append to the old one!)
		//this bit is evil: it causes the app to die by deleting setup.obj
		//std::filesystem::path Existing(ProjectDir + "/setup.obj");
		//if (std::filesystem::exists(Existing))
			//std::filesystem::remove(Existing);

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
			if (exitCode != STILL_ACTIVE) { flag = true;}
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
	else
		SetStatusText("Please Select a Project before running.");
}

void RunPanel::ConfigureProject(wxCommandEvent& event)
{
	ConfigureWindow* NewInstance = new ConfigureWindow();
	NewInstance->Show();
}

void RunPanel::SetStatusText(std::string Value)
{
	StatusText = Value;
}

std::string RunPanel::GetStatusText()
{
	return StatusText;
}

RunPanel::~RunPanel()
{
	this->SetSizer(NULL);
	button_RunProject->Destroy();
	button_ApplyChanges->Destroy();
	button_ConfigureProject->Destroy();
}

