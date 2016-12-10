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
    ERDSRDSetup();
public:
    static ERDSRDSetup* Instance();
    ClassDef(ERDSRDSetup,1)

    Float_t SensorNb();
    Float_t SectorNb();
};

#endif

