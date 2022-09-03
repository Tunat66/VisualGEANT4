#include "Process.h"

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
	//all app executables are saved as main.exe, otherwise an error will be raised (this issue may be solved in future versions)
	LastArg += "/main.exe";
	char* AppDirectory = new char[LastArg.length() + 1];
	strcpy(AppDirectory, LastArg.c_str());
	system(AppDirectory);
}