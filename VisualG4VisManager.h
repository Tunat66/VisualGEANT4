#pragma once
#include "wxG4OpenGLImmediateX.h"
#include "G4VisManager.hh"
//this is just a copy of the G4VisExecutive, the application instantiates visualization as:
//G4VisManager* visManager = new VisualG4VisManager()
//instead of:
//G4VisManager* visManager = new VisualG4VisManager()

class VisualG4VisManager : public G4VisManager
{
public:	
	VisualG4VisManager(wxWindow* Parent);
};

#include "VisualG4VisManager.cpp"

