// -------------------------------------------------------------------------
// -----                        ERNeuRadDigitizer header file          -----
// -----                  Created 03/15  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERNEURADDIGITIZER_H
#define ERNEURADDIGITIZER_H

#include <vector>

#include "TStopwatch.h"

#include "FairTask.h"

#include "ERNeuRadPoint.h"
#include "ERNeuRadPhotoElectron.h"
#include "ERNeuRadPixelSignal.h"
#include "ERNeuRadSetup.h"

class TObjectArray;

class ERNeuRadDigitizer : public FairTask {

public:
  /** Default constructor **/
  ERNeuRadDigitizer();

  /** Constructor 
  ** verbose: 1 - only standard log print, 2 - print digi information 
  **/
  ERNeuRadDigitizer(Int_t verbose);

  /** Destructor **/
  ~ERNeuRadDigitizer();

  /** Virtual method Init **/
  virtual InitStatus Init();

  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

  /** Virtual method Finish **/
  virtual void Finish();

  /** Virtual method Reset **/
  virtual void Reset();
  
  /** Modifiers **/
  inline void SetPixelJitter(const Double_t PixelJitter)    {fPixelJitter = PixelJitter;}
  inline void SetPixelDelay(const Double_t PixelDelay)      {fPixelDelay = PixelDelay;}
  inline void SetScincilationTau(const Double_t tau)    {fScincilationTau = tau;}
  inline void SetUseCrosstalks(const Bool_t use)        {fUseCrosstalks = use;}

  /** Accessors **/ 
  Int_t PhotoElectronCount()  const;
  Int_t PixelSignalCount()   const;
protected:
  //Digitization parameters
  ERNeuRadSetup* fNeuRadSetup;
  //Input arrays
  TClonesArray *fNeuRadPoints;

  //Output arrays
  TClonesArray *fNeuRadPhotoElectron;
  TClonesArray *fNeuRadPixelSignal;

  //Event header variable
  Int_t fPECountF;
  Int_t fPECountB;
  Float_t fSumAmplitudeF;
  Float_t fSumAmplitudeB;
  
  //constants
  static const Double_t cSciFiLightYield; // [photons/MeV]
  static const Double_t cSpeedOfLight; //[cm/ns]
  static const Double_t cMaterialSpeedOfLight;//[cm/ns]
  static const Double_t cLightFractionInTotalIntReflection;
  //доля света захватываемая файбером в полное внутренне отражение в каждую сторону.
  static const Double_t cPixelDelay; //[ns]
  static const Double_t cPixelJitter; //[ns]
  static const Double_t cScincilationTau; //[ns]

  //Allow for user params
  Double_t fPixelJitter; //[ns]
  Double_t fPixelDelay; //[ns]
  Double_t fExcessNoiseFactor;
  Double_t fScincilationTau; //[ns]

  TStopwatch fPhotoElectronsCreatingTimer;
  Double_t fPhotoElectronsCreatingTime;
  TStopwatch fPixelSignalCreatingTimer;
  Double_t fPixelSignalCreatingTime;
  Bool_t fUseCrosstalks;
protected:
  ERNeuRadPhotoElectron* AddPhotoElectron(Int_t i_point, Int_t side, Double_t lytime, Double_t cathode_time, Double_t anode_time, 
									Int_t photon_count,Double_t amplitudes);

  virtual ERNeuRadPixelSignal* AddPixelSignal(Int_t iModule, Int_t iPixel, Int_t fpoints_count, Int_t side);
  
  virtual void PhotoElectronsCreating(Int_t i_point, ERNeuRadPoint *point,
                          std::vector<ERNeuRadPhotoElectron* >** pePerPixels, Int_t side, Int_t& sumPECount,Float_t& sumAmplitude);
                        
  virtual void PixelSignalsCreating(Int_t iModule, Int_t iPixel,
                                std::vector<ERNeuRadPhotoElectron* >** pePerPixels, Int_t side);

  Int_t Crosstalks(Int_t pointModule, Int_t pointPixel, Int_t& peModule, Int_t& pePixel);
private:
  virtual void SetParContainers();
  
  ClassDef(ERNeuRadDigitizer,1)
};

#endif
