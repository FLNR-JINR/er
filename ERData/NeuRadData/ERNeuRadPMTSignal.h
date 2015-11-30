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
  SignalPointsMap fSignalPoints;  //sum signal
  std::vector<SignalPointsMap> fSeparateSignals; //points of fiberPoinsSignals
 public:

  /** Default constructor **/
  ERNeuRadPMTSignal();

  /** Constructor with arguments **/
  //-------------------------------

  virtual ~ERNeuRadPMTSignal();
  
  void AddFiberPoint(ERNeuRadFiberPoint* fpoint);

  SignalPointsMap GetSignalPoints() {return fSignalPoints;}
  
  bool Exist(){return (fSignalPoints.size() > 0);}
  int GetNumberOfSeparatesSignals(){return fSeparateSignals.size();};
  SignalPointsMap GetSeparateSignal(int i){return fSeparateSignals[i];}
  std::vector<Double_t> GetIntersections(Double_t discriminatorThreshold);
  
  ClassDef(ERNeuRadPMTSignal,1);
};

#endif
