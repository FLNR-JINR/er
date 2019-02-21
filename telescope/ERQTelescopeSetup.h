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
  Double_t fGlobalX;
  Double_t fGlobalY;
  Double_t fGlobalZ;
  Double_t fLocalX;
  Double_t fLocalY;
  Double_t fLocalZ;
  ERQTelescopeStrip(Double_t globalX, Double_t globalY, Double_t globalZ,
                    Double_t  localX, Double_t  localY, Double_t localZ);
  ERQTelescopeStrip(Double_t* globTrans, Double_t* localTrans);
};

class ERQTelescopeSetup : public ERSetup {
public:
  ~ERQTelescopeSetup();

  static ERQTelescopeSetup* Instance();

  /* Modifiers */
  /* Accessors */
  Double_t GetStripGlobalX(TString componentBranchName, Int_t stripNb);
  Double_t GetStripGlobalY(TString componentBranchName, Int_t stripNb);
  Double_t GetStripGlobalZ(TString componentBranchName, Int_t stripNb);
  Double_t GetStripLocalX (TString componentBranchName, Int_t stripNb);
  Double_t GetStripLocalY (TString componentBranchName, Int_t stripNb);
  Double_t GetStripLocalZ (TString componentBranchName, Int_t stripNb);
  void GetTransInMotherNode (TGeoNode const* node, Double_t b[3]);

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

