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
	CMakeCommand = "cmake -DGeant4_DIR=" + GEANT4_DIR + " -B" + CMakeListsTxt_DIR + "/build" + " -S" + CMakeListsTxt_DIR;
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

}