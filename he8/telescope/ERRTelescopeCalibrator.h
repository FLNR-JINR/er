// -------------------------------------------------------------------------
// -----                  ERRTelescopeCalibrator header file          -----
// -----                  Created 04/17  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERRTelescopeCalibrator_H
#define ERRTelescopeCalibrator_H

#include "TRandom.h"

#include "FairTask.h"


#include "ERRTelescopeEvent.h"
#include "ERRTelescopeCalEvent.h"

class ERRTelescopeCalibrator : public FairTask {

public:
  /** Default constructor **/
  ERRTelescopeCalibrator();

  /** Constructor 
  ** verbose: 1 - only standard log print
  **/
  ERRTelescopeCalibrator(Int_t verbose);

  /** Destructor **/
  ~ERRTelescopeCalibrator();

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
  ERRTelescopeEvent* fInEvent;

  ERRTelescopeCalEvent* fOutEvent;

  Int_t fEvent;

  TRandom Ran;
  TString fPath;

  double CLB[8][10][32][5];
private:

  virtual void SetParContainers();
  
  ClassDef(ERRTelescopeCalibrator,1)
};

#endif
