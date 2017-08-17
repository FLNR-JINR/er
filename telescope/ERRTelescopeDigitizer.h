// -------------------------------------------------------------------------
// -----                        ERRTelescopeDigitizer header file          -----
// -----                        Created   by                 -----
// -------------------------------------------------------------------------

#ifndef ERRTelescopeDigitizer_H
#define ERRTelescopeDigitizer_H

#include "TClonesArray.h"

#include "FairTask.h"

#include "ERRTelescopeSiPoint.h"
#include "ERRTelescopeSiDigi.h"


class ERRTelescopeDigitizer : public FairTask {

public:
  /** Default constructor **/
  ERRTelescopeDigitizer();

  /** Constructor
  ** verbose: 1 - only standard log print, 2 - print digi information
  **/
  ERRTelescopeDigitizer(Int_t verbose);

  /** Destructor **/
  ~ERRTelescopeDigitizer();

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
  void SetElossSigma(Float_t sigma) {fElossSigma = sigma;}
  void SetTimeSigma(Float_t sigma) {fTimeSigma = sigma;}
  /** Accessors **/
  Float_t ElossDispersion() const {return fElossSigma;}
  Float_t TimeDispersionPar() const {return fTimeSigma;}
protected:
  //Input arrays
  TClonesArray *fSiPoints;
  //Output arrays
  TClonesArray *fRTelescopeDigi;

  Float_t fElossSigma;
  Float_t fTimeSigma;

  Float_t fElossThreshold;

  Float_t fDigiEloss;

  //ERBeamDetSetup* fBeamDetSetup;
protected:
  ERRTelescopeSiDigi* AddDigi(Float_t edep, Double_t time, Int_t sectorNb, Int_t sensorNb);

private:
  virtual void SetParContainers();

  ClassDef(ERRTelescopeDigitizer,1)
};

#endif
