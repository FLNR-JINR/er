// -------------------------------------------------------------------------
// -----                      ERNeuRadPMTSignal header file            -----
// -----                  Created 02/03/15  by v. Schetinin            -----
// -------------------------------------------------------------------------


#ifndef ERNeuRadPMTSignal_H
#define ERNeuRadPMTSignal_H


#include "TObject.h"
#include "TVector3.h"

#include <map>

#include "FairMultiLinkedData.h"

#include "ERNeuRadFiberPoint.h"

typedef std::pair<double, double> SignalPoint; //<time, amplitude>
typedef std::map<double, double> SignalPointsMap; 

class ERNeuRadPMTSignal : public FairMultiLinkedData
{
 private:
  Int_t fFiberIndex;
  SignalPointsMap fSignalPoints;  //sum signal
  std::vector<SignalPointsMap> fSeparateSignals; //points of fiberPoinsSignals
 public:

  /** Default constructor **/
  ERNeuRadPMTSignal();

  /** Constructor with arguments **/
  ERNeuRadPMTSignal(Int_t iFiber);

  virtual ~ERNeuRadPMTSignal();
  
  void AddFiberPoint(ERNeuRadFiberPoint* fpoint);

  SignalPointsMap GetSignalPoints() {return fSignalPoints;}
  
  bool Exist(){return (fSignalPoints.size() > 0);}
  Int_t FiberIndex() const {return fFiberIndex;}
  int GetNumberOfSeparatesSignals(){return fSeparateSignals.size();};
  SignalPointsMap GetSeparateSignal(int i){return fSeparateSignals[i];}
  std::vector<Double_t> GetIntersections(Double_t discriminatorThreshold);
  Double_t GetMaxInteg(const Double_t window, const Double_t dt);
  Double_t GetInteg(const Double_t start,const Double_t finish);
  Double_t GetFirstInteg(const Double_t window);
  Double_t GetMean(const Double_t time);
  Double_t GetStartTime();
  Double_t GetFinishTime();
  
  ClassDef(ERNeuRadPMTSignal,1);
};

#endif
