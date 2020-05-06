/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERQTelescopeDigitizer_H
#define ERQTelescopeDigitizer_H

#include <limits>

#include "TClonesArray.h"

#include "ERDigitizer.h"
#include "ERQTelescopeSiPoint.h"
#include "ERQTelescopeCsIPoint.h"
#include "ERQTelescopeSiDigi.h"
#include "ERQTelescopeCsIDigi.h"
#include "ERQTelescopeSetup.h"

class ERQTelescopeSetup;

class ERQTelescopeDigitizer : public ERDigitizer {
 public:
  ERQTelescopeDigitizer();
  /** Constructor 
  ** verbose: 1 - only standard log print, 2 - print digi information 
  **/
  ERQTelescopeDigitizer(Int_t verbose);
  virtual ~ERQTelescopeDigitizer() = default;
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
  ERQTelescopeSiDigi* AddSiDigi(Float_t edep, Double_t time, Int_t stripNb, TString digiBranchName);
  ERQTelescopeCsIDigi* AddCsIDigi(Float_t edep, Double_t time, Int_t blockNb, TString digiBranchName);
private:
  ClassDef(ERQTelescopeDigitizer,1)
};

#endif
