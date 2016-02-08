#ifndef ERNEURADGEOPAR_H
#define ERNEURADGEOPAR_H

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
                           const char* title="FairTutorialDet2 Geometry Parameters",
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

#endif /* ERNEURADGEOPAR_H */
