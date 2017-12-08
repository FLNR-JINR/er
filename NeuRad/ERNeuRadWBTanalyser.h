/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERNEURADWBTANALYSER_H
#define ERNEURADWBTANALYSER_H

#include "FairTask.h"

class TClonesArray;

class ERNeuRadWBTanalyser : public FairTask
{
public:
  /** Default constructor **/
  ERNeuRadWBTanalyser();

  /** Destructor **/
  ~ERNeuRadWBTanalyser();

  /** Virtual method Init **/
  virtual InitStatus Init();

  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

  /** Virtual method Finish **/
  virtual void Finish();

  /** Virtual method Reset **/
  virtual void Reset();

private:

  /** Input arrays from digitization **/
  TClonesArray *fNeuRadPhotoElectron;
  TClonesArray *fNeuRadPixelSignal;

  ClassDef(ERNeuRadWBTanalyser, 1);
};

#endif // ERNEURADWBTANALYSER_H
