// -------------------------------------------------------------------------

// -----                        ERDSRDSetup header file              -----

// -----                        V.Schetinin (sch_vitaliy@mail.ru)      -----

// -------------------------------------------------------------------------

#ifndef ERNDSETUP_H
#define ERNDSETUP_H

#include "Rtypes.h"
#include "TVector3.h"
#include "TGeoNode.h"

class ERDSRDSetup {
    static ERDSRDSetup* fInstance;
    static TGeoNode* fDSRDnode;
    static Float_t fZ;
    static Float_t fRmin, fRmax;
    static Int_t fSecNb, fSenNb;
    ERDSRDSetup();
public:
    static ERDSRDSetup* Instance();
    
    Float_t SensorNb() {return fSenNb;}
    Float_t SectorNb() {return fSecNb;}
    Float_t Rmin() {return fRmin;}
    Float_t Rmax() {return fRmax;}
    Float_t Z() {return fZ;}

    void Print();

    ClassDef(ERDSRDSetup,1)
};

#endif

