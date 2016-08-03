///////////////////////////////////////////
//
// Apr/2015  E. Nacher -> EventAction.cc
//
///////////////////////////////////////////

#include "EventAction.hh"

#include "RunAction.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4UnitsTable.hh"

#include "globals.hh"

//#include "iomanip"

using namespace std;
using namespace CLHEP;

EventAction::EventAction(RunAction* RunAct)
:runAction(RunAct)
{
	// HistogramS length and range
	TNumberChannel = 10000;  // number of channels
	Trange = 1000.*ns;       // time range in ns
	ENumberChannel = 2000;   // number of channels
	Erange = 1000.*keV;      // energy range in keV
	LNumberChannel = 15000;  // number of channels
	Lrange = 15000.;         // max # of photons
	
	// reset histograms and other variables
	for(G4int i=0; i < TNumberChannel; i++) Thistogram[i] = 0.;
	for(G4int i=0; i < ENumberChannel; i++) Ehistogram[i] = 0.;
	for(G4int i=0; i < LNumberChannel; i++) Lhistogram[i] = 0.;
	
}

EventAction::~EventAction()
{}

void EventAction::BeginOfEventAction(const G4Event*)
{
	totEnergyDep = 0.;
	nAbsPhotons = 0;
}

void EventAction::EndOfEventAction(const G4Event* evt)
{
	
	fillEhistogram(totEnergyDep);
	fillLhistogram(nAbsPhotons);
	
	G4int event = evt -> GetEventID();
	
	//cout << "Event #" << event << "\t Absorbed photons: " << nAbsPhotons << endl;
	
	if(event == ((int)(event/100)*100)) {
		
		cout << "............ " << event << " events Simulated" << endl;
		cout << " writing outputs... " << endl;
		
		system("/bin/date"); 
		cout <<endl<< endl;
		
		ThistogramOutputFile.open("../data/TimeHisto.vec");
		for(int i=0; i<TNumberChannel; i++) ThistogramOutputFile << i << "\t\t" << Thistogram[i] << endl;
		ThistogramOutputFile.close();
		
		EhistogramOutputFile.open("../data/EnergyHisto.vec");
		for(int i=0; i<ENumberChannel; i++) EhistogramOutputFile << i << "\t\t" << Ehistogram[i] << endl;
		EhistogramOutputFile.close();
		
		LhistogramOutputFile.open("../data/LightHisto.vec");
		for(int i=0; i<LNumberChannel; i++) LhistogramOutputFile << i << "\t\t" << Lhistogram[i] << endl;
		LhistogramOutputFile.close();
		
	}
	
}

void EventAction::fillThistogram(G4double T)
{
	
	G4double binT;
	G4int    iT;
	
	if(TNumberChannel > 0)
    {
		binT = (T/Trange)*((G4double) TNumberChannel);
		iT = 1 + (G4int) binT;
		if(iT > TNumberChannel-1) iT = TNumberChannel-1;
		if(iT < 0) iT = 0;
		Thistogram[iT] += 1.;
		
    }
	
}

void EventAction::fillEhistogram(G4double E)
{
	
	G4double binE;
	G4int    iE;
	
	if(ENumberChannel > 0)
    {
		binE = (E/Erange)*((G4double) ENumberChannel);
		iE = 1 + (G4int) binE;
		if(iE > ENumberChannel-1) iE = ENumberChannel-1;
		if(iE < 0) iE = 0;
		Ehistogram[iE]++;
		
    }
	
}

void EventAction::fillLhistogram(G4double L)
{
	
	G4double binL;
	G4int    iL;
	
	if(LNumberChannel > 0)
    {
		binL = (L/Lrange)*((G4double) LNumberChannel);
		iL = 1 + (G4int) binL;
		if(iL > LNumberChannel-1) iL = LNumberChannel-1;
		if(iL < 0) iL = 0;
		Lhistogram[iL]++;
		
    }
	
}
