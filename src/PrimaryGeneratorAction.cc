///////////////////////////////////////////////////////////////////////////////////////
//
// Apr/2015  E. Nacher --> PrimaryGeneratorAction.cc
//
// Based on the /gps method. This simplifies life, but...
// The PrimaryGeneratorAction must be instantiated after initialization of the 
// runManager in the main.cc:  
//                          runManager->Initialize();
//                          runManager->SetUserAction(new PrimaryGeneratorAction);
//
///////////////////////////////////////////////////////////////////////////////////////

#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

#include "G4ThreeVector.hh"
#include "globals.hh"

#include "G4ios.hh"
#include "fstream"
#include "iomanip"
#include "G4GeneralParticleSource.hh" 

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
	
	// Default values  
	
	particleGun = new G4GeneralParticleSource();
	particleGun->SetCurrentSourceIntensity (1);
	particleGun->SetParticlePosition(G4ThreeVector());
	
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
	delete particleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
	//create vertex
	
	particleGun->GeneratePrimaryVertex(anEvent);
}
