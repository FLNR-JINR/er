/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERRTelescopeGeoComponentSingleSi_H
#define ERRTelescopeGeoComponentSingleSi_H

#include "ERTelescopeGeoComponentSingleSi.h"

#include "TString.h"
#include "TVector3.h"

class ERRTelescopeGeoComponentSingleSi : public ERTelescopeGeoComponentSingleSi {
public:
  using ERTelescopeGeoComponentSingleSi::ERTelescopeGeoComponentSingleSi;
  virtual void ConstructGeometryVolume(void);
protected:
  virtual void ParseXmlParameters();
  Double_t fRMin = 0.;
  Double_t fRMax = 0.;
  Double_t fSensetiveRMin = 0;
  Double_t fSensetiveRMax = 0;
  Double_t fSensetiveZ = 0;
  ClassDef(ERRTelescopeGeoComponentSingleSi,1)
};
#endif
