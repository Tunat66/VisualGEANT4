#include "Process.h"
#include "wx/wx.h"

Process::Process(std::vector<std::string> Kernel_args) {
	std::cout << "Processing Command..." << std::endl;
	std::string FirstCommand = Kernel_args.at(0);

	if (FirstCommand == "exit")
		isExit = true;
	else if (FirstCommand == "open")
		OpenExisting(Kernel_args);
	else if (FirstCommand == "build") { //format: build Cmakelist_DIR Geant4_DIR
		BuildApplication* builder = new BuildApplication(Kernel_args);
		delete builder;
	}
	else if (FirstCommand == "config") {
		Configure* configurer = new Configure(Kernel_args);
		delete configurer;
	}
	else if (FirstCommand == "geom") {
		Geometry* GeometryModifier = new Geometry(Kernel_args);
		delete GeometryModifier;
	}
	else
		isValidCommand = false;
		

}

Process::~Process() {
	if (!isValidCommand)
		std::cout << "INVALID COMMAND, PROCESS FAILED" << std::endl;
}


//some simple functions are within this class
void Process::OpenExisting(std::vector<std::string> Kernel_args) { //NTS: add a checker to see if CONFIG EXISTS
	std::string LastArg /*which is always the app dir*/ = Kernel_args.at(Kernel_args.size() - 1);
	std::string ChildDir = LastArg;
	//all app executables are saved as main.exe, otherwise an error will be raised (this issue may be solved in future versions)
	LastArg += "/main.exe";
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
	// additional information
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	// set the size of the structures
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	// start the program up
	CreateProcess(ptr,   // the path
		NULL,        // Command line
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
	//system(AppDirectory);
}