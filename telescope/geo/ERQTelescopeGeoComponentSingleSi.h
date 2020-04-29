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
  ERQTelescopeGeoComponentSingleSi() = default;
  ERQTelescopeGeoComponentSingleSi(TString typeFromXML, TString id, TString orientAroundZ); 
  ERQTelescopeGeoComponentSingleSi(TString typeFromXML, TString id, TVector3 position, 
                                   TVector3 rotation, TString orientAroundZ);
  /* Modifiers */
  /* Accessors */
public:
  virtual void ConstructGeometryVolume(void);
protected:
  virtual void ParseXmlParameters();
  TString  fOrientAroundZ;
  Double_t fSizeX = 0.;
  Double_t fSizeY = 0.;
  Double_t fSizeZ = 0.;
  Double_t fSensX = 0.;
  Double_t fSensY = 0.;
  Double_t fSensZ = 0.;
  Double_t fDeadLayerThicknessFrontSide = 0.;
  Double_t fDeadLayerThicknessBackSide = 0.;
  Int_t    fStripCount = 0;
  TString  fMedia;

  ClassDef(ERQTelescopeGeoComponentSingleSi,1)
};
#endif
