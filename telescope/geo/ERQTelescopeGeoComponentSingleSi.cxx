/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERQTelescopeGeoComponentSingleSi.h"

#include "TGeoManager.h"
#include "TGeoMatrix.h"
#include <TDOMParser.h>
#include <TXMLAttr.h>
#include <TXMLNode.h>
#include <TList.h>

#include "FairLogger.h"

#include "ERQTelescopeSetup.h"

using namespace std;

//--------------------------------------------------------------------------------------------------
ERQTelescopeGeoComponentSingleSi::ERQTelescopeGeoComponentSingleSi(
    const TString& typeFromXML, const TString& id, const TString& orientAroundZ) 
: ERQTelescopeGeoComponentSensetive(typeFromXML, id)
{ 
  TString volumeNameInd = (orientAroundZ == "X") ? "_X" : "_Y";  
  fOrientAroundZ = (orientAroundZ == "X") ? OrientationAroundZ::X : OrientationAroundZ::Y;
  fVolumeName += volumeNameInd;
}
//--------------------------------------------------------------------------------------------------
ERQTelescopeGeoComponentSingleSi::ERQTelescopeGeoComponentSingleSi(
    const TString& typeFromXML, const TString& id, const TVector3& position, 
    const TVector3& rotation, const TString& orientAroundZ)
: ERQTelescopeGeoComponentSensetive(typeFromXML, id, position, rotation)
{
  TString volumeNameInd = (orientAroundZ == "X") ? "_X" : "_Y";  
  fOrientAroundZ = (orientAroundZ == "X") ? OrientationAroundZ::X : OrientationAroundZ::Y;
  fVolumeName += volumeNameInd;
}
//--------------------------------------------------------------------------------------------------
void ERQTelescopeGeoComponentSingleSi::ConstructGeometryVolume(void) {
  ParseXmlParameters();
  auto* media = CreateMaterial(fMedia);
  media->GetMaterial()->Print();
  // ---------------- SingleSi-------------------------------------------------
  TGeoVolume* singleSiStrip;
  fVolume = gGeoManager->MakeBox(this->GetVolumeName(), media, fSizeX / 2., fSizeY / 2., fSizeZ / 2.);
  //------------------ Single Si strip --------------------------------------
  Double_t singleSiStripX = fSensX / fStripCount; 
  Double_t singleSiStripY = fSensY;   
  Double_t singleSiStripZ = fSensZ - fDeadLayerThicknessFrontSide - fDeadLayerThicknessBackSide;   
  singleSiStrip = gGeoManager->MakeBox("SensitiveSingleSiStrip", media, singleSiStripX / 2., 
                                       singleSiStripY / 2., singleSiStripZ / 2.);
  //------------------ STRUCTURE  ---------------------------------------------
  //----------------------- Single Si structure -------------------------------
  //------------------ Add fibers to station  along x -----------------------
  for (Int_t iStrip = 0; iStrip < fStripCount; iStrip++) {
    Double_t transX = (-1) * fSensX / 2
                        + singleSiStripX/2. + iStrip*singleSiStripX;
    Double_t transZ = (fDeadLayerThicknessFrontSide 
                        - fDeadLayerThicknessBackSide)/2.;
    fVolume->AddNode(singleSiStrip, iStrip, new TGeoCombiTrans(transX, 0., transZ, new TGeoRotation()));
  }
  if (fOrientAroundZ == OrientationAroundZ::Y) {
    fRotation.RotateZ(90.);
  }
}
//--------------------------------------------------------------------------------------------------
void ERQTelescopeGeoComponentSingleSi::ParseXmlParameters() {
  TString xmlFile = ERQTelescopeSetup::Instance()->GetXMLParametersFile();
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
    if (!strcasecmp(detPartNode->GetNodeName(), "SiTypes")) {
      SiTypeNodes = detPartNode->GetChildren();
      for ( ; SiTypeNodes; SiTypeNodes = SiTypeNodes->GetNextNode()) {
        if (!strcasecmp(SiTypeNodes->GetNodeName(), "singleSiTypes")) {
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
                if(!strcasecmp(curNode2->GetNodeName(), "singleSiSize")) {
                  attrList = curNode2->GetAttributes();
                  attr = 0;
                  TIter nextSizeAttr(attrList);
                  while ((attr=(TXMLAttr*)nextSizeAttr())) {
                    if (!strcasecmp("singleSiX", attr->GetName())) {
                      fSizeX = atof(attr->GetValue());
                    }
                    if (!strcasecmp("singleSiY", attr->GetName())) {
                      fSizeY = atof(attr->GetValue());
                    }
                    if (!strcasecmp("singleSiZ", attr->GetName())) {
                      fSizeZ = atof(attr->GetValue());
                    }
                  }
                }
                if(!strcasecmp(curNode2->GetNodeName(), "singleSiSensAreaSize")) {
                  attrList = curNode2->GetAttributes();
                  attr = 0;
                  TIter nextSensSizeAttr(attrList);
                  while ((attr=(TXMLAttr*)nextSensSizeAttr())) {
                    if (!strcasecmp("singleSiSensX", attr->GetName())) {
                      fSensX = atof(attr->GetValue());
                    }
                    if (!strcasecmp("singleSiSensY", attr->GetName())) {
                      fSensY = atof(attr->GetValue());
                    }
                    if (!strcasecmp("singleSiSensZ", attr->GetName())) {
                      fSensZ = atof(attr->GetValue());
                    }
                  }
                }
                if(!strcasecmp(curNode2->GetNodeName(), "deadLayerThicknessFrontSide")) {
                  fDeadLayerThicknessFrontSide = atof(curNode2->GetText());
                }
                if(!strcasecmp(curNode2->GetNodeName(), "deadLayerThicknessBackSide")) {
                  fDeadLayerThicknessBackSide = atof(curNode2->GetText());
                }
                if(!strcasecmp(curNode2->GetNodeName(), "stripCount")) {
                  fStripCount = atof(curNode2->GetText());
                }
                if(!strcasecmp(curNode2->GetNodeName(), "singleSiMedia")) {
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
TString ERQTelescopeGeoComponentSingleSi::GetBranchName(
    ERDataObjectType objectType, OrientationAroundZ /*orientationAroundZ = OrientationAroundZ::Default*/,
    ChannelSide channelSide /*= ChannelSide::None*/) const {
  return GetBranchNamePrefix(SensetiveType::Si, objectType)
         + (channelSide != ChannelSide::None ? TString("_") + ChannelSideStr(channelSide) : "");
}
//--------------------------------------------------------------------------------------------------
std::list<OrientationAroundZ> ERQTelescopeGeoComponentSingleSi::GetOrientationsAroundZ() const {
  return {fOrientAroundZ};
}
//--------------------------------------------------------------------------------------------------
std::list<ChannelSide> ERQTelescopeGeoComponentSingleSi::GetChannelSides() const {
  if (fHasTwoSidedChannel) {
    return {ChannelSide::First, ChannelSide::Second};
  }
  return {ChannelSide::None};
}
//--------------------------------------------------------------------------------------------------
Int_t ERQTelescopeGeoComponentSingleSi::GetChannelFromSensetiveNodePath(
    const TString& path, OrientationAroundZ /*orientation = OrientationAroundZ::Default*/) const {
  const TString channelStr(path(path.Last('_') + 1, path.Length()));
  return channelStr.Atoi();
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERQTelescopeGeoComponentSingleSi)

