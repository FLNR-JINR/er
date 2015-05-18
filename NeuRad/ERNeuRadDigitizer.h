// -------------------------------------------------------------------------
// -----                        ERNeuRadDigitizer header file      -----
// -----                  Created 03/15  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERNEURADDIGITIZER_H
#define ERNEURADDIGITIZER_H

#include <vector>
using std::vector;

#include "FairTask.h"
#include "ERNeuRadDigi.h"

class TClonesArray;
class TObjectArray;
class TH1F;
class TH2F;
class TF1;
class TRandom3;


/** Structure for fiber point properties **/
struct ERNeuRadFiberPoint
{
  Double_t time;
  Double_t lightQDC;
  Double_t energy;
};

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
  inline void      SetBeamEnergy(const Double_t& beamEnergy) {fBeamEnergy = beamEnergy; }
  inline void      SetTOFRange(const Double_t& TOFRange) {  fTOFRange = TOFRange;  }
  inline void      SetSaturationCoefficient(const Double_t& saturationCoefficient) {  fSaturationCoefficient = saturationCoefficient;  }
  inline void      SetFiberThreshold(const Double_t& fiberThreshold) {fFiberThreshold = fiberThreshold; }
  
  /** Accessors **/
  inline Double_t  GetBeamEnergy() const { return fBeamEnergy; }
  inline Double_t  GetTOFRange() const  {  return fTOFRange;  }
  inline Double_t  GetSaturationCoefficient() const  {  return fSaturationCoefficient; }
  inline Double_t  GetFiberThreshold() const { return fFiberThreshold; }
  
protected:
  //Geometry parameters
  Int_t fNFibers;
  
  //Input arrays
  TClonesArray *fNeuRadPoints;
  TClonesArray *fNeuRadFirstStep;

  //Output arrays
  TClonesArray *fNeuRadDigi;
  
  //constants
  static const Double_t DEFAULT_SATURATION_COEFFICIENT;
  static const Double_t LIGHT_ATTENUATION ;   // [1/cm]
  static const Double_t BC408_DECAY_CONSTANT; // [1/ns]
  static const Int_t    ERROR_POINTS_IN_MODULE_COUNT;
  static const Double_t SPEED_OF_FLIGHT_IN_MATERIAL; //[cm/ns]
  static const Double_t MEASURING_ERROR_WIDTH_COEF;
  
  //Allow for user params
  Double_t fBeamEnergy;
  Double_t fTOFRange;                 // Allow change of length of time gate for QDC ("Integration time") at runtime
  Double_t fSaturationCoefficient;    // Allow change of the PMT Saturation at runtime. ERNeuRadDigitizer is initialized with the default value
  Double_t fFiberThreshold;
  
protected:
  inline Double_t BuildTOFRangeFromBeamEnergy(const Double_t &e);
  
  ERNeuRadDigi* AddDigi(Int_t digi_nr, Double_t frontTDC, Double_t backTDC,
                                      Double_t TDC, Double_t frontQDC, Double_t backQDC, Double_t QDC,
                                      Int_t fiber_nr);

private:

private:
  virtual void SetParContainers();

  ClassDef(ERNeuRadDigitizer,1)
};

#endif
