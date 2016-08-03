#include "EventAction.hh"
#include "Analysis.hh"
//#include "Randomize.hh" // do we really need this?
#include <iomanip>

#include "RunAction.hh"
#include "G4GeneralParticleSource.hh"

#include "PrimaryGeneratorAction.hh"
#include "G4RunManager.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4UnitsTable.hh"

#include "globals.hh"

//#include "iomanip"

using namespace std;
using namespace CLHEP;

EventAction::EventAction(RunAction*)
:G4UserEventAction()//, EdepInCrystal(0.), nAbsPhotons(0.), absTime(0.)
{}

EventAction::~EventAction()
{}

void EventAction::BeginOfEventAction(const G4Event*)
{
	// initialisation per event
	EdepInCrystal = 0.;
	nAbsPhotons = 0.;
	absTime = 0.;

    // Get energy of the primary particles (created with GPS)
   const PrimaryGeneratorAction* generatorAction
   = static_cast<const PrimaryGeneratorAction*>
     (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());

    G4GeneralParticleSource* particleGun = generatorAction->GetParticleGun();
    k_primary = particleGun->GetParticleEnergy();
    // G4cout << "\n And the energy is: BeginOfEventAction" << k_primary << "MeV" << G4endl;

}

void EventAction::EndOfEventAction(const G4Event* evt)
{

	  // Accumulate statistics
	  //

	  // get analysis manager
	  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

	  // fill histograms
	  analysisManager->FillH1(0, EdepInCrystal);
	  analysisManager->FillH1(1, nAbsPhotons);
	  analysisManager->FillH1(2, absTime);
	  analysisManager->FillH1(3, k_primary);

	  // fill ntuple
	  analysisManager->FillNtupleDColumn(0, EdepInCrystal);
	  analysisManager->FillNtupleDColumn(1, nAbsPhotons);
	  analysisManager->FillNtupleDColumn(2, absTime);
	  analysisManager->FillNtupleDColumn(3, k_primary);
	  analysisManager->AddNtupleRow();

  // Print per event (modulo n)
  //
  G4int eventID = 1 + evt->GetEventID();
  //G4int printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
  G4int printModulo = 100;
  if ( ( printModulo > 0 ) && ( eventID % printModulo == 0 ) )
  {
    G4cout << "---> End of event: " << eventID << G4endl;
  }
}
