// -------------------------------------------------------------------------
// -----                        ERQTelescopeDigitizer header file          -----
// -----                        Created   by                 -----
// -------------------------------------------------------------------------

#ifndef ERQTelescopeDigitizer_H
#define ERQTelescopeDigitizer_H

#include "TClonesArray.h"

#include "FairTask.h"

#include "ERQTelescopeSiPoint.h"
#include "ERQTelescopeSiDigi.h"


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
  void SetSiElossSigma(Float_t sigma) {fSiElossDispersion = sigma;}
  void SetSiTimeSigma(Float_t sigma) {fSiTimeDispersion = sigma;}
protected:
  //Input arrays
  TClonesArray *fQTelescopeSiPoints;
  //Output arrays
  TClonesArray *fQTelescopeSiDigi;
  
  Float_t fSiElossThreshold;
  Float_t fSiElossDispersion;
  Float_t fSiTimeDispersion;

  Float_t fDigiEloss;

  //ERBeamDetSetup* fBeamDetSetup;
protected:
  ERQTelescopeSiDigi* AddSiDigi(Float_t edep, Double_t time, Int_t stationNb, Int_t stripNb);
private:
  virtual void SetParContainers();
  
  ClassDef(ERQTelescopeDigitizer,1)
};

#endif
