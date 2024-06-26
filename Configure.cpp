#include "Configure.h"


using namespace std;

//to test: config vis <stuff> D:/VisualGEANT4/Example
//to test: config vis trajectories drawByCharge <macroname>.mac D:/VisualGEANT4/Example
void Configure::SetArgs(std::vector<std::string> val)
{
	Args = val;
}

std::vector<std::string> Configure::GetArgs()
{
	return Args;
}
Configure::Configure(std::vector<std::string> Kernel_args)
{
	//this is a layer 2 class examining the second arg in Kernel_args and invoking another if neccesary
	std::string SecondCommand = Kernel_args.at(1);
	ProjectDir = Kernel_args.at(Kernel_args.size() - 1);
	Args = Kernel_args;

	if (SecondCommand == "run") {
		RunHandle();
	}
	if (SecondCommand == "gun") {
		GunHandle();
	}
	else if (SecondCommand == "vis") {
		VisHandle();
	}
	else
		isValid = false;

}

Configure::~Configure() {
	if (!isValid)
		std::cout << "INVALID COMMAND, PROCESS FAILED" << std::endl;
}

void Configure::VisHandle() 
{
	/*current properties:
	*style(surface, wireframe, auxiliary edges) 1string
	*trajectories 1string : <drawby> (particleID, charge, LATER: originvolume, encounteredvolume)
	*	if by Particle color 2 string: <particle> <color>
	*   if by Charge color 2 string <charge> <color>
	*filtering addparticle <name1> addparticle <name2>
	*/
	//now reading args (the modified file is always written second last):
	std::string FileToModify = Args.at(Args.size() - 2); //it was -1
	if (Args.at(2) == "new")
	{
		NewConfig("vis", Args.at(3));
	}
	else if (Args.at(2) == "trajectories")
	{
		//wxLogMessage(wxString(Args.at(3)));
		std::ifstream f("Templates/" + Args.at(3) + ".mac"); //taking file as inputstream, templates exist at VisualGEANT4 Install
		string Template;
		//get contents, I took this small bit of code from: https://www.tutorialspoint.com/what-is-the-best-way-to-read-an-entire-file-into-a-std-string-in-cplusplus
		if (f) {
			std::ostringstream ss;
			ss << f.rdbuf(); // reading data
			Template = ss.str();
		}
		f.close();

		ifstream f2(ProjectDir + "/Macros/" + FileToModify);
		string newstr2 = " ";
		if (f2) {
			ostringstream ss1;
			ss1 << f2.rdbuf(); // reading data
			newstr2 = ss1.str();
		}
		else
			cout << "The config you are trying to modify: " + FileToModify + " does not exist.";
		
		//to track replacements, print them
		cout << "OLD:\n" + newstr2 << endl;
		newstr2 = std::regex_replace(newstr2, std::regex("#TRAJMOD[^*]+#TRAJMOD"), Template);
		f2.close();
		cout << "NEW:\n" + newstr2 << endl;

		ofstream ofs;
		ofs.open(ProjectDir + "/Macros/" + FileToModify, std::ofstream::out | std::ofstream::trunc);
		ofs << newstr2;
		ofs.close();
	}
	else if (Args.at(2) == "filter")
	{
		//OLD: (was not working)
		/*int j = 3;
		ofstream ofs;
		ofs.open(ProjectDir + "/Macros/" + FileToModify, std::ofstream::app); //open in append mode
		ofs << "/vis/filtering/trajectories/create/particleFilter" << endl;
		while (Args.at(j) == "addparticle") {
			//addparticle <particle> ...... <addparticle> <particle> <macroname.mac>
			ofs << "/vis/filtering/trajectories/particleFilter-0/add " + Args.at(j + 1) << endl;
			j += 2;
		}
		ofs.close();*/
		//NEW:
		ofstream ofs;
		std::string VisFile = ProjectDir + "/Macros/" + FileToModify;
		ofs.open(VisFile, std::ofstream::app); //open in append mode
		if (Args.at(3) == "addparticle")
		{
			ofs << "/vis/filtering/trajectories/particleFilter-0/add " + Args.at(4) << endl;
		}
		else if (Args.at(3) == "removeparticle")
		{
			FullChangeWithRegex(VisFile, "/vis/filtering/trajectories/particleFilter-0/add " + Args.at(4), "#");
			//the '#' is essential as a comment line beginner, as the regex adds a "+" unexpectedly when removing positrons, corrupting the vis.mac file
			//I couldn't find the reason for the above behavior yet
		}
	}
	else if (Args.at(2) == "style")
	{
		std::string NewSetting = Args.at(3);
		ChangeWithRegex(ProjectDir + "/Macros/" + FileToModify, "/vis/viewer/set/style", NewSetting);
	}

}

