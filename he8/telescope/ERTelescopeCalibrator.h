// -------------------------------------------------------------------------
// -----                  ERTelescopeCalibrator header file          -----
// -----                  Created 04/17  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERTelescopeCalibrator_H
#define ERTelescopeCalibrator_H

#include "TRandom.h"

#include "FairTask.h"


#include "ERTelescopeEvent.h"
#include "ERTelescopeCalEvent.h"

class ERTelescopeCalibrator : public FairTask {

public:
  /** Default constructor **/
  ERTelescopeCalibrator();

  /** Constructor 
  ** verbose: 1 - only standard log print
  **/
  ERTelescopeCalibrator(Int_t verbose);

  /** Destructor **/
  ~ERTelescopeCalibrator();

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
  ERTelescopeEvent* fInEvent;

  ERTelescopeCalEvent* fOutEvent;

  Int_t fEvent;

  TRandom Ran;
  TString fPath;

  double CLB[8][10][32][5];
private:

  virtual void SetParContainers();
  
  ClassDef(ERTelescopeCalibrator,1)
};

#endif
