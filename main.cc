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

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

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
  // Initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();
#endif
  // initialize G4 kernel
  runManager->Initialize();
  
  // set mandatory user action class
  runManager->SetUserAction(new PrimaryGeneratorAction);

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if (argc!=1) {
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }
  else {
    // interactive mode : define UI session
#ifdef G4UI_USE
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
    UImanager->ApplyCommand("/control/execute init_vis.mac"); 
#else
    UImanager->ApplyCommand("/control/execute init.mac"); 
#endif
    ui->SessionStart();
    delete ui;
#endif
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted 
  // in the main() program !
  
#ifdef G4VIS_USE
  delete visManager;
#endif
  delete runManager;

  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
