/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERQTelescopeSetup_H
#define ERQTelescopeSetup_H

#include <map>

#include "TString.h"
#include <TXMLNode.h>
#include "Rtypes.h"
#include <TVector3.h>

using namespace std;

struct ERQTelescopeStrip{
  Double_t fX;
  Double_t fY;
  Double_t fZ;
  ERQTelescopeStrip(Double_t x, Double_t y, Double_t z) {fX = x; fY = y; fZ = z;}
  ERQTelescopeStrip(Double_t* trans) {fX = trans[0]; fY = trans[1]; fZ = trans[2];}
};

class ERQTelescopeSetup {
public:
  ERQTelescopeSetup();
  ~ERQTelescopeSetup();

  static ERQTelescopeSetup* Instance();

  /* Modifiers */
  static void SetXmlParametersFile(TString xmlFileName) {fParamsXmlFileName = xmlFileName;}
  static void AddSi(TString type, TVector3 position, TString orientAroundZ); 
  static void AddSi(TString type, TVector3 position, TString orientAroundZ, 
                                                     Double_t deadLayerFront, 
                                                     Double_t deadLayerBack); 
  static void AddCsI(TString type, Double_t position);

  /* Accessors */
  static Double_t GetStripX(TString stationId, Int_t stripNb);
  static Double_t GetStripY(TString stationId, Int_t stripNb);
  static Double_t GetStripZ(TString stationId, Int_t stripNb);
  static vector<TString>* GetDetectorStations();

public:
  static Int_t SetParContainers();
  static void ReadGeoParamsFromParContainer();
  static void ConstructGeometry();
  static void PrintDetectorParameters(void);
  static void PrintDetectorParametersToFile(TString fileName);

private:
  static void ParseXmlParameters();
  static void GetCsIParameters(TXMLNode *node);
  static void GetSingleSiParameters(TXMLNode *node);
  static void GetDoubleSiParameters(TXMLNode *node);

  static ERQTelescopeSetup*                                 fInstance;
  static TString                                            fParamsXmlFileName;
  static vector<TString>                                    fDetectorStations;
  static map<TString, vector<ERQTelescopeStrip*>> fStrips;            

  // ----- SingleSi parameters --------------------------------------------------
  static Int_t            fDoubleSiCount;
  static vector<TString>  fDoubleSiType;
  static vector<TVector3> fDoubleSiPos;
  static vector<TString>  fDoubleSiOrientAroundZ;
  static vector<Double_t> fDoubleSiX;
  static vector<Double_t> fDoubleSiY;
  static vector<Double_t> fDoubleSiZ;
  static vector<Double_t> fDoubleSiSensX;
  static vector<Double_t> fDoubleSiSensY;
  static vector<Double_t> fDoubleSiSensZ;
  static vector<Double_t> fDoubleSiDeadLayerThicknessFrontSide;
  static vector<Double_t> fDoubleSiDeadLayerThicknessBackSide;
  static vector<Int_t>    fDoubleSiStripCountX;
  static vector<Int_t>    fDoubleSiStripCountY;
  static vector<TString>  fDoubleSiMedia;
  static vector<Bool_t>   fDoubleSiIsDeadLayerSet;
  // ----- DoubleSi parameters --------------------------------------------------
  static Int_t            fSingleSiCount;
  static vector<TString>  fSingleSiType;  
  static vector<TVector3> fSingleSiPos;
  static vector<TString>  fSingleSiOrientAroundZ;
  static vector<Double_t> fSingleSiX;
  static vector<Double_t> fSingleSiY;
  static vector<Double_t> fSingleSiZ;
  static vector<Double_t> fSingleSiSensX;
  static vector<Double_t> fSingleSiSensY;
  static vector<Double_t> fSingleSiSensZ;
  static vector<Double_t> fSingleSiDeadLayerThicknessFrontSide;
  static vector<Double_t> fSingleSiDeadLayerThicknessBackSide;
  static vector<Int_t>    fSingleSiStripCount;
  static vector<TString>  fSingleSiMedia;
  static vector<Bool_t>   fSingleSiIsDeadLayerSet;
  // ----- CsI parameters -------------------------------------------------------
  static Int_t            fCsICount;
  static vector<TString>  fCsIType;
  static vector<Double_t> fCsIPosZ;  
  static vector<TString>  fCsIOrientAroundZ;
  static vector<Double_t> fCsIX;
  static vector<Double_t> fCsIY;
  static vector<Double_t> fCsIZ;
  static vector<Int_t>    fCsICubesCountX;
  static vector<Int_t>    fCsICubesCountY;
  static vector<TString>  fCsIMedia;

  ClassDef(ERQTelescopeSetup,1)
};
#endif

