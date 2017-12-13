/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERNEURADSETUP_H
#define ERNEURADSETUP_H

#include <vector>

#include "Rtypes.h"
#include "TArrayF.h"

#include "ERNeuRadDigiPar.h" // used by inline methods

/*
//TODO not used by now
struct ERNeuRadModule {
    Float_t fX;
    Float_t fY;
    ERNeuRadModule(Float_t x, Float_t y) { fX = x; fY = y; }
};
*/

struct ERNeuRadFiber {
    Float_t fX;
    Float_t fY;
    ERNeuRadFiber(Float_t x, Float_t y) { fX = x; fY = y; }
};

class ERNeuRadSetup {

public:

  // No constructor in public section. This is a singleton class!

  // Only this method has to be static.
  // For others it makes no sense.
  // An object of this class is anyway the only object of this class.
  static ERNeuRadSetup* Instance(void); // (public!) Singleton management

private:
  ERNeuRadSetup();  // (private!) Singleton management
  static ERNeuRadSetup* fInstance; // (private!)  Singleton management

public:
/*
  //TODO not used by now
  Float_t GetModuleX(Int_t iPmtId) const;
  Float_t GetModuleY(Int_t iPmtId) const;
*/
  Float_t GetFiberX(Int_t iPmtId, Int_t iChId) const;
  Float_t GetFiberY(Int_t iPmtId, Int_t iChId) const;

  void Print(void) const;

  /** TODO what is this? **/
  Int_t SetParContainers(void);

  /** These methods acquire the ERNeuRadDigiPar class object and return the value from there **/
  Bool_t UseCrosstalks(void) const;
  Float_t GetPixelQuantumEfficiency(Int_t iPmtId, Int_t iChId) const;
  Float_t GetPixelGain(Int_t iPmtId, Int_t iChId) const;
  Float_t GetPixelSigma(Int_t iPmtId, Int_t iChId) const;
  void Crosstalks(Int_t iPMT, Int_t iChannel, TArrayF& crosstalks) const;

  /** Accessors **/
  Float_t GetZ(void) const { return fZ; }
  Float_t GetFiberLength(void) const { return fLength; }
  Float_t GetFiberWidth(void)const { return fFiberWidth; }

  Int_t GetRowNofFibers(void) const { return fRowNofFibers; }
  Int_t GetRowNofPixels(void) const { return fRowNofPixels; }
  Int_t GetRowNofModules(void) const { return fRowNofModules; }

  /*Int_t GetNofFibers(void) const { return fRowNofFibers * fRowNofFibers; }*/
  Int_t GetNofPixelsPerPMT(void) const { return fRowNofPixels * fRowNofPixels; }

  //TODO take care!
  //TODO Probably this is not always true!
  Int_t GetNofModules(void) const {return fRowNofModules * fRowNofModules; }

private: // methods

  void AnalyseGeoManager(void);

private: // data members
/*
  //TODO not used by now
  std::vector<ERNeuRadModule*> fModules;
*/
  std::vector<std::vector<ERNeuRadFiber*> > fFibers;

  ERNeuRadDigiPar* fDigiPar;

  /** Z position of NeuRad. **/
  Float_t fZ;
  /** Z length of the module. In principle it should be the same as fibers' Z length. **/ //TODO check
  Float_t fLength;
  /** Width of the fiber along X direction. In principle should be the same as along Y direction. **/ //TODO check
  Float_t fFiberWidth;


  Int_t fRowNofFibers;
  Int_t fRowNofPixels;
  Int_t fRowNofModules;

  ClassDef(ERNeuRadSetup, 1);
};

#endif // ERNEURADSETUP_H
