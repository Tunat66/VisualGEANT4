#pragma once
#include "G4OpenGLImmediateXViewer.hh"
#include "wx/wx.h"
#include "wx/glcanvas.h"
//derived class
//to implement the opertaions on a wxGLcanvas instead of the computer screen
class wxG4OpenGLImmediateXViewer : public G4OpenGLImmediateXViewer, public wxGLCanvas
{
public:
	wxG4OpenGLImmediateXViewer(wxWindow* Parent, G4OpenGLImmediateSceneHandler& scene);
};

