#include "Counterbox.h"

Counterbox::Counterbox(std::vector<std::string> Kernel_args)
{
	//some initializations
	Args = Kernel_args;
	Project_dir = Args.at(Args.size() - 1);

	//now selecting command
	std::string Brancher = Args.at(level);
	if (Brancher == "resize")
	{
		ResizeObject();
	}
	else if (Brancher == "translate")
	{
		TranslateObject();
	}
	else
		isValid = false;
}

void Counterbox::ResizeObject()
{
	//this has two parts, one for modifying the g4geom.txt file
	//the other one for modifying run.mac which also includes the scorer commands

	//g4geom.txt modifier part (affects visualization in GLGeometryViewer)
	std::string ObjectName = "counterbox";
	std::string newXsize = Args.at(level + 1);
	std::string newYsize = Args.at(level + 2);
	std::string newZsize = Args.at(level + 3);
	std::string NewValue = newXsize + ". " + newYsize + ". " + newZsize + ". " + "G4_Ar"; //the decimal points may lead to bugs
	std::string Prefix = ":VOLU " + ObjectName + " BOX";
	std::string GeometryFilePath = Project_dir + "/Geometry/g4geom.txt";
	ChangeWithRegex(GeometryFilePath, Prefix, NewValue);

	//run.mac modifier part (affects actual functionality)
	Prefix = "/score/mesh/boxSize";
	NewValue = newXsize + ". " + newYsize + ". " + newZsize + ". " + "mm";
	std::string MacroFilePath = Project_dir + "/Macros/run.mac";
	ChangeWithRegex(MacroFilePath, Prefix, NewValue);

}

void Counterbox::TranslateObject()
{
	//this has two parts, one for modifying the g4geom.txt file
	//the other one for modifying run.mac which also includes the scorer commands

	//g4geom.txt modifier part (affects visualization in GLGeometryViewer)
	std::string ObjectName = "counterbox";
	std::string newX = Args.at(level + 1);
	std::string newY = Args.at(level + 2);
	std::string newZ = Args.at(level + 3);
	std::string NewValue = "1 world R00 " + newX + ". " + newY + ". " + newZ + "."; //the decimal points may lead to bugs
	std::string Prefix = ":PLACE " + ObjectName;
	std::string GeometryFilePath = Project_dir + "/Geometry/g4geom.txt";
	ChangeWithRegex(GeometryFilePath, Prefix, NewValue);

	//run.mac modifier part (affects actual functionality)
	Prefix = "/score/mesh/translate/xyz";
	NewValue = newX + ". " + newY + ". " + newZ + ". " + "mm";
	std::string MacroFilePath = Project_dir + "/Macros/run.mac";
	ChangeWithRegex(MacroFilePath, Prefix, NewValue);
}


//UTILITY METHODS:
//the most useful of them all: replace lines with a certain beginning (called "Prefix")
void Counterbox::ChangeWithRegex(std::string FileWithDir, std::string Prefix, std::string NewValue)
{
	std::ifstream f(FileWithDir); //taking file as inputstream, templates exist at VisualGEANT4 Install
	std::string TempContent;
	//get contents
	if (f) {
		std::ostringstream ss;
		ss << f.rdbuf(); // reading data
		TempContent = ss.str();
	}
	f.close();

	std::cout << "OLD:\n" + TempContent << std::endl;
	std::string beginning = "(";
	std::string end = ")(.*)";
	std::string middle = Prefix;
	std::string PrefixCStr = beginning + middle + end;
	TempContent = std::regex_replace(TempContent, std::regex(PrefixCStr.c_str()), Prefix + " " + NewValue);
	f.close();
	std::cout << "NEW:\n" + TempContent << std::endl;
	//wxString tmp(PrefixCStr);
	//wxLogMessage(tmp);

	std::ofstream ofs;
	ofs.open(FileWithDir, std::ofstream::out | std::ofstream::trunc);
	ofs << TempContent;
	ofs.close();
}

//It is unused in this version, but is kept for modularity
//this one replaces the entire line with "NewValue," not just the part after the "Prefix"
void Counterbox::FullChangeWithRegex(std::string FileWithDir, std::string Prefix, std::string NewValue)
{
	std::ifstream f(FileWithDir); //taking file as inputstream, templates exist at VisualGEANT4 Install
	std::string TempContent;
	//get contents
	if (f) {
		std::ostringstream ss;
		ss << f.rdbuf(); // reading data
		TempContent = ss.str();
	}
	f.close();

	std::cout << "OLD:\n" + TempContent << std::endl;
	std::string beginning = "(";
	std::string end = ")(.*)";
	std::string middle = Prefix;
	std::string PrefixCStr = beginning + middle + end;
	TempContent = std::regex_replace(TempContent, std::regex(PrefixCStr.c_str()), NewValue);
	f.close();
	std::cout << "NEW:\n" + TempContent << std::endl;
	//wxString tmp(PrefixCStr);
	//wxLogMessage(tmp);

	std::ofstream ofs;
	ofs.open(FileWithDir, std::ofstream::out | std::ofstream::trunc);
	ofs << TempContent;
	ofs.close();
}


