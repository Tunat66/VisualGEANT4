#pragma once
#include <string>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <regex>
#include <sstream>
#include <math.h>

#include "wx/wx.h"

#define M_PI 3.14159265359

class Counterbox
{
public:
	Counterbox(std::vector<std::string> Kernel_args);

	//the constructor reads the argument after to implement
	//it in one of the methods
	std::vector<std::string> Args; //so that Kernel_args can be stored locally
	std::string Project_dir; //the current project directory extracted by constructor
	bool isValid = true; //to check the Kernel_args has an illegal word
	int level = 1; //this class is invoked by the first argument of Kernel_args, so it is "level 1"

	//transforms
	void TranslateObject();
	void RotateObject();
	void ResizeObject();

	//REUSABLE METHODS:
	void ChangeWithRegex(std::string FileWithDir, std::string Prefix, std::string NewValue);
	void FullChangeWithRegex(std::string FileWithDir, std::string Prefix, std::string NewValue);
};

