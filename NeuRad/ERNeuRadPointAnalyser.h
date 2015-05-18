// -------------------------------------------------------------------------
// -----                        ERNeuRadPointAnalyser header file  -----
// -----                  Created 03/15  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERNEURADPOINTANALYSER_H
#define ERNEURADPOINTANALYSER_H

#include <vector>
using std::vector;

#include "FairTask.h"

class TClonesArray;
class TObjectArray;
class TH1F;
class TH2F;
class TF1;
class TRandom3;

class ERNeuRadPointAnalyser : public FairTask {

public:
  /** Default constructor **/
  ERNeuRadPointAnalyser();

  /** Constructor 
  ** verbose: 1 - only standard log print, 2 - print digi information 
  **/
  ERNeuRadPointAnalyser(Int_t verbose);

  /** Destructor **/
  ~ERNeuRadPointAnalyser();

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
  
  //Input arrays
  TClonesArray *fNeuRadPoints;
  TClonesArray *fNeuRadFirstStep;
  
  //Control Histos
  TH1F *fhSumEnergyLossOfAllFibers;    //! Histo Events per sum of ELoss
  TH1F *fhFibersPerSumEnergyLoss;      //! Histo Modules with point count per Energy loss

  //Constants
  static const Double_t fHistoELossThreshold; //! Threshold of ELoss for analyze histos
protected:

private:


private:
  virtual void SetParContainers();

  ClassDef(ERNeuRadPointAnalyser,1)
};

#endif
