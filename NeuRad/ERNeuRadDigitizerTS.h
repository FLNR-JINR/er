// -------------------------------------------------------------------------
// -----                        ERNeuRadDigitizerTS header file          -----
// -----                  Created 02/16  by V.Schetinin                -----
// -------------------------------------------------------------------------

#ifndef ERNEURADDIGITIZERTS_H
#define ERNEURADDIGITIZERTS_H

#include "ERNeuRadDigitizer.h"

class ERNeuRadDigitizerTS : public ERNeuRadDigitizer{
public:
  /** Default constructor **/
  ERNeuRadDigitizerTS();

  /** Constructor 
  ** verbose: 1 - only standard log print, 2 - print digi information 
  **/
  ERNeuRadDigitizerTS(Int_t verbose);

  /** Destructor **/
  ~ERNeuRadDigitizerTS();

protected:
  virtual void FiberPointsCreating(Int_t i_point, ERNeuRadPoint *point,
                          std::vector<ERNeuRadFiberPoint* >** frontPointsPerFibers,
                          std::vector<ERNeuRadFiberPoint* >** backPointsPerFibers);

  ClassDef(ERNeuRadDigitizerTS,1)
};

#endif
