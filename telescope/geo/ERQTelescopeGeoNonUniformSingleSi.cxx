/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERQTelescopeGeoNonUniformSingleSi.h"

#include "TGeoManager.h"
#include "TGeoBBox.h"
#include "TGeoCompositeShape.h"
#include "TGeoMatrix.h"
#include "TDOMParser.h"
#include "TXMLAttr.h"
#include "TXMLNode.h"
#include "TList.h"
#include "TFile.h"

#include "FairLogger.h"

//--------------------------------------------------------------------------------------------------
ERQTelescopeGeoNonUniformSingleSi::ERQTelescopeGeoNonUniformSingleSi(
    const TString& xmlTypeSingleSi, const TString& id, const TVector3& position, 
    const TVector3& rotation, const TString& orientAroundZ, const TString& thicknessMapFileName)
: ERQTelescopeGeoComponentSingleSi(xmlTypeSingleSi, id, position, rotation, orientAroundZ) {
  auto thicknessMapFile = TFile::Open(thicknessMapFileName.Data());
  if (!thicknessMapFile->IsOpen())
    LOG(FATAL) << "[ERQTelescopeGeoNonUniformSingleSi] Unable to open file with thickness map: " 
               << thicknessMapFileName << FairLogger::endl;
  LOG(DEBUG) << "[ERQTelescopeGeoNonUniformSingleSi] File  " << thicknessMapFileName << " has opened\n";
  const TString thicknessMapHistName = thicknessMapFile->GetListOfKeys()->At(0)->GetName();
  LOG(DEBUG) << "[ERQTelescopeGeoNonUniformSingleSi] Thickness map name is  " << thicknessMapHistName << FairLogger::endl;
  fThicknessMap = static_cast<TH2D*>(thicknessMapFile->Get(thicknessMapHistName));
  fXPseudoStripCount = fThicknessMap->GetNbinsX();
  fYPseudoStripCount = fThicknessMap->GetNbinsY();
  LOG(DEBUG) << "[ERQTelescopeGeoNonUniformSingleSi] X pseudo strip count " << fXPseudoStripCount
             << ", y pseudo strip count " << fYPseudoStripCount << FairLogger::endl;
}
//--------------------------------------------------------------------------------------------------
void ERQTelescopeGeoNonUniformSingleSi::ConstructGeometryVolume(void) {
  ParseXmlParameters();
  if (fOrientAroundZ == OrientationAroundZ::X && fXPseudoStripCount != fStripCount
      || fOrientAroundZ == OrientationAroundZ::Y && fYPseudoStripCount != fStripCount)
    LOG(FATAL) << "[ERQTelescopeGeoNonUniformSingleSi] Discretization in thickness map " << fXPseudoStripCount 
               << "x" << fYPseudoStripCount << " and station strip count " << fStripCount << " are inconsistent.\n";  
  auto* media = CreateMaterial(fMedia); 
  fVolume = new TGeoVolumeAssembly(this->GetVolumeName());
  const Double_t boxX = fSensX / fXPseudoStripCount;  
  const Double_t boxY = fSensY / fYPseudoStripCount;
  TGeoRotation* zeroRotation = new TGeoRotation();
  zeroRotation->RotateX(0.); zeroRotation->RotateY(0.); zeroRotation->RotateZ(0.);
  for (Int_t iStripX = 0; iStripX < fXPseudoStripCount; iStripX++) {
    LOG(DEBUG) << "[ERQTelescopeGeoNonUniformSingleSi] Creating strip " << iStripX << FairLogger::endl;
    auto* strip = new TGeoVolumeAssembly("pseudoSiStrip_" + TString::Itoa(iStripX, 10 /*base*/));
    const Double_t stripInStationTranslateX = -fSensX / 2 + boxX * iStripX + boxX / 2;
    fVolume->AddNode(strip, iStripX, new TGeoCombiTrans(stripInStationTranslateX, 0, 0, zeroRotation));
    for (Int_t iStripY = 0; iStripY < fYPseudoStripCount; iStripY++) {
      const Double_t fullThickness = (fComponentId.Contains("SSD20_3") || fComponentId.Contains("SSD20_4")) 
         ? fThicknessMap->GetBinContent(iStripX + 1, iStripY + 1) * 1e-4 /* mkm to cm */
         : fThicknessMap->GetBinContent(fXPseudoStripCount - iStripX, fYPseudoStripCount - iStripY) * 1e-4 /* mkm to cm */;
      const auto sensetiveThickness = fullThickness - fDeadLayerThicknessFrontSide - fDeadLayerThicknessBackSide;
      LOG(DEBUG) << "[ERQTelescopeGeoNonUniformSingleSi] Create box " << iStripY 
                 << " with full thickness = " << fullThickness << " and sensetive thickness = " 
                 << sensetiveThickness << FairLogger::endl;
      const TString boxNamePostfix = "_X" + TString::Itoa(iStripX, 10) + "_Y" + TString::Itoa(iStripY, 10);
      auto* sensetiveBox = gGeoManager->MakeBox("SensitivePixelSiBox" + boxNamePostfix, media,
                                                 boxX / 2, boxY / 2, sensetiveThickness / 2);
      sensetiveBox->SetTransparency(60);
      auto* frontDeadBox = gGeoManager->MakeBox("DeadFrontPixelSiBox" + boxNamePostfix, media, 
                                                 boxX / 2, boxY / 2, fDeadLayerThicknessFrontSide / 2);
      frontDeadBox->SetLineColor(kRed);
      frontDeadBox->SetTransparency(60);
      auto* backDeadBox = gGeoManager->MakeBox("DeadBackPixelSiBox" + boxNamePostfix, media, 
                                                boxX / 2, boxY / 2, fDeadLayerThicknessBackSide / 2);
      backDeadBox->SetLineColor(kGreen);
      backDeadBox->SetTransparency(60);
      const Double_t translationInStripY = -(fSensY / 2) + boxY / 2 + boxY * iStripY;
      strip->AddNode(sensetiveBox, 0, new TGeoCombiTrans(0, translationInStripY, 0, zeroRotation));
      const Double_t transFrontDeadZ = - (fDeadLayerThicknessFrontSide + sensetiveThickness) / 2;
      strip->AddNode(frontDeadBox, 0, new TGeoCombiTrans(0, translationInStripY, transFrontDeadZ, zeroRotation));
      const Double_t transBackDeadZ = (fDeadLayerThicknessBackSide + sensetiveThickness) / 2;
      strip->AddNode(backDeadBox, 0, new TGeoCombiTrans(0, translationInStripY, transBackDeadZ, zeroRotation));
    }
  }
  if (fOrientAroundZ == OrientationAroundZ::Y) {
    fRotation.RotateZ(90.);
  }
}
//--------------------------------------------------------------------------------------------------
Int_t ERQTelescopeGeoNonUniformSingleSi::GetChannelFromSensetiveNodePath(
    const TString& path, OrientationAroundZ /*orientation = OrientationAroundZ::Default*/) const {
  TString pathWithChannelPostfix = path;
  pathWithChannelPostfix.Remove(pathWithChannelPostfix.Last('/'), pathWithChannelPostfix.Length());
  const TString channelStr(pathWithChannelPostfix(pathWithChannelPostfix.Last('_') + 1,
                                                  pathWithChannelPostfix.Length()));
  return channelStr.Atoi();
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERGeoComponent)
