/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERQTelescopeDigitizer_H
#define ERQTelescopeDigitizer_H

#include "TClonesArray.h"

#include "FairTask.h"

#include "ERQTelescopeSiPoint.h"
#include "ERQTelescopeCsIPoint.h"
#include "ERQTelescopeSiDigi.h"
#include "ERQTelescopeCsIDigi.h"
#include "ERQTelescopeSetup.h"

class ERQTelescopeSetup;

class ERQTelescopeDigitizer : public FairTask {

public:
  /** Default constructor **/
  ERQTelescopeDigitizer();

  /** Constructor 
  ** verbose: 1 - only standard log print, 2 - print digi information 
  **/
  ERQTelescopeDigitizer(Int_t verbose);

  /** Destructor **/
  ~ERQTelescopeDigitizer();

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
  void SetCsIElossError(Float_t a, Float_t b, Float_t c) {fCsIElossErrorA = a;fCsIElossErrorB = b;fCsIElossErrorC = c;}
  void SetCsITimeSigma(Float_t sigma)  {fCsITimeSigma = sigma;}
protected:
  //Input arrays
  std::map<TString, TClonesArray*> fQTelescopePoints;
  //Output arrays
  std::map<TString, TClonesArray*> fQTelescopeDigi;
  
  Float_t       fSiElossThreshold;
  Float_t       fSiElossSigma;
  Float_t       fSiTimeSigma;
  Float_t       fCsIElossThreshold;
  Float_t       fCsITimeSigma;
  Float_t       fCsIElossErrorA,fCsIElossErrorB,fCsIElossErrorC;
protected:
  ERQTelescopeSiDigi* AddSiDigi(Float_t edep, Double_t time, Int_t stationNb, 
                                                             Int_t stripNb, 
                                                             TString digiBranchName);
  ERQTelescopeCsIDigi* AddCsIDigi(Float_t edep, Double_t time, Int_t wallNb,
                                                               Int_t blockNb,
                                                               TString digiBranchName);
private:
  virtual void SetParContainers();
  
  ClassDef(ERQTelescopeDigitizer,1)
};

#endif
