/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERRTelescopeGeoComponentDoubleSiSi_H
#define ERRTelescopeGeoComponentDoubleSiSi_H

#include "ERTelescopeGeoComponentDoubleSi.h"

#include "TString.h"
#include "TVector3.h"

class ERRTelescopeGeoComponentDoubleSi : public ERTelescopeGeoComponentDoubleSi {
public:
  using ERTelescopeGeoComponentDoubleSi::ERTelescopeGeoComponentDoubleSi;
  virtual void ConstructGeometryVolume(void);
protected:
  virtual void ParseXmlParameters();
  Double_t fRMin = 0.;
  Double_t fRMax = 0.;
  Double_t fSensetiveRMin = 0;
  Double_t fSensetiveRMax = 0;
  Double_t fSensetiveZ = 0;
  ClassDef(ERRTelescopeGeoComponentDoubleSi,1)
};
#endif
