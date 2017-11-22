#include "ERQTelescopeSetup.h"

#include <ostream>

#include "TGeoTube.h"
#include "TError.h"
#include "TMath.h"
#include "TGeoManager.h"
#include "TGeoMatrix.h"
#include "TROOT.h"
#include <Riostream.h>
#include <TDOMParser.h>
#include <TXMLAttr.h>
#include <TXMLNode.h>
#include <TList.h>

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairGeoLoader.h"
#include "FairGeoMedium.h"
#include "FairGeoInterface.h"
#include "FairGeoBuilder.h"
#include "FairGeoMedia.h"
#include "FairLogger.h"

using namespace std;

ERQTelescopeSetup* ERQTelescopeSetup::fInstance = NULL;
TString  ERQTelescopeSetup::fParamsXmlFileName = "qtelescope_params.xml";
vector<TString>    ERQTelescopeSetup::fSensVolumes;
// ----- SingleSi parameters --------------------------------------------------
Int_t            ERQTelescopeSetup::fDoubleSiCount = 0;
vector<TString>  ERQTelescopeSetup::fDoubleSiType;
vector<Double_t> ERQTelescopeSetup::fDoubleSiPosZ;
vector<TString>  ERQTelescopeSetup::fDoubleSiOrientAroundZ;
vector<Double_t> ERQTelescopeSetup::fDoubleSiX;
vector<Double_t> ERQTelescopeSetup::fDoubleSiY;
vector<Double_t> ERQTelescopeSetup::fDoubleSiZ;
vector<Double_t> ERQTelescopeSetup::fDoubleSiSensX;
vector<Double_t> ERQTelescopeSetup::fDoubleSiSensY;
vector<Double_t> ERQTelescopeSetup::fDoubleSiSensZ;
vector<Int_t>    ERQTelescopeSetup::fDoubleSiStripCountX;
vector<Int_t>    ERQTelescopeSetup::fDoubleSiStripCountY;
vector<TString>  ERQTelescopeSetup::fDoubleSiMedia;
// ----- DoubleSi parameters --------------------------------------------------
Int_t            ERQTelescopeSetup::fSingleSiCount = 0;
vector<TString>  ERQTelescopeSetup::fSingleSiType;  
vector<Double_t> ERQTelescopeSetup::fSingleSiPosZ;
vector<TString>  ERQTelescopeSetup::fSingleSiOrientAroundZ;
vector<Double_t> ERQTelescopeSetup::fSingleSiX;
vector<Double_t> ERQTelescopeSetup::fSingleSiY;
vector<Double_t> ERQTelescopeSetup::fSingleSiZ;
vector<Double_t> ERQTelescopeSetup::fSingleSiSensX;
vector<Double_t> ERQTelescopeSetup::fSingleSiSensY;
vector<Double_t> ERQTelescopeSetup::fSingleSiSensZ;
vector<Int_t>    ERQTelescopeSetup::fSingleSiStripCount;
vector<TString>  ERQTelescopeSetup::fSingleSiMedia;
// ----- CsI parameters -------------------------------------------------------
Int_t            ERQTelescopeSetup::fCsICount = 0;
vector<TString>  ERQTelescopeSetup::fCsIType;
vector<Double_t> ERQTelescopeSetup::fCsIPosZ;  
vector<TString>  ERQTelescopeSetup::fCsIOrientAroundZ;
vector<Double_t> ERQTelescopeSetup::fCsIX;
vector<Double_t> ERQTelescopeSetup::fCsIY;
vector<Double_t> ERQTelescopeSetup::fCsIZ;
vector<Int_t>    ERQTelescopeSetup::fCsICubesCountX;
vector<Int_t>    ERQTelescopeSetup::fCsICubesCountY;
vector<TString>  ERQTelescopeSetup::fCsIMedia;
//--------------------------------------------------------------------------------------------------
ERQTelescopeSetup::ERQTelescopeSetup() {
  std::cout << "ERQTelescopeSetup initialized! "<< std::endl;
}
//--------------------------------------------------------------------------------------------------
void ERQTelescopeSetup::AddSi(TString type, Double_t position, TString orientAroundZ) {
  if (type.BeginsWith("Double")) {
    TString firstBranchInd = "_X"; 
    TString secondSiBranchInd = "_Y";
    if (orientAroundZ == "Y") {  // if first plane  
      firstBranchInd = "_Y";
      secondSiBranchInd = "_X";
    } 
    fDoubleSiCount++;  
    fSensVolumes.push_back(type + firstBranchInd + TString::Itoa(fDoubleSiCount, 10));
    fSensVolumes.push_back(type + secondSiBranchInd + TString::Itoa(fDoubleSiCount, 10));
    fDoubleSiOrientAroundZ.push_back(orientAroundZ);
    fDoubleSiPosZ.push_back(position);
    fDoubleSiType.push_back(type);
  } else {
    fSingleSiCount++;
    if (orientAroundZ == "X") {
      fSensVolumes.push_back(type + "_X" + TString::Itoa(fSingleSiCount, 10));
    } else {
      fSensVolumes.push_back(type + "_Y" + TString::Itoa(fSingleSiCount, 10));
    }
    fSingleSiOrientAroundZ.push_back(orientAroundZ);
    fSingleSiPosZ.push_back(position);
    fSingleSiType.push_back(type);
  }
}
void ERQTelescopeSetup::AddCsI(TString type, Double_t position) {
  fCsICount++;
  fSensVolumes.push_back(type + TString::Itoa(fCsICount, 10));
  fCsIPosZ.push_back(position);
  fCsIType.push_back(type);
}
//--------------------------------------------------------------------------------------------------
ERQTelescopeSetup* ERQTelescopeSetup::Instance(){
  if (fInstance == NULL)
    return new ERQTelescopeSetup();
  else
    return fInstance;
}
//--------------------------------------------------------------------------------------------------
Int_t ERQTelescopeSetup::SetParContainers(){
      // Get run and runtime database
  FairRun* run = FairRun::Instance();
  if ( ! run ) Fatal("SetParContainers", "No analysis run");

  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  if ( ! rtdb ) Fatal("SetParContainers", "No runtime database");

}
void ERQTelescopeSetup::GetCsIParameters(TXMLNode *node) {
  node = node->GetNextNode();
  for (Int_t i = 0; i < fCsICount; i++) {
    TXMLNode* curNode = node;
    for (; curNode; curNode = curNode->GetNextNode()) {
      cout << "Pasrsing CsI " << curNode->GetNodeName() << endl;
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
      cout << "CsI type " << fCsIType[i] << " " << attr->GetValue() << endl;
      if (!strcasecmp(fCsIType[i], attr->GetValue())) {
        cout << "CsI value " << attr->GetValue() << endl;
        TXMLNode* curNode2 = curNode->GetChildren();
        for (; curNode2; curNode2 = curNode2->GetNextNode()) {
          if (!strcasecmp(curNode2->GetNodeName(), "CsISize")) {
            attrList = curNode2->GetAttributes();
            attr = 0;
            TIter nextPlasticAttr(attrList);
            while ((attr=(TXMLAttr*)nextPlasticAttr())) {
              if (!strcasecmp("CsIX", attr->GetName())) {
                fCsIX.push_back(atof(attr->GetValue()));
              }
              if (!strcasecmp("CsIY", attr->GetName())) {
                fCsIY.push_back(atof(attr->GetValue()));
              }
              if (!strcasecmp("CsIZ", attr->GetName())) {
                fCsIZ.push_back(atof(attr->GetValue()));
              }
            }
          }
          if (!strcasecmp(curNode2->GetNodeName(), "cubesCountX")) {
            fCsICubesCountX.push_back(atof(curNode2->GetText()));
          }
          if (!strcasecmp(curNode2->GetNodeName(), "cubesCountY")) {
            fCsICubesCountY.push_back(atof(curNode2->GetText()));
          }
          if (!strcasecmp(curNode2->GetNodeName(), "CsIMedia")) {
            fCsIMedia.push_back(curNode2->GetText());
          }
        }
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------
void ERQTelescopeSetup::GetSingleSiParameters(TXMLNode *node) {
  node = node->GetNextNode();
  for(Int_t i = 0; i < fSingleSiCount; i++) {
    TXMLNode* curNode = node;
    for(; curNode; curNode = curNode->GetNextNode()) {
      //cout << "Pasrsing ToF " << node->GetNodeName() << endl;
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
      cout << "Pasrsing MWPC " << node->GetNodeName() << endl;
      if(!strcasecmp(fSingleSiType[i], attr->GetValue())) {
        TXMLNode* curNode2 = curNode->GetChildren();
        for(; curNode2; curNode2 = curNode2->GetNextNode()) {
          if(!strcasecmp(curNode2->GetNodeName(), "singleSiSize")) {
            attrList = curNode2->GetAttributes();
            attr = 0;
            TIter nextSingleSiSizeAttr(attrList);
            while ((attr=(TXMLAttr*)nextSingleSiSizeAttr())) {
              if (!strcasecmp("singleSiX", attr->GetName())) {
                fSingleSiX.push_back(atof(attr->GetValue()));
              }
              if (!strcasecmp("singleSiY", attr->GetName())) {
                fSingleSiY.push_back(atof(attr->GetValue()));
              }
              if (!strcasecmp("singleSiZ", attr->GetName())) {
                fSingleSiZ.push_back(atof(attr->GetValue()));
              }
            }
          }
          if(!strcasecmp(curNode2->GetNodeName(), "singleSiSensAreaSize")) {
            attrList = curNode2->GetAttributes();
            attr = 0;
            TIter nextSingleSiSensSizeAttr(attrList);
            while ((attr=(TXMLAttr*)nextSingleSiSensSizeAttr())) {
              if (!strcasecmp("singleSiSensX", attr->GetName())) {
                fSingleSiSensX.push_back(atof(attr->GetValue()));
              }
              if (!strcasecmp("singleSiSensY", attr->GetName())) {
                fSingleSiSensY.push_back(atof(attr->GetValue()));
              }
              if (!strcasecmp("singleSiSensZ", attr->GetName())) {
                fSingleSiSensZ.push_back(atof(attr->GetValue()));
              }
            }
          }
          if(!strcasecmp(curNode2->GetNodeName(), "stripCount")) {
            fSingleSiStripCount.push_back(atof(curNode2->GetText()));
          }
          if(!strcasecmp(curNode2->GetNodeName(), "singleSiMedia")) {
            fSingleSiMedia.push_back(curNode2->GetText());
          }
        }
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------
void ERQTelescopeSetup::GetDoubleSiParameters(TXMLNode *node) {
  node = node->GetNextNode();
  for(Int_t i = 0; i < fDoubleSiCount; i++) {
    TXMLNode* curNode = node;
    for(; curNode; curNode = curNode->GetNextNode()) {
      //cout << "Pasrsing ToF " << node->GetNodeName() << endl;
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
      //cout << "Pasrsing MWPC " << node->GetNodeName() << endl;
      if(!strcasecmp(fDoubleSiType[i], attr->GetValue())) {
        TXMLNode* curNode2 = curNode->GetChildren();
        for(; curNode2; curNode2 = curNode2->GetNextNode()) {
          if(!strcasecmp(curNode2->GetNodeName(), "doubleSiSize")) {
            attrList = curNode2->GetAttributes();
            attr = 0;
            TIter nextDoubleSiSizeAttr(attrList);
            while ((attr=(TXMLAttr*)nextDoubleSiSizeAttr())) {
              if (!strcasecmp("doubleSiX", attr->GetName())) {
                fDoubleSiX.push_back(atof(attr->GetValue()));
              }
              if (!strcasecmp("doubleSiY", attr->GetName())) {
                fDoubleSiY.push_back(atof(attr->GetValue()));
              }
              if (!strcasecmp("doubleSiZ", attr->GetName())) {
                fDoubleSiZ.push_back(atof(attr->GetValue()));
              }
            }
          }
          if(!strcasecmp(curNode2->GetNodeName(), "doubleSiSensAreaSize")) {
            attrList = curNode2->GetAttributes();
            attr = 0;
            TIter nextDoubleSiSensSizeAttr(attrList);
            while ((attr=(TXMLAttr*)nextDoubleSiSensSizeAttr())) {
              if (!strcasecmp("doubleSiSensX", attr->GetName())) {
                fDoubleSiSensX.push_back(atof(attr->GetValue()));
              }
              if (!strcasecmp("doubleSiSensY", attr->GetName())) {
                fDoubleSiSensY.push_back(atof(attr->GetValue()));
              }
              if (!strcasecmp("doubleSiSensZ", attr->GetName())) {
                fDoubleSiSensZ.push_back(atof(attr->GetValue()));
              }
            }
          }
          if(!strcasecmp(curNode2->GetNodeName(), "stripCountX")) {
            fDoubleSiStripCountX.push_back(atof(curNode2->GetText()));
          }
          if(!strcasecmp(curNode2->GetNodeName(), "stripCountY")) {
            fDoubleSiStripCountY.push_back(atof(curNode2->GetText()));
          }
          if(!strcasecmp(curNode2->GetNodeName(), "doubleSiMedia")) {
            fDoubleSiMedia.push_back(curNode2->GetText());
          }
        }
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------
void ERQTelescopeSetup::PrintDetectorParameters(void) {
  cout << "------------------------------------------------" << endl;
  cout << "Detector's parameters from " << fParamsXmlFileName << endl;
  cout << "------------------------------------------------" << endl;
  cout << "CsI parameters:" << endl;
  for(Int_t i = 0; i < fCsICount; i++) {
    cout << "SingleSi_"+TString::Itoa(i+1, 10) << " is " 
         << fCsIType[i] << " with parameters:" << endl
         << "\tpositionZ = " << fCsIPosZ[i] << endl
         << "\tCsISizeX = " << fCsIX[i]
         << "; CsISizeY = " << fCsIY[i] 
         << "; CsISizeZ = " << fCsIZ[i] << endl
         << "\tCsICubesCountX = " << fCsICubesCountX[i] << endl 
         << "\tCsICubesCountY = " << fCsICubesCountY[i] << endl 
         << "\tCsIMedia = " << fCsIMedia[i] << endl 
         << endl;
  }
  cout << "------------------------------------------------" << endl;
  cout << "DoubleSi parameters:" << endl;
  for(Int_t i = 0; i < fDoubleSiCount; i++) {
    cout << "DoubleSi_"+TString::Itoa(i+1, 10) << " is " 
         << fDoubleSiType[i] << " with parameters:" << endl
         << "\tpositionZ = " << fDoubleSiPosZ[i] << endl
         << "\tdoubeSiSizeX = " << fDoubleSiX[i]
         << "; doubeSiSizeY = " << fDoubleSiY[i] 
         << "; doubeSiSizeZ = " << fDoubleSiZ[i] << endl
         << "\tdoubleSiSensX = " << fDoubleSiSensX[i]
         << "; doubleSiSensY = " << fDoubleSiSensY[i] 
         << "; doubleSiSensZ = " << fDoubleSiSensZ[i] << endl
         << "\tdoubleSiStripCountX = " << fDoubleSiStripCountX[i] << endl 
         << "\tdoubleSiStripCountX = " << fDoubleSiStripCountY[i] << endl 
         << "\tdoubleSiMedia = " << fDoubleSiMedia[i] << endl 
         << endl;
  }
  cout << "------------------------------------------------" << endl;
  cout << "SingleSi parameters:" << endl;
  for(Int_t i = 0; i < fSingleSiCount; i++) {
    cout << "SingleSi_"+TString::Itoa(i+1, 10) << " is " 
         << fSingleSiType[i] << " with parameters:" << endl
         << "\tpositionZ = " << fSingleSiPosZ[i] << endl
         << "\tsingleSiSizeX = " << fSingleSiX[i]
         << "; singleSiSizeY = " << fSingleSiY[i] 
         << "; singleSiSizeZ = " << fSingleSiZ[i] << endl
         << "\tsingleSiSensX = " << fSingleSiSensX[i]
         << "; singleSiSensY = " << fSingleSiSensY[i] 
         << "; singleSiSensZ = " << fSingleSiSensZ[i] << endl
         << "\tsingleSiStripCount = " << fSingleSiStripCount[i] << endl 
         << "\tsingleSiMedia = " << fSingleSiMedia[i] << endl 
         << endl;
  }
  cout << "------------------------------------------------" << endl;
}
//--------------------------------------------------------------------------------------------------
void ERQTelescopeSetup::PrintDetectorParametersToFile(TString fileName) {
  ofstream listingFile;
  listingFile.open(fileName);
}
//--------------------------------------------------------------------------------------------------
void ERQTelescopeSetup::ParseXmlParameters() {
  TDOMParser *domParser;//
  //gROOT->ProcessLine(".O 0");
  domParser = new TDOMParser;
  domParser->SetValidate(false); // do not validate with DTD

  Int_t parsecode = domParser->ParseFile(fParamsXmlFileName);
  if (parsecode < 0) {
     cerr << domParser->GetParseCodeMessage(parsecode) << endl;
 //    return -1;
  }

  TXMLNode *rootNode = domParser->GetXMLDocument()->GetRootNode();
  TXMLNode *detPartNode = rootNode->GetChildren();
  TXMLNode *SiTypeNodes;
  for ( ; detPartNode; detPartNode = detPartNode->GetNextNode()) { // detector's part
    if (!strcasecmp(detPartNode->GetNodeName(), "CsITypes")) {
      cout << "Cmp CsI " << detPartNode->GetNodeName() << endl;
      GetCsIParameters(detPartNode->GetChildren());
    }
    if (!strcasecmp(detPartNode->GetNodeName(), "SiTypes")) {
     // cout << "Cmp Si " << detPartNode->GetNodeName() << endl;
      SiTypeNodes = detPartNode->GetChildren();
      for ( ; SiTypeNodes; SiTypeNodes = SiTypeNodes->GetNextNode()) {
        if (!strcasecmp(SiTypeNodes->GetNodeName(), "singleSiTypes")) {
          GetSingleSiParameters(SiTypeNodes->GetChildren());
        }
        if (!strcasecmp(SiTypeNodes->GetNodeName(), "doubleSiTypes")) {
          GetDoubleSiParameters(SiTypeNodes->GetChildren());
        }
      }
    }
  }
}
//--------------------------------------------------------------------------------------------------
void ERQTelescopeSetup::ConstructGeometry() {
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
  // Create a 90 degree rotation around X axis
  TGeoRotation *f90XRotation = new TGeoRotation();
  f90XRotation->RotateX(90.);
  f90XRotation->RotateY(0.);
  f90XRotation->RotateZ(0.);

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

  // ----- Create media for Single Si------------------------------------------
  vector<FairGeoMedium*> mSingleSi;
  vector<TGeoMedium*> pMedSingleSi; 
  for (Int_t i = 0; i < fCsICount; i++) { 
    mSingleSi.push_back(geoMedia->getMedium(fSingleSiMedia[i]));
    if ( ! mSingleSi[i] ) Fatal("Main", "FairMedium Plastic not found");
    geoBuild->createMedium(mSingleSi[i]);
    pMedSingleSi.push_back(gGeoMan->GetMedium(fSingleSiMedia[i]));
    if ( ! pMedSingleSi[i] ) Fatal("Main", "Medium Plastic not found");
  }
  // ----- Create media for Double Si -----------------------------------------
  vector<FairGeoMedium*> mDoubleSi;
  vector<TGeoMedium*> pMedDoubleSi; 
  for (Int_t i = 0; i < fCsICount; i++) { 
    mDoubleSi.push_back(geoMedia->getMedium(fDoubleSiMedia[i]));
    if ( ! mDoubleSi[i] ) Fatal("Main", "FairMedium Plastic not found");
    geoBuild->createMedium(mDoubleSi[i]);
    pMedDoubleSi.push_back(gGeoMan->GetMedium(fDoubleSiMedia[i]));
    if ( ! pMedDoubleSi[i] ) Fatal("Main", "Medium Plastic not found");
  }
  // ------ Create media for CsI ----------------------------------------------
  vector<FairGeoMedium*> mCsI;
  vector<TGeoMedium*> pMedCsI; 
  for (Int_t i = 0; i < fCsICount; i++) { 
    mCsI.push_back(geoMedia->getMedium(fCsIMedia[i]));
    if ( ! mCsI[i] ) Fatal("Main", "FairMedium Plastic not found");
    geoBuild->createMedium(mCsI[i]);
    pMedCsI.push_back(gGeoMan->GetMedium(fCsIMedia[i]));
    if ( ! pMedCsI[i] ) Fatal("Main", "Medium Plastic not found");
  }
  // ------ Create vacuum media -----------------------------------------------
  FairGeoMedium* vacuum = geoMedia->getMedium("vacuum");
  if ( ! vacuum ) Fatal("Main", "FairMedium vacuum not found");
  geoBuild->createMedium(vacuum);
  TGeoMedium* pMed0 = gGeoMan->GetMedium("vacuum");
  if ( ! pMed0 ) Fatal("Main", "Medium vacuum not found");
  //------------------------- VOLUMES -----------------------------------------

  // --------------   Create geometry and top volume  -------------------------
  gGeoMan = (TGeoManager*)gROOT->FindObject("FAIRGeom");
  gGeoMan->SetName("BeamDetGeom");
  TGeoVolume* top   = new TGeoVolumeAssembly("TOP");
  //gGeoMan->SetTopVolume(top);
  TGeoVolume* qtelescope = new TGeoVolumeAssembly("QTelescope");
  // ---------------- SingleSi-------------------------------------------------
  vector<TGeoVolume*> singleSi;
  vector<TGeoVolume*> singleSiStrip;

  for (Int_t i = 0; i < fSingleSiCount; i++) {

    singleSi.push_back(gGeoManager->MakeBox("singleSi", pMedSingleSi[i], 
                                                        fSingleSiX[i] / 2., 
                                                        fSingleSiY[i] / 2., 
                                                        fSingleSiZ[i] / 2.));
    //------------------ Single Si strip --------------------------------------
    Double_t singleSiStripX = fSingleSiSensX[i] / fSingleSiStripCount[i]; 
    Double_t singleSiStripY = fSingleSiSensY[i];   
    Double_t singleSiStripZ = fSingleSiSensZ[i];   
    singleSiStrip.push_back(gGeoManager->MakeBox("strip", pMedSingleSi[i], 
                                                          singleSiStripX / 2., 
                                                          singleSiStripY / 2., 
                                                          singleSiStripZ / 2.));
  }
  // ----------------- DoubleSi -----------------------------------------------
  vector<TGeoVolume*> doubleSi;
  vector<TGeoVolume*> doubleSiStrip;
  vector<TGeoVolume*> doubleSiBox;
  for (Int_t i = 0; i < fDoubleSiCount; i++) {
    doubleSi.push_back( gGeoManager->MakeBox("doubleSi", pMedDoubleSi[i], 
                                                         fDoubleSiX[i] / 2,
                                                         fDoubleSiY[i] / 2, 
                                                         fDoubleSiZ[i] / 2));
    //------------------ Silicon strip   ---------------------------------------
    Double_t doubleSiStripX = fDoubleSiSensX[i] / fDoubleSiStripCountX[i];
    Double_t doubleSiStripY = fDoubleSiSensY[i];
    Double_t doubleSiStripZ = fDoubleSiSensZ[i];
    doubleSiStrip.push_back(gGeoManager->MakeBox("doubleSiStrip", pMedDoubleSi[i], 
                                                                  doubleSiStripX / 2, 
                                                                  doubleSiStripY / 2, 
                                                                  doubleSiStripZ / 2));
    //------------------ Silicon box   -----------------------------------------
    Double_t doubleSiBoxX = doubleSiStripX;  
    Double_t doubleSiBoxY = doubleSiStripY / fDoubleSiStripCountY[i]; 
    Double_t doubleSiBoxZ = doubleSiStripZ; 
    doubleSiBox.push_back(gGeoManager->MakeBox("doubleSiBox", pMedDoubleSi[i], 
                                                              doubleSiBoxX / 2, 
                                                              doubleSiBoxY / 2, 
                                                              doubleSiBoxZ / 2));
  }
  // ---------------- CsI -----------------------------------------------------
  vector<TGeoVolume*> stationCsI;
  vector<TGeoVolume*> boxCsI;
  for (Int_t i = 0; i < fCsICount; i++) {
    stationCsI.push_back(gGeoManager->MakeBox("stationCsI", pMedCsI[i], fCsIX[i] / 2, 
                                                                        fCsIY[i] / 2, 
                                                                        fCsIZ[i] / 2));
    Double_t doubleSiBoxX = fCsIX[i] / fCsICubesCountX[i];  
    Double_t doubleSiBoxY = fCsIY[i] / fCsICubesCountY[i]; 
    Double_t doubleSiBoxZ = fCsIZ[i]; 
    boxCsI.push_back(gGeoManager->MakeBox("boxCsI", pMedCsI[i], doubleSiBoxX / 2, 
                                                                doubleSiBoxY / 2, 
                                                                doubleSiBoxZ / 2));
  }
  //------------------ STRUCTURE  ---------------------------------------------
  //----------------------- Double Si structure -------------------------------
  for (Int_t i = 0; i < fDoubleSiCount; i++) {
    //------------------ Add fibers to station  along x -----------------------
    Double_t doubleSiBoxX = fCsIX[i] / fCsICubesCountX[i];  
    Double_t doubleSiStripX = fDoubleSiSensX[i] / fDoubleSiStripCountX[i];
    Double_t doubleSiStripY = fDoubleSiSensY[i];
    for (Int_t iDoubleSiBox = 0; iDoubleSiBox < fDoubleSiStripCountX[i] ; ++iDoubleSiBox ) {
      Double_t translateX = (doubleSiStripX / 2) 
                          - doubleSiBoxX - doubleSiBoxX * 2. * iDoubleSiBox ;
      doubleSiStrip[i]->AddNode(boxCsI[i], iDoubleSiBox, new TGeoCombiTrans(translateX,
                                                                            0,
                                                                            0,
                                                                            fZeroRotation));
    }

    for (Int_t iStripY = 0; iStripY < fDoubleSiStripCountY[i]; iStripY++) {
      Double_t translateY = fDoubleSiSensY[i] / 2 
                          - doubleSiStripY *(iStripY)-(doubleSiStripY / 2);
      doubleSi[i]->AddNode(doubleSiStrip[i], iStripY, new TGeoCombiTrans(0,
                                                                      translateY,
                                                                      0,
                                                                      fZeroRotation));
    }
    TGeoRotation *rotation = new TGeoRotation();
    if (fDoubleSiOrientAroundZ[i] == "Y") {
      rotation = f90ZRotation;
    }
    qtelescope->AddNode(doubleSi[i], i, new TGeoCombiTrans(0.,
                                                           0.,
                                                           fDoubleSiPosZ[i], 
                                                           rotation));
  }
  //----------------------- Single Si structure -------------------------------
  for (Int_t i = 0; i < fSingleSiCount; i++) {
    //------------------ Add fibers to station  along x -----------------------
    Double_t singleSiStripX = fSingleSiSensX[i] / fSingleSiStripCount[i]; 
    for (Int_t iStrip = 0; iStrip < fSingleSiStripCount[i]; iStrip++) {
      Double_t translateX = (-1)*singleSiStripX*(fSingleSiStripCount[i]/2) 
                          + singleSiStripX/2. + iStrip*singleSiStripX;
      singleSi[i]->AddNode(singleSiStrip[i], iStrip, new TGeoCombiTrans(translateX,
                                                                     0.,
                                                                     0., 
                                                                     fZeroRotation));
    }
    TGeoRotation *rotation = new TGeoRotation();
    if (fSingleSiOrientAroundZ[i] == "Y") {
      rotation = f90ZRotation;
    }
    qtelescope->AddNode(singleSi[i], i, new TGeoCombiTrans(0.,
                                                           0.,
                                                           fSingleSiPosZ[i], 
                                                           rotation));
  }
  //----------------------- CsI structure -------------------------------------
  for (Int_t i = 0; i < fCsICount; i++) {
    Int_t iBox = 0;
    Double_t doubleSiBoxX = fCsIX[i] / fCsICubesCountX[i];  
    Double_t doubleSiBoxY = fCsIY[i] / fCsICubesCountY[i]; 
    for (Int_t iCsIX = 0; iCsIX < fCsICubesCountX[i]; iCsIX) {
      for (Int_t iCsIY = 0; iCsIY < fCsICubesCountY[i]; iCsIY) {
        Double_t translateX = (-1)*doubleSiBoxX*(fCsICubesCountX[i]/2) 
                            + doubleSiBoxX/2. + iCsIX*doubleSiBoxX;
        Double_t translateY = fCsIY[i] / 2 
                            - doubleSiBoxY *(iCsIY)-(doubleSiBoxY / 2);
        stationCsI[i]->AddNode(boxCsI[i], iBox, new TGeoCombiTrans(translateX,
                                                                   translateY,
                                                                   0., 
                                                                   fZeroRotation));
        iBox++;
      }
    }
    qtelescope->AddNode(stationCsI[i], i, new TGeoCombiTrans(0.,
                                                             0.,
                                                             fCsIPosZ[i], 
                                                             fZeroRotation));
  }

  top->AddNode(qtelescope, 1, new TGeoCombiTrans(global_X ,global_Y, global_Z, fGlobalRotation));
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
ClassImp(ERQTelescopeSetup)