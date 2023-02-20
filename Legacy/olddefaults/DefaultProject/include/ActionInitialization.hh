#ifndef stopActionInitialization_h
#define stopActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

/// Action initialization class.

namespace stop
{

    class ActionInitialization : public G4VUserActionInitialization
    {
    public:
        ActionInitialization();
        ~ActionInitialization() override;

        void BuildForMaster() const override;
        void Build() const override;
    };

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif