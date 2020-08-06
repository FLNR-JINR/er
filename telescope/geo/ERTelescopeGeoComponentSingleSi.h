/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERTelescopeGeoComponentSingleSi_H
#define ERTelescopeGeoComponentSingleSi_H

#include "ERTelescopeGeoComponentSensetive.h"

#include "TString.h"
#include "TVector3.h"

class ERTelescopeGeoComponentSingleSi : public ERTelescopeGeoComponentSensetive {
public:
  ERTelescopeGeoComponentSingleSi() = default;
  ERTelescopeGeoComponentSingleSi(const TString& typeFromXML, const TString& id,
                                   const TString& orientAroundZ); 
  ERTelescopeGeoComponentSingleSi(const TString& typeFromXML, const TString& id, 
                                   const TVector3& position, const TVector3& rotation,
                                   const TString& orientAroundZ);
  virtual TString GetBranchName(ERDataObjectType object, 
                                OrientationAroundZ orientationAroundZ = OrientationAroundZ::Default,
                                ChannelSide side = ChannelSide::None) const;
  virtual std::list<OrientationAroundZ> GetOrientationsAroundZ() const;
  virtual std::list<ChannelSide> GetChannelSides() const;
  virtual Int_t GetChannelFromSensetiveNodePath(
    const TString& path, OrientationAroundZ orientation = OrientationAroundZ::Default) const;
protected:
  OrientationAroundZ fOrientAroundZ = OrientationAroundZ::Default;
  Double_t fDeadLayerThicknessFrontSide = 0.;
  Double_t fDeadLayerThicknessBackSide = 0.;
  Int_t    fStripCount = 0;
  ClassDef(ERTelescopeGeoComponentSingleSi,1)
};
#endif
