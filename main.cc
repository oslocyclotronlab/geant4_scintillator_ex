/////////////////////////////////////////////////////////
//                                                     //
//  Oct/2013  E. Nacher  -->  main.cc                  //
//  Practical work for the SWG2013 Workshop            //
//                                                     //
/////////////////////////////////////////////////////////

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"

#ifdef G4UI_USE_TCSH
#include "G4UItcsh.hh"
#endif

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"

#include "Randomize.hh"
#include "globals.hh"

#include "G4ios.hh"
#include "fstream"
#include "iomanip"

using namespace std;	 

int main(int argc, char** argv)
{
	// construct the default run manager
	G4RunManager* runManager = new G4RunManager;
	
	// set mandatory initialization classes
	runManager->SetUserInitialization(new DetectorConstruction);
	runManager->SetUserInitialization(new PhysicsList);
		
	// set aditional user action classes
	RunAction* run = new RunAction;
	runManager->SetUserAction(run);
	
	EventAction* event = new EventAction(run);
	runManager->SetUserAction(event);
	
	SteppingAction* step = new SteppingAction(event);
	runManager->SetUserAction(step);
	
	
#ifdef G4VIS_USE
	// visualization manager
	//
	G4VisManager* visManager = new G4VisExecutive;
	visManager->Initialize();
#endif
	
	
	// initialize G4 kernel
	runManager->Initialize();
	
	// set mandatory user action class
	runManager->SetUserAction(new PrimaryGeneratorAction);
	
	//get the pointer to the User Interface manager 
	G4UImanager* UI = G4UImanager::GetUIpointer();  
	
	G4UIsession* session=0;
	
	if(argc==1)
	{
		// define UI terminal for interactive mode
#ifdef G4UI_USE_TCSH
		session = new G4UIterminal(new G4UItcsh);      
#else
		session = new G4UIterminal();
#endif
		
		session->SessionStart();
		delete session;
	}
	else
		// Batch mode
	{ 
		G4String command = "/control/execute ";
		G4String fileName = argv[1];
		UI->ApplyCommand(command+fileName);
	}
	
	// job termination
#ifdef G4VIS_USE
	delete visManager;
#endif
	delete runManager;
	return 0;
}
