// -------------------------------------------------------------------------

// -----                        ERNDSetup header file              -----

// -----                        V.Schetinin (sch_vitaliy@mail.ru)      -----

// -------------------------------------------------------------------------

#ifndef ERNDSETUP_H
#define ERNDSETUP_H

#include <unordered_map>

#include "Rtypes.h"
#include "TVector3.h"
#include "TGeoNode.h"
#include "TGeoManager.h"

class ERNDSetup {
    static ERNDSetup* fInstance;
    static TGeoNode* fNDnode;
    ERNDSetup();
public:
    static ERNDSetup* Instance();
    ClassDef(ERNDSetup,1)
    void PMTPos(TVector3 pos, TVector3& pmtPos);
    TVector3 Pos(Int_t channel);
    void ReadGeoParamsFromParContainer();
    static TGeoHMatrix GetGlobalToLocalMatrix(const TString& path);
protected:
    std::unordered_map<Int_t, const TGeoNode*> fChannel2Node;
    bool fInited = false;
};

#endif

