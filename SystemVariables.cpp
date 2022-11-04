#include "SystemVariables.h"
#include "wx/wx.h"

//dont forget to initialize the statics, as it gives a compiler error
bool SystemVariables::Project_isOpen = false;
std::vector<std::string> SystemVariables::Kernel_args = {"init"};
std::string SystemVariables::CurrentProjectDir = "";

//to send the variables to an invoked backend
void SystemVariables::Conclude()
{
	//this is an instance of the class, as cpp requires it, but the class is wholly static-membered
	//stick the current project dir at the end of Kernel_args and send it to Process class
	
	//delete the first value assigned through initialization
	if (Kernel_args.at(0)=="init")
		Kernel_args.erase(Kernel_args.begin());

	//conclude
	Kernel_args.push_back(CurrentProjectDir);
	Process* ProcessCommand = new Process(Kernel_args);
	//Rhe delete might seem counterintuitive, but the process is handled entirely by the Process constructor
	//delete will not initiate before constructor (thus the Process itself) concludes
	delete ProcessCommand;
	Kernel_args.clear();
}