// -------------------------------------------------------------------------
// -----                  ERNeuRadGeoPar header file                   -----
// -----                  Created 03/15  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERNeuRadGeoPar_H
#define ERNeuRadGeoPar_H

#include "FairParGenericSet.h"

class TObjArray;
class FairParamList;

class ERNeuRadGeoPar : public FairParGenericSet
{
  public:

    /** List of FairGeoNodes for sensitive  volumes */
    TObjArray*      fGeoSensNodes;

    /** List of FairGeoNodes for sensitive  volumes */
    TObjArray*      fGeoPassNodes;

    ERNeuRadGeoPar(const char* name="ERNeuRadGeoPar",
                           const char* title="NeuRad Geometry Parameters",
                           const char* context="TestDefaultContext");
    ~ERNeuRadGeoPar(void);
    void clear(void);
    void putParams(FairParamList*);
    Bool_t getParams(FairParamList*);
    TObjArray* GetGeoSensitiveNodes() {return fGeoSensNodes;}
    TObjArray* GetGeoPassiveNodes()   {return fGeoPassNodes;}

  private:
    ERNeuRadGeoPar(const ERNeuRadGeoPar&);
    ERNeuRadGeoPar& operator=(const ERNeuRadGeoPar&);

    ClassDef(ERNeuRadGeoPar,1)
};

#endif /* ERNeuRadGeoPar_H */
