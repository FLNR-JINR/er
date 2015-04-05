// -------------------------------------------------------------------------
// -----                        ExpertNeuRadDigiAnalyser header file   -----
// -----                  Created 03/15  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef EXPERTNEURADDIGIANALYSER_H
#define EXPERTNEURADDIGIANALYSER_H 

#include <vector>
using std::vector;

#include "FairTask.h"

class TClonesArray;
class TObjectArray;
class TH1F;
class TH2F;
class TF1;
class TRandom3;

class ExpertNeuRadDigiAnalyser : public FairTask {

public:
  /** Default constructor **/
  ExpertNeuRadDigiAnalyser();

  /** Constructor 
  ** verbose: 1 - only standard log print, 2 - print digi information 
  **/
  ExpertNeuRadDigiAnalyser(Int_t verbose);

  /** Destructor **/
  ~ExpertNeuRadDigiAnalyser();

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
  TClonesArray *fNeuRadDigi;
  
  //Control Histos
  TH1F* fhFrontQDCcmXMinusFirst;
  TH1F* fhFrontQDCcmYMinusFirst;
  TH1F* fhBackQDCcmXMinusFirst;
  TH1F* fhBackQDCcmYMinusFirst;  

protected:

private:


private:
  virtual void SetParContainers();

  ClassDef(ExpertNeuRadDigiAnalyser,1)
};

#endif
