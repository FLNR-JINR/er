// -------------------------------------------------------------------------
// -----                        ERBeamDetDigitizer header file          -----
// -----                        Created   by                 -----
// -------------------------------------------------------------------------

#ifndef ERBeamDetDigitizer_H
#define ERBeamDetDigitizer_H

#include "TClonesArray.h"

#include "FairTask.h"

#include "ERBeamDetMWPCPoint.h"
#include "ERBeamDetTOFPoint.h"

class ERBeamDetDigitizer : public FairTask {

public:
  /** Default constructor **/
  ERBeamDetDigitizer();

  /** Constructor 
  ** verbose: 1 - only standard log print, 2 - print digi information 
  **/
  ERBeamDetDigitizer(Int_t verbose);

  /** Destructor **/
  ~ERBeamDetDigitizer();

  /** Virtual method Init **/
  virtual InitStatus Init();

  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

  /** Virtual method Finish **/
  virtual void Finish();

  /** Virtual method Reset **/
  virtual void Reset();
  
  /** Modifiers **/
  void SetElossThreshold(Float_t th){fElossThreshold = th;}
  void SetTofElossSigma(Float_t sigma) {fElossDispersionTof = sigma;}
  void SetTofTimeSigma(Float_t sigma) {fTimeDispersionTof = sigma;}
  /** Accessors **/ 
  Float_t ElossDispersion() const {return fElossDispersionTof;}
  Float_t TimeDispersionPar() const {return fTimeDispersionTof;}
protected:
  //Input arrays
  TClonesArray *fBeamDetTOFPoints;
  TClonesArray *fBeamDetMWPCPoints;
  //Output arrays
  TClonesArray *fBeamDetTOFDigi;
  TClonesArray *fBeamDetMWPCDigi;

  static Int_t fEvent;
  Float_t fElossDispersionTof;
  Float_t fTimeDispersionTof;
  Float_t fElossThreshold;

  //ERBeamDetSetup* fBeamDetSetup;
protected:
  ERBeamDetMWPCDigi* AddMWPCDigi();
  ERBeamDetTOFDigi*  AddTOFDigi();
private:
  virtual void SetParContainers();
  
  ClassDef(ERBeamDetDigitizer,1)
};

#endif
