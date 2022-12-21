#pragma once
#include <string>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <regex>
#include <sstream>

//to manipulate .obj and g4geom.txt files
//multiple obj viewing is not yet supported, multiple obj files will be created
//but only the first will be viewed
#include "WavefrontObj.h"
#include "WavefrontOBJUtility.h"

//DISCLAIMER: CGS unit system to be used, all lengths in cm!!!!

class Geometry : public WavefrontOBJUtility
{
public:
	Geometry(std::vector<std::string> Kernel_args); //the constructor reads the argument after to implement
	//it in one of the methods
	std::vector<std::string> Args; //so that Kernel_args can be stored locally
	std::string Project_dir; //the current project directory extracted by constructor
	bool isValid = true; //to check the Kernel_args has an illegal word
	int level = 1; //this class is invoked by the first argument of Kernel_args, so it is "level 1"

	//METHODS:
	//please note that these methods both modify the obj file and the g
	void AddObject();
	void DeleteObject();

	//transforms
	void TranslateObject();

	//REUSABLE METHODS:
	void WriteFromTemplateObj(std::ofstream& TargetFile, std::ifstream& TemplateFile);
	void ChangeWithRegex(std::string FileWithDir, std::string Prefix, std::string NewValue);
	void FullChangeWithRegex(std::string FileWithDir, std::string Prefix, std::string NewValue);
	
};

