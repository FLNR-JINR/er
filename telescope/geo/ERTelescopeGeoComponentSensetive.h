/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERTelescopeGeoComponentSensetive_H
#define ERTelescopeGeoComponentSensetive_H

#include "TList.h"

#include "ERGeoComponent.h"

class ERTelescopeGeoComponentSensetive : public ERGeoComponent {
public:
  ERTelescopeGeoComponentSensetive() = default;
  ERTelescopeGeoComponentSensetive(const TString& typeFromXML, const TString& id)
    : ERGeoComponent(typeFromXML, id) {}
  ERTelescopeGeoComponentSensetive(const TString& typeFromXML, const TString& id, 
                           const TVector3& position, const TVector3& rotation)
    : ERGeoComponent(typeFromXML, id, position, rotation) {}
  virtual ~ERTelescopeGeoComponentSensetive() = default;
  Bool_t HasTwoSidedChannel() const { return fHasTwoSidedChannel; }
  virtual std::list<TString> GetBranchNames(ERDataObjectType object) const;
  virtual TString GetBranchName(ERDataObjectType object, 
                                OrientationAroundZ orientationAroundZ = OrientationAroundZ::Default,
                                ChannelSide side = ChannelSide::None) const = 0;
  virtual std::list<OrientationAroundZ> GetOrientationsAroundZ() const = 0;
  virtual std::list<ChannelSide> GetChannelSides() const = 0;
  virtual Int_t GetChannelFromSensetiveNodePath(
      const TString& path, OrientationAroundZ orientation = OrientationAroundZ::Default) const = 0;
protected:
  TString GetBranchNamePrefix(SensetiveType sensetiveType, ERDataObjectType object) const;
  void FillTwoSidedChannelAttribute(const TList* attributes);
  TString  fMedia;
  Double_t fSizeX = 0.;
  Double_t fSizeY = 0.;
  Double_t fSizeZ = 0.;
  Bool_t fHasTwoSidedChannel = false;
  ClassDef(ERTelescopeGeoComponentSensetive,1)
};

#endif