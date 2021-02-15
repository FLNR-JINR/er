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
#include "TVector3.h"
#include "TGeoMatrix.h"

struct ERQTelescopeStrip{
  Double_t fGlobalX;
  Double_t fGlobalY;
  Double_t fGlobalZ;
  Double_t fLocalX;
  Double_t fLocalY;
  Double_t fLocalZ;
  Double_t fWidth;
  ERQTelescopeStrip(Double_t globalX, Double_t globalY, Double_t globalZ,
                    Double_t  localX, Double_t  localY, Double_t localZ, Double_t width);
  ERQTelescopeStrip(Double_t* globTrans, Double_t* localTrans, Double_t width);
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
  Double_t GetStripWidth(TString componentBranchName, Int_t stripNb) const;
  TVector3 GetStripLocalPosition(const TString& componentBranchName,
                                 unsigned int stripNb) const;
  TVector3 ToStationCoordinateSystem(const TString& componentBranchName, 
                                     const TVector3& vectorInGlobalCS) const;
  TVector3 ToGlobalCoordinateSystem(const TString& componentBranchName,
                                    const TVector3& vectorInStationCS) const;
public:
  virtual void ReadGeoParamsFromParContainer();
  // static void PrintDetectorParameters(void);
  // static void PrintDetectorParametersToFile(TString fileName);

private:
  ERQTelescopeSetup();
  void GetTransInMotherNode (TGeoNode const* node, Double_t b[3]);

  static std::map<TString, std::vector<const ERQTelescopeStrip*>> fStrips;
  static std::map<TString, TGeoHMatrix> fStationGlobalToLocalMatrixies; 
  static ERQTelescopeSetup* fInstance;

  ClassDef(ERQTelescopeSetup,1)
};
#endif
