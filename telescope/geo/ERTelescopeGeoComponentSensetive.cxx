#include "ERTelescopeGeoComponentSensetive.h"

#include <TXMLAttr.h>

//--------------------------------------------------------------------------------------------------
TString ERTelescopeGeoComponentSensetive::GetBranchNamePrefix(
    SensetiveType sensetiveType, ERDataObjectType objectType) const {
  return TString("Telescope") + ERDataObjectTypeStr(objectType) + "_" + GetVolumeName();
}
//--------------------------------------------------------------------------------------------------
std::list<TString> ERTelescopeGeoComponentSensetive::GetBranchNames(ERDataObjectType objectType) const {
  std::list<TString> branchNames;
  for (const auto orientation : GetOrientationsAroundZ()) {
    for (const auto channelSide : GetChannelSides()) {
      branchNames.push_back(GetBranchName(objectType, orientation, channelSide));
    }
  }
  return branchNames;
}
//--------------------------------------------------------------------------------------------------
void ERTelescopeGeoComponentSensetive::FillTwoSidedChannelAttribute(const TList* attributes) {
  TIter next(attributes);
  while (auto* attr = static_cast<TXMLAttr*>(next())) {
    if (!strcasecmp("twoSidedChannel", attr->GetName()) && !strcasecmp("yes", attr->GetValue())) { 
      fHasTwoSidedChannel = true;
    }
  }
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERTelescopeGeoComponentSensetive)