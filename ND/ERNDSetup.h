// -------------------------------------------------------------------------

// -----                        ERNDSetup header file              -----

// -----                        V.Schetinin (sch_vitaliy@mail.ru)      -----

// -------------------------------------------------------------------------

#ifndef ERNDSETUP_H
#define ERNDSETUP_H

#include "Rtypes.h"
#include "TVector3.h"
#include "TGeoNode.h"

class ERNDSetup {
    static ERNDSetup* fInstance;
    static TGeoNode* fNDnode;
    ERNDSetup();
public:
    static ERNDSetup* Instance();
    ClassDef(ERNDSetup,1)

    void PMTPos(TVector3 pos, TVector3& pmtPos);
};

#endif

