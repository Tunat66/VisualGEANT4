#ifndef stopDetectorConstruction_h
#define stopDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

/// Detector construction class to define materials and geometry.

namespace stop
{

    class DetectorConstruction : public G4VUserDetectorConstruction
    {
    public:
        DetectorConstruction();
        ~DetectorConstruction() override;


        G4VPhysicalVolume* Construct() override;

        G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }

    protected:
        G4LogicalVolume* fScoringVolume = nullptr;
    };

}
#endif