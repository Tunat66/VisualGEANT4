#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <regex>
#include <cmath>
#include <iostream>
#include <cstring>

/*  Credit for designing the parsers and viewers goes to: Justin Tennant:
*https://github.com/justint/obj-viewer/blob/main/obj-viewer.cpp
*/


#include "WavefrontOBJUtility.h"

class WavefrontObj : public WavefrontOBJUtility
{
public:
	WavefrontObj(std::string File);
	~WavefrontObj(); //deletes the vectors

	std::string FileName;

	
	void open_obj(std::string Filename);
	void obj_parse(char* file_line); //parses a line
	void PrintToFile(std::string File);

	//transforms to position in space
	void TranslateBy(std::vector<double> Translation);
	//virtual void RotateBy(std::vector<double> EulerAngles); //will be implemented in future versions
	
	//transformations to geometry
	void Scale1D(vertex center, int coordinate, double ScaleFactor);
	void Scale(vertex center, double ScaleFactor);
	void BoxResize(std::vector<double> NewDimensions);
	double SeperationAxial(vertex point1, vertex point2, int coordinate);

	//determines bounding values and an AABB
	std::vector<double> FindLengthWidthHeight();
	std::vector<double> GetBoundingBoxCenter();

	//to modularize functions acting on 1 coordinate
	enum Dimensions {
		x, y, z
	};

	
	//utility
	void calculate_normal(face_triangle f, GLdouble* normal);

};

