// -------------------------------------------------------------------------
// -----                        ERNeuRadDigitizerFullMC header file          -----
// -----                  Created 01/16  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERNeuRadDigitizerFullMCFULLMC_H
#define ERNeuRadDigitizerFullMCFULLMC_H

#include <vector>
using std::vector;

#include "TRandom3.h"

#include "FairTask.h"
#include "ERNeuRadDigitizer.h"
#include "ERNeuRadDigi.h"
#include "ERNeuRadFiberPoint.h"
#include "ERNeuRadPMTSignal.h"
#include "ERNeuRadDigiPar.h"

class TObjectArray;
class TH1F;
class TH2F;
class TF1;
class TRandom;

class ERNeuRadDigitizerFullMC : public FairTask {

public:
  /** Default constructor **/
  ERNeuRadDigitizerFullMC();

  /** Constructor 
  ** verbose: 1 - only standard log print, 2 - print digi information 
  **/
  ERNeuRadDigitizerFullMC(Int_t verbose);

  /** Destructor **/
  ~ERNeuRadDigitizerFullMC();

  virtual InitStatus Init();

  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);

  /** Virtual method Finish **/
  virtual void Finish();

  /** Virtual method Reset **/
  virtual void Reset();
  
  /** Modifiers **/
  inline void SetDiscriminatorThreshold(const Double_t discrThreshold)
                                                        {fDiscriminatorThreshold = discrThreshold; }
  inline void SetPMTJitter(const Double_t PMTJitter)    {fPMTJitter = PMTJitter;}
  inline void SetPMTDelay(const Double_t PMTDelay)      {fPMTDelay = PMTDelay;}
  inline void SetPECountForOneElectronsSim(const Double_t count)
                                                        {fPECountForOneElectronsSim = count;}
  inline void SetExcessNoiseFactor(const Double_t enf)  {fExcessNoiseFactor = enf;}
  inline void SetScincilationTau(const Double_t tau)    {fScincilationTau = tau;}
  inline void SetScincilationDT(const Double_t dt)      {fScincilationDT = dt;}

  /** Accessors **/ 
  Int_t FiberPointCount()  const;
  Int_t PMTSignalCount()   const;
  Int_t DigiCount()        const;
protected:
  //Digitization parameters
  ERNeuRadDigiPar* fDigiPar;
  
  //Input arrays
  TClonesArray *fNeuRadPoints;
  TClonesArray *fNeuRadFirstStep;

  //Output arrays
  TClonesArray *fNeuRadFiberPoint;
  TClonesArray *fNeuRadPMTSignal;
  TClonesArray *fNeuRadDigi;
  
  //constants
  static const Double_t cSciFiLightYield; // [photons/MeV]
  static const Double_t cSpeedOfLight; //[cm/ns]
  static const Double_t cMaterialSpeedOfLight;//[cm/ns]
  static const Int_t    cErrorPointsInModuleCount;
  static const Double_t cLightFractionInTotalIntReflection;
  //доля света захватываемая файбером в полное внутренне отражение в каждую сторону.
  static const Double_t cExcessNoiseFactor;
  static const Double_t cPMTDelay; //[ns]
  static const Double_t cPMTJitter; //[ns]
  static const Int_t    cPECountForOneElectronsSim;
  static const Double_t cScincilationTau; //[ns]
  static const Double_t cScincilationDT;  //[ns]
  static const Double_t cMaxPointLength; //[cm] //старый параметр, для идеи дробления поинта в диджитизации. Будет удален в следующих версиях
  
  //Allow for user params
  Double_t fDiscriminatorThreshold; //[mV]
  Double_t fPMTJitter; //[ns]
  Double_t fPMTDelay; //[ns]
  Int_t fPECountForOneElectronsSim;
  Double_t fExcessNoiseFactor;
  Double_t fScincilationTau; //[ns]
  Double_t fScincilationDT;  //[ns]
protected:
  ERNeuRadFiberPoint* AddFiberPoint(Int_t i_point, Int_t side, Double_t lytime, Double_t cathode_time, Double_t anode_time, 
									Int_t photon_count, Int_t photoel_count,Double_t amplitude, Int_t onePE);

  ERNeuRadPMTSignal* AddPMTSignal(Int_t iFiber);
  
  ERNeuRadDigi* AddDigi(Double_t frontTDC, Double_t backTDC, Double_t TDC, Double_t frontQDC,
                        Double_t backQDC, Double_t QDC, Int_t fiber_nr);

private:
  TRandom3  *fRand;
private:
  virtual void SetParContainers();
  
  void LongPointSeparating(ERNeuRadPoint* point, std::vector<ERNeuRadPoint*> * points);
  
  void FiberPointsCreating(Int_t i_point, ERNeuRadPoint *point,
                          std::vector<ERNeuRadFiberPoint* >* frontPointsPerFibers,
                          std::vector<ERNeuRadFiberPoint* >* backPointsPerFibers);
                        
  void PMTSignalsAndDigiCreating(Int_t iFiber,
                                std::vector<ERNeuRadFiberPoint* >* frontPointsPerFibers,
                                std::vector<ERNeuRadFiberPoint* >* backPointsPerFibers);

  ClassDef(ERNeuRadDigitizerFullMC,1)
};

#endif
