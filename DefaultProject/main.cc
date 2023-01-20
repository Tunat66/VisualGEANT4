#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "DetectorConstruction.hh"
#include "QBBC.hh" //I just used a premade physics-list after failing to create one by myself
#include "ActionInitialization.hh"
using namespace stop;

int main(int argc, char** argv)
{
	//detect interactive mode and define UI
	G4UIExecutive* ui = nullptr;
	if (argc == 1) { ui = new G4UIExecutive(argc, argv); } //removed purposefully
	//ui = new G4UIExecutive(argc, argv);

	// construct the default run manager
	auto* runManager = G4RunManagerFactory::CreateRunManager();

	// set mandatory initialization classes
	runManager->SetUserInitialization(new DetectorConstruction);
	runManager->SetUserInitialization(new QBBC);
	runManager->SetUserInitialization(new ActionInitialization);

	// initialize G4 kernel
	runManager->Initialize();

	// get the pointer to the UI manager and set verbosities
	G4UImanager* UI = G4UImanager::GetUIpointer();
	UI->ApplyCommand("/run/verbose 1");
	UI->ApplyCommand("/event/verbose 1");
	UI->ApplyCommand("/tracking/verbose 1");

	G4VisManager* visManager = new G4VisExecutive;
	// G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
	// G4VisManager* visManager = new G4VisExecutive("Quiet");
	visManager->Initialize();

	// Get the pointer to the User Interface manager
	G4UImanager* UImanager = G4UImanager::GetUIpointer();

	if (!ui) {
		// batch mode
		G4String command = "/control/execute ";
		G4String fileName = argv[1];
		UImanager->ApplyCommand(command + fileName);
	}
	else {
		// interactive mode
		UImanager->ApplyCommand("/control/macroPath Macros");
		UImanager->ApplyCommand("/control/execute init_vis.mac");
		//this is to create a wrl file used by VisualGEANT4
		//UImanager->ApplyCommand("/control/execute vis2.mac");
		ui->SessionStart();
		//UImanager->ApplyCommand(strcat("/control/macroPath ", argv[1]));
		
		delete ui;
	}


	// job termination
	delete runManager;
	return 0;
}