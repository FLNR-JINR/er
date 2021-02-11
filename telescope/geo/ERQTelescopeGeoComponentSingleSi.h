/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERQTelescopeGeoComponentSingleSi_H
#define ERQTelescopeGeoComponentSingleSi_H

#include "ERTelescopeGeoComponentSingleSi.h"

#include "TString.h"
#include "TVector3.h"

class ERQTelescopeGeoComponentSingleSi : public ERTelescopeGeoComponentSingleSi {
public:
  using ERTelescopeGeoComponentSingleSi::ERTelescopeGeoComponentSingleSi;
  virtual void ConstructGeometryVolume(void);
protected:
  virtual void ParseXmlParameters();
  Double_t fSensX = 0.;
  Double_t fSensY = 0.;
  Double_t fSensZ = 0.;
  ClassDef(ERQTelescopeGeoComponentSingleSi,1)
};
#endif
