#pragma once
#include "wx/wx.h"
#include "SystemVariables.h"
class GeometryPanel : public wxPanel
{
	RunPanel(wxFrame* MainFrame);
	~RunPanel();

	SystemVariables SystemManager;


};

