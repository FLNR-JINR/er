/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

// -------------------------------------------------------------------------
// -----                  ERNeuRadMatcher header file                  -----
// -----                  Created 03/16  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERNeuRadMatcher_H
#define ERNeuRadMatcher_H

#include "FairTask.h" // mother class

class TH1F;
class TClonesArray;

class ERNeuRadMatcher : public FairTask {

public:
  /** Default constructor **/
  ERNeuRadMatcher();

  /** Standard constructor **/
  ERNeuRadMatcher(Int_t verbose);

  /** Destructor **/
  ~ERNeuRadMatcher();

  /** Virtual method Init **/
  virtual InitStatus Init();

  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

  /** Virtual method Finish **/
  virtual void Finish();

  /** Virtual method Reset **/
  virtual void Reset();
  
  /** Modifiers **/

  /** Accessors **/

protected:

  // Input arrays
  TClonesArray* fNeuRadHits;
  TClonesArray* fNeuRadFirstSteps;

  // Output arrays

  // Output histos
  TH1F* fHdxy;
  TH1F* fHdxyLess6;
  TH1F* fHdxyOF;
  TH1F* fHdxyLess6OF;
  TH1F* fHdxyTF;
  TH1F* fHdxyLess6TF;

private:

  virtual void SetParContainers();
  
  ClassDef(ERNeuRadMatcher, 1);
};

#endif // ERNeuRadMatcher_H
