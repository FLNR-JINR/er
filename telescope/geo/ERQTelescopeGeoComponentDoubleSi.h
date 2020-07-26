/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERQTelescopeGeoComponentDoubleSi_H
#define ERQTelescopeGeoComponentDoubleSi_H

#include "ERTelescopeGeoComponentSensetive.h"

#include "TString.h"
#include "TVector3.h"

class ERQTelescopeGeoComponentDoubleSi : public ERTelescopeGeoComponentSensetive {
public:
  ERQTelescopeGeoComponentDoubleSi() = default;
  ERQTelescopeGeoComponentDoubleSi(const TString& typeFromXML, const TString& id,
                                   const TString& orientAroundZ);
  ERQTelescopeGeoComponentDoubleSi(const TString& typeFromXML, const TString& id, 
                                   const TVector3& position, const TVector3& rotation,
                                   const TString& orientAroundZ);
  virtual ~ERQTelescopeGeoComponentDoubleSi() = default;
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
  static Int_t fConstructedObjCount;
  TString  fOrientAroundZ = "X";
  Double_t fSensX = 0.;
  Double_t fSensY = 0.;
  Double_t fSensZ = 0.;
  Double_t fDeadLayerThicknessFrontSide = 0.;
  Double_t fDeadLayerThicknessBackSide = 0.;
  Int_t    fStripCountX = 0;
  Int_t    fStripCountY = 0;
  ClassDef(ERQTelescopeGeoComponentDoubleSi,1)
};
#endif
