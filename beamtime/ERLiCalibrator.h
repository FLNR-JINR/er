/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERLiCalibrator_H
#define ERLiCalibrator_H

#include "FairTask.h"

#include "TLiEvent.h"

class ERLiCalibrator : public FairTask {

public:
  /** Default constructor **/
  ERLiCalibrator();

  /** Constructor 
  ** verbose: 1 - only standard log print, 2 - print digi information 
  **/
  ERLiCalibrator(Int_t verbose);

  /** Destructor **/
  ~ERLiCalibrator();

  /** Virtual method Init **/
  virtual InitStatus Init();

  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

  /** Virtual method Finish **/
  virtual void Finish();

  /** Virtual method Reset **/
  virtual void Reset();

protected:
  //Input objects
  TLiEvent* fRawEvent;

  Int_t fEvent;

private:
  virtual void SetParContainers();
  
  ClassDef(ERLiCalibrator,1)
};

#endif
