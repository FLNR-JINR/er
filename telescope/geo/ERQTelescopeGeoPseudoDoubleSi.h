/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERQTelescopeGeoPseudoDoubleSi_H
#define ERQTelescopeGeoPseudoDoubleSi_H

#include "ERGeoComponent.h"

#include "TString.h"
#include "TVector3.h"

class ERQTelescopeGeoPseudoDoubleSi : public ERGeoComponent {
public:
  ERQTelescopeGeoPseudoDoubleSi();
  ERQTelescopeGeoPseudoDoubleSi(TString typeFromXML1, TString id1,
                                TString typeFromXML2, TString id2,
                                TString pathInhomogenMap,
                                TString pathFrontDeadMap,
                                TString pathBackDeadMap); 
  ~ERQTelescopeGeoPseudoDoubleSi();
  /* Modifiers */
  
  /* Accessors */
public:
  virtual void ConstructGeometryVolume(void);

private:
  static Int_t fConstructedObjCount;

  TString  fOrientAroundZ;
  TString  fComponentId1; // may be in coflict with parent class parameter - bad style
  TString  fComponentId2;
  Double_t fSizeX; // equals to first (thin) detector size
  Double_t fSizeY; // equals to second (thick) detector size
  Double_t fSizeZ; // equals to max pixel thickness in strip => an obsolete parameter
  Double_t fSensX;
  Double_t fSensY;
  Double_t fSensZ;
  Int_t    fStripCountX;
  Int_t    fStripCountY;
  TString  fMedia;
  TString  fPathInhomogenMap; // full thickness of the pseudo-pixel including thickness of the dead layer in thick detector
  TString  fPathFrontDeadMap; // estimation of the dead layer in pixel front side
  TString  fPathBackDeadMap; // estimation of the dead layer in pixel back side
  void ParseXmlParameters();

  ClassDef(ERQTelescopeGeoPseudoDoubleSi,1)
};
#endif
