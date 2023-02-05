#pragma once

#include "wx/wx.h"
#include "MainWindow.h"
#include"Process.h"
#include"GetCommand.h"

//this class basically does nothing except starting the app

class cApp : public wxApp
{
public:
	cApp();
	~cApp();

private:
	//this object to display the app window
	MainWindow* m_frame1 = nullptr;

public:
	virtual bool OnInit();

};



