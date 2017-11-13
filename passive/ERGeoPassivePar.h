// -------------------------------------------------------------------------
// -----                       ERGeoPassivePar header file                   -----
// -----           Created 11/12/15  by V.Schetinin        			   -----
// -------------------------------------------------------------------------

#ifndef ERGEOPASSIVEPAR_H
#define ERGEOPASSIVEPAR_H

#include "FairParGenericSet.h"
#include "TH1F.h"

class ERGeoPassivePar : public FairParGenericSet {
public:
  TObjArray            *fGeoSensNodes; // List of FairGeoNodes for sensitive volumes
  TObjArray            *fGeoPassNodes; // List of FairGeoNodes for sensitive volumes

  ERGeoPassivePar(const char* name="ERGeoPassivePar",
             const char* title="Passive Geometry Parameters",
             const char* context="TestDefaultContext");
  ~ERGeoPassivePar(void);
  void clear(void);
  void putParams(FairParamList*);
  Bool_t getParams(FairParamList*);
  TObjArray             *GetGeoSensitiveNodes(){return fGeoSensNodes;}
  TObjArray             *GetGeoPassiveNodes(){return fGeoPassNodes;}

  ClassDef(ERGeoPassivePar,1)
};

#endif /* !ERGeoPassivePar_H */
