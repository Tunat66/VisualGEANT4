#pragma once
#pragma warning(disable:4996)
#include <string>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <regex> //to modifiy files
#include<sstream> //to get file content

using namespace std;

class Configure //this class essentially creates and modifies .mac files which are fed into GEANT4
//the generation/modification of files is handled by the seperate method, 
//a new file is initially generated as a copy of the standard template
//command syntax config <configtype> <projectdir>
{
public:
	Configure(std::vector<std::string> Kernel_args);
	~Configure();
	bool isValid = true;
	std::string ProjectDir;
	//getter setter
	void SetArgs(std::vector<std::string> val);
	std::vector<std::string> GetArgs();
	
	//creating a new config:
	void NewConfig(std::string type, std::string FileName);
	
	//vis methods
	void VisHandle();
	void WriteVisMac();

	//run methods
	void RunHandle();
	
	//gun methods (particle type and energy)
	void GunHandle();

	//IN FUTURE VERSIONS, a plane irradiator (instead of a point gun) will also be available.

	//to change patterns with regex
	void ChangeWithRegex(std::string File, std::string Prefix, std::string NewValue);
private:
	std::vector<std::string> Args; //set to Kernel_args by constructor
};


