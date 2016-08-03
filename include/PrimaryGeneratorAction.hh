#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4GeneralParticleSource;
class G4Event;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    PrimaryGeneratorAction();
    ~PrimaryGeneratorAction();
	
public:
    void GeneratePrimaries(G4Event* anEvent);
	
	// method to access particle gun
    G4GeneralParticleSource* GetParticleGun() const { return particleGun; }
private:
    G4GeneralParticleSource* particleGun;
	
};

#endif
