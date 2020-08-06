/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERTelescopeSetup_H
#define ERTelescopeSetup_H

#include "ERSetup.h"

#include "TString.h"
#include <TXMLNode.h>
#include "Rtypes.h"
#include "TVector3.h"
#include "TGeoMatrix.h"

struct ERTelescopeStrip{
  Double_t fGlobalX = -1.;
  Double_t fGlobalY = -1.;
  Double_t fGlobalZ = -1.;
  Double_t fLocalX = -1.;
  Double_t fLocalY = -1.;
  Double_t fLocalZ = -1.;
  ERTelescopeStrip() = default;
  ERTelescopeStrip(Double_t globalX, Double_t globalY, Double_t globalZ,
                    Double_t  localX, Double_t  localY, Double_t localZ);
  ERTelescopeStrip(Double_t* globTrans, Double_t* localTrans);
};

struct ERRTelescopeStrip {
  Double_t fPhi = -1.;
  Double_t fR = -1.;
  ERRTelescopeStrip() = default;
  ERRTelescopeStrip(const Double_t phi, const Double_t r) : fPhi(phi), fR(r) {}
};

class ERTelescopeSetup : public ERSetup {
public:
  enum StationType {QStation, RStation};
  ~ERTelescopeSetup();

  static ERTelescopeSetup* Instance();

  /* Modifiers */
  /* Accessors */
  Double_t GetStripGlobalX(const TString& componentBranchName, Int_t stripNb) const;
  Double_t GetStripGlobalY(const TString& componentBranchName, Int_t stripNb) const;
  Double_t GetStripGlobalZ(const TString& componentBranchName, Int_t stripNb) const;
  Double_t GetStripLocalX(const TString& componentBranchName, Int_t stripNb) const;
  Double_t GetStripLocalY(const TString& componentBranchName, Int_t stripNb) const;
  Double_t GetStripLocalZ(const TString& componentBranchName, Int_t stripNb) const;
  Double_t GetStripPhi(const TString& componentBranchName, Int_t stripNb) const;
  Double_t GetStripR(const TString& componentBranchName, Int_t stripNb) const;
  TVector3 GetStationTranslation(const TString& componentBranchName) const;
  TVector3 ToStationCoordinateSystem (const TString& componentBranchName, 
                                      const TVector3& vectorInGlobalCS) const;
  StationType GetStationType(const TString& componentBranchName) const; 

public:
  virtual void ReadGeoParamsFromParContainer();
  // static void PrintDetectorParameters(void);
  // static void PrintDetectorParametersToFile(TString fileName);

private:
  ERTelescopeSetup();
  void GetTransInMotherNode (TGeoNode const* node, Double_t b[3]);
  void FillRStrips(TGeoNode* r_station, const TString& branch_name);
  std::map<TString, std::vector<ERTelescopeStrip>> fStrips;
  std::map<TString, std::vector<ERRTelescopeStrip>> fRStrips;
  std::map<TString, TGeoHMatrix> fStationGlobalToLocalMatrixies;
  std::map<TString, StationType> fStationTypes;
  static ERTelescopeSetup* fInstance;
  bool fGeometryInited = false;

  ClassDef(ERTelescopeSetup,1)
};
#endif
