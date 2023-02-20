#include "Source.h"

using namespace std;

//to test: config vis <stuff> D:/VisualGEANT4/Example
//to test: config vis trajectories drawByCharge <macroname>.mac D:/VisualGEANT4/Example
void Source::SetArgs(std::vector<std::string> val)
{
	Args = val;
}

std::vector<std::string> Source::GetArgs()
{
	return Args;
}
Source::Source(std::vector<std::string> Kernel_args)
{
	//this is a layer 2 class examining the second arg in Kernel_args and invoking another if neccesary
	std::string SecondCommand = Kernel_args.at(1);
	ProjectDir = Kernel_args.at(Kernel_args.size() - 1);
	Args = Kernel_args;

	if (SecondCommand == "position") 
	{
		std::string NewSetting_x = Args.at(2);
		std::string NewSetting_y = Args.at(3);
		std::string NewSetting_z = Args.at(4);
		std::string NewSetting = NewSetting_x + " " + NewSetting_y + " " + NewSetting_z;
		ChangeWithRegex(ProjectDir + "/Macros/gun.mac", "/gun/position", NewSetting);
	}
	else if (SecondCommand == "direction") 
	{
		std::string NewSetting_x = Args.at(2);
		std::string NewSetting_y = Args.at(3);
		std::string NewSetting_z = Args.at(4);
		std::string NewSetting = NewSetting_x + " " + NewSetting_y + " " + NewSetting_z;
		ChangeWithRegex(ProjectDir + "/Macros/gun.mac", "/gun/direction", NewSetting);
	}
	else
		isValid = false;

}

Source::~Source() {
	if (!isValid)
		std::cout << "INVALID COMMAND, PROCESS FAILED" << std::endl;
}

void Source::ChangeWithRegex(std::string FileWithDir, std::string Prefix, std::string NewValue)
{
	std::ifstream f(FileWithDir); //taking file as inputstream, templates exist at VisualGEANT4 Install
	string TempContent;
	//get contents taken from https://www.tutorialspoint.com/what-is-the-best-way-to-read-an-entire-file-into-a-std-string-in-cplusplus
	if (f) {
		std::ostringstream ss;
		ss << f.rdbuf(); // reading data
		TempContent = ss.str();
	}
	f.close();

	cout << "OLD:\n" + TempContent << endl;
	std::string beginning = "(";
	std::string end = ")(.*)";
	std::string middle = Prefix;
	std::string PrefixCStr = beginning + middle + end;
	TempContent = std::regex_replace(TempContent, std::regex(PrefixCStr.c_str()), Prefix + " " + NewValue);
	f.close();
	cout << "NEW:\n" + TempContent << endl;
	//debug
	//wxString tmp(PrefixCStr);
	//wxLogMessage(tmp);

	ofstream ofs;
	ofs.open(FileWithDir, std::ofstream::out | std::ofstream::trunc);
	ofs << TempContent;
	ofs.close();
}
