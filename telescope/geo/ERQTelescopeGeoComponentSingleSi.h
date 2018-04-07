/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERQTelescopeGeoComponentSingleSi_H
#define ERQTelescopeGeoComponentSingleSi_H

#include "ERGeoComponent.h"

#include "TString.h"
#include "TVector3.h"

class ERQTelescopeGeoComponentSingleSi : public ERGeoComponent {
public:
  ERQTelescopeGeoComponentSingleSi();
  ERQTelescopeGeoComponentSingleSi(TString name);
  ERQTelescopeGeoComponentSingleSi(TString typeFromXML, TString id, 
                                   TVector3 position, 
                                   TVector3 rotation,
                                   TString  orientAroundZ);
  ~ERQTelescopeGeoComponentSingleSi();
  /* Modifiers */
  
  /* Accessors */
public:
  virtual void ConstructGeometryVolume(void);

private:
  TString  fOrientAroundZ;
  Double_t fSizeX;
  Double_t fSizeY;
  Double_t fSizeZ;
  Double_t fSensX;
  Double_t fSensY;
  Double_t fSensZ;
  Double_t fDeadLayerThicknessFrontSide;
  Double_t fDeadLayerThicknessBackSide;
  Int_t    fStripCount;
  TString  fMedia;

  void ParseXmlParameters();

  ClassDef(ERQTelescopeGeoComponentSingleSi,1)
};
#endif
