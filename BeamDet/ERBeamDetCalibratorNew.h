// -------------------------------------------------------------------------
// -----                  ERBeamDetCalibratorNew header file          -----
// -----                  Created 04/17  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERBeamDetCalibratorNew_H
#define ERBeamDetCalibratorNew_H

#include "TRandom.h"
#include "TClonesArray.h"

#include "FairTask.h"


#include "ERBeamDetEvent.h"
#include "ERBeamDetMWPCDigi.h"
#include "ERBeamDetTOFDigi.h"

class ERBeamDetCalibratorNew : public FairTask {

public:
  /** Default constructor **/
  ERBeamDetCalibratorNew();

  /** Constructor 
  ** verbose: 1 - only standard log print
  **/
  ERBeamDetCalibratorNew(Int_t verbose);

  /** Destructor **/
  ~ERBeamDetCalibratorNew();

  /** Virtual method Init **/
  virtual InitStatus Init();

  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

  /** Virtual method Finish **/
  virtual void Finish();

  /** Virtual method Reset **/
  virtual void Reset();

  void SetParametersFile(TString path){fPath = path;}
protected:
  //Input objects
  ERBeamDetEvent* fInEvent;
  //Output arrays
  TClonesArray *fBeamDetToFDigi;
  TClonesArray *fBeamDetMWPCDigi;

  Int_t fEvent;

  TRandom Ran;
  TString fPath;

  double CLB[8][10][32][5];

  ERBeamDetMWPCDigi* AddMWPCDigi(Float_t edep, Double_t time, Int_t mwpcNb, Int_t planeNb, Int_t wireNb);
  ERBeamDetTOFDigi*  AddToFDigi(Float_t edep, Double_t time, Int_t ToFNb);
private:

  virtual void SetParContainers();
  
  ClassDef(ERBeamDetCalibratorNew,1)
};

#endif
