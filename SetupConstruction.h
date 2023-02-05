#pragma once
#include <string>
#include <math.h>
#include <regex>
#include <fstream>
#include <wx/wx.h>
class SetupConstruction
{
	//the constructor parses the Geometry.txt file and creates a list of existing geometry:
	SetupConstruction();
	

	
	
	//this class essentially creates ASCII text to define geometry in Geant4
	void ConstructNewBody(std::string BodyType, double xPos, double yPos, double zPos, double Euler1, double Euler2, double Euler3);
	void ModifyPos();
	void ConstructNewBeam();

	//later implemented
	void ConstructDetector();


};

