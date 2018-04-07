/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERQTelescopeSetup_H
#define ERQTelescopeSetup_H

#include "ERSetup.h"

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

class ERQTelescopeSetup : public ERSetup {
public:
  ~ERQTelescopeSetup();

  static ERQTelescopeSetup* Instance();

  /* Modifiers */
  /* Accessors */
  Double_t GetStripX(TString componentBranchName, Int_t stripNb);
  Double_t GetStripY(TString componentBranchName, Int_t stripNb);
  Double_t GetStripZ(TString componentBranchName, Int_t stripNb);

public:
  virtual void ReadGeoParamsFromParContainer();
  // static void PrintDetectorParameters(void);
  // static void PrintDetectorParametersToFile(TString fileName);

private:
  ERQTelescopeSetup();

  static map<TString, vector<ERQTelescopeStrip*>> fStrips; 
  static ERQTelescopeSetup* fInstance;

  ClassDef(ERQTelescopeSetup,1)
};
#endif

