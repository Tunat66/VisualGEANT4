#pragma once
#pragma warning(disable:4996)
#include<iostream>
#include<string>
#include<stdlib.h>
#include<vector>
//the kernel commands are recieved as a whole string an converted to an argv-style array
//SPACES SEPERATE ARGUMENTS
class GetCommand
{
public:
	std::string KernelCommand;
	char** Kernel_argv;
	std::vector<std::string> Kernel_args;
	int Kernel_argc;
	GetCommand();
	void ParseToArgv(std::string cmd);
};

