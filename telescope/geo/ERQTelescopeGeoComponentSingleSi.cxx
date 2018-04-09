/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERQTelescopeGeoComponentSingleSi.h"

#include "FairLogger.h"

#include "ERQTelescopeSetup.h"

using namespace std;

//--------------------------------------------------------------------------------------------------
ERQTelescopeGeoComponentSingleSi::ERQTelescopeGeoComponentSingleSi() {
}
//--------------------------------------------------------------------------------------------------
ERQTelescopeGeoComponentSingleSi::ERQTelescopeGeoComponentSingleSi(TString typeFromXML, 
                                                                   TString id, 
                                                                   TString orientAroundZ) 
: ERGeoComponent(typeFromXML, id)
{
  TString volumeNameInd = (orientAroundZ == "X") ? "_X" : "_Y";  
  fOrientAroundZ = volumeNameInd;
  fVolumeName += volumeNameInd;
  fDeadLayerThicknessFrontSide = 0.;
  fDeadLayerThicknessBackSide  = 0.;
}
//--------------------------------------------------------------------------------------------------
ERQTelescopeGeoComponentSingleSi::ERQTelescopeGeoComponentSingleSi(TString typeFromXML, TString id, 
                                                                   TVector3 position, 
                                                                   TVector3 rotation,
                                                                   TString  orientAroundZ)
: ERGeoComponent(typeFromXML, id, position, rotation)
{
  TString volumeNameInd = (orientAroundZ == "X") ? "_X" : "_Y";  
  fOrientAroundZ = volumeNameInd;
  fVolumeName += volumeNameInd;
  fDeadLayerThicknessFrontSide = 0.;
  fDeadLayerThicknessBackSide  = 0.;
}
//--------------------------------------------------------------------------------------------------
ERQTelescopeGeoComponentSingleSi::~ERQTelescopeGeoComponentSingleSi() {
}
//--------------------------------------------------------------------------------------------------
void ERQTelescopeGeoComponentSingleSi::ConstructGeometryVolume(void) {
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
  // --------------------------------------------------------------------------
  // -------   Geometry file name (output)   ----------------------------------
  TString geoFileName = geoPath + "/geometry/QTelescope.temp.root";
  // --------------------------------------------------------------------------
  // -----------------   Get and create the required media    -----------------
  FairGeoMedia*   geoMedia = geoFace->getMedia();
  FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();
  // ----- Create media for Single Si -----------------------------------------
  FairGeoMedium* mSingleSi;
  TGeoMedium*    pMed; 
  mSingleSi = geoMedia->getMedium(fMedia);

  if ( ! mSingleSi ) Fatal("Main", "Medium for singleSi not found");
  geoBuild->createMedium(mSingleSi);
  pMed = gGeoMan->GetMedium(fMedia);
  if ( ! pMed ) Fatal("Main", "Medium for SingleSi not found");
  LOG(DEBUG) << "Created single Si media" << FairLogger::endl;

  // --------------   Create geometry and top volume  -------------------------
  gGeoMan = (TGeoManager*)gROOT->FindObject("FAIRGeom");
  // ---------------- SingleSi-------------------------------------------------
  TGeoVolume* singleSiStrip;

  fVolume = gGeoManager->MakeBox(this->GetVolumeName(), pMed, fSizeX / 2., fSizeY / 2., fSizeZ / 2.);

  //------------------ Single Si strip --------------------------------------
  Double_t singleSiStripX = fSensX / fStripCount; 
  Double_t singleSiStripY = fSensY;   
  Double_t singleSiStripZ = fSensZ - fDeadLayerThicknessFrontSide - fDeadLayerThicknessBackSide;   
  singleSiStrip = gGeoManager->MakeBox("SensitiveSingleSiStrip"+fOrientAroundZ, pMed, 
                                                                        singleSiStripX / 2., 
                                                                        singleSiStripY / 2., 
                                                                        singleSiStripZ / 2.);
  //------------------ STRUCTURE  ---------------------------------------------
  //----------------------- Single Si structure -------------------------------
  //------------------ Add fibers to station  along x -----------------------
  for (Int_t iStrip = 0; iStrip < fStripCount; iStrip++) {
    Double_t transX = (-1) * fSensX / 2
                        + singleSiStripX/2. + iStrip*singleSiStripX;
    Double_t transZ = fDeadLayerThicknessFrontSide 
                        - fDeadLayerThicknessBackSide;
    fVolume->AddNode(singleSiStrip, iStrip, new TGeoCombiTrans(transX, 0., transZ, new TGeoRotation()));
  }
  if (fOrientAroundZ == "Y") {
    fRotation->RotateZ(90.);
  }
}
//--------------------------------------------------------------------------------------------------
void ERQTelescopeGeoComponentSingleSi::ParseXmlParameters() {
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
ClassImp(ERGeoComponent)

