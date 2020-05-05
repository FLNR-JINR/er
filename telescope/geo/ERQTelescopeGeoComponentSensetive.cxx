#include "ERQTelescopeGeoComponentSensetive.h"

#include <TXMLAttr.h>

//--------------------------------------------------------------------------------------------------
TString ERQTelescopeGeoComponentSensetive::GetBranchNamePrefix(
    SensetiveType sensetiveType, ERDataObjectType objectType) const {
  return TString("ERQTelescope") + SensetiveTypeStr(sensetiveType) 
         + ERDataObjectTypeStr(objectType) + "_" + GetVolumeName();
}
//--------------------------------------------------------------------------------------------------
std::list<TString> ERQTelescopeGeoComponentSensetive::GetBranchNames(ERDataObjectType objectType) const {
  std::list<TString> branchNames;
  for (const auto orientation : GetOrientationsAroundZ()) {
    for (const auto channelSide : GetChannelSides()) {
      branchNames.push_back(GetBranchName(objectType, orientation, channelSide));
    }
  }
  return branchNames;
}
//--------------------------------------------------------------------------------------------------
void ERQTelescopeGeoComponentSensetive::FillTwoSidedChannelAttribute(const TList* attributes) {
  TIter next(attributes);
  while (auto* attr = static_cast<TXMLAttr*>(next())) {
    if (!strcasecmp("twoSidedChannel", attr->GetName()) && !strcasecmp("yes", attr->GetValue())) { 
      fHasTwoSidedChannel = true;
    }
  }
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERQTelescopeGeoComponentSensetive)