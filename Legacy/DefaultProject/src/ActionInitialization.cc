#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"

namespace stop
{

	//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

	ActionInitialization::ActionInitialization()
	{}

	//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

	ActionInitialization::~ActionInitialization()
	{}

	//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

	void ActionInitialization::BuildForMaster() const
	{
		RunAction* runAction = new RunAction;
		SetUserAction(runAction);
	}

	//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

	void ActionInitialization::Build() const
	{

		RunAction* runAction = new RunAction;
		SetUserAction(runAction);

		SetUserAction(new ExG4PrimaryGeneratorAction01);

		EventAction* eventAction = new EventAction(runAction);
		SetUserAction(eventAction);

		SetUserAction(new SteppingAction(eventAction));
	}

	//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}
