/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERQTelescopeGeoComponentSingleSi_H
#define ERQTelescopeGeoComponentSingleSi_H

#include "ERQTelescopeGeoComponentSensetive.h"

#include "TString.h"
#include "TVector3.h"

class ERQTelescopeGeoComponentSingleSi : public ERQTelescopeGeoComponentSensetive {
public:
  ERQTelescopeGeoComponentSingleSi() = default;
  ERQTelescopeGeoComponentSingleSi(const TString& typeFromXML, const TString& id,
                                   const TString& orientAroundZ); 
  ERQTelescopeGeoComponentSingleSi(const TString& typeFromXML, const TString& id, 
                                   const TVector3& position, const TVector3& rotation,
                                   const TString& orientAroundZ);
  virtual void ConstructGeometryVolume(void);
  virtual TString GetBranchName(ERDataObjectType object, 
                                OrientationAroundZ orientationAroundZ = OrientationAroundZ::Default,
                                ChannelSide side = ChannelSide::None) const;
  virtual std::list<OrientationAroundZ> GetOrientationsAroundZ() const;
  virtual std::list<ChannelSide> GetChannelSides() const;
  virtual Int_t GetChannelFromSensetiveNodePath(
    const TString& path, OrientationAroundZ orientation = OrientationAroundZ::Default) const;
protected:
  virtual void ParseXmlParameters();
  OrientationAroundZ fOrientAroundZ = OrientationAroundZ::Default;
  Double_t fSensX = 0.;
  Double_t fSensY = 0.;
  Double_t fSensZ = 0.;
  Double_t fDeadLayerThicknessFrontSide = 0.;
  Double_t fDeadLayerThicknessBackSide = 0.;
  Int_t    fStripCount = 0;

  ClassDef(ERQTelescopeGeoComponentSingleSi,1)
};
#endif
