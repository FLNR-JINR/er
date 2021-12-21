/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERTelescopeDigitizer_H
#define ERTelescopeDigitizer_H

#include <limits>

#include "TClonesArray.h"

#include "ERDigitizer.h"
#include "ERDigi.h"
#include "ERTelescopeSetup.h"

class ERTelescopeSetup;

class ERTelescopeDigitizer : public ERDigitizer {
 public:
  ERTelescopeDigitizer();
  /** Constructor 
  ** verbose: 1 - only standard log print, 2 - print digi information 
  **/
  ERTelescopeDigitizer(Int_t verbose);
  virtual ~ERTelescopeDigitizer() = default;
  /** Virtual method Init **/
  virtual InitStatus Init();
  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);
  /** Virtual method Finish **/
  virtual void Finish();
  /** Virtual method Reset **/
  virtual void Reset();
  /** Modifiers **/
  void SetSiElossThreshold(float th){fSiElossThreshold = th;}
  void SetSiElossSigma(float sigma);
  void SetSiTimeSigma(float sigma);
  void SetCsIElossThreshold(float th){fCsIElossThreshold = th;}
  void SetCsIElossSigma(float sigma);
  void SetCsITimeSigma(float sigma);
  void SetSiElossModel(const std::function<float(float)>& model) {fSiElossModel = model;}
  void SetCsIElossModel(const std::function<float(float)>& model) {fCsIElossModel = model;}
  void SetSiTimeModel(const std::function<float(float)>& model) {fSiTimeModel = model;}
  void SetCsITimeModel(const std::function<float(float)>& model) {fCsITimeModel = model;}
protected:
  //Input arrays
  std::map<TString, TClonesArray*> fQTelescopePoints;
  //Output arrays
  std::map<TString, TClonesArray*> fQTelescopeDigi;
  Float_t       fSiElossThreshold = 0.;
  Float_t       fCsIElossThreshold = 0.;
  std::function<float(float)> fSiElossModel = [](float eloss) { return eloss; };
  std::function<float(float)> fCsIElossModel = [](float eloss) { return eloss; };
  std::function<float(float)> fSiTimeModel = [](float time) { return time; };
  std::function<float(float)> fCsITimeModel = [](float time) { return time; };
protected:
  ERDigi* AddDigi(Float_t edep, Double_t time, Int_t stripNb, TString digiBranchName);
private:
  ClassDef(ERTelescopeDigitizer,1)
};

#endif
