#ifndef ExG4PrimaryGeneratorAction01_h
#define ExG4PrimaryGeneratorAction01_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"
#include "G4ParticleGun.hh"

class G4ParticleGun;
class G4Event;

namespace stop {
    

    class ExG4PrimaryGeneratorAction01 : public G4VUserPrimaryGeneratorAction
    {

    public:
        ExG4PrimaryGeneratorAction01(
            const G4String& particleName = "gamma",
            G4double energy = 0.5 * MeV, //adjust if needed
            G4ThreeVector position = G4ThreeVector(0, 0, -10), //change according to the world dimensions
            G4ThreeVector momentumDirection = G4ThreeVector(0, 0, 1));
        ~ExG4PrimaryGeneratorAction01();

        // method implementation from base class
        virtual void GeneratePrimaries(G4Event*);

        // method to access particle gun
        const G4ParticleGun* GetParticleGun() const { return fParticleGun; }

    private:
        // data members
        G4ParticleGun* fParticleGun; //pointer a to G4 service class
    };



#endif

}