void Configure::RunHandle()
{
	//usual console syntax
	std::string FileToModify = Args.at(Args.size() - 2);
	
	//create a new file if requested as: config run <projectdir> <neworexistingfilename.mac>
	if (Args.at(2) == "new")
	{
		NewConfig("run", Args.at(3));
	}
	if (Args.at(2) == "beamOnTimes") //how many particles to shoot
	{
		std::string NewSetting = Args.at(3);
		ChangeWithRegex(ProjectDir + "/Macros/" + FileToModify, "/run/beamOn", NewSetting);
	}

	// the rest of Kernel_args has the form: <runproperty1> <runproperty1value> ...
	/*current properties:
	* 
	*
	*/
}


void Configure::GunHandle()
{
	//usual console syntax
	std::string FileToModify = Args.at(Args.size() - 2);
	
	//create a new file if requested as: config run <projectdir> <neworexistingfilename.mac>
	if (Args.at(2) == "new")
	{
		NewConfig("gun", Args.at(3));
	}
	if (Args.at(2) == "setParticle") //select particle to be shot
	{
		std::string NewSetting = Args.at(3);
		ChangeWithRegex(ProjectDir + "/Macros/" + FileToModify, "/gun/particle", NewSetting);
	}
	if (Args.at(2) == "setEnergy") //select energy
	{
		std::string NewSetting = Args.at(3);
		ChangeWithRegex(ProjectDir + "/Macros/" + FileToModify, "/gun/energy", NewSetting);
	}
}


void Configure::NewConfig(std::string type, std::string FileName) {
	//define defualt .mac files, VERY important
	ofstream NewFile(ProjectDir + "/Macros/" + type + "_" + FileName + ".mac");

	std::ifstream f("DefaultMacros/" + type + ".mac"); //taking file as inputstream, templates exist at VisualGEANT4 Install
	string Content;
	//get contents
	if (f) {
		std::ostringstream ss;
		ss << f.rdbuf(); // reading data
		Content = ss.str();
	}
	f.close();
	//write the newly created setting default settings, later to be modified
	NewFile << Content;

	NewFile.close();
}

void Configure::ChangeWithRegex(std::string FileWithDir, std::string Prefix, std::string NewValue)
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
	//wxString tmp(PrefixCStr);
	//wxLogMessage(tmp);

	ofstream ofs;
	ofs.open(FileWithDir, std::ofstream::out | std::ofstream::trunc);
	ofs << TempContent;
	ofs.close();
}

void Configure::FullChangeWithRegex(std::string FileWithDir, std::string OldValue, std::string NewValue)
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
	std::string end = ")";
	std::string middle = OldValue;
	std::string OldValueCStr = beginning + middle + end;
	TempContent = std::regex_replace(TempContent, std::regex(OldValueCStr.c_str()), NewValue);
	f.close();
	cout << "NEW:\n" + TempContent << endl;
	//wxString tmp(PrefixCStr);
	//wxLogMessage(tmp);

	ofstream ofs;
	ofs.open(FileWithDir, std::ofstream::out | std::ofstream::trunc);
	ofs << TempContent;
	ofs.close();
}




