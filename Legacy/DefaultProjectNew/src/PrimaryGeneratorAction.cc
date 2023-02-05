// this is the program allowing the initial state of the simulation and the boundary conditions (the beam etc.) to be defined
#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

namespace stop{

    ExG4PrimaryGeneratorAction01::ExG4PrimaryGeneratorAction01(
        const G4String& particleName,
        G4double energy,
        G4ThreeVector position,
        G4ThreeVector momentumDirection)
        : G4VUserPrimaryGeneratorAction(),
        fParticleGun(0)
    {
        G4int nofParticles = 1;
        fParticleGun = new G4ParticleGun(nofParticles);

        // default particle kinematic
        G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
        G4ParticleDefinition* particle
            = particleTable->FindParticle(particleName);
        fParticleGun->SetParticleDefinition(particle);
        fParticleGun->SetParticleEnergy(energy);
        fParticleGun->SetParticlePosition(position);
        fParticleGun->SetParticleMomentumDirection(momentumDirection);
    }

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    ExG4PrimaryGeneratorAction01::~ExG4PrimaryGeneratorAction01()
    {
        delete fParticleGun;
    }

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    void ExG4PrimaryGeneratorAction01::GeneratePrimaries(G4Event* anEvent)
    {
        // this function is called at the beginning of event

        fParticleGun->GeneratePrimaryVertex(anEvent);
    }

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
}


