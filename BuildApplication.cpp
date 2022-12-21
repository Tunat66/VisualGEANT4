 #include "BuildApplication.h"
#include "wx/wx.h" 

//AS TEST COMMAND build C:/Users/Dell/GEANT4TEMP/geant4-v11.0.0/examples/basic/B3/B3a C:/Users/Dell/GEANT4/Geant4-11.0/lib/Geant4-11.0.0

BuildApplication::BuildApplication(std::vector<std::string> Kernel_args) 
{
	CMakeListsTxt_DIR = Kernel_args.at(Kernel_args.size()-1);
	wxString out(CMakeListsTxt_DIR);
	GEANT4_DIR = Kernel_args.at(1);
	wxLogMessage(out);

	bool res = CMakeGenerate();
	if (res) { Build_MSBuild(); }
	
	//refresh the g4_00.wrl file:
	create_wrl(CMakeListsTxt_DIR);

	//finally, refresh the setup.obj file (OLD, the method now in RunPanel.cpp)
	//create_obj_from_wrl(CMakeListsTxt_DIR);

	//finally finally, instruct the viewer to view the new .obj file
	//MainWindow::GeometryViewer->refresh_view();
	//MainWindow::GeometryViewer->load_obj();
}

bool BuildApplication::CMakeGenerate()
{
	//first check wheater CMakeLists.txt exists //the problem with cmakelistsdir

	bool flag = false;
	std::cout << CMakeListsTxt_DIR << std::endl;
	//std::ifstream f(CMakeListsTxt_DIR.c_str());
	std::filesystem::path MakeDir = std::string(CMakeListsTxt_DIR + "/build");
		
	//create a build directory
	std::filesystem::create_directories(MakeDir);
		
	// cd to build and cmake
	std::string cdCommand;
	//cdCommand = "cd " + CMakeListsTxt_DIR + "/build";
	std::string CMakeCommand;
	CMakeCommand = "cmake -D Geant4_DIR=" + GEANT4_DIR + " -B " + CMakeListsTxt_DIR + "/build" + " -S " + CMakeListsTxt_DIR;
	//std::string Command = cdCommand + " && " + CMakeCommand;
	system(CMakeCommand.c_str());
	//std::cout << CMakeCommand << std::endl;
	//DEBUG
	wxString CMakeCmd(CMakeCommand);
	wxLogMessage(CMakeCmd);


	//succesful execution
	flag = true;
	wxLogMessage("CMake Successful");
	return flag;
}

void BuildApplication::Build_MSBuild()
{
	std::string cdCommand;
	//NTS: change this depending on the MSVS version
	cdCommand = "cd C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/MSBuild/Current/Bin";
	std::string BuildCommand = "MSBuild.exe " + CMakeListsTxt_DIR + "/build/Project.sln " + "/property:Configuration=Release";
	std::string command;
	command = cdCommand + " && " + BuildCommand;
	//std::cout << command << std::endl;
	//system(command.c_str());
	std::string command2 = "C:/\"Program Files (x86)\"/\"Microsoft Visual Studio\"/2019/Community/MSBuild/Current/Bin/MSBuild.exe " 
		+ CMakeListsTxt_DIR + "/build/Project.sln " + "/property:Configuration=Release";

	char* MSBuildDirectory = new char[command2.length() + 1];
	strcpy(MSBuildDirectory, command2.c_str());
	system(MSBuildDirectory);

	//finally, copy the executable from the build/Release directory

	std::filesystem::path from(CMakeListsTxt_DIR + "/build/Release/main.exe");
	std::filesystem::path to(CMakeListsTxt_DIR + "/main.exe");
	std::filesystem::copy(from, to, std::filesystem::copy_options::overwrite_existing);
	delete[] MSBuildDirectory;
}


void BuildApplication::create_wrl(std::string ProjectDir)
{
	std::filesystem::path Existing(ProjectDir + "/g4_00.wrl");
	if (std::filesystem::exists(Existing))
		std::filesystem::remove(Existing);

	//creates a wavefront file setup.obj to view the experiment geometry, using the file g4_00.wrl created by the compiled Geant4 executables
	std::string LastArg = ProjectDir + "/main.exe";
	std::string ChildDir = ProjectDir;
	char cmdArgs[] = "main.exe vis2.mac";

	//all app executables are saved as main.exe, otherwise an error will be raised (this issue may be solved in future versions)
	//allocation to C type string and then casting to wchar_t (the type used by CreateProcess)
	char* AppDirectory = new char[LastArg.length() + 1];
	strcpy(AppDirectory, LastArg.c_str());
	wchar_t wtext[100];
	mbstowcs(wtext, AppDirectory, strlen(AppDirectory) + 1);//Plus null
	LPWSTR ptr = wtext;
	char* ChildDirectory = new char[LastArg.length() + 1];
	strcpy(ChildDirectory, ChildDir.c_str());
	wchar_t wtext2[100];
	mbstowcs(wtext2, ChildDirectory, strlen(ChildDirectory) + 1);//Plus null
	LPWSTR dir = wtext2;

	wchar_t wtext3[100];
	mbstowcs(wtext3, cmdArgs, strlen(cmdArgs) + 1);//Plus null
	LPWSTR cmd = wtext3;

	// additional information
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	// set the size of the structures
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	// start the program up
	CreateProcess(ptr,   // the path
		cmd,        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		dir,           // Use project directory as starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
	);
	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	//wxLogMessage("wrl creation complete!");
	delete[] AppDirectory;
	delete[] ChildDirectory;

	wxMessageBox(wxT("Hello World!"));
}


/*This guy used to be here, but moved it to RunPanel class to avoid "Race Condition" bugs (the program attempting to open obj files before
writing to them is completed
void BuildApplication::create_obj_from_wrl(std::string ProjectDir)
{
	std::filesystem::path Existing(ProjectDir + "/setup.obj");
	if(std::filesystem::exists(Existing))
		std::filesystem::remove(Existing);

	//instruct the python script:
	//std::string InstructScript = "python " + ProjectDir + "\\meshconvOpener.py";
	//system(InstructScript.c_str());
	//simpler approach
	//std::string LastArg = ProjectDir + "\\meshconv.exe -c obj " + ProjectDir + "\\g4_00.wrl";
	//wxString dbg(LastArg);
	//wxLogMessage(dbg);
	//system(LastArg.c_str());

	//finally, rename it
	//std::filesystem::path Old(ProjectDir + "/g4_00.obj");
	//std::filesystem::path New(ProjectDir + "/setup.obj");
	//std::filesystem::rename(Old, New);


	
	//creates a wavefront file setup.obj to view the experiment geometry, using the file g4_00.wrl created by the compiled Geant4 executables
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
	system(AppDirectory);
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

	/*
	wchar_t wtext3[30];
	mbstowcs(wtext3, cmdArgs, strlen(cmdArgs) + 1);//Plus null
	LPWSTR cmd = wtext3;

	
	// additional information
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	// set the size of the structures
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	// start the program up, test 
	if(CreateProcess(ptr,   // the path
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
	

	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	//finally, rename the obj file
	//std::filesystem::path Old(ProjectDir + "/g4_00.obj");
	//std::filesystem::path New(ProjectDir + "/setup.obj");
	//std::filesystem::rename(Old, New);

	delete[] AppDirectory;
	//delete[] ChildDirectory;
}

*/