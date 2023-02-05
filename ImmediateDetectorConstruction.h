#pragma once
#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"

class G4tgrMessenger;

/// Detector construction class using text geometry file

class ImmediateDetectorConstruction : public G4VUserDetectorConstruction
{
public:

    ImmediateDetectorConstruction(std::string CurrentProject_Dir);
    ~ImmediateDetectorConstruction();
    G4String ProjectDir;

    G4VPhysicalVolume* Construct(G4String FileWithRelativeDirectory); //must supply Geometry/g4geom.txt

private:

    G4tgrMessenger* fMessenger;
};

