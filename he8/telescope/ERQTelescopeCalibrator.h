// -------------------------------------------------------------------------
// -----                  ERQTelescopeCalibrator header file          -----
// -----                  Created 04/17  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERQTelescopeCalibrator_H
#define ERQTelescopeCalibrator_H

#include "TRandom.h"

#include "FairTask.h"


#include "ERQTelescopeEvent.h"
#include "ERQTelescopeCalEvent.h"

class ERQTelescopeCalibrator : public FairTask {

public:
  /** Default constructor **/
  ERQTelescopeCalibrator();

  /** Constructor 
  ** verbose: 1 - only standard log print
  **/
  ERQTelescopeCalibrator(Int_t verbose);

  /** Destructor **/
  ~ERQTelescopeCalibrator();

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
  ERQTelescopeEvent* fInEvent;

  ERQTelescopeCalEvent* fOutEvent;

  Int_t fEvent;

  TRandom Ran;
  TString fPath;

  double CLB[8][10][32][5];
private:

  virtual void SetParContainers();
  
  ClassDef(ERQTelescopeCalibrator,1)
};

#endif
