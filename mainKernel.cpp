#include"Process.h"
#include"GetCommand.h"
#pragma warning(disable:4996)
//this is just the kernel, which is called by the ui with an argument to execute a specific task

using namespace std;

int main(int argc, char** argv) {
	
	bool exit = false;
	while (!exit) {
		//get the command and process it
		GetCommand* CommandServant = new GetCommand();
		Process* ProcessCommand = new Process(CommandServant->Kernel_args);
		if (ProcessCommand->isExit) { exit = true; }
		delete ProcessCommand;
		delete CommandServant;
	}

	return 0;
}