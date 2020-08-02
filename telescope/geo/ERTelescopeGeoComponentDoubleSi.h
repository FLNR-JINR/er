
/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERTelescopeGeoComponentDoubleSi_H
#define ERTelescopeGeoComponentDoubleSi_H

#include "ERTelescopeGeoComponentSensetive.h"

#include "TString.h"
#include "TVector3.h"

class ERTelescopeGeoComponentDoubleSi : public ERTelescopeGeoComponentSensetive {
public:
  ERTelescopeGeoComponentDoubleSi() = default;
  ERTelescopeGeoComponentDoubleSi(const TString& typeFromXML, const TString& id,
                                   const TString& orientAroundZ);
  ERTelescopeGeoComponentDoubleSi(const TString& typeFromXML, const TString& id, 
                                   const TVector3& position, const TVector3& rotation,
                                   const TString& orientAroundZ);
  virtual ~ERTelescopeGeoComponentDoubleSi() = default;
  virtual TString GetBranchName(ERDataObjectType object, 
                                OrientationAroundZ orientationAroundZ = OrientationAroundZ::Default,
                                ChannelSide side = ChannelSide::None) const;
  virtual std::list<OrientationAroundZ> GetOrientationsAroundZ() const;
  virtual std::list<ChannelSide> GetChannelSides() const;
  virtual Int_t GetChannelFromSensetiveNodePath(
    const TString& path, OrientationAroundZ orientation = OrientationAroundZ::Default) const;
protected:
  TString  fOrientAroundZ = "X";
  Double_t fDeadLayerThicknessFrontSide = 0.;
  Double_t fDeadLayerThicknessBackSide = 0.;
  Int_t    fStripCountX = 0;
  Int_t    fStripCountY = 0;
  ClassDef(ERTelescopeGeoComponentDoubleSi, 1)
};
#endif
