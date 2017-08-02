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
#include "ERBeamDetMWPCDigi.h"
#include "ERBeamDetTOFDigi.h"


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
  void SetTofElossThreshold(Float_t th){fTOFElossThreshold = th;}
  void SetMWPCElossThreshold(Float_t th){fMWPCElossThreshold = th;}
  void SetTofElossSigma(Float_t sigma) {fElossSigmaTOF = sigma;}
  void SetMWPCElossSigma(Float_t sigma) {fElossSigmaMWPC = sigma;}
  void SetTofTimeSigma(Float_t sigma) {fTimeSigmaTOF = sigma;}
  void SetMWPCTimeSigma(Float_t sigma) {fTimeSigmaMWPC = sigma;}


  void SetTofElossSigmaOverEloss(Float_t sigma)  // set parameter to formula: fElossSigmaTOF = edep * fElossSigmaOverElossTOF / TMath::Sqrt(edep);
  {
    fElossSigmaOverElossTOF = sigma; 
    fSigmaEOverETOFIsSet = kTRUE;
  }

  /** Accessors **/ 
  Float_t TimeDispersionPar() const {return fTimeSigmaTOF;}
protected:
  //Input arrays
  TClonesArray *fBeamDetTOFPoints;
  TClonesArray *fBeamDetMWPCPoints;
  //Output arrays
  TClonesArray *fBeamDetTOFDigi1;
  TClonesArray *fBeamDetTOFDigi2;

  TClonesArray *fBeamDetMWPCDigiX1;
  TClonesArray *fBeamDetMWPCDigiX2;
  TClonesArray *fBeamDetMWPCDigiY1;
  TClonesArray *fBeamDetMWPCDigiY2;
  
  Float_t fElossSigmaOverElossTOF;
  Float_t fElossSigmaTOF;
  Float_t fTimeSigmaTOF;
  Float_t fElossSigmaMWPC;
  Float_t fTimeSigmaMWPC;

  Float_t fTOFElossThreshold;
  Float_t fMWPCElossThreshold;

  Float_t fDigiEloss;

  Bool_t  fSigmaEOverETOFIsSet;

protected:
  ERBeamDetMWPCDigi* AddMWPCDigi(Float_t edep, Double_t time, Int_t mwpcNb, Int_t planeNb, Int_t wireNb);
  ERBeamDetTOFDigi*  AddTOFDigi(Float_t edep, Double_t time, Int_t tofNb);
private:
  virtual void SetParContainers();
  
  ClassDef(ERBeamDetDigitizer,1)
};

#endif
