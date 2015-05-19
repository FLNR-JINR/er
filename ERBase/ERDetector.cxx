#include "TGeoMatrix.h"

#include "ERDetector.h"

ERDetector::ERDetector(const char* Name, Bool_t Active, Int_t DetId/*=0*/)
: FairDetector(Name, Active, DetId)
{
}

void ERDetector::ConstructGeometry() {
  TString fileName = GetGeometryFileName();
  if(fileName.EndsWith(".root")) {
    LOG(INFO) << "Constructing geometry from ROOT file " << fileName.Data() << FairLogger::endl;
    ConstructRootGeometry();
  }
  else if (fileName.EndsWith(".gdml")) {
    LOG(INFO) << "Constructing geometry from GDML file " << fileName.Data() << FairLogger::endl;
    TGeoRotation *zeroRotation = new TGeoRotation();
    zeroRotation->RotateX(0.);
    zeroRotation->RotateY(0.);
    zeroRotation->RotateZ(0.);
    ConstructGDMLGeometry(new TGeoCombiTrans(.0,.0,.0, zeroRotation));
  }
  else {
    LOG(FATAL) << "Geometry file name is not set" << FairLogger::endl;
  }
}
