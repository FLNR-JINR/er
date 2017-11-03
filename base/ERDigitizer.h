// -------------------------------------------------------------------------
// -----                        ERDigitizer header file          -----
// -----                        Created   by                 -----
// -------------------------------------------------------------------------

#ifndef ERDigitizer_H
#define ERDigitizer_H

#include "TClonesArray.h"
#include "TString.h"

#include "FairTask.h"

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

class ERDigitizer : public FairTask {

public:
  /** Default constructor **/
  ERDigitizer();

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
  
protected:
  
protected:
  ERDigi* AddDigi(TClonesArray* digi);

private:
  virtual void SetParContainers();

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