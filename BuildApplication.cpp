#include "BuildApplication.h"

BuildApplication::BuildApplication(std::vector<std::string> Kernel_args) 
{
	CMakeListsTxt_DIR = Kernel_args.at(1);
	GEANT4_DIR = Kernel_args.at(2);

	bool res = CMakeGenerate();
	if (res) { Build_MSBuild(); }
}

bool BuildApplication::CMakeGenerate()
{
	//first check wheater CMakeLists.txt exists //the problem with cmakelistsdir
	bool flag = false;
	std::ifstream f(CMakeListsTxt_DIR.c_str());
	std::cout << CMakeListsTxt_DIR << std::endl;
	if (true) {
		std::string cdCommand;
		cdCommand = "cd " + CMakeListsTxt_DIR + " && " + "mkdir build" + " && " + "cd build";
		std::string CMakeCommand;
		CMakeCommand = "cmake -DGeant4_DIR=" + GEANT4_DIR + " " + CMakeListsTxt_DIR;
		std::string command = cdCommand + " && " + CMakeCommand;
		//std::cout << command << std::endl;
		system(cdCommand.c_str());
		bool flag = true;
	}
	else
		std::cout << "CMakeLists.txt is not found in:" + CMakeListsTxt_DIR << std::endl;

	return flag;
}

void BuildApplication::Build_MSBuild()
{
	std::string cdCommand;
	//NTS: change this depending on the MSVS version
	cdCommand = "C:/Program Files(x86)/Microsoft Visual Studio/2019/Community/MSBuild/Current/Bin";
	std::string BuildCommand = "MSBuild.exe " + CMakeListsTxt_DIR + "/build/Project.sln " + "/property:Configuration=Release";
	std::string command;
	command = "cd " + cdCommand + " && " + BuildCommand;
	//std::cout << command << std::endl;
	system(command.c_str());

}