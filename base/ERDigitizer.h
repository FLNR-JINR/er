/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/


#ifndef ERDigitizer_H
#define ERDigitizer_H

#include <vector>

#include "TClonesArray.h"
#include "TString.h"

#include "ERTask.h"
#include "ERDigi.h"

struct ERDigitizerError
{
  ERDigitizerError(){}
  ERDigitizerError(Float_t ea, Float_t eb, Float_t ec){
    a = ea;
    b = eb;
    c = ec;
  }
  Float_t a;
  Float_t b;
  Float_t c;
};

struct ERTrigger
{
  Int_t   fValue;
  Int_t   fPriority;
  ERTrigger(){}
  ERTrigger(Int_t value, Int_t priority){
    fValue = value;
    fPriority = priority;
  }
};

class ERDigitizer : public ERTask {

public:
  /** Default constructor **/
  ERDigitizer();

  /** Constructor **/
  ERDigitizer(TString name);

  /** Constructor 
  ** verbose: 1 - only standard log print, 2 - print digi information 
  **/
  ERDigitizer(TString name, Int_t verbose);

  /** Destructor **/
  ~ERDigitizer();

  /** Virtual method Init **/
  virtual InitStatus Init();

  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

  /** Virtual method Finish **/
  virtual void Finish();

  /** Virtual method Reset **/
  virtual void Reset();

  void AddError(TString volName,Float_t a, Float_t b, Float_t c);

  void AddTrigger(TString stationSID, Int_t value, Int_t priority);
  void ApplyTrigger(TString stationSID, TClonesArray* digiCollection);
  
protected:
  std::map<TString, ERTrigger> fTriggers;
  
protected:
  ERDigi* AddDigi(TClonesArray* digi);

private:
  std::map<TString,TClonesArray*> fSenVolDigis;
  std::map<TString,TClonesArray*> fSenVolPoints;
  std::map<TString,ERDigitizerError> fSenVolErrors; 
  Float_t fEdep;
  Float_t fTime;
  Int_t fVolNb;
  
  ClassDef(ERDigitizer,1)
};

#endif