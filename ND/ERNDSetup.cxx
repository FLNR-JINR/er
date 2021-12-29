#include "ERNDSetup.h"

#include <iostream>

#include "FairLogger.h"

ERNDSetup* ERNDSetup::fInstance = NULL;
TGeoNode* ERNDSetup::fNDnode = NULL;

ERNDSetup::ERNDSetup(){
    // --- Catch absence of TGeoManager
    if ( ! gGeoManager ) {
            std::cerr << "ERNDSetup: cannot initialise without TGeoManager!"<< std::endl;
    }
    gGeoManager->CdTop();
    TGeoNode* cave = gGeoManager->GetCurrentNode();
    fNDnode  = NULL;
    for (Int_t iNode = 0; iNode < cave->GetNdaughters(); iNode++) {
        TString name = cave->GetDaughter(iNode)->GetName();
        if ( name.Contains("ND", TString::kIgnoreCase) ) {
            fNDnode = cave->GetDaughter(iNode);
        }
    }
}

ERNDSetup* ERNDSetup::Instance(){
        if (fInstance == NULL)
                return new ERNDSetup();
        else
                return fInstance;
}

TGeoHMatrix ERNDSetup::GetGlobalToLocalMatrix(const TString& path) {
  TGeoIterator nextNode(gGeoManager->GetTopVolume());
  while(nextNode()) {
    TString nodePath;
    nextNode.GetPath(nodePath);
    if (nodePath.Contains(path)) {
      return (*static_cast<const TGeoHMatrix*>(nextNode.GetCurrentMatrix()));
    }
  }
  LOG(FATAL) << "Path " << path << " not found in geometry" << FairLogger::endl;
  return nullptr;
}

void ERNDSetup::ReadGeoParamsFromParContainer() {
  if (fInited)
    return;
  if (! gGeoManager) 
    LOG(FATAL) << "ERNDSetup: cannot initialise without TGeoManager!"<< FairLogger::endl;
  gGeoManager->CdTop();
  TGeoNode* cave = gGeoManager->GetCurrentNode();
  for (Int_t iNode = 0; iNode < cave->GetNdaughters(); iNode++) { // cycle by volumes in TOP
    TString detectorName = cave->GetDaughter(iNode)->GetName();
    if (!detectorName.Contains("ND", TString::kIgnoreCase))
        continue;
    const auto* nd = cave->GetDaughter(iNode);
    if (nd->GetNdaughters() != 1)
      LOG(FATAL) << "Wrong ND geometry structure" << FairLogger::endl;
    const auto* airBox = nd->GetDaughter(0);
    for (Int_t iStilben = 0; iStilben < airBox->GetNdaughters(); iStilben++) {
        const auto* node = airBox->GetDaughter(iStilben);
        const auto* translation = node->GetMatrix()->GetTranslation();
        fChannel2Node[node->GetNumber()] = node;
    }
  }
  fInited = true;
}

void ERNDSetup::PMTPos(TVector3 pos, TVector3& pmtPos){
	//@TODO пока что высчитывается координата центра стилбена.
	TGeoNode* stilben = gGeoManager->FindNode(pos.X(), pos.Y(), pos.Z());

	TGeoMatrix* stilbenMatrix = stilben->GetMatrix();
	pmtPos.SetXYZ(stilbenMatrix->GetTranslation()[0],
				stilbenMatrix->GetTranslation()[1],
				stilbenMatrix->GetTranslation()[2]);
}

TVector3 ERNDSetup::Pos(Int_t channel) {
    if (fChannel2Node.find(channel) == fChannel2Node.end()) {
        LOG(FATAL) << "ERNDSetup: Node for channel " << channel << " not found in geometry."
                   << FairLogger::endl;
    }
    auto matrix = GetGlobalToLocalMatrix(fChannel2Node[channel]->GetName());
    const auto* translation = matrix.GetTranslation();

    return TVector3(translation[0], translation[1], translation[2]);
}