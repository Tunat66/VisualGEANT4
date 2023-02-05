#pragma once
#pragma warning(disable:4996)
#include <string>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <regex> //to modifiy files
#include <sstream> //to get file content
#include "wx/wx.h"

class Source
{
public:
	Source(std::vector<std::string> Kernel_args);
	~Source();
	bool isValid = true;
	std::string ProjectDir;
	//getter setter
	void SetArgs(std::vector<std::string> val);
	std::vector<std::string> GetArgs();

	//IN FUTURE VERSIONS, a plane irradiator (instead of a point gun) will also be available.

	//REULSABLE METHODS:
	//to change patterns with regex
	void ChangeWithRegex(std::string File, std::string Prefix, std::string NewValue);
private:
	std::vector<std::string> Args; //set to Kernel_args by constructor
};

