// -------------------------------------------------------------------------
// -----                  ERGadastGeoPar header file                   -----
// -----                  Created 03/15  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERGadastGeoPar_H
#define ERGadastGeoPar_H

#include "FairParGenericSet.h"

class TObjArray;
class FairParamList;

class ERGadastGeoPar : public FairParGenericSet
{
  public:

    /** List of FairGeoNodes for sensitive  volumes */
    TObjArray*      fGeoSensNodes;

    /** List of FairGeoNodes for sensitive  volumes */
    TObjArray*      fGeoPassNodes;

    ERGadastGeoPar(const char* name="ERGadastGeoPar",
                           const char* title="Gadast Geometry Parameters",
                           const char* context="TestDefaultContext");
    ~ERGadastGeoPar(void);
    void clear(void);
    void putParams(FairParamList*);
    Bool_t getParams(FairParamList*);
    TObjArray* GetGeoSensitiveNodes() {return fGeoSensNodes;}
    TObjArray* GetGeoPassiveNodes()   {return fGeoPassNodes;}

  private:
    ERGadastGeoPar(const ERGadastGeoPar&);
    ERGadastGeoPar& operator=(const ERGadastGeoPar&);

    ClassDef(ERGadastGeoPar,1)
};

#endif /* ERGadastGeoPar_H */