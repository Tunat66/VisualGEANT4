#pragma once
#include <vector>
#include <string>
#include "Process.h"
#include <fstream>
#include "wx/wx.h"

// include OpenGL
#ifdef __WXMAC__
#include "OpenGL/glu.h"
#include "OpenGL/gl.h"
#else
#include <GL/glu.h>
#include <GL/gl.h>
#endif

//these are the system variables, which the frontend modifies and passes to the backend using the Conclude() method
class SystemVariables
{
public:	
	static std::vector<std::string> Kernel_args;
	static bool Project_isOpen;
	static std::string CurrentProjectDir;

	static GLfloat latitude_current;
	static GLfloat longitude_current;
	static GLfloat zoom;
	
	void Conclude();
	void ReadError();

	static int CurrentPanel; //unused
};




