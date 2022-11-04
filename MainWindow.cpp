#include "MainWindow.h"
#include <wx/filename.h>
//#include "ConfigWindow.h"

//list all events here

//add all button functionalities here so that they are implemented
wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
	//file menu	
	EVT_MENU(ID_NewProject, MainWindow::OnNewProject)
	EVT_MENU(ID_OpenProject, MainWindow::OnOpenProject)
	EVT_MENU(ID_Save, MainWindow::OnSave)
	EVT_MENU(ID_SaveAs, MainWindow::OnSaveAs)
	EVT_MENU(wxID_EXIT, MainWindow::OnExit)
	//command menu
	EVT_MENU(ID_CommandLine, MainWindow::OnCommandLine)
	EVT_TOOL(20001, RunPanelShow)
	EVT_TOOL(20002, GeometryPanelShow)
	EVT_TOOL(20003, SourcePanelShow)
wxEND_EVENT_TABLE()


MainWindow::MainWindow() : wxFrame(nullptr, wxID_ANY, "VisualGEANT4", /*setting initial position*/ wxPoint(30, 30), wxSize(1200, 700))
{
//MENU BAR ELEMENTS
	
	//MENU: File
	wxMenu* menuFile = new wxMenu;
	menuFile->Append(ID_NewProject, "&New...\tCtrl-N", //wxWidgets can naturally sense the shortcut from this string!!
		"Create a new project."); //3rd argument is shown in status bar
	menuFile->Append(ID_OpenProject, "&Open...\t", "Open an existing project by navigating to its directory.");
	menuFile->Append(ID_Save, "&Save...\tCtrl-S",
		"Save changes in project.");
	menuFile->Append(ID_SaveAs, "&Save As...\t",
		"Save changes in project.");
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT);
		
	//MENU: Help
	wxMenu* menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT);


	//MENU: Control
	wxMenu* menuControl = new wxMenu;
	menuControl->Append(ID_CommandLine, "&Command Line", "Command Line interface used for troubleshooting and testing.");
	
	
//IMPLEMENT MENU BAR
	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	menuBar->Append(menuHelp, "&Help");
	menuBar->Append(menuControl, "&Control");
	SetMenuBar(menuBar);
	
//status bar, display current app state/process to the user
	CreateStatusBar();
	SetStatusText("Please Select A Project...");

//LEFT PANEL: This will be mostly for embedding geometry view:
	LeftPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200,100));
	LeftPanel->SetBackgroundColour(wxColor(205, 205, 205));
	//for testing purposes:
	GeometryViewer = new GLGeometryViewer(LeftPanel);
	//SetupViewer = new G4OpenGLImmediateXViewer();
	wxBoxSizer* ViewerSizer = new wxBoxSizer(wxVERTICAL);
	ViewerSizer->Add(GeometryViewer, 1, wxEXPAND | wxALL);
	LeftPanel->SetSizer(ViewerSizer);

	
//RIGHT PANEL: This is the panel where buttons and controls are located, controlled by a toolbar	
	//default window is the run window
	RightPanel = new RunPanel(this);
	RightPanel->SetBackgroundColour(wxColor(205, 205, 205));
	
//TOOLBAR, which changes the RightPanel:	
	wxToolBar* RightPanelNav = this->CreateToolBar(wxTB_HORIZONTAL, wxID_ANY);;
	wxToolBarToolBase* RunTool = RightPanelNav->AddTool(20001, wxT("Run"), wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);
	wxToolBarToolBase* GeomteryTool = RightPanelNav->AddTool(20002, wxT("Geometry"), wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);;
	wxToolBarToolBase* SourceTool = RightPanelNav->AddTool(20003, wxT("Particle Source"), wxNullBitmap, wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL);;
	RightPanelNav->Realize();
	
	
//MAIN WINDOW: Sizers to incorporate both panels into the window:
	MainWindowSizer->Add(LeftPanel, 3, wxEXPAND | wxALL, 5); //args are proportion, borderedsides, bordervalue(px)
	MainWindowSizer->Add(RightPanel, 1, wxEXPAND | wxRIGHT | wxBOTTOM | wxTOP, 5);
	SetSizer(MainWindowSizer);
		
	/*m_btn1 = new wxButton(this, 10001, "Click Here", wxPoint(10, 10), wxSize(150, 50));
	/*m_txt1 = new wxTextCtrl(this, wxID_ANY, "This is my app", wxPoint(10, 70), wxSize(300, 30));
	m_list1 = new wxListBox(this, wxID_ANY, wxPoint(10, 110), wxSize(300, 300));
	new_window_btn = new wxButton(this, 10002, "Array Of Buttons", wxPoint(10 + 150 + 10, 10), wxSize(150, 50));*/
}


MainWindow::~MainWindow()
{
	//deletes are not included since wxWidgets performs garbage collection for objects created under its classes
}

