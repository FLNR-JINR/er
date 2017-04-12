// -------------------------------------------------------------------------
// -----                  ERDsrdCalibrator header file          -----
// -----                  Created 04/17  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERDsrdCalibrator_H
#define ERDsrdCalibrator_H

#include "TRandom.h"

#include "FairTask.h"


#include "ERDsrdEvent.h"
#include "ERDsrdCalEvent.h"

class ERDsrdCalibrator : public FairTask {

public:
  /** Default constructor **/
  ERDsrdCalibrator();

  /** Constructor 
  ** verbose: 1 - only standard log print
  **/
  ERDsrdCalibrator(Int_t verbose);

  /** Destructor **/
  ~ERDsrdCalibrator();

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
  ERDsrdEvent* fInEvent;

  ERDsrdCalEvent* fOutEvent;

  Int_t fEvent;

  TRandom Ran;
  TString fPath;

  double CLB[8][10][32][5];
private:

  virtual void SetParContainers();
  
  ClassDef(ERDsrdCalibrator,1)
};

#endif
