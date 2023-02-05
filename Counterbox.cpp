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
	else if (Brancher == "rotate")
	{
		RotateObject();
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

void Counterbox::RotateObject()
{
	//note that the convention used for euler angles: https://en.wikipedia.org/wiki/File:Eulerangles.svg
	//this has two parts, one for modifying the g4geom.txt file
	//the other one for modifying run.mac which also includes the scorer commands
	//wxMessageBox(wxT("I am in!"));
	//g4geom.txt modifier part (affects visualization in GLGeometryViewer)
	std::string ObjectName = "counterbox";
	std::string RotationMatrixName = "R_" + ObjectName; //add this standard to crit c
	double newAlpha_deg = std::stod(Args.at(level + 1));
	double newBeta_deg = std::stod(Args.at(level + 2));
	double newGamma_deg = std::stod(Args.at(level + 3));
	double phi = M_PI / 180 * newAlpha_deg;
	double theta = M_PI / 180 * newBeta_deg;
	double psi = M_PI / 180 * newGamma_deg;
	//wxMessageBox(wxString(std::to_string(phi)));
	//wxMessageBox(wxString(std::to_string(theta)));
	//wxMessageBox(wxString(std::to_string(psi)));
	//we need to process these angles into a rotation matrix, using a 2d array is suitable:
	//see page 153 of goldstein classical mechanics for the matrix
	double RotationMatrix[3][3] = {
		{ cos(psi)*cos(phi)-cos(theta)*sin(phi)*sin(psi),  cos(psi)*sin(phi)+cos(theta)*cos(phi)*sin(psi), sin(psi)*sin(theta)},
		{-sin(psi)*cos(phi)-cos(theta)*sin(phi)*cos(psi), -sin(psi)*sin(phi)+cos(theta)*cos(phi)*cos(psi), cos(psi)*sin(theta)},
		{             sin(theta)*sin(phi)               ,               -sin(theta)*cos(phi)            ,       cos(theta)    }
	};
	std::string NewValue = "";
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			NewValue += (" " + std::to_string(RotationMatrix[i][j]));

	std::string Prefix = ":ROTM " + RotationMatrixName;
	std::string GeometryFilePath = Project_dir + "/Geometry/g4geom.txt";
	ChangeWithRegex(GeometryFilePath, Prefix, NewValue);

	//run.mac modifier part (affects actual functionality)
	std::string MacroFilePath = Project_dir + "/Macros/run.mac";
	
	Prefix = "/score/mesh/rotate/rotateY";
	NewValue = " " + std::to_string(newAlpha_deg) + " deg";
	ChangeWithRegex(MacroFilePath, Prefix, NewValue);

	Prefix = "/score/mesh/rotate/rotateX";
	NewValue = " " + std::to_string(newBeta_deg) + " deg";
	ChangeWithRegex(MacroFilePath, Prefix, NewValue);

	Prefix = "/score/mesh/rotate/rotateZ";
	NewValue = " " + std::to_string(newGamma_deg) + " deg";
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


