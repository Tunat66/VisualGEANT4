#include "wxG4OpenGLImmediateXViewer.h"

wxG4OpenGLImmediateXViewer::wxG4OpenGLImmediateXViewer(wxWindow* Parent, G4OpenGLImmediateSceneHandler& scene) : G4OpenGLImmediateXViewer(scene,""),
	wxGLCanvas(Parent, wxID_ANY, NULL, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE)
{
}