#include "GetCommand.h"

GetCommand::GetCommand() 
{
	std::cout << "\n$";
	while (true) {
		std::string NewArg;
		std::cin >> NewArg;
		Kernel_args.push_back(NewArg);
		if (std::cin.peek() == '\n') { break;}
	}
	
	/*while (std::getline(std::cin, KernelCommand, ' '))
	{
		Kernel_args.push_back(KernelCommand); //push the arg into the vector of them (args)
		Kernel_argc++;
		std::cout << Kernel_argc << std::endl;
		std::cout << Kernel_args.at(i);
		i++;
	}*/
}

/*void GetCommand::ParseToArgv(std::string cmd) //out of order
{
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
	/*int j = 0;
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
	}*/
	
