
#ifndef EventAction_h
#define EventAction_h 1

#include <fstream>

#include "G4UserEventAction.hh"
#include "globals.hh"

using namespace std;


class G4Event;
class RunAction;

class EventAction : public G4UserEventAction
{
  public:
    EventAction(RunAction*);
    ~EventAction();

  public:
    void BeginOfEventAction(const G4Event*);
    void EndOfEventAction(const G4Event*);

    void fillThistogram(G4double);
	void fillEhistogram(G4double);
    void fillLhistogram(G4double);
	
	void accumulateEnergy(G4double);

	G4int        nAbsPhotons;
	G4double     totEnergyDep;

   
  private:
    RunAction*       runAction;

    G4int                 TNumberChannel;
    G4double              Trange;
    G4int                 ENumberChannel;
    G4double              Erange;
    G4int                 LNumberChannel;
    G4double              Lrange;
    G4double              Thistogram[10000];
	G4double              Ehistogram[2000];
    G4double              Lhistogram[15000];

	ofstream	ThistogramOutputFile;
	ofstream	EhistogramOutputFile;
	ofstream	LhistogramOutputFile;
	

};

#endif
