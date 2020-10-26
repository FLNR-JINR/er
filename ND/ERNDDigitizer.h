/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERNDDigitizer_H
#define ERNDDigitizer_H

#include "TClonesArray.h"

#include "FairTask.h"

#include "ERNDDigi.h"
#include "ERNDSetup.h"

class ERNDDigitizer : public FairTask {

public:
  /** Default constructor **/
  ERNDDigitizer();

  /** Constructor 
  ** verbose: 1 - only standard log print, 2 - print digi information 
  **/
  ERNDDigitizer(Int_t verbose);

  /** Destructor **/
  ~ERNDDigitizer();

  /** Virtual method Init **/
  virtual InitStatus Init();

  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

  /** Virtual method Finish **/
  virtual void Finish();

  /** Virtual method Reset **/
  virtual void Reset();
  
  /** Modifiers **/
  void SetLYError(Float_t a, Float_t b, Float_t c) {fLYErrorA = a;fLYErrorB = b;fLYErrorC = c;}
  void SetEdepError(Float_t a, Float_t b, Float_t c) {fEdepErrorA = a;fEdepErrorB = b;fEdepErrorC = c;}
  void SetTimeError(Float_t a) {fTimeErrorA = a;}
  
  void SetQuenchThreshold(Float_t th) {fQuenchThreshold = th;}
  void SetLYThreshold(Float_t th) {fLYThreshold = th;}
  void SetProbabilityB(Float_t b) {fProbabilityB = b;}
  void SetProbabilityC(Float_t c) {fProbabilityC = c;}
  /** Accessors **/ 
  Float_t ElossThreshold() const {return fQuenchThreshold;}
  Float_t LYThreshold() const {return fLYThreshold;}
  Float_t ProbabilityB() const {return fProbabilityB;}
  Float_t ProbabilityC() const {return fProbabilityC;}
protected:
  //Input arrays
  TClonesArray* fNDPoints = nullptr;
  //Output arrays
  TClonesArray* fNDDigis = nullptr;

  static Int_t fEvent;
  Float_t fEdepErrorA, fEdepErrorB, fEdepErrorC;
  Float_t fLYErrorA, fLYErrorB, fLYErrorC;
  Float_t fTimeErrorA;
  Float_t fQuenchThreshold;
  Float_t fLYThreshold;
  Float_t fProbabilityB;
  Float_t fProbabilityC;

  ERNDSetup* fSetup = nullptr;
protected:
  ERNDDigi* AddDigi(Int_t stilbenNb, Float_t edep, Float_t lightYield, Float_t time, Float_t neutronProb);
  Float_t NeutronProbability(Float_t edep, Float_t ly);
private:
  virtual void SetParContainers();
  
  ClassDef(ERNDDigitizer,1)
};

#endif
