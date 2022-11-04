#pragma once
#pragma warning(disable:4996)
#include<string.h>
#include<stdlib.h>
#include<string>
#include<vector>
#include<iostream>
//this is the class which activates when the user starts VisualGEANT4 kernel via itself or the GUI
//it contains all the functionalities of the kernel connected to it as:
#include"BuildApplication.h"
#include"Configure.h"
#include"GetCommand.h"
#include"Geometry.h"


class Process
{
public:
	Process(std::vector<std::string> Kernel_args);
	~Process();
	bool isExit = false;
	bool isValidCommand = true;
	void OpenExisting(std::vector<std::string> Kernel_args); //opens and operates an existing .exe Geant application at "directory"
};

