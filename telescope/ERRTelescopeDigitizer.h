// -------------------------------------------------------------------------
// -----                        ERRTelescopeDigitizer header file          -----
// -----                        Created   by                 -----
// -------------------------------------------------------------------------

#ifndef ERRTelescopeDigitizer_H
#define ERRTelescopeDigitizer_H

#include "TClonesArray.h"

#include "FairTask.h"

#include "ERRTelescopeSiPoint.h"
#include "ERRTelescopeSiDigi.h"
#include "ERRTelescopeCsIDigi.h"
#include "ERRTelescopeCsIPoint.h"


class ERRTelescopeDigitizer : public FairTask 
{
    public:
        /** Default constructor **/
        ERRTelescopeDigitizer();

        /** Constructor
        ** verbose: 1 - only standard log print, 2 - print digi information
        **/
        ERRTelescopeDigitizer(Int_t verbose);

        /** Destructor **/
        ~ERRTelescopeDigitizer();

        /** Virtual method Init **/
        virtual InitStatus Init();

        /** Virtual method Exec **/
        virtual void Exec(Option_t* opt);

        /** Virtual method Finish **/
        virtual void Finish();

        /** Virtual method Reset **/
        virtual void Reset();

        /** Modifiers **/
        void SetSiElossThreshold(Float_t th){fElossThreshold = th;}
        void SetSiElossSigma(Float_t sigma) {fElossSigma = sigma;}
        void SetSiTimeSigma(Float_t sigma) {fTimeSigma = sigma;}
        /** Accessors **/
        Float_t ElossDispersion() const {return fElossSigma;}
        Float_t TimeDispersionPar() const {return fTimeSigma;}
    protected:
        //Input arrays
        TClonesArray *fSiPoints;
        TClonesArray *fCsIPoints;

        //Output arrays
        TClonesArray *fRTelescopeSiDigi;
        TClonesArray *fRTelescopeCsIDigi;

        Float_t fElossSigma;
        Float_t fTimeSigma;
        Float_t fElossThreshold;
        Float_t fDigiEloss;

        //ERBeamDetSetup* fBeamDetSetup;
        ERRTelescopeSiDigi* AddSiDigi(Int_t side, Int_t Nb, Int_t telescopeNb, Int_t detectorNb, Double_t time, Float_t edep);
        ERRTelescopeCsIDigi* AddCsIDigi(Int_t telescopeNb, Int_t detectorNb, Float_t edep, Double_t time, Int_t crystall);

    private:
        virtual void SetParContainers();
        ClassDef(ERRTelescopeDigitizer,1)
};

#endif
