#pragma once
#include <vector>
#include <string>
#include "Process.h"

//these are the system variables, which the frontend modifies and passes to the backend using the Conclude() method
class SystemVariables
{
public:	
	static std::vector<std::string> Kernel_args;
	static bool Project_isOpen;
	static std::string CurrentProjectDir;
	void Conclude();
};





