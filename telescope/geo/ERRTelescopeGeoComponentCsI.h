/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERRTelescopeGeoComponentCsI_H
#define ERRTelescopeGeoComponentCsI_H

#include "ERTelescopeGeoComponentSensetive.h"

#include "TString.h"
#include "TVector3.h"

class ERRTelescopeGeoComponentCsI : public ERTelescopeGeoComponentSensetive {
public:
  ERRTelescopeGeoComponentCsI() = default;
  ERRTelescopeGeoComponentCsI(const TString& typeFromXML, const TString& id)
    : ERTelescopeGeoComponentSensetive(typeFromXML, id) {}
  ERRTelescopeGeoComponentCsI(const TString& typeFromXML, const TString& id, 
                              const TVector3& position, const TVector3& rotation)
    : ERTelescopeGeoComponentSensetive(typeFromXML, id, position, rotation) {}
  virtual ~ERRTelescopeGeoComponentCsI() = default;
  virtual void ConstructGeometryVolume(void);
  virtual TString GetBranchName(ERDataObjectType object, 
                                OrientationAroundZ orientationAroundZ = OrientationAroundZ::Default,
                                ChannelSide side = ChannelSide::None) const;
  virtual std::list<OrientationAroundZ> GetOrientationsAroundZ() const;
  virtual std::list<ChannelSide> GetChannelSides() const;
  virtual Int_t GetChannelFromSensetiveNodePath(
    const TString& path, OrientationAroundZ orientation = OrientationAroundZ::Default) const;
private:
  virtual void ParseXmlParameters();
  Int_t    fCrystalCount = 0;
  Double_t fY1 = 0.;
  Double_t fY2 = 0.;
  Double_t fX1 = 0.;
  Double_t fZ1 = 0.;
  Double_t fZ2 = 0.;
  Double_t fX3 = 0.;
  Double_t fZ3 = 0.;
  Double_t fSplitSize = 0.;
  Double_t fDeadLayer = 0.;
  TString fDeadLayerMedia;
  Double_t fDeadLayerPeriphery = 0.;
  TString fDeadLayerPeripheryMedia;
  ClassDef(ERRTelescopeGeoComponentCsI,1)
};
#endif
