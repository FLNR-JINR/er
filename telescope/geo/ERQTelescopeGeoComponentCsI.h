/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERQTelescopeGeoComponentCsI_H
#define ERQTelescopeGeoComponentCsI_H

#include "ERGeoComponent.h"

#include "TString.h"
#include "TVector3.h"

class ERQTelescopeGeoComponentCsI : public ERGeoComponent {
public:
  ERQTelescopeGeoComponentCsI();
  ERQTelescopeGeoComponentCsI(TString name);
  ERQTelescopeGeoComponentCsI(TString name, TString typeFromXML, 
                                            TVector3 position, 
                                            TVector3 rotation);
  ~ERQTelescopeGeoComponentCsI();
  /* Modifiers */
  
  /* Accessors */
public:
  virtual void ConstructGeometryVolume(void);

private:
  Double_t fSizeX;
  Double_t fSizeY;
  Double_t fSizeZ;
  Int_t    fCubesCountX;
  Int_t    fCubesCountY;
  Double_t fSplitSize;
  Double_t fDeadLayer;
  TString  fMedia;

  void ParseXmlParameters();

  ClassDef(ERQTelescopeGeoComponentCsI,1)
};
#endif
