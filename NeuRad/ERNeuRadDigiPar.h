/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

// -------------------------------------------------------------------------
// -----                  ERNeuRadDigiPar header file                  -----
// -----                  Created 03/15  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERNEURADDIGIPAR_H
#define ERNEURADDIGIPAR_H

#include "FairParGenericSet.h" // mother class

#include "TArrayF.h"

class FairParIo;
class FairParamList;

class ERNeuRadDigiPar : public FairParGenericSet
{
public:

  /** Standard constructor **/
  ERNeuRadDigiPar(const char* name    = "ERNeuRadDigiPar",
                  const char* title   = "ERNeuRad Digitization Parameters",
                  const char* context = "Default");

  /** Destructor **/
  virtual ~ERNeuRadDigiPar();

  /** Initialization from input device **/
  virtual Bool_t init(FairParIo* input); // Yes, it starts NOT with the capital i... :-(

  /** Output to file **/
  //virtual Int_t write(FairParIo* output);

  virtual void print();

  /** Reset all parameters **/
  virtual void clear();

  void putParams(FairParamList*);
  Bool_t getParams(FairParamList*);

  /** Modifiers**/

  /** Accessors  **/
  Int_t GetRowNofPixelsPerPMT() const { return fRowNofPixelsPerPMT; }
  Int_t GetNofPixelsPerPMT() const { return fNofPixelsPerPMT; }
  Int_t GetNofPMTs() const { return fNofPMTs; }

  Bool_t UseCrosstalks()  const { return fUseCrosstalks; }

  //TODO äîáàâèòü îáðàáîòêó âûõîäà ççà ãðàíèöó
  //TODO TArrayF is an array of FLOATs, so return float
  //FIXME In case you want to work with several PMTs with different characteristics:
  //FIXME compute array index correctly taking total number of modules (PMTs) into account!
  Float_t GetPixelQuantumEfficiency (Int_t iPmtId, Int_t iChId) const { return (*fPixelQuantumEfficiency)[iChId]; }
  Float_t GetPixelGain (Int_t iPmtId, Int_t iChId) const { return (*fPixelGain)[iChId]; }
  Float_t GetPixelSigma (Int_t iPmtId, Int_t iChId) const { return (*fPixelSigma)[iChId]; }

  void Crosstalks(Int_t iChId, TArrayF& crosstalks) const;

private:
  
  /** Data members **/

  Int_t fRowNofPixelsPerPMT;
  Int_t fNofPixelsPerPMT;
  Int_t fNofPMTs;

  Bool_t fUseCrosstalks;

  TArrayF* fPixelQuantumEfficiency;
  TArrayF* fPixelGain;
  TArrayF* fPixelSigma;
  TArrayF* fPixelCrosstalks;
  //TArrayF* fFiberCrosstalks; //FIXME Keep it commented for time being

  /** Common stuff **/

  /** Copy constructor **/
  ERNeuRadDigiPar(const ERNeuRadDigiPar&);

  /** Copy operator **/
  ERNeuRadDigiPar& operator=(const ERNeuRadDigiPar&);

  ClassDef(ERNeuRadDigiPar, 1);
};

#endif // ERNEURADDIGIPAR_H
