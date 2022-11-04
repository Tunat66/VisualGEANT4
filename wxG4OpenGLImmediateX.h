
#pragma once

#include "G4VisFeaturesOfOpenGL.hh"
#include "G4VSceneHandler.hh"
#include "G4OpenGLSceneHandler.hh"
#include "G4OpenGLViewer.hh"
#include "G4OpenGLImmediateX.hh"
#include "wxG4OpenGLImmediateXViewer.h"
#include "G4OpenGLViewerMessenger.hh"
#include "G4OpenGLImmediateSceneHandler.hh"

#include "G4VGraphicsSystem.hh"

class wxG4OpenGLImmediateX : public G4VGraphicsSystem {
public:
	wxG4OpenGLImmediateX();
	virtual ~wxG4OpenGLImmediateX();
	G4VSceneHandler* CreateSceneHandler(const G4String& name = "");
	G4VViewer* CreateViewer(G4VSceneHandler&, const G4String& name = "");
};

