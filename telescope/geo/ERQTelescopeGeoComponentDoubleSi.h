/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERQTelescopeGeoComponentDoubleSi_H
#define ERQTelescopeGeoComponentDoubleSi_H

#include "ERGeoComponent.h"

#include "TString.h"
#include "TVector3.h"

class ERQTelescopeGeoComponentDoubleSi : public ERGeoComponent {
public:
  ERQTelescopeGeoComponentDoubleSi();
  ERQTelescopeGeoComponentDoubleSi(TString name);
  ERQTelescopeGeoComponentDoubleSi(TString name, TString typeFromXML, 
                                   TVector3 position, 
                                   TVector3 rotation,
                                   TString  orientAroundZ);
  ~ERQTelescopeGeoComponentDoubleSi();
  /* Modifiers */
  
  /* Accessors */
public:
  virtual void ConstructGeometryVolume(void) = 0;

private:
  static Int_t fConstructedObjCount;

  TString  fOrientAroundZ;
  Double_t fSizeX;
  Double_t fSizeY;
  Double_t fSizeZ;
  Double_t fSensX;
  Double_t fSensY;
  Double_t fSensZ;
  Double_t fDeadLayerThicknessFrontSide;
  Double_t fDeadLayerThicknessBackSide;
  Int_t    fStripCountX;
  Int_t    fStripCountY;
  TString  fMedia;

  void ParseXmlParameters() = 0;

  ClassDef(ERQTelescopeGeoComponentDoubleSi,1)
};
#endif
