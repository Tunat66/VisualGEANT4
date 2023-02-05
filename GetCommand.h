#pragma once
#pragma warning(disable:4996)
#include<iostream>
#include<string>
#include<stdlib.h>
#include<vector>
#include<sstream>
//the kernel commands are recieved as a whole string an converted to an argv-style array
//SPACES SEPERATE ARGUMENTS
class GetCommand
{
public:
	std::string KernelCommand;
	//char** Kernel_argv;
	std::vector<std::string> Kernel_args;
	int Kernel_argc = 1;
	GetCommand(int Mode, std::string Command);
	//void ParseToArgv(std::string cmd);

	enum Modes {
		BATCH=1,
		EXTERNAL=2 //the GUI also uses this class to convert std::string commands to std::vector<std::string>
	};

};

