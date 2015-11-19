// -------------------------------------------------------------------------
// -----                      ERNeuRadPMTSignal header file            -----
// -----                  Created 02/03/15  by v. Schetinin            -----
// -------------------------------------------------------------------------


#ifndef ERNeuRadPMTSignal_H
#define ERNeuRadPMTSignal_H


#include "TObject.h"
#include "TVector3.h"

#include <map>

#include "ERNeuRadFiberPoint.h"

typedef std::pair<double, double> SignalPoint; //<time, amplitude>
typedef std::map<double, double> SignalPointsMap; 

class ERNeuRadPMTSignal : public TObject
{
 private:
  SignalPointsMap fSignalPoints;  

 public:

  /** Default constructor **/
  ERNeuRadPMTSignal();

  /** Constructor with arguments **/
  //-------------------------------

  virtual ~ERNeuRadPMTSignal();
  
  void AddFiberPoint(ERNeuRadFiberPoint* fpoint);

  SignalPointsMap GetSignalPoints() {return fSignalPoints;}
  
  ClassDef(ERNeuRadPMTSignal,1);
};

#endif
