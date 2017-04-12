// -------------------------------------------------------------------------
// -----                  ERTofCalibrator header file          -----
// -----                  Created 04/17  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERTofCalibrator_H
#define ERTofCalibrator_H

#include "TRandom.h"

#include "FairTask.h"


#include "ERTofEvent.h"
#include "ERTofCalEvent.h"

class ERTofCalibrator : public FairTask {

public:
  /** Default constructor **/
  ERTofCalibrator();

  /** Constructor 
  ** verbose: 1 - only standard log print
  **/
  ERTofCalibrator(Int_t verbose);

  /** Destructor **/
  ~ERTofCalibrator();

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
  ERTofEvent* fInEvent;

  ERTofCalEvent* fOutEvent;

  Int_t fEvent;

  TRandom Ran;
  TString fPath;

  double CLB[8][10][32][5];
private:

  virtual void SetParContainers();
  
  ClassDef(ERTofCalibrator,1)
};

#endif
