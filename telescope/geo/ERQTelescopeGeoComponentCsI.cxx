/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERQTelescopeGeoComponentCsI.h"

#include "FairLogger.h"

#include "ERQTelescopeSetup.h"

using namespace std;

//--------------------------------------------------------------------------------------------------
ERQTelescopeGeoComponentCsI::ERQTelescopeGeoComponentCsI() {
}
//--------------------------------------------------------------------------------------------------
ERQTelescopeGeoComponentCsI::ERQTelescopeGeoComponentCsI(TString typeFromXML, TString id) 
: ERGeoComponent(typeFromXML, id)
{
}
//--------------------------------------------------------------------------------------------------
ERQTelescopeGeoComponentCsI::ERQTelescopeGeoComponentCsI(TString typeFromXML, TString id, 
                                                                              TVector3 position, 
                                                                              TVector3 rotation)
: ERGeoComponent(typeFromXML, id, position, rotation)
{
}
//--------------------------------------------------------------------------------------------------
ERQTelescopeGeoComponentCsI::~ERQTelescopeGeoComponentCsI() {
}
//--------------------------------------------------------------------------------------------------
void ERQTelescopeGeoComponentCsI::ConstructGeometryVolume(void) {
  ParseXmlParameters();
  TGeoManager*   gGeoMan = NULL;
  // -------   Load media from media file   -----------------------------------
  FairGeoLoader* geoLoad = FairGeoLoader::Instance();//
  FairGeoInterface* geoFace = geoLoad->getGeoInterface();
  TString geoPath = gSystem->Getenv("VMCWORKDIR");
  TString medFile = geoPath + "/geometry/media.geo";
  geoFace->setMediaFile(medFile);
  geoFace->readMedia();
  gGeoMan = gGeoManager;
  // -----------------   Get and create the required media    -----------------
  FairGeoMedia*   geoMedia = geoFace->getMedia();
  FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();
  // ----- Create media for Single Si -----------------------------------------
  FairGeoMedium* mCsI;
  TGeoMedium*    pMed; 

  mCsI = geoMedia->getMedium(fMedia);
  if ( ! mCsI ) Fatal("Main", "Medium for CsI not found");
  geoBuild->createMedium(mCsI);
  pMed = gGeoMan->GetMedium(fMedia);
  if ( ! pMed ) Fatal("Main", "Medium for CsI not found");
  LOG(DEBUG) << "Created single Si media" << FairLogger::endl;

  // --------------   Create geometry and top volume  -------------------------
  gGeoMan = (TGeoManager*)gROOT->FindObject("FAIRGeom");
  // ---------------- CsI-------------------------------------------------
  Float_t fullX = fSizeX*fCubesCountX + fSplitSize*fCubesCountX;
  Float_t fullY = fSizeY*fCubesCountY + fSplitSize*fCubesCountY;

  fVolume = gGeoManager->MakeBox(this->GetVolumeName(), 
                                 pMed, 
                                 fullX/2., 
                                 fullY/2., 
                                 fSizeZ/2.);
 

  TGeoVolume* shellCsI = gGeoManager->MakeBox("CsIBoxShell", pMed, fSizeX / 2, 
                                                                   fSizeY / 2, 
                                                                   fSizeZ / 2);

  TGeoVolume* boxCsISensitive = gGeoManager->MakeBox("SensitiveCsIBox", pMed, fSizeX / 2 - fDeadLayer, 
                                                                              fSizeY / 2 - fDeadLayer, 
                                                                              fSizeZ / 2 - fDeadLayer);
  shellCsI->AddNode(boxCsISensitive, 1, new TGeoCombiTrans(0, 0, 0, new TGeoRotation()));

  //------------------ STRUCTURE  ---------------------------------------------
  Int_t iBox = 0;
  for (Int_t iCsIX = 0; iCsIX < fCubesCountX; iCsIX++) {
    Double_t transX = -fullX/2 + fSizeX / 2. + fSplitSize / 2 + iCsIX *(fSizeX +fSplitSize);
    for (Int_t iCsIY = 0; iCsIY < fCubesCountY; iCsIY++) {
      Double_t transY = -fullY/2 + fSizeY / 2. + fSplitSize / 2 + iCsIY*(fSizeY + fSplitSize);
      fVolume->AddNode(shellCsI, iBox++, new TGeoCombiTrans(-transX,
                                                            transY,
                                                            0., 
                                                            new TGeoRotation()));
    }
  }
}
//--------------------------------------------------------------------------------------------------
void ERQTelescopeGeoComponentCsI::ParseXmlParameters() {
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
    if (!strcasecmp(detPartNode->GetNodeName(), "CsITypes")) {
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
            if (!strcasecmp(curNode2->GetNodeName(), "CsISize")) {
              attrList = curNode2->GetAttributes();
              attr = 0;
              TIter nextPlasticAttr(attrList);
              while ((attr=(TXMLAttr*)nextPlasticAttr())) {
                if (!strcasecmp("CsIX", attr->GetName())) {
                  fSizeX = atof(attr->GetValue());
                }
                if (!strcasecmp("CsIY", attr->GetName())) {
                  fSizeY = atof(attr->GetValue());
                }
                if (!strcasecmp("CsIZ", attr->GetName())) {
                  fSizeZ = atof(attr->GetValue());
                }
              }
            }
            if (!strcasecmp(curNode2->GetNodeName(), "cubesCountX")) {
              fCubesCountX = atof(curNode2->GetText());
            }
            if (!strcasecmp(curNode2->GetNodeName(), "cubesCountY")) {
              fCubesCountY = atof(curNode2->GetText());
            }
            if (!strcasecmp(curNode2->GetNodeName(), "splitSize")) {
              fSplitSize = atof(curNode2->GetText());
            }
            if (!strcasecmp(curNode2->GetNodeName(), "deadLayer")) {
              fDeadLayer = atof(curNode2->GetText());
            }
            if (!strcasecmp(curNode2->GetNodeName(), "CsIMedia")) {
              fMedia = curNode2->GetText();
            }
          }
        }
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERGeoComponent)

