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
  void SetSiElossThreshold(Float_t th){fSiElossThreshold = th;}
  void SetSiElossSigma(Float_t sigma) {fSiElossSigma = sigma;}
  void SetSiTimeSigma(Float_t sigma)  {fSiTimeSigma = sigma;}
  void SetCsIElossThreshold(Float_t th){fCsIElossThreshold = th;}
  void SetCsIElossSigma(Float_t sigma) {fCsIElossSigma = sigma;}
  void SetCsITimeSigma(Float_t sigma)  {fCsITimeSigma = sigma;}
protected:
  //Input arrays
  std::map<TString, TClonesArray*> fQTelescopePoints;
  //Output arrays
  std::map<TString, TClonesArray*> fQTelescopeDigi;
  Float_t       fSiElossThreshold = 0.;
  Float_t       fSiElossSigma = 0.;
  Float_t       fSiTimeSigma = 0.;
  Float_t       fCsIElossThreshold = 0.;
  Float_t       fCsIElossSigma = 0.;
  Float_t       fCsITimeSigma = 0.;
protected:
  ERDigi* AddDigi(Float_t edep, Double_t time, Int_t stripNb, TString digiBranchName);
private:
  ClassDef(ERTelescopeDigitizer,1)
};

#endif
