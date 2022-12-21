#pragma once
#pragma warning(disable:4996)
#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<filesystem>
//this allows the application to be built from its sources
class BuildApplication
{
public:
	BuildApplication(std::vector<std::string> Kernel_args);
	std::string CMakeListsTxt_DIR; //needed for compilation, located at the same point as the sources
	std::string GEANT4_DIR;
	bool CMakeGenerate();
	void Build_MSBuild();
	void create_wrl(std::string ProjectDir);
	void create_obj_from_wrl(std::string ProjectDir);
};

