#include "Geometry.h"

Geometry::Geometry(std::vector<std::string> Kernel_args)
{
	//some initializations
	Args = Kernel_args;
	Project_dir = Args.at(Args.size() - 1);

	//now selecting command
	std::string Brancher = Args.at(level);
	if (Brancher == "add")
	{
		AddObject();
	}
	if (Brancher == "delete")
	{
		DeleteObject();
	}
	else if (Brancher == "translate")
	{
		TranslateObject();
	}
	else
		isValid = false;
}

void Geometry::WriteFromTemplateObj(std::ofstream& TargetFile, std::ifstream& TemplateFile)
{
	std::string Content;
	//get contents
	if (TemplateFile) {
		std::ostringstream ss;
		ss << TemplateFile.rdbuf(); // reading data
		Content = ss.str();
	}
	TemplateFile.close();
	//write the newly created setting default settings, later to be modified
	TargetFile << Content;
}

void Geometry::AddObject()
{
	//see ListOfBackendCommands.docx for the syntax of "Args"
	std::string ObjectName = Args.at(level + 2);
	std::string ObjectType = Args.at(level + 1);

	//create the wavefront file and append the info to the g4geom.txt file
	std::string OBJDir = Project_dir + "/Geometry/" + ObjectName + ".obj";
	std::ofstream WavefrontFile(OBJDir);
	std::ofstream TxtModifier;
	TxtModifier.open(Project_dir + "/Geometry/g4geom.txt", std::ios_base::app);

	//needed for most cases
	vertex Center = { 0,0,0 };

	if (ObjectType == "box")
	{
		std::string posx = Args.at(level + 3);
		std::string posy = Args.at(level + 4);
		std::string posz = Args.at(level + 5);

		std::string length = Args.at(level + 6);
		std::string width = Args.at(level + 7);
		std::string height = Args.at(level + 8);

		std::string material = Args.at(level + 9);
		
		//add to g4geom.txt
		TxtModifier << "\n";
		TxtModifier << ":VOLU " + ObjectName + " BOX " + length + " " + width + " " + height + " " + material << std::endl;
		TxtModifier << ":PLACE " + ObjectName + " 1 world R00 " + posx + " " + posy + " " + posz << std::endl;

		//create the obj file: first copy the template
		std::ifstream TemplateBoxFile;
		TemplateBoxFile.open("Primitives/Box/Box.obj");
		WriteFromTemplateObj(WavefrontFile, TemplateBoxFile);
		TemplateBoxFile.close();
		WavefrontFile.close();

		//manipulate, parse string values back to double
		WavefrontObj* Box = new WavefrontObj(OBJDir);
		std::vector<double> NewDimensions{ stod(length), stod(width), stod(height) };
		std::vector<double> Translation{ stod(posx), stod(posy), stod(posz) };
		Box->BoxResize(NewDimensions);
		Box->TranslateBy(Translation);
		delete Box;
	}
	else if (ObjectType == "sphere")
	{
		std::string posx = Args.at(level + 3);
		std::string posy = Args.at(level + 4);
		std::string posz = Args.at(level + 5);

		std::string radius = Args.at(level + 6);

		std::string material = Args.at(level + 7);

		//add to g4geom.txt
		TxtModifier << "\n";
		TxtModifier << ":VOLU " + ObjectName + " ORB " + radius + " " + material << std::endl;
		TxtModifier << ":PLACE " + ObjectName + " 1 world R00 " + posx + " " + posy + " " + posz << std::endl;

		//create the obj file: first copy the template
		std::ifstream TemplateSphereFile;
		TemplateSphereFile.open("Primitives/Sphere/Sphere.obj");
		WriteFromTemplateObj(WavefrontFile, TemplateSphereFile);
		TemplateSphereFile.close();
		WavefrontFile.close();

		//manipulate, parse string values back to double
		WavefrontObj* Sphere = new WavefrontObj(OBJDir);
		std::vector<double> Translation{ stod(posx), stod(posy), stod(posz) };
		Sphere->Scale(Center, stod(radius));
		Sphere->TranslateBy(Translation);
		delete Sphere;
		
	}
	else
	{
		isValid = false;
	}
	
	TxtModifier.close();
	WavefrontFile.close();
}

void::Geometry::DeleteObject()
{
	//see documentation for assignments
	//Comment: search is based on filenames, which can be easily modified, causing errors
	//Future versions: store the name of the object "in the file", not as the name of the file
	std::string ObjectName = Args.at(level + 1);
	std::string ObjectToBeDeleted = Project_dir + "/Geometry/" + ObjectName + ".obj";
	try {
		if (std::filesystem::remove(ObjectToBeDeleted))
			std::cout << "file " << ObjectToBeDeleted << " deleted.\n";
		else
			std::cout << "file " << ObjectToBeDeleted << " not found.\n";
	}
	catch (const std::filesystem::filesystem_error& err) {
		std::cout << "filesystem error: " << err.what() << '\n';
	}
}

void Geometry::TranslateObject()
{
	std::string ObjectName = Args.at(level + 1);
	std::string ObjectToBeTranslated = Project_dir + "/Geometry/" + ObjectName + ".obj";

}