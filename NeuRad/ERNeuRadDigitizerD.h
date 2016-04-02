// -------------------------------------------------------------------------
// -----                        ERNeuRadDigitizerD header file          -----
// -----                  Created 02/16  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERNEURADDIGITIZERD_H
#define ERNEURADDIGITIZERD_H

#include "ERNeuRadDigitizer.h"

class ERNeuRadDigitizerD : public ERNeuRadDigitizer{
public:
  /** Default constructor **/
  ERNeuRadDigitizerD();

  ERNeuRadDigitizerD(Int_t verbose);

  /** Destructor **/
  ~ERNeuRadDigitizerD();

  /** Virtual method Init **/
  virtual InitStatus Init();

protected:

  virtual void PMTSignalsAndDigiCreating(Int_t iBundle, Int_t iFiber,
                                std::vector<ERNeuRadFiberPoint* >** frontPointsPerFibers,
                                std::vector<ERNeuRadFiberPoint* >** backPointsPerFibers);

  //virtual ERNeuRadPMTSignal* AddPMTSignal(Int_t iBundle, Int_t iFiber);
  ERNeuRadPMTSignal* AddPMTSignal(Int_t iBundle, Int_t iFiber, Int_t fpoints_count);
  ClassDef(ERNeuRadDigitizerD,1)
};

#endif