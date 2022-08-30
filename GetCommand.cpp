#include "GetCommand.h"

GetCommand::GetCommand() 
{
	std::cout << "\n$";
	std::getline(std::cin, KernelCommand);
	ParseToArgv(KernelCommand);
}

void GetCommand::ParseToArgv(std::string cmd)
{
	Kernel_argc = 0;
	//allocate argv (Defunct)
	/*Kernel_argv = new char* [cmd.length()];
	for (int k = 0; k < cmd.length(); k++)
		Kernel_argv[k] = new char[cmd.length()];
	
	//fill argv
	int j = 0;
	int argpos = 0;
	for (int i = 0; i < cmd.length(); i++) {
		if (cmd.at(i) == ' ') {
			Kernel_argc++;
			strcpy(Kernel_argv[Kernel_argc], cmd.substr(argpos, argpos + j).c_str());
			j = 0;
			argpos = i + 1;
		}
		else 
			j++;
	}*/
	//allocate args
	int j = 0;
	int argpos = 0;
	for (int i = 0; i < cmd.length(); i++) {
		if (cmd.at(i) == ' ' || i == cmd.length() - 1) {
			if (i == cmd.length() - 1) j++;
			Kernel_argc++;
			Kernel_args.push_back(cmd.substr(argpos, argpos + j));
			j = 0;
			argpos = i + 1;
		}
		else {
			j++;
		}
	}
	
}
