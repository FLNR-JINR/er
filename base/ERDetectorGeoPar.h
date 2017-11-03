// -------------------------------------------------------------------------
// -----                  ERDetectorGeoPar header file                   -----
// -----                  Created 03/15  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERDetectorGeoPar_H
#define ERDetectorGeoPar_H

#include "FairParGenericSet.h"

class TObjArray;
class FairParamList;

class ERDetectorGeoPar : public FairParGenericSet
{
  public:

    /** List of FairGeoNodes for sensitive  volumes */
    TObjArray*      fGeoSensNodes;

    /** List of FairGeoNodes for sensitive  volumes */
    TObjArray*      fGeoPassNodes;

    ERDetectorGeoPar(const char* name="ERDetectorGeoPar",
                           const char* title="Detector Geometry Parameters",
                           const char* context="TestDefaultContext");
    ~ERDetectorGeoPar(void);
    void clear(void);
    void putParams(FairParamList*);
    Bool_t getParams(FairParamList*);
    TObjArray* GetGeoSensitiveNodes() {return fGeoSensNodes;}
    TObjArray* GetGeoPassiveNodes()   {return fGeoPassNodes;}

  private:
    ERDetectorGeoPar(const ERDetectorGeoPar&);
    ERDetectorGeoPar& operator=(const ERDetectorGeoPar&);

    ClassDef(ERDetectorGeoPar,1)
};

#endif /* ERDetectorGeoPar_H */