//MENU BAR METHODS
void MainWindow::OnExit(wxCommandEvent& event)
{
	Close(true);
}
void MainWindow::OnAbout(wxCommandEvent& event)
{
	wxMessageBox("VisualGEANT4\nVersion: 1.0.0\n2022",
		"About Hello World", wxOK | wxICON_INFORMATION);
}
void MainWindow::OnNewProject(wxCommandEvent& event)
{
	
	//NewDirPicker* Picker = new NewDirPicker();
	//Picker->Show();
	static const wxChar* FILETYPES = _T(
		"VisualGEANT4 files |*.vg4|"
		"All files|*.*"
	);

	wxFileDialog* openFileDialog =
		new wxFileDialog(this, _("Create VisualGEANT4 Project"), "", "", FILETYPES,
			wxFD_SAVE, wxDefaultPosition);

	if (openFileDialog->ShowModal() == wxID_OK)
	{
		wxString ProjectPath;
		ProjectPath.append(openFileDialog->GetDirectory());
		wxString ProjectName = openFileDialog->GetFilename();
		wxString ProjectFolderName = ProjectPath + "/" + ProjectName;

		//create a new folder to put the project files in it
		std::filesystem::path New = std::string(ProjectFolderName.mb_str());
		std::filesystem::create_directories(New);

		//create a vg4proj file inside this new folder, opened by the user when accessing the project later
		std::ofstream ProjectFile(std::string(ProjectFolderName.mb_str()) + "/" + std::string(ProjectName.mb_str()) + "proj");

		//copy the contents of the "default project" into this "new project"

		
		//lastly, set it as the current project dir:
		SystemManager.CurrentProjectDir = ProjectPath.mb_str(); //convert wxString to std::string

	}

	SystemManager.Project_isOpen = true; //some methods will run if only this is true;

	//delete the status text "please select a project"
	SetStatusText("Project Selected.");
}
void MainWindow::OnOpenProject(wxCommandEvent& event)
{
	static const wxChar* FILETYPES = _T(
		"VisualGEANT4 marker files |*.vg4proj|"
		"All files|*.*"
	);
	
	wxFileDialog* openFileDialog =
		new wxFileDialog(this, _("Open VisualGEANT4 Project"), "", "", FILETYPES,
			wxFD_OPEN, wxDefaultPosition);

	if (openFileDialog->ShowModal() == wxID_OK)
	{
		wxString path;
		path.append(openFileDialog->GetDirectory());
		//path.append(wxFileName::GetPathSeparator());
		//path.append(openFileDialog->GetFilename());
		//theText->LoadFile(path);
		
		//MOST IMPORTANT PART
		SystemManager.CurrentProjectDir = path.mb_str(); //convert wxString to std::string
		
		SetStatusText(path, 0);
		SetStatusText(openFileDialog->GetDirectory(), 1);
	}
	
	SystemManager.Project_isOpen = true; //some methods will run if only this is true;

	//delete the status text "please select a project"
	SetStatusText("Project Selected.");
}
void MainWindow::OnSave(wxCommandEvent& event) {
	wxLogMessage("Will be available when custom app build is implemented");
}
void MainWindow::OnSaveAs(wxCommandEvent& event) {
	wxLogMessage("Will be available when custom app build is implemented");
}

void MainWindow::OnCommandLine(wxCommandEvent& event)
{
	//creates text entry
	wxTextEntryDialog* CommandEntry = new wxTextEntryDialog();
	CommandEntry->Create(this, "Enter command, refer to documentation for syntax:");
	CommandEntry->ShowModal();

	wxString Out = CommandEntry->GetValue();
	SetStatusText(Out);
	GetCommand* Parse = new GetCommand(2, std::string(Out.mb_str()));
	
	SystemManager.Kernel_args = Parse->Kernel_args;
	delete Parse;
	SystemManager.Conclude();

	//try testing with the command:


}





//RIGHT PANEL METHODS, accessed by toolbar
void MainWindow::RunPanelShow(wxCommandEvent& event)
{
	ReallocatePanel<RunPanel>();
	
}
void MainWindow::GeometryPanelShow(wxCommandEvent& event)
{
	//Choice button dropdown, new button(opens dialog), set postion and set angle
	//wxStaticText* PickExisting = new wxStaticText(RightPanel, wxID_ANY, wxT("PickExisting"), wxDefaultPosition, wxDefaultSize, 0);
	ReallocatePanel<GeometryPanel>();
	
}
//begin sourcepanel
void MainWindow::SourcePanelShow(wxCommandEvent& event)
{
	ReallocatePanel<SourcePanel>();
}
//end sourcepanel


//FINALLY: Conclude method applying to all completed tasks, MOVED TO A SEPERATE CLASS
/*
void MainWindow::Conclude() //conclude the command with project dir send it to "Process"
{
	//stick the current project dir at the end of Kernel_args and send it to Process class
	Kernel_args.push_back(SystemManager.CurrentProjectDir);
	Process* ProcessCommand = new Process(Kernel_args);
	delete ProcessCommand;
}
*/