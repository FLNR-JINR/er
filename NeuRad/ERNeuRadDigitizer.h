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
#include "ERNeuRadFiberPoint.h"

class TClonesArray;
class TObjectArray;
class TH1F;
class TH2F;
class TF1;
class TRandom;

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
  inline void SetFiberThreshold(const Double_t& fiberThreshold) {fFiberThreshold = fiberThreshold; }
  
  /** Accessors **/
  inline Double_t  GetFiberThreshold() const { return fFiberThreshold; }
  
protected:
  //Geometry parameters
  Int_t fNFibers;
  
  //Input arrays
  TClonesArray *fNeuRadPoints;
  TClonesArray *fNeuRadFirstStep;

  //Output arrays
  TClonesArray *fNeuRadFiberPoint;
  TClonesArray *fNeuRadDigi;
  
  //constants
  static const Double_t SciFi_LIGHT_YIELD; // [photons/MeV]
  static const Double_t SPEED_OF_LIGHT; //[cm/ns]
  static const Double_t SPEED_OF_LIGHT_IN_MATERIAL;//[cm/ns]
  static const Int_t    ERROR_POINTS_IN_MODULE_COUNT;
  static const Double_t LIGHT_FRACTION_IN_TOTAL_INT_REFLECTION;
  //доля света захватываемая файбером в полное внутренне отражение в каждую сторону.
  static const Double_t PMT_QUANTUM_EFFICIENCY;
  static const Double_t PMT_GAIN;
  static const Double_t EXCESS_NOISE_FACTOR;
  static const Double_t PMT_DELAY;
  static const Double_t PMT_JITTER;
  
  //Allow for user params
  Double_t fFiberThreshold;
  
protected:
  ERNeuRadFiberPoint* AddFiberPoint(Int_t side, Double_t cathode_time, Double_t anode_time, 
									Int_t photon_count, Int_t photoel_count, 
									Double_t amplitude);
  
  ERNeuRadDigi* AddDigi(Int_t digi_nr, Double_t frontTDC, Double_t backTDC,
                                      Double_t TDC, Double_t frontQDC, Double_t backQDC, Double_t QDC,
                                      Int_t fiber_nr);

private:

private:
  virtual void SetParContainers();

  ClassDef(ERNeuRadDigitizer,1)
};

#endif
