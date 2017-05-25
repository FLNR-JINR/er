// -------------------------------------------------------------------------
// -----                  ERBeamDetCalibrator header file          -----
// -----                  Created 04/17  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERBeamDetCalibrator_H
#define ERBeamDetCalibrator_H

#include "TRandom.h"

#include "FairTask.h"


#include "ERBeamDetEvent.h"
#include "ERBeamDetCalEvent.h"

class ERBeamDetCalibrator : public FairTask {

public:
  /** Default constructor **/
  ERBeamDetCalibrator();

  /** Constructor 
  ** verbose: 1 - only standard log print
  **/
  ERBeamDetCalibrator(Int_t verbose);

  /** Destructor **/
  ~ERBeamDetCalibrator();

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

  ERBeamDetCalEvent* fOutEvent;

  Int_t fEvent;

  TRandom Ran;
  TString fPath;

  double CLB[8][10][32][5];
private:

  virtual void SetParContainers();
  
  ClassDef(ERBeamDetCalibrator,1)
};

#endif
