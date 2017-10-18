// -------------------------------------------------------------------------
// -----                        ERBeamDetDigitizer header file          -----
// -----                        Created   by                 -----
// -------------------------------------------------------------------------

#ifndef ERBeamDetDigitizer_H
#define ERBeamDetDigitizer_H

#include "TClonesArray.h"

#include "FairTask.h"

#include "ERBeamDetMWPCPoint.h"
#include "ERBeamDetToFPoint.h"
#include "ERBeamDetMWPCDigi.h"
#include "ERBeamDetToFDigi.h"


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
  void SetToFElossThreshold(Float_t th){fToFElossThreshold = th;}
  void SetMWPCElossThreshold(Float_t th){fMWPCElossThreshold = th;}
  void SetToFElossSigma(Float_t sigma) {fElossSigmaToF = sigma;}
  void SetMWPCElossSigma(Float_t sigma) {fElossSigmaMWPC = sigma;}
  void SetToFTimeSigma(Float_t sigma) {fTimeSigmaToF = sigma;}
  void SetMWPCTimeSigma(Float_t sigma) {fTimeSigmaMWPC = sigma;}


  void SetToFElossSigmaOverEloss(Float_t sigma)  // set parameter to formula: fElossSigmaToF = edep * fElossSigmaOverElossToF / TMath::Sqrt(edep);
  {
    fElossSigmaOverElossToF = sigma; 
    fSigmaEOverEToFIsSet = kTRUE;
  }

  /** Accessors **/ 
  Float_t TimeDispersionPar() const {return fTimeSigmaToF;}
protected:
  //Input arrays
  TClonesArray *fBeamDetToFPoints;
  TClonesArray *fBeamDetMWPCPoints;
  //Output arrays
  TClonesArray *fBeamDetToFDigi1;
  TClonesArray *fBeamDetToFDigi2;

  TClonesArray *fBeamDetMWPCDigiX1;
  TClonesArray *fBeamDetMWPCDigiX2;
  TClonesArray *fBeamDetMWPCDigiY1;
  TClonesArray *fBeamDetMWPCDigiY2;
  
  Float_t fElossSigmaOverElossToF;
  Float_t fElossSigmaToF;
  Float_t fTimeSigmaToF;
  Float_t fElossSigmaMWPC;
  Float_t fTimeSigmaMWPC;

  Float_t fToFElossThreshold;
  Float_t fMWPCElossThreshold;

  Float_t fDigiEloss;

  Bool_t  fSigmaEOverEToFIsSet;

protected:
  ERBeamDetMWPCDigi* AddMWPCDigi(Float_t edep, Double_t time, Int_t mwpcNb, Int_t planeNb, Int_t wireNb);
  ERBeamDetToFDigi*  AddToFDigi(Float_t edep, Double_t time, Int_t tofNb);
private:
  virtual void SetParContainers();
  
  ClassDef(ERBeamDetDigitizer,1)
};

#endif
