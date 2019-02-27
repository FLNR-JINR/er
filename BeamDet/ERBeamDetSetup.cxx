/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "ERBeamDetSetup.h"

#include <ostream>

#include "TGeoTube.h"
#include "TError.h"
#include "TMath.h"
#include "TGeoManager.h"
#include "TGeoMatrix.h"
#include "TGeoCompositeShape.h"
#include "TGeoSphere.h"
#include "TROOT.h"
#include <Riostream.h>
#include <TDOMParser.h>
#include <TXMLAttr.h>
#include <TXMLNode.h>
#include <TList.h>
#include "TSystem.h"

#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4EmCalculator.hh"
#include "G4NistManager.hh"

#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRuntimeDb.h"
#include "FairGeoLoader.h"
#include "FairGeoMedium.h"
#include "FairGeoInterface.h"
#include "FairGeoBuilder.h"
#include "FairGeoMedia.h"
#include "FairLogger.h"

using namespace std;


ERBeamDetSetup* ERBeamDetSetup::fInstance = NULL;
Double_t        ERBeamDetSetup::fTargetR = 0;
Int_t           ERBeamDetSetup::fMWPCCount = 0;
Int_t           ERBeamDetSetup::fToFCount = 0;
Double_t        ERBeamDetSetup::fDistanceBetweenToF = 0;
Double_t        ERBeamDetSetup::fDistanceBetweenMWPC = 0;
map<Int_t, map<Int_t, map<Int_t, ERBeamDetWire*>>>ERBeamDetSetup::fWires;
// ----- ToF parameters -----------------------------------------------------
vector<Double_t> ERBeamDetSetup::fPlasticX;
vector<Double_t> ERBeamDetSetup::fPlasticY;
vector<Double_t> ERBeamDetSetup::fPlasticZ;
vector<TString>  ERBeamDetSetup::fPlasticMedia;
// --------------------------------------------------------------------------
// ----- MWPC parameters ----------------------------------------------------
vector<Double_t> ERBeamDetSetup::fGasVolX;
vector<Double_t> ERBeamDetSetup::fGasVolY;
vector<Double_t> ERBeamDetSetup::fGasVolZ;
vector<Double_t> ERBeamDetSetup::fGasStripX;
vector<Double_t> ERBeamDetSetup::fGasStripY;
vector<Double_t> ERBeamDetSetup::fGasStripZ; //cm
vector<Double_t> ERBeamDetSetup::fDistBetweenXandY;
vector<Double_t> ERBeamDetSetup::fCathodeThickness;
vector<Double_t> ERBeamDetSetup::fKaptonWindowThickness;
vector<Double_t> ERBeamDetSetup::fAnodeWireDiameter;
vector<TString>  ERBeamDetSetup::fKaptonWindowMedia;
vector<TString>  ERBeamDetSetup::fCathodeMedia;
vector<TString>  ERBeamDetSetup::fAnodeWireMedia;
vector<TString>  ERBeamDetSetup::fGasMedia;
vector<TString>  ERBeamDetSetup::fMWPCnumberingOrderX;
vector<TString>  ERBeamDetSetup::fMWPCnumberingOrderY;
// --------------------------------------------------------------------------
// ------ fPosition of detector's parts relative to zero ---------------------
vector<Double_t> ERBeamDetSetup::fPositionToF;
vector<Double_t> ERBeamDetSetup::fPositionMWPC;
// -------- fTarget parameters -----------------------------------------------
Double_t ERBeamDetSetup::fTargetH2R = 10.;   //cm
Double_t ERBeamDetSetup::fTargetH2Z = 0.4;   //cm
Double_t ERBeamDetSetup::fTargetShellThicknessSide = 20 * 1e-4;
Double_t ERBeamDetSetup::fTargetShellThicknessZ = 6 * 1e-4;
Bool_t   ERBeamDetSetup::fSensitiveTargetIsSet = false;

TString  ERBeamDetSetup::fParamsXmlFileName = "equip.xml";
vector<TString>  ERBeamDetSetup::fToFType;
vector<TString>  ERBeamDetSetup::fMWPCType;
Bool_t           ERBeamDetSetup::fGeoFromContainerIsRead = kFALSE;



