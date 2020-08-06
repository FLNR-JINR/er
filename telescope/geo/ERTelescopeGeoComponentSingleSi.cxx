/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERTelescopeGeoComponentSingleSi.h"

#include "TGeoManager.h"
#include "TGeoMatrix.h"
#include <TDOMParser.h>
#include <TXMLAttr.h>
#include <TXMLNode.h>
#include <TList.h>

#include "FairLogger.h"

using namespace std;

//--------------------------------------------------------------------------------------------------
ERTelescopeGeoComponentSingleSi::ERTelescopeGeoComponentSingleSi(
    const TString& typeFromXML, const TString& id, const TString& orientAroundZ) 
: ERTelescopeGeoComponentSensetive(typeFromXML, id)
{ 
  TString volumeNameInd = (orientAroundZ == "X") ? "_X" : "_Y";  
  fOrientAroundZ = (orientAroundZ == "X") ? OrientationAroundZ::X : OrientationAroundZ::Y;
  fVolumeName += volumeNameInd;
}
//--------------------------------------------------------------------------------------------------
ERTelescopeGeoComponentSingleSi::ERTelescopeGeoComponentSingleSi(
    const TString& typeFromXML, const TString& id, const TVector3& position, 
    const TVector3& rotation, const TString& orientAroundZ)
: ERTelescopeGeoComponentSensetive(typeFromXML, id, position, rotation)
{
  TString volumeNameInd = (orientAroundZ == "X") ? "_X" : "_Y";  
  fOrientAroundZ = (orientAroundZ == "X") ? OrientationAroundZ::X : OrientationAroundZ::Y;
  fVolumeName += volumeNameInd;
}
//--------------------------------------------------------------------------------------------------
TString ERTelescopeGeoComponentSingleSi::GetBranchName(
    ERDataObjectType objectType, OrientationAroundZ /*orientationAroundZ = OrientationAroundZ::Default*/,
    ChannelSide channelSide /*= ChannelSide::None*/) const {
  return GetBranchNamePrefix(SensetiveType::Si, objectType)
         + (channelSide != ChannelSide::None ? TString("_") + ChannelSideStr(channelSide) : "");
}
//--------------------------------------------------------------------------------------------------
std::list<OrientationAroundZ> ERTelescopeGeoComponentSingleSi::GetOrientationsAroundZ() const {
  return {fOrientAroundZ};
}
//--------------------------------------------------------------------------------------------------
std::list<ChannelSide> ERTelescopeGeoComponentSingleSi::GetChannelSides() const {
  if (fHasTwoSidedChannel) {
    return {ChannelSide::First, ChannelSide::Second};
  }
  return {ChannelSide::None};
}
//--------------------------------------------------------------------------------------------------
Int_t ERTelescopeGeoComponentSingleSi::GetChannelFromSensetiveNodePath(
    const TString& path, OrientationAroundZ /*orientation = OrientationAroundZ::Default*/) const {
  const TString channelStr(path(path.Last('_') + 1, path.Length()));
  return channelStr.Atoi();
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERTelescopeGeoComponentSingleSi)

