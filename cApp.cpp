
#include"cApp.h"
#pragma warning(disable:4996)
#pragma optimize("", off) //important: removing it will cause variables to be destroyed (optimized away, my ass) in buffer.h
//this is to inform the system main function to
//be informed of the entry point
wxIMPLEMENT_APP(cApp);

cApp::cApp()
{
}

cApp::~cApp()
{
}

bool cApp::OnInit()
{
	bool GUI = true;
	
	if(true) //UI mode
	{
		m_frame1 = new MainWindow();
		m_frame1->Show();
			
	}

	return true;
}


//this part is dangerous, move it to another project
	/*if (true) //if any argument is supplied, start in console mode
	{
		bool exit = false;
		while (!exit) {
			//get the command and process it
			GetCommand* CommandServant = new GetCommand();
			Process* ProcessCommand = new Process(CommandServant->Kernel_args);
			if (ProcessCommand->isExit) { exit = true; }
			delete ProcessCommand;
			delete CommandServant;
		}
	}*/