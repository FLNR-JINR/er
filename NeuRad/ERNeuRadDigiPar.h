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
  virtual Bool_t init(FairParIo* input);

  /** Output to file **/
  //virtual Int_t write(FairParIo* output);

  virtual void print();

  /** Reset all parameters **/
  virtual void clear();

  void putParams(FairParamList*);
  Bool_t getParams(FairParamList*);

  /** Modifiers**/

  /** Accessories  **/
  Int_t   NofFibers()     const {return fNofPixels;}
  Int_t   NofModules()    const {return fNofModules;}
  //TODO äîáàâèòü îáðàáîòêó âûõîäà ççà ãðàíèöó
  Double_t PixelQuantumEfficiency (Int_t iFiber) const {return (*fPixelQuantumEfficiency)[iFiber];}
  Double_t PixelGain (Int_t iFiber) const {return (*fPixelGain)[iFiber];}
  Double_t PixelSigma (Int_t iFiber) const {return (*fPixelSigma)[iFiber];}
  Bool_t UseCrosstalks() const {return fUseCrosstalks;}
  void Crosstalks(Int_t iFiber, TArrayF& crosstalks) const;
  Int_t RowNofFibers() const {return fRowNofFibers;}

private:

  TArrayF* fPixelQuantumEfficiency;
  TArrayF* fPixelGain;
  TArrayF* fPixelSigma;
  TArrayF* fPixelCrosstalks;
  TArrayF* fFiberCrosstalks;
  Int_t fNofPixels;
  Int_t fNofModules;
  Bool_t fUseCrosstalks;
  Int_t fRowNofFibers;

  ERNeuRadDigiPar(const ERNeuRadDigiPar&);
  ERNeuRadDigiPar& operator=(const ERNeuRadDigiPar&);

  ClassDef(ERNeuRadDigiPar, 1);
};

#endif // ERNEURADDIGIPAR_H