ERBeamDetSetup::ERBeamDetSetup() {
  LOG(DEBUG) << "ERBeamDetSetup initialized! "<< FairLogger::endl;
}
//-------------------------------------------------------------------------
ERBeamDetSetup::~ERBeamDetSetup() {
}
//-------------------------------------------------------------------------
void ERBeamDetSetup::AddMWPC(TString type, Double_t position) {
  fPositionMWPC.push_back(position);
  fMWPCType.push_back(type);
  fMWPCnumberingOrderX.push_back("dir");
  fMWPCnumberingOrderY.push_back("dir");
  fMWPCCount++;
}
//-------------------------------------------------------------------------
void ERBeamDetSetup::AddToF(TString type, Double_t position) {
  fPositionToF.push_back(position);
  fToFType.push_back(type);
  fToFCount++;
}
//-------------------------------------------------------------------------
void ERBeamDetSetup::SetMWPCnumberingInvOrderX() {
  fMWPCnumberingOrderX.back() = "inv";
}
//-------------------------------------------------------------------------
void ERBeamDetSetup::SetMWPCnumberingInvOrderY() {
  fMWPCnumberingOrderY.back() = "inv";  
}
//-------------------------------------------------------------------------
void ERBeamDetSetup::GetGeoParamsFromParContainer() {
  if (fGeoFromContainerIsRead) {
    return ;
  }
  // --- Catch absence of TGeoManager
  if ( ! gGeoManager ) {
    LOG(FATAL) << "ERBeamDetSetup: cannot initialise without TGeoManager!"<< FairLogger::endl;
  }
 
  gGeoManager->CdTop();
  TGeoNode* cave = gGeoManager->GetCurrentNode();
  TGeoNode* beamDet  = NULL;
  for (Int_t iNode = 0; iNode < cave->GetNdaughters(); iNode++) {
    TString name = cave->GetDaughter(iNode)->GetName();
    if ( name.Contains("BeamDet", TString::kIgnoreCase) ) {
      beamDet = cave->GetDaughter(iNode);
      break;
    }
  }

  // ---- Getting MWPC geometry parameters---------------------------------
  TGeoNode*   mwpc = NULL;
  TGeoNode*   mwpcStation = NULL;
  Double_t    mwpcStationZ;
  Double_t    mwpcStationZ1;
  Double_t    mwpcStationZ2;
  TGeoMatrix* mwpcLocalPos;
  Double_t    mwpcMasterPos[3];
  TGeoNode*   plane = NULL;
  TGeoNode*   wire = NULL;
  Int_t       mwpcNb = 0;
  for (Int_t iNode = 0; iNode < beamDet->GetNdaughters(); iNode++) {
    TString name = beamDet->GetDaughter(iNode)->GetName();
    if (name.Contains("MWPC", TString::kIgnoreCase) ) {
      mwpc = beamDet->GetDaughter(iNode);
      mwpcStationZ = mwpc->GetMatrix()->GetTranslation()[2]; 
      (name.EndsWith("1", TString::kIgnoreCase)) ? mwpcStationZ1 = mwpcStationZ 
                                                 : mwpcStationZ2 = mwpcStationZ;
      mwpcStation = mwpc->GetDaughter(0);
      //--------------------------------------------------------------------
      for (Int_t planeNb = 0; planeNb < mwpcStation->GetNdaughters(); planeNb++) {
        plane = mwpcStation->GetDaughter(planeNb);
        for (Int_t wireNb = 0; wireNb < plane->GetNdaughters(); wireNb++) {
          wire = plane->GetDaughter(wireNb);
          Double_t x = wire->GetMatrix()->GetTranslation()[0];
          Double_t y = wire->GetMatrix()->GetTranslation()[1];
          Double_t z = wire->GetMatrix()->GetTranslation()[2];
          Double_t localCoords[3];
          Double_t inPlaneCoords[3];
          Double_t inStationCoords[3];
          Double_t globalCoords[3];
          localCoords[0] = x;
          localCoords[1] = y;
          localCoords[2] = z;
          plane->LocalToMaster(localCoords, inPlaneCoords);
          mwpcStation->LocalToMaster(inPlaneCoords, inStationCoords);
          mwpc->LocalToMaster(inStationCoords, globalCoords);
          TString wirePlaneOrient;
          if (planeNb == 0) {
            wirePlaneOrient = "X";
            fWires[mwpcNb][planeNb].insert(std::make_pair(wireNb, new ERBeamDetWire(globalCoords[0], 
                                                                                    globalCoords[1], 
                                                                                    globalCoords[2])));
          } else {
            wirePlaneOrient = "Y";
            fWires[mwpcNb][planeNb].insert(std::make_pair(wireNb, new ERBeamDetWire(globalCoords[0], 
                                                                                    globalCoords[1], 
                                                                                    globalCoords[2])));
          }
          
          LOG(DEBUG) << "Wire" << wirePlaneOrient << " " << wireNb 
                     << " position (" << fWires[mwpcNb][planeNb][wireNb]->fX << ", " 
                     << fWires[mwpcNb][planeNb][wireNb]->fY << ", " 
                     << fWires[mwpcNb][planeNb][wireNb]->fZ << ") cm" << FairLogger::endl;
        }
      } 
      mwpcNb++;
    }
  }
  // Stations located simmetrically relative to local center
  fDistanceBetweenMWPC = TMath::Abs(mwpcStationZ1 - mwpcStationZ2);
  LOG(DEBUG) << "The distance between MWPC stations: " << fDistanceBetweenMWPC << " cm;" << FairLogger::endl;
  //-----------------------------------------------------------------------
  // ---- Getting tofPlastic geometry parameters ---------------------------------
  TGeoNode* tofPlastic = NULL;
  Double_t tofPlastic1Pos, tofPlastic2Pos;
  for (Int_t iNode = 0; iNode < beamDet->GetNdaughters(); iNode++) {
    TString name = beamDet->GetDaughter(iNode)->GetName();
    if ( name.Contains("plastic", TString::kIgnoreCase) ) {
      tofPlastic = beamDet->GetDaughter(iNode);
      fPositionToF.push_back(tofPlastic->GetMatrix()->GetTranslation()[2]);
    }
  }
  fToFCount = fPositionToF.size();
  fDistanceBetweenToF = TMath::Abs(fPositionToF[0] - fPositionToF[GetToFCount() - 1]);
  LOG(DEBUG)<< "The distance between plastics: " << fDistanceBetweenToF << " cm;" << FairLogger::endl;
  //-----------------------------------------------------------------------
  // ---- Getting fTarget geometry parameters ------------------------------
  TGeoNode* fTarget = NULL;
  for (Int_t iNode = 0; iNode < beamDet->GetNdaughters(); iNode++) {
    TString name = beamDet->GetDaughter(iNode)->GetName();
    if ( name.Contains("Target", TString::kIgnoreCase) ) {
      fTarget = beamDet->GetDaughter(iNode);
      TGeoNode* shell = fTarget->GetDaughter(0);
      TGeoNode* h2 = shell->GetDaughter(0);
      TGeoTube* h2Tube = (TGeoTube*)h2->GetVolume()->GetShape();
      fTargetR = h2Tube->GetRmax();
      LOG(DEBUG)<< "Target radius " << fTargetR << " cm;" << FairLogger::endl;
      break;
    }
  } 
  fGeoFromContainerIsRead = kTRUE;  
  LOG(DEBUG) << "ERBeamDetSetup: read parameters from parContainer! "<< FairLogger::endl; 
}
//--------------------------------------------------------------------------------------------------
ERBeamDetSetup* ERBeamDetSetup::Instance(){
  if (fInstance == NULL)
    return new ERBeamDetSetup();
  else
    return fInstance;
}
//--------------------------------------------------------------------------------------------------
Int_t ERBeamDetSetup::SetParContainers(){
      // Get run and runtime database
  FairRun* run = FairRun::Instance();
  if ( ! run ) Fatal("SetParContainers", "No analysis run");

  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  if ( ! rtdb ) Fatal("SetParContainers", "No runtime database");

}
//--------------------------------------------------------------------------------------------------
Double_t ERBeamDetSetup::WireX(Int_t mwpcNb, Int_t planeNb, Int_t wireNb){
  return fWires[mwpcNb][planeNb][wireNb]->fX;
}
//--------------------------------------------------------------------------------------------------
Double_t ERBeamDetSetup::WireY(Int_t mwpcNb, Int_t planeNb, Int_t wireNb){
  return fWires[mwpcNb][planeNb][wireNb]->fY;
}
//--------------------------------------------------------------------------------------------------
Double_t ERBeamDetSetup::WireZ(Int_t mwpcNb, Int_t planeNb, Int_t wireNb){
  return fWires[mwpcNb][planeNb][wireNb]->fZ;
}
//--------------------------------------------------------------------------------------------------
void ERBeamDetSetup::GetToFParameters(TXMLNode *node) {
  node = node->GetNextNode();
  for(Int_t i = 0; i < fToFCount; i++) {
    TXMLNode* curNode = node;
    for(; curNode; curNode = curNode->GetNextNode()) {
      LOG(DEBUG) << "Pasrsing ToF " << curNode->GetNodeName() << FairLogger::endl;
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
      LOG(DEBUG) << "ToF type " << fToFType[i] << " " << attr->GetValue() << FairLogger::endl;
      if(!strcasecmp(fToFType[i], attr->GetValue())) {
        LOG(DEBUG) << "Tof value " << attr->GetValue() << FairLogger::endl;
        TXMLNode* curNode2 = curNode->GetChildren();
        for(; curNode2; curNode2 = curNode2->GetNextNode()) {
          if(!strcasecmp(curNode2->GetNodeName(), "plasticGeometry")) {
            attrList = curNode2->GetAttributes();
            attr = 0;
            TIter nextPlasticAttr(attrList);
            while ((attr=(TXMLAttr*)nextPlasticAttr())) {
              if (!strcasecmp("X", attr->GetName())) {
                fPlasticX.push_back(atof(attr->GetValue()));
              }
              if (!strcasecmp("Y", attr->GetName())) {
                fPlasticY.push_back(atof(attr->GetValue()));
              }
              if (!strcasecmp("Z", attr->GetName())) {
                fPlasticZ.push_back(atof(attr->GetValue()));
              }
            }
          }
          if(!strcasecmp(curNode2->GetNodeName(), "plasticMedia")) {
            fPlasticMedia.push_back(curNode2->GetText());
          }
        }
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------
void ERBeamDetSetup::GetMWPCParameters(TXMLNode *node) {
  node = node->GetNextNode();
  for(Int_t i = 0; i < fMWPCCount; i++) {
    TXMLNode* curNode = node;
    for(; curNode; curNode = curNode->GetNextNode()) {
      //LOG(DEBUG) << "Pasrsing ToF " << node->GetNodeName() << FairLogger::endl;
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
      //LOG(DEBUG) << "Pasrsing MWPC " << node->GetNodeName() << FairLogger::endl;
      if(!strcasecmp(fMWPCType[i], attr->GetValue())) {
        TXMLNode* curNode2 = curNode->GetChildren();
        for(; curNode2; curNode2 = curNode2->GetNextNode()) {
          if(!strcasecmp(curNode2->GetNodeName(), "gasVolGeometry")) {
            attrList = curNode2->GetAttributes();
            attr = 0;
            TIter nextGasVolAttr(attrList);
            while ((attr=(TXMLAttr*)nextGasVolAttr())) {
              if (!strcasecmp("X", attr->GetName())) {
                fGasVolX.push_back(atof(attr->GetValue()));
              }
              if (!strcasecmp("Y", attr->GetName())) {
                fGasVolY.push_back(atof(attr->GetValue()));
              }
              if (!strcasecmp("Z", attr->GetName())) {
                fGasVolZ.push_back(atof(attr->GetValue()));
              }
            }
          }
          if(!strcasecmp(curNode2->GetNodeName(), "gasStripGeometry")) {
            attrList = curNode2->GetAttributes();
            attr = 0;
            TIter nextGasStripAttr(attrList);
            while ((attr=(TXMLAttr*)nextGasStripAttr())) {
              if (!strcasecmp("X", attr->GetName())) {
                fGasStripX.push_back(atof(attr->GetValue()));
              }
              if (!strcasecmp("Y", attr->GetName())) {
                fGasStripY.push_back(atof(attr->GetValue()));
              }
              if (!strcasecmp("Z", attr->GetName())) {
                fGasStripZ.push_back(atof(attr->GetValue()));
              }
            }
          }
          if(!strcasecmp(curNode2->GetNodeName(), "distBetweenXandYStrips")) {
            fDistBetweenXandY.push_back(atof(curNode2->GetText()));
          }
          if(!strcasecmp(curNode2->GetNodeName(), "cathodeThickness")) {
            fCathodeThickness.push_back(atof(curNode2->GetText()));
          }
          if(!strcasecmp(curNode2->GetNodeName(), "kaptonWindowThickness")) {
            fKaptonWindowThickness.push_back(atof(curNode2->GetText()));
          }
          if(!strcasecmp(curNode2->GetNodeName(), "anodeWireDiameter")) {
            fAnodeWireDiameter.push_back(atof(curNode2->GetText()));
          }
          if(!strcasecmp(curNode2->GetNodeName(), "kaptonWindowMedia")) {
            fKaptonWindowMedia.push_back(curNode2->GetText());
          }
          if(!strcasecmp(curNode2->GetNodeName(), "cathodeMedia")) {
            fCathodeMedia.push_back(curNode2->GetText());
          }
          if(!strcasecmp(curNode2->GetNodeName(), "anodeWireMedia")) {
            fAnodeWireMedia.push_back(curNode2->GetText());
          }
          if(!strcasecmp(curNode2->GetNodeName(), "gasMedia")) {
            fGasMedia.push_back(curNode2->GetText());
          }
        }
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------
Double_t ERBeamDetSetup::GetDistanceBetweenToF(Int_t tof1Ind, Int_t tof2Ind) {
  return TMath::Abs(fPositionToF[0] - fPositionToF[tof2Ind]);
}
//--------------------------------------------------------------------------------------------------
void ERBeamDetSetup::PrintDetectorParameters(void) {
  LOG(DEBUG) << "------------------------------------------------" << "\r\n";
  LOG(DEBUG) << "Detector's parameters from " << fParamsXmlFileName << "\r\n";
  LOG(DEBUG) << "------------------------------------------------" << "\r\n";
  LOG(DEBUG) << "ToFs parameters:" << "\r\n";
  for(Int_t i = 0; i < fToFCount; i++) {
    LOG(DEBUG) << "ToF_"+TString::Itoa(i+1, 10) << " is " << fToFType[i] << " with parameters:" << "\r\n"
         << "\tpositionZ = " << fPositionToF[i] << "\r\n"
         << "\tX = " << fPlasticX[i]
         << "; Y = " << fPlasticY[i] 
         << "; Z = " << fPlasticZ[i] << "\r\n"
         << "\tplasticMedia = " << fPlasticMedia[i] << "\r\n" << "\r\n";
  }
  LOG(DEBUG) << "------------------------------------------------" << "\r\n";
  LOG(DEBUG) << "MWPCs parameters:" << "\r\n";
  for(Int_t i = 0; i < fMWPCCount; i++) {
    LOG(DEBUG) << "MWPC_"+TString::Itoa(i+1, 10) << " is " << fMWPCType[i] << " with parameters: " << "\r\n"
         << "\tpositionZ = " << fPositionMWPC[i] << "\r\n"
         << "\tGasVolX = " << fGasVolX[i]
         << "; GasVolY = " << fGasVolY[i] 
         << "; GasVolZ = " << fGasVolZ[i] << "\r\n"
         << "\tGasStripX = " << fGasStripX[i]
         << "; GasStripY = " << fGasStripY[i] 
         << "; GasStripZ = " << fGasStripZ[i] << "\r\n"
         << "\tDistance between X & Y strips = " << fDistBetweenXandY[i] << "\r\n"
         << "\tCathode thickness = " << fCathodeThickness[i] << "\r\n"
         << "\tKaptonWindow thickness = " << fKaptonWindowThickness[i] << "\r\n"
         << "\tWire diameter = " << fAnodeWireDiameter[i] << "\r\n"
         << "\tKaptonWindow media = " << fKaptonWindowMedia[i] << "\r\n"
         << "\tCathode media = " << fCathodeMedia[i] << "\r\n"
         << "\tAnodeWire media = " << fAnodeWireMedia[i] << "\r\n"
         << "\tgasStrip media = " << fGasMedia[i] << "\r\n" << "\r\n";
  }
}
//--------------------------------------------------------------------------------------------------
void ERBeamDetSetup::PrintDetectorParametersToFile(TString fileName) {
  ofstream listingFile;
  listingFile.open(fileName);
  listingFile << "------------------------------------------------" << "\r\n";
  listingFile << "Detector's parameters from " << fParamsXmlFileName << "\r\n";
  listingFile << "------------------------------------------------" << "\r\n";
  listingFile << "ToFs parameters:" << "\r\n";
  for(Int_t i = 0; i < fToFCount; i++) {
    listingFile << "ToF_"+TString::Itoa(i+1, 10) << " is " << fToFType[i] << " with parameters:" << "\r\n"  
         << "\tpositionZ = " << fPositionToF[i] << "\r\n"
         << "\tX = " << fPlasticX[i]
         << "; Y = " << fPlasticY[i] 
         << "; Z = " << fPlasticZ[i] << "\r\n"
         << "\tplasticMedia = " << fPlasticMedia[i] << "\r\n" << "\r\n";
  }
  listingFile << "------------------------------------------------" << "\r\n";
  listingFile << "MWPCs parameters:" << "\r\n";
  for(Int_t i = 0; i < fMWPCCount; i++) {
    listingFile << "MWPC_"+TString::Itoa(i+1, 10) << " is " << fMWPCType[i] << " with parameters: " << "\r\n"
         << "\tpositionZ = " << fPositionMWPC[i] << "\r\n"
         << "\tGasVolX = " << fGasVolX[i]
         << "; GasVolY = " << fGasVolY[i] 
         << "; GasVolZ = " << fGasVolZ[i] << "\r\n"
         << "\tGasStripX = " << fGasStripX[i]
         << "; GasStripY = " << fGasStripY[i] 
         << "; GasStripZ = " << fGasStripZ[i] << "\r\n"
         << "\tDistance between X & Y strips = " << fDistBetweenXandY[i] << "\r\n"
         << "\tCathode thickness = " << fCathodeThickness[i] << "\r\n"
         << "\tKaptonWindow thickness = " << fKaptonWindowThickness[i] << "\r\n"
         << "\tWire diameter = " << fAnodeWireDiameter[i] << "\r\n"
         << "\tKaptonWindow media = " << fKaptonWindowMedia[i] << "\r\n"
         << "\tCathode media = " << fCathodeMedia[i] << "\r\n"
         << "\tAnodeWire media = " << fAnodeWireMedia[i] << "\r\n"
         << "\tgasStrip media = " << fGasMedia[i] << "\r\n" << "\r\n";
  }
}
//--------------------------------------------------------------------------------------------------
void ERBeamDetSetup::ParseXmlParameters() {
  TDOMParser *domParser;//
  //gROOT->ProcessLine(".O 0"); 
  domParser = new TDOMParser;
  domParser->SetValidate(false); // do not validate with DTD
  Int_t parsecode = domParser->ParseFile(fParamsXmlFileName);
  if (parsecode < 0) {
     cerr << domParser->GetParseCodeMessage(parsecode) << FairLogger::endl;
 //    return -1;
  }
  TXMLNode *rootNode = domParser->GetXMLDocument()->GetRootNode();
  TXMLNode *detPartNode = rootNode->GetChildren()->GetNextNode();//->GetChildren();
  TXMLNode *curNode;
        LOG(DEBUG) << "Cmp ToF "  << FairLogger::endl;

  for ( ; detPartNode; detPartNode = detPartNode->GetNextNode()) { // detector's part
    if(!strcasecmp(detPartNode->GetNodeName(), "ToFTypes")) {
      LOG(DEBUG) << "Cmp ToF " << detPartNode->GetNodeName() << FairLogger::endl;
      GetToFParameters(detPartNode->GetChildren());
    }
    if(!strcasecmp(detPartNode->GetNodeName(), "MWPCTypes")) {
     // LOG(DEBUG) << "Cmp MWPC " << detPartNode->GetNodeName() << FairLogger::endl;
      GetMWPCParameters(detPartNode->GetChildren());
    }
  }
  //return 0;
}
//--------------------------------------------------------------------------------------------------
void ERBeamDetSetup::ConstructGeometry() {
  ParseXmlParameters();
  PrintDetectorParameters();
  // ----- BeamDet parameters -------------------------------------------------
  Double_t transTargetX = 0.;
  Double_t transTargetY = 0.; 
  Double_t transTargetZ = 0.;
  // --------------------------------------------------------------------------
  // Create a global translation
  Float_t global_X = 0.;
  Float_t global_Y = 0.;
  Float_t global_Z = 0.;
  //Create gloabal Rotation
  TGeoRotation *fGlobalRotation = new TGeoRotation();
  fGlobalRotation->RotateX(0.);
  fGlobalRotation->RotateY(0.);
  fGlobalRotation->RotateZ(0.);
  // Create a zero rotation
  TGeoRotation *fZeroRotation = new TGeoRotation();
  fZeroRotation->RotateX(0.);
  fZeroRotation->RotateY(0.);
  fZeroRotation->RotateZ(0.);
  // Create a 90 degree rotation around Z axis
  TGeoRotation *f90ZRotation = new TGeoRotation();
  f90ZRotation->RotateX(0.);
  f90ZRotation->RotateY(0.);
  f90ZRotation->RotateZ(90.);
  // Create a 270 degree rotation around Z axis
  TGeoRotation *f270ZRotation = new TGeoRotation();
  f270ZRotation->RotateX(0.);
  f270ZRotation->RotateY(0.);
  f270ZRotation->RotateZ(270.);
  // Create a 180 degree rotation around Z axis
  TGeoRotation *f180ZRotation = new TGeoRotation();
  f180ZRotation->RotateX(0.);
  f180ZRotation->RotateY(0.);
  f180ZRotation->RotateZ(180.);
  // Create a 90 degree rotation around X axis
  TGeoRotation *f90XRotation = new TGeoRotation();
  f90XRotation->RotateX(90.);
  f90XRotation->RotateY(0.);
  f90XRotation->RotateZ(0.);

  TGeoRotation *fRotationY = new TGeoRotation();
  fRotationY->RotateX(0.);
  fRotationY->RotateY(30.);
  fRotationY->RotateZ(0.);

  TGeoManager*   gGeoMan = NULL;
  // -------   Load media from media file   -----------------------------------
  FairGeoLoader*    geoLoad = FairGeoLoader::Instance();
  FairGeoInterface* geoFace = geoLoad->getGeoInterface();
  TString geoPath = gSystem->Getenv("VMCWORKDIR");
  TString medFile = geoPath + "/geometry/media.geo";
  geoFace->setMediaFile(medFile);
  geoFace->readMedia();
  gGeoMan = gGeoManager;
  // -------   Geometry file name (output)   ----------------------------------
  TString geoFileName = geoPath + "/geometry/beamdet.temp.root";
  // -----------------   Get and create the required media    -----------------
  FairGeoMedia*   geoMedia = geoFace->getMedia();
  FairGeoBuilder* geoBuild = geoLoad->getGeoBuilder();

  // ----- Create media for ToF -----------------------------------------------
  vector<FairGeoMedium*> mPlastic;
  vector<TGeoMedium*> pMedPlastic; 
  for(Int_t i = 0; i < fToFCount; i++) { 
    mPlastic.push_back(geoMedia->getMedium(fPlasticMedia[i]));
    if ( ! mPlastic[i] ) Fatal("Main", "FairMedium Plastic not found");
    geoBuild->createMedium(mPlastic[i]);
    pMedPlastic.push_back(gGeoMan->GetMedium(fPlasticMedia[i]));
    if ( ! pMedPlastic[i] ) Fatal("Main", "Medium Plastic not found");
  }
  // ----- Create media for MWPC ----------------------------------------------
  vector<FairGeoMedium*> mCF4;  
  vector<TGeoMedium*> pMedCF4;
  vector<FairGeoMedium*> mKaptonWindow;
  vector<TGeoMedium*> pMedKaptonWindow;
  vector<FairGeoMedium*> mCathode;
  vector<TGeoMedium*> pMedCathode;
  vector<FairGeoMedium*> mAnodeWire;
  vector<TGeoMedium*> pMedAnodeWire;
  for(Int_t i = 0; i < fMWPCCount; i++) {
    mCF4.push_back(geoMedia->getMedium(fGasMedia[i]));
    if ( ! mCF4[i] ) Fatal("Main", "FairMedium for gasStrip not found");
    geoBuild->createMedium(mCF4[i]);
    pMedCF4.push_back(gGeoMan->GetMedium(fGasMedia[i]));
    if ( ! pMedCF4[i] ) Fatal("Main", "Medium for gasStrip not found");

    mKaptonWindow.push_back(geoMedia->getMedium(fKaptonWindowMedia[i]));
    if ( ! mKaptonWindow[i] ) Fatal("Main", "FairMedium kapton not found");
    geoBuild->createMedium(mKaptonWindow[i]);
    pMedKaptonWindow.push_back(gGeoMan->GetMedium(fKaptonWindowMedia[i]));
    if ( ! pMedKaptonWindow[i] ) Fatal("Main", "Medium kapton not found");

    mCathode.push_back(geoMedia->getMedium(fCathodeMedia[i]));
    if ( ! mCathode[i] ) Fatal("Main", "FairMedium aluminium not found");
    geoBuild->createMedium(mCathode[i]); 
    pMedCathode.push_back(gGeoMan->GetMedium(fCathodeMedia[i]));
    if ( ! pMedCathode[i] ) Fatal("Main", "Medium aluminium not found");

    mAnodeWire.push_back(geoMedia->getMedium(fAnodeWireMedia[i]));
    if ( ! mAnodeWire[i] ) Fatal("Main", "FairMedium tungsten not found");
    geoBuild->createMedium(mAnodeWire[i]);
    pMedAnodeWire.push_back(gGeoMan->GetMedium(fAnodeWireMedia[i]));
    if ( ! pMedAnodeWire[i] ) Fatal("Main", "Medium tungsten not found");
  }
  // ------ Create media for Target -------------------------------------------
  FairGeoMedium* mH2 = geoMedia->getMedium("H2");
  if ( ! mH2 ) Fatal("Main", "FairMedium H2 not found");
  geoBuild->createMedium(mH2);
  TGeoMedium* pH2 = gGeoMan->GetMedium("H2");
  if ( ! pH2 ) Fatal("Main", "Medium H2 not found"); 

  FairGeoMedium* mSteel = geoMedia->getMedium("Steel");
  if ( ! mSteel ) Fatal("Main", "FairMedium Steel not found");
  geoBuild->createMedium(mSteel);
  TGeoMedium* pSteel = gGeoMan->GetMedium("Steel");
  if ( ! pSteel ) Fatal("Main", "Medium vacuum not found");
  // ------ Create vacuum media -----------------------------------------------
  FairGeoMedium* vacuum = geoMedia->getMedium("vacuum");
  if ( ! vacuum ) Fatal("Main", "FairMedium vacuum not found");
  geoBuild->createMedium(vacuum);
  TGeoMedium* pMed0 = gGeoMan->GetMedium("vacuum");
  if ( ! pMed0 ) Fatal("Main", "Medium vacuum not found");
  //------------------------- VOLUMES -----------------------------------------
  // --------------   Create geometry and top volume  -------------------------
  gGeoMan = (TGeoManager*)gROOT->FindObject("FAIRGeom");
  //gGeoMan->SetName("BeamDetGeom");
  TGeoVolume* top   = new TGeoVolumeAssembly("TOP");
  //gGeoMan->SetTopVolume(top);
  TGeoVolume* beamdet = new TGeoVolumeAssembly("beamdet");
  //TGeoVolume* MWPC    = new TGeoVolumeAssembly("MWPC");
  TGeoVolume* target  = new TGeoVolumeAssembly("target");

  // ---------------- Target --------------------------------------------------
  if (fSensitiveTargetIsSet) {
    Double_t fTargetShellR = fTargetH2R + fTargetShellThicknessSide;
    Double_t fTargetShellZ = fTargetH2Z/2 + fTargetShellThicknessZ;

    TGeoVolume *targetH2 = gGeoManager->MakeTube("targetBodyH2", pH2, 0, fTargetH2R, fTargetH2Z/2);
    TGeoVolume *targetShell = gGeoManager->MakeTube("targetShell", pSteel, 0, fTargetShellR, fTargetShellZ);
    
    targetShell->AddNode(targetH2, 1, new TGeoCombiTrans(.0, .0, .0, fZeroRotation));
    target->AddNode(targetShell, 1, new TGeoCombiTrans(.0,.0,.0, fZeroRotation)); 

    beamdet->AddNode(target, 1, new TGeoCombiTrans(transTargetX, transTargetY, transTargetZ, fRotationY));
  }
  // ----------------- MWPC ---------------------------------------------------
  vector<TGeoVolume*> gasVol;
  vector<TGeoVolume*> MWPC;
  vector<TGeoVolume*> gasStrip;
  vector<TGeoVolume*> gasPlane;
  vector<TGeoVolume*> anodeWire;
  for(Int_t i = 0; i < fMWPCCount; i++) {
    gasVol.push_back(gGeoManager->MakeBox("MWPCVol_" + fMWPCType[i], pMedCF4[i], 
                                          fGasVolX[i]/2, fGasVolY[i]/2, fGasVolZ[i]/2));
    MWPC.push_back(gGeoManager->MakeBox("MWPC_" + fMWPCType[i], pMedKaptonWindow[i], 
                                        fGasVolX[i]/2, fGasVolY[i]/2, fGasVolZ[i]/2 + fKaptonWindowThickness[i]));
    gasStrip.push_back(gGeoManager->MakeBox("gasStrip_" + fMWPCType[i], pMedCF4[i], 
                                            fGasStripX[i]/2, fGasStripY[i]/2, fGasStripZ[i]/2));
    gasPlane.push_back(gGeoManager->MakeBox("gasPlane_" + fMWPCType[i], pMedCathode[i], 
                                            fGasVolX[i]/2, fGasVolY[i]/2, fGasStripZ[i]/2 + fCathodeThickness[i]));
    anodeWire.push_back(gGeoManager->MakeTube("anodeWire_" + fMWPCType[i], pMedAnodeWire[i], 
                                              0, fAnodeWireDiameter[i] / 2, fGasStripY[i]/2));
  }
  // ---------------- ToF -----------------------------------------------------

  TGeoVolume* fictitiousFirstPlastic; // fictitious first plastic for correct data reading from ParContainer
  fictitiousFirstPlastic = gGeoManager->MakeBox("plastic_Fictitous", pMed0, 
                                                fPlasticX[0]/2, fPlasticY[0]/2, fPlasticZ[0]/2);
  vector<TGeoVolume*> plastic;
  for(Int_t i = 0; i < fToFCount; i++) {
    plastic.push_back(gGeoManager->MakeBox("plastic_" + fToFType[i], pMedPlastic[i], 
                                           fPlasticX[i]/2, fPlasticY[i]/2, fPlasticZ[i]/2));
  }
  //------------------ STRUCTURE  ---------------------------------------------
  // ----------------- MWPC ---------------------------------------------------
  for(Int_t i = 0; i < fMWPCCount; i++) {
    gasStrip[i]->AddNode(anodeWire[i], 1, new TGeoCombiTrans(0, 0, 0, f90XRotation));
    Int_t gasCount = (fGasVolX[i]/2) / (fGasStripX[i]);
    Double_t gasPosX;
    for(Int_t i_gas = 1; i_gas <= 2*gasCount; i_gas++)
    {
      gasPosX = fGasVolX[i]/2 - fGasStripX[i] * (i_gas - 1) - fGasStripX[i]/2;
      gasPlane[i]->AddNode(gasStrip[i], i_gas, new TGeoCombiTrans(gasPosX, 0, 0, fZeroRotation));
    }

    if (fMWPCnumberingOrderX[i].Contains("inv", TString::kIgnoreCase)) {
      gasVol[i]->AddNode(gasPlane[i], 1, new TGeoCombiTrans(0, 0, -fDistBetweenXandY[i] / 2, fZeroRotation));
    } else {
      gasVol[i]->AddNode(gasPlane[i], 1, new TGeoCombiTrans(0, 0, -fDistBetweenXandY[i] / 2, f180ZRotation));
    }
    if (fMWPCnumberingOrderY[i].Contains("inv", TString::kIgnoreCase)) {
      gasVol[i]->AddNode(gasPlane[i], 2, new TGeoCombiTrans(0, 0, fDistBetweenXandY[i] / 2, f90ZRotation));
    } else {
      gasVol[i]->AddNode(gasPlane[i], 2, new TGeoCombiTrans(0, 0, fDistBetweenXandY[i] / 2, f270ZRotation));
    }
    MWPC[i]->AddNode(gasVol[i], 1, new TGeoCombiTrans(0, 0, 0, fZeroRotation));
    beamdet->AddNode(MWPC[i], i+1, new TGeoCombiTrans(global_X, global_Y, fPositionMWPC[i], fGlobalRotation));
  }
  // ---------------- ToF -----------------------------------------------------
  beamdet->AddNode(fictitiousFirstPlastic, 0, new TGeoCombiTrans(global_X, global_Y, fPositionToF[0], fGlobalRotation));
  for(Int_t i = 0; i < fToFCount; i++) {
    if (!i) {
      beamdet->AddNode(plastic[i], i+1, new TGeoCombiTrans(global_X, 
                                                           global_Y, 
                                                           fPositionToF[GetToFCount() - 1] - fPlasticZ[i] / 2, 
                                                           fGlobalRotation));      
      continue;
    }
    beamdet->AddNode(plastic[i], i+1, new TGeoCombiTrans(global_X, global_Y, fPositionToF[i], fGlobalRotation));
  }

  top->AddNode(beamdet, 1, new TGeoCombiTrans(global_X ,global_Y, global_Z, fGlobalRotation));
   // ---------------   Finish   -----------------------------------------------
  //gGeoMan->CloseGeometry();
  //gGeoMan->CheckOverlaps(0.001);
  //gGeoMan->PrintOverlaps();
  //gGeoMan->Test();

  //gGeoManager = gGeoMan;

  TFile* geoFile = new TFile(geoFileName, "RECREATE");
  top->Write();
  geoFile->Close();
  // --------------------------------------------------------------------------
}
//--------------------------------------------------------------------------------------------------
Double_t ERBeamDetSetup::CalcEloss(ERBeamDetTrack& track, Int_t pid, Float_t mom, Float_t mass){
  
  FairRun* run = FairRun::Instance();
  if (!TString(run->ClassName()).Contains("ERRunAna")){
    LOG(FATAL) << "Use ERRunAna for ERBeamDetSetup::CalcEloss!!!" << FairLogger::FairLogger::endl;
    return 0;
  }

  //calclculation ion energy loss in BeamDet volumes
  TVector3 targetVertex = track.GetTargetVertex();
  LOG(DEBUG) << " [CalcEloss] Eloss calculation with target vertex = (" << targetVertex.X() << ","
            << targetVertex.Y() << "," << targetVertex.Z() << "), direction on target = ("
            << track.GetVector().X() << "," << track.GetVector().Y() << "," << track.GetVector().Z() << ")" << FairLogger::FairLogger::endl;
  
  Float_t zStart = -3000.;//@TODO change to BeamDet start position
  Float_t xStart = targetVertex.X() + zStart*TMath::Sin(track.GetVector().Theta()) * TMath::Cos(track.GetVector().Phi());
  Float_t yStart = targetVertex.Y() + zStart*TMath::Sin(track.GetVector().Theta()) * TMath::Sin(track.GetVector().Phi());

  LOG(DEBUG) << " [CalcEloss] Eloss calculation start vertex = (" << xStart << "," << yStart << "," << zStart << ")" << FairLogger::FairLogger::endl; 

  G4IonTable* ionTable = G4IonTable::GetIonTable();
  G4ParticleDefinition* ion =  ionTable->GetIon(pid);
  G4EmCalculator* calc = new G4EmCalculator();
  G4NistManager* nist = G4NistManager::Instance();

  TGeoNode* node;
  node = gGeoManager->InitTrack(xStart,yStart,zStart,track.GetVector().X(),track.GetVector().Y(),track.GetVector().Z());
  
  Float_t E = TMath::Sqrt(mom*mom + mass*mass);
  Float_t T = E - mass;
  Float_t sumLoss = 0.;

  Bool_t inTarget = kFALSE;
  Float_t tarEdep = 0.;

  while(!gGeoManager->IsOutside()){
    
    TString matName = node->GetMedium()->GetMaterial()->GetName();
    G4Material* mat = nist->FindOrBuildMaterial(matName.Data());
    
    node = gGeoManager->FindNextBoundary();

    if (inTarget && !(TString(gGeoManager->GetPath()).Contains("target")))
      break;
    
    Double_t range = gGeoManager->GetStep();
    Double_t edep = calc->GetDEDX(T*1e3,ion,mat)*range*10*1e-3;

    node = gGeoManager->GetCurrentNode();
    
    LOG(DEBUG) <<" [CalcEloss] Kinetic Energy  = " << T << FairLogger::FairLogger::endl;
    LOG(DEBUG) <<" [CalcEloss] path  = " <<  gGeoManager->GetPath() << FairLogger::FairLogger::endl;
    LOG(DEBUG) <<" [CalcEloss] medium " << matName << FairLogger::FairLogger::endl;
    LOG(DEBUG) <<" [CalcEloss] range  = " << range << FairLogger::FairLogger::endl;
    LOG(DEBUG) <<" [CalcEloss] edep = " << edep << FairLogger::FairLogger::endl;

    if (TString(gGeoManager->GetPath()).Contains("target"))
      inTarget = kTRUE;

    if (inTarget)
      tarEdep+=edep;

    T -= edep;
    sumLoss += edep;
    node = gGeoManager->Step();
  }
  
  T += tarEdep/2.;
  sumLoss -= tarEdep/2.;
  
  LOG(DEBUG) <<" [CalcEloss] Target Eloss = " <<  tarEdep << FairLogger::FairLogger::endl;
  LOG(DEBUG) <<" [CalcEloss] Sum Eloss = " <<  sumLoss << FairLogger::FairLogger::endl;
  return T;
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERBeamDetSetup)
