/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERTelescopeGeoComponentDoubleSi.h"

#include "TGeoManager.h"
#include "TGeoMatrix.h"
#include <TDOMParser.h>
#include <TXMLAttr.h>
#include <TXMLNode.h>
#include <TList.h>

#include "FairLogger.h"

//--------------------------------------------------------------------------------------------------
ERTelescopeGeoComponentDoubleSi::ERTelescopeGeoComponentDoubleSi(
    const TString& typeFromXML, const TString& id, const TString& orientAroundZ) 
: ERTelescopeGeoComponentSensetive(typeFromXML, id)
{
  TString volumeNameInd = (orientAroundZ == "X") ? "_XY" : "_YX";  
  fOrientAroundZ = volumeNameInd;
  fVolumeName += volumeNameInd;
}
//--------------------------------------------------------------------------------------------------
ERTelescopeGeoComponentDoubleSi::ERTelescopeGeoComponentDoubleSi(
    const TString& typeFromXML, const TString& id, const TVector3& position, 
    const TVector3& rotation, const TString& orientAroundZ)
: ERTelescopeGeoComponentSensetive(typeFromXML, id, position, rotation)
{
  TString volumeNameInd = (orientAroundZ == "X") ? "_XY" : "_YX";  
  fOrientAroundZ = volumeNameInd;
  fVolumeName += volumeNameInd;
}
//--------------------------------------------------------------------------------------------------
TString ERTelescopeGeoComponentDoubleSi::GetBranchName(
    ERDataObjectType objectType, OrientationAroundZ orientationAroundZ /*= OrientationAroundZ::Default*/,
    ChannelSide channelSide /*= ChannelSide::None*/) const {
  return GetBranchNamePrefix(SensetiveType::Si, objectType)
         + "_" + OrientationAroundZStr(orientationAroundZ)
         + (channelSide != ChannelSide::None ? TString("_") + ChannelSideStr(channelSide) : "");
}
//--------------------------------------------------------------------------------------------------
std::list<OrientationAroundZ> ERTelescopeGeoComponentDoubleSi::GetOrientationsAroundZ() const {
  return {OrientationAroundZ::X, OrientationAroundZ::Y};
}
//--------------------------------------------------------------------------------------------------
std::list<ChannelSide> ERTelescopeGeoComponentDoubleSi::GetChannelSides() const {
  if (fHasTwoSidedChannel) {
    return {ChannelSide::First, ChannelSide::Second};
  }
  return {ChannelSide::None};
}
//--------------------------------------------------------------------------------------------------
Int_t ERTelescopeGeoComponentDoubleSi::GetChannelFromSensetiveNodePath(
    const TString& path, OrientationAroundZ orientation /*= OrientationAroundZ::Default*/) const {
  TString pathWithChannelPostfix = path;
  if (orientation == OrientationAroundZ::X)
    pathWithChannelPostfix.Remove(pathWithChannelPostfix.Last('/'), pathWithChannelPostfix.Length());
  const TString channelStr(pathWithChannelPostfix(pathWithChannelPostfix.Last('_') + 1,
                                                  pathWithChannelPostfix.Length()));
  return channelStr.Atoi();
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERTelescopeGeoComponentDoubleSi)

