/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERRTelescopeGeoComponentCsI.h"

#include "TGeoMedium.h"
#include "TGeoManager.h"
#include "TGeoMatrix.h"
#include <TDOMParser.h>
#include <TXMLAttr.h>
#include <TXMLNode.h>
#include <TList.h>

#include "FairLogger.h"

#include <iostream>

#include "ERQTelescopeSetup.h"
//--------------------------------------------------------------------------------------------------
void ERRTelescopeGeoComponentCsI::ConstructGeometryVolume(void) {
  ParseXmlParameters();
  if (fY1 == 0 || fY2 == 0 || fX1 == 0 || fZ1 == 0 || fZ2 == 0
      ||fX3 == 0 || fZ3 == 0 || fMedia.Length() == 0 || fCrystalCount == 0)
    LOG(FATAL) << "Not all fields are field for radial CsI geometry." << FairLogger::endl;
  auto* media = CreateMaterial(fMedia);
  const Double_t sector_angle = 360. / fCrystalCount;
  const Double_t tan_sector_angle = TMath::Tan(sector_angle /2. * TMath::DegToRad());
  const Double_t r_min = fX1 / (2. * tan_sector_angle);
  const Double_t x2 = (r_min + fY1) * tan_sector_angle * 2.;
  const Double_t sin_sector_angle = TMath::Sin(sector_angle /2. * TMath::DegToRad());
  const Double_t delta = fSplitSize / 2. / sin_sector_angle;
  auto* dead_layer_front_media = fDeadLayerFrontMedia.Length() ? CreateMaterial(fDeadLayerFrontMedia) : media;
  auto* dead_layer_periphery_media = fDeadLayerFrontMedia.Length() ? CreateMaterial(fDeadLayerPeripheryMedia) : media;
  fVolume = new TGeoVolumeAssembly(this->GetVolumeName());
  auto* crystal_shell = gGeoManager->MakeTrd2("CrystalShell", media, fX1/2.,  x2/2., fZ1/2., fZ2/2., fY1/2.);
  auto* periphery_shell = gGeoManager->MakeTrd2("PeripheryShell", media, x2/2., fX3/2., fZ2/2., fZ3/2., fY2/2.);
  auto* crystal = gGeoManager->MakeTrd2("SensitiveCrystal", media, fX1/2.,  x2/2., fZ1/2., fZ2/2., fY1/2.);
  auto* periphery = gGeoManager->MakeTrd2("SensitivePeriphery", media, x2/2., fX3/2., fZ2/2., fZ3/2., fY2/2.);
  crystal_shell->AddNode(crystal, 0, new TGeoCombiTrans(0., 0., 0., new TGeoRotation()));
  periphery_shell->AddNode(periphery, 0, new TGeoCombiTrans(0., 0., 0., new TGeoRotation()));
  for (Int_t i_crystal = 0; i_crystal < fCrystalCount; ++i_crystal) {
    const auto get_h_matrix = [](const Double_t angle, const Double_t z) {
        auto* h_matrix = new TGeoHMatrix();
        auto* rotation = new TGeoRotation();
        rotation->RotateX(90.);
        rotation->RotateZ(angle);
        auto* translation = new TGeoTranslation();
        translation->SetDz(z);
        h_matrix->Multiply(rotation);
        h_matrix->Multiply(translation);
        return h_matrix;
    };
    fVolume->AddNode(crystal_shell, i_crystal, get_h_matrix(sector_angle * i_crystal, r_min + delta + fY1/2.));
    fVolume->AddNode(periphery_shell, i_crystal, get_h_matrix(sector_angle * i_crystal, r_min + delta + fY1 + fY2/2.));
  }
}
//--------------------------------------------------------------------------------------------------
void ERRTelescopeGeoComponentCsI::ParseXmlParameters() {
  TString xmlFile = ERQTelescopeSetup::Instance()->GetXMLParametersFile();
  TDOMParser *domParser;//
  //gROOT->ProcessLine(".O 0");
  domParser = new TDOMParser;
  domParser->SetValidate(false); // do not validate with DTD

  Int_t parsecode = domParser->ParseFile(xmlFile);
  if (parsecode < 0) {
     LOG(FATAL) << domParser->GetParseCodeMessage(parsecode) << FairLogger::FairLogger::endl;
    return ;
  }
  TXMLNode *rootNode = domParser->GetXMLDocument()->GetRootNode();
  TXMLNode *detPartNode = rootNode->GetChildren();
  for ( ; detPartNode; detPartNode = detPartNode->GetNextNode()) { // detector's part
    if (!strcasecmp(detPartNode->GetNodeName(), "RCsITypes")) {
      TXMLNode* curNode = detPartNode->GetChildren()->GetNextNode();
      for (; curNode; curNode = curNode->GetNextNode()) {
        TList *attrList;
        TXMLAttr *attr = 0;
        if (curNode->HasAttributes()) {
          attrList = curNode->GetAttributes();
          TIter next(attrList);
          while ((attr=(TXMLAttr*)next())) {
            if (!strcasecmp("id", attr->GetName())) { 
              break;
            }
          }
        }
        else {
          continue;
        }
        if (!strcasecmp(fComponentId, attr->GetValue())) {
          TXMLNode* curNode2 = curNode->GetChildren();
          for (; curNode2; curNode2 = curNode2->GetNextNode()) {
            if (!strcasecmp(curNode2->GetNodeName(), "size")) {
              attrList = curNode2->GetAttributes();
              attr = 0;
              TIter nextPlasticAttr(attrList);
              while ((attr=(TXMLAttr*)nextPlasticAttr())) {
                if (!strcasecmp("y1", attr->GetName())) {
                  fY1 = atof(attr->GetValue());
                }
                if (!strcasecmp("y2", attr->GetName())) {
                  fY2 = atof(attr->GetValue());
                }
                if (!strcasecmp("x1", attr->GetName())) {
                  fX1 = atof(attr->GetValue());
                }
                if (!strcasecmp("z1", attr->GetName())) {
                  fZ1 = atof(attr->GetValue());
                }
                if (!strcasecmp("z2", attr->GetName())) {
                  fZ2 = atof(attr->GetValue());
                }
                if (!strcasecmp("x3", attr->GetName())) {
                  fX3 = atof(attr->GetValue());
                }
                if (!strcasecmp("z3", attr->GetName())) {
                  fZ3 = atof(attr->GetValue());
                }
              }
            }
            if (!strcasecmp(curNode2->GetNodeName(), "count")) {
              fCrystalCount = atof(curNode2->GetText());
            }
            if (!strcasecmp(curNode2->GetNodeName(), "split_size")) {
              fSplitSize = atof(curNode2->GetText());
            }
            if (!strcasecmp(curNode2->GetNodeName(), "media")) {
              fMedia = curNode2->GetText();
            }
            if (!strcasecmp(curNode2->GetNodeName(), "dead_layer_front")) {
              fDeadLayerFront = atof(curNode2->GetText());
            }
            if (!strcasecmp(curNode2->GetNodeName(), "dead_layer_front_media")) {
              fDeadLayerFrontMedia = curNode2->GetText();
            }
            if (!strcasecmp(curNode2->GetNodeName(), "dead_layer_periphery")) {
              fDeadLayerPeriphery = atof(curNode2->GetText());
            }
            if (!strcasecmp(curNode2->GetNodeName(), "dead_layer_periphery_media")) {
              fDeadLayerPeripheryMedia = curNode2->GetText();
            }
          }
        }
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------
TString ERRTelescopeGeoComponentCsI::GetBranchName(
    ERDataObjectType objectType, OrientationAroundZ orientationAroundZ /*= OrientationAroundZ::Default*/,
    ChannelSide side /*= ChannelSide::None*/) const {
  return GetBranchNamePrefix(SensetiveType::CsI, objectType);
}
//--------------------------------------------------------------------------------------------------
std::list<OrientationAroundZ> ERRTelescopeGeoComponentCsI::GetOrientationsAroundZ() const {
  return {OrientationAroundZ::X};
}
//--------------------------------------------------------------------------------------------------
std::list<ChannelSide> ERRTelescopeGeoComponentCsI::GetChannelSides() const {
  return {ChannelSide::None};
}
//--------------------------------------------------------------------------------------------------
Int_t ERRTelescopeGeoComponentCsI::GetChannelFromSensetiveNodePath(
    const TString& path, OrientationAroundZ orientation /*= OrientationAroundZ::Default*/) const {
  TString pathWithChannelPostfix = path;
  pathWithChannelPostfix.Remove(pathWithChannelPostfix.Last('/'), pathWithChannelPostfix.Length());
  const TString channelStr(pathWithChannelPostfix(pathWithChannelPostfix.Last('_') + 1,
                                                  pathWithChannelPostfix.Length()));
  return channelStr.Atoi();
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERGeoComponent)

