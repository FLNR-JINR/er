// -------------------------------------------------------------------------
// -----                      ERNeuRadPMTSignalLP header file            -----
// -----                  Created 02/03/15  by v. Schetinin            -----
// -------------------------------------------------------------------------


#ifndef ERNeuRadPMTSignalLP_H
#define ERNeuRadPMTSignalLP_H


#include "TObject.h"
#include "TVector3.h"

#include <map>

#include "FairMultiLinkedData.h"

#include "ERNeuRadFiberPoint.h"
#include "ERNeuRadPMTSignal.h"

typedef std::pair<double, double> SignalPoint; //<time, amplitude>
typedef std::map<double, double> SignalPointsMap; 

class ERNeuRadPMTSignalLP : public ERNeuRadPMTSignal
{
 private:
  Int_t fFiberIndex;
  Int_t fBundleIndex;
  SignalPointsMap fSignalPoints;  //sum signal
  std::vector<SignalPointsMap> fSeparateSignals; //points of fiberPoinsSignals
 public:

  /** Default constructor **/
  ERNeuRadPMTSignalLP();

  /** Constructor with arguments **/
  ERNeuRadPMTSignalLP(Int_t iBundle, Int_t iFiber);

  virtual ~ERNeuRadPMTSignalLP();
  
  virtual void AddFiberPoint(ERNeuRadFiberPoint* fpoint);

  virtual void Generate() {}

  SignalPointsMap GetSignalPoints() {return fSignalPoints;}
  
  virtual bool Exist(){return (fSignalPoints.size() > 0);}
  Int_t FiberIndex() const {return fFiberIndex;}
  Int_t BundleIndex() const {return fBundleIndex;}
  int GetNumberOfSeparatesSignals(){return fSeparateSignals.size();};
  SignalPointsMap GetSeparateSignal(int i){return fSeparateSignals[i];}
  std::vector<Double_t> GetIntersections(Double_t discriminatorThreshold);
  Double_t GetMaxInteg(const Double_t window, const Double_t dt);
  Double_t GetInteg(const Double_t start,const Double_t finish);
  Double_t GetFirstInteg(const Double_t window);
  Double_t GetMean(const Double_t time);
  virtual Double_t GetStartTime();
  virtual Double_t GetFinishTime();
  
  ClassDef(ERNeuRadPMTSignalLP,1);
};

#endif
