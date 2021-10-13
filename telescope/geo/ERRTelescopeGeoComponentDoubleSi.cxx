/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERRTelescopeGeoComponentDoubleSi.h"

#include "TGeoManager.h"
#include "TGeoMatrix.h"
#include <TDOMParser.h>
#include <TXMLAttr.h>
#include <TXMLNode.h>
#include <TList.h>

#include "FairLogger.h"

#include "ERTelescopeSetup.h"

#include <iostream>
using namespace std;

//--------------------------------------------------------------------------------------------------
void ERRTelescopeGeoComponentDoubleSi::ConstructGeometryVolume(void) {
  ParseXmlParameters();
  if (fRMax <= fRMin)
    LOG(FATAL) << "Wrong radial telescope station parameters: r_max <= r_min; r_max = " 
               << fRMax << " r_min = " << fRMin << FairLogger::endl;
  auto* media = CreateMaterial(fMedia);
  media->GetMaterial()->Print();
  const Double_t sphere_r = 30000.;
  //Due to a bug in the tubes, the sphere with a big radius is used instead.
  const auto make_tubes = [sphere_r](const TString& name, TGeoMedium* material,
                                        const Double_t r_min, const Double_t r_max,
                                        const Double_t zplus, const Double_t zminus, 
                                        const Double_t phi_min, const Double_t phi_max) 
                                        -> TGeoVolume* {
    const Double_t sphere_r_max = sphere_r + zplus; const Double_t sphere_r_min = sphere_r - zminus;
    const Double_t sphere_theta_min = TMath::ASin(r_min/sphere_r) * TMath::RadToDeg();
    const Double_t sphere_theta_max = TMath::ASin(r_max/sphere_r) * TMath::RadToDeg();
    return gGeoManager->MakeSphere(name, material, sphere_r_min, sphere_r_max, 
                                   sphere_theta_min, sphere_theta_max, phi_min, phi_max);
  };
   const auto Zplus = fSensetiveZ/2 + fDeadLayerThicknessBackSide;		
   const auto Zminus = fSensetiveZ/2 + fDeadLayerThicknessFrontSide;
   fVolume = new TGeoVolumeAssembly(this->GetVolumeName());
   auto* station = make_tubes("r_station", media, fRMin*sphere_r/(sphere_r-Zminus), 
                             fRMax*sphere_r/(sphere_r+Zplus), Zplus, Zminus, 0., 360.);
  fVolume->AddNode(station, 0, new TGeoCombiTrans(0, 0., -(sphere_r), new TGeoRotation()));
  const Double_t segmentDPhi = 360. / fStripCountX;
  const Double_t segmentDR = (fSensetiveRMax-fSensetiveRMin) / fStripCountY;
  TGeoVolume* segment = make_tubes("Sector", media, fSensetiveRMin, fSensetiveRMax, Zplus, Zminus,
                                   -segmentDPhi/2., segmentDPhi/2.);
  for (Int_t iRing = 0; iRing < fStripCountY; iRing++) {
    const Double_t r_min = fSensetiveRMin + iRing * segmentDR;
    const Double_t r_max = fSensetiveRMin + (iRing + 1) * segmentDR;
    TGeoVolume* sensetive_segment = make_tubes("SensitiveSegment", media, r_min, r_max, 
                                               fSensetiveZ/2, fSensetiveZ/2,
                                               -segmentDPhi/2., segmentDPhi/2.);
    segment->AddNode(sensetive_segment, iRing, new TGeoCombiTrans(0, 0., 0, new TGeoRotation()));
  }
  for (Int_t iSegment = 0; iSegment < fStripCountX; iSegment++) {
    auto* rotation = new TGeoRotation();
    rotation->RotateZ(segmentDPhi * iSegment);
	  station->AddNode(segment, iSegment, new TGeoCombiTrans(0, 0., 0, rotation));
  }
}
//--------------------------------------------------------------------------------------------------
void ERRTelescopeGeoComponentDoubleSi::ParseXmlParameters() {
  TString xmlFile = ERTelescopeSetup::Instance()->GetXMLParametersFile();
  TDOMParser *domParser;
  domParser = new TDOMParser;
  domParser->SetValidate(false); // do not validate with DTD
  Int_t parsecode = domParser->ParseFile(xmlFile);
  if (parsecode < 0) {
     LOG(FATAL) << domParser->GetParseCodeMessage(parsecode) << FairLogger::FairLogger::endl;
    return ;
  }
  TXMLNode *rootNode = domParser->GetXMLDocument()->GetRootNode();
  TXMLNode *detPartNode = rootNode->GetChildren();
  TXMLNode *SiTypeNodes;
  for ( ; detPartNode; detPartNode = detPartNode->GetNextNode()) { // detector's part
    if (!strcasecmp(detPartNode->GetNodeName(), "RSiTypes")) {
      SiTypeNodes = detPartNode->GetChildren();
      for ( ; SiTypeNodes; SiTypeNodes = SiTypeNodes->GetNextNode()) {
        if (!strcasecmp(SiTypeNodes->GetNodeName(), "doubleSiTypes")) {
          TXMLNode* curNode = SiTypeNodes->GetChildren()->GetNextNode();
          for(; curNode; curNode = curNode->GetNextNode()) {
            TList *attrList;
            TXMLAttr *attr = 0;
            if (curNode->HasAttributes()){
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
            if(!strcasecmp(fComponentId, attr->GetValue())) {
              FillTwoSidedChannelAttribute(curNode->GetAttributes());
              TXMLNode* curNode2 = curNode->GetChildren();
              for(; curNode2; curNode2 = curNode2->GetNextNode()) {
                if(!strcasecmp(curNode2->GetNodeName(), "size")) {
                  attrList = curNode2->GetAttributes();
                  attr = 0;
                  TIter nextSizeAttr(attrList);
                  while ((attr=(TXMLAttr*)nextSizeAttr())) {
                    if (!strcasecmp("r_min", attr->GetName())) {
                      fRMin = atof(attr->GetValue());
                    }
                    if (!strcasecmp("r_max", attr->GetName())) {
                      fRMax = atof(attr->GetValue());
                    }
                  }
                }
                if(!strcasecmp(curNode2->GetNodeName(), "sensetive_size")) {
                  attrList = curNode2->GetAttributes();
                  attr = 0;
                  TIter nextSensSizeAttr(attrList);
                  while ((attr=(TXMLAttr*)nextSensSizeAttr())) {
                    if (!strcasecmp("r_min", attr->GetName())) {
                      fSensetiveRMin = atof(attr->GetValue());
                    }
                    if (!strcasecmp("r_max", attr->GetName())) {
                      fSensetiveRMax = atof(attr->GetValue());
                    }
                    if (!strcasecmp("z", attr->GetName())) {
                      fSensetiveZ = atof(attr->GetValue());
                    }
                  }
                }
                if(!strcasecmp(curNode2->GetNodeName(), "dead_layer_front")) {
                  fDeadLayerThicknessFrontSide = atof(curNode2->GetText());
                }
                if(!strcasecmp(curNode2->GetNodeName(), "dead_layer_back")) {
                  fDeadLayerThicknessBackSide = atof(curNode2->GetText());
                }
                if(!strcasecmp(curNode2->GetNodeName(), "strip_count_phi")) {
                  fStripCountX = atof(curNode2->GetText());
                }
                if(!strcasecmp(curNode2->GetNodeName(), "strip_count_r")) {
                  fStripCountY = atof(curNode2->GetText());
                }
                if(!strcasecmp(curNode2->GetNodeName(), "media")) {
                  fMedia = curNode2->GetText();
                }
              }
            }
          }
        }
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERRTelescopeGeoComponentDoubleSi)
