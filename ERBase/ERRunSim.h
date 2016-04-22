// -------------------------------------------------------------------------
// -----                      ERRunSim header file                     -----
// -----                  Created 04/16  by V. Schetinin               -----
// -------------------------------------------------------------------------

#ifndef ERRUNSIM_H
#define ERRUNSIM_H

#include "FairRunSim.h"                    // for FairRunSim

#include "ERMCApplication.h"
#include "ERDecayer.h"

class ERRunSim : public FairRunSim
{
  public:
    /** default ctor*/
    ERRunSim(Bool_t isMaster = kTRUE);
    /** default dtor*/
    virtual ~ERRunSim();
    /**
    *       Initialize the Simulation
    */
    virtual void        Init();

    void SetDecayer(ERDecayer* deacyer){((ERMCApplication*)fApp)->SetDecayer(deacyer);}

  private:
    ERRunSim(const ERRunSim& M);
    ERRunSim& operator= (const  ERRunSim&) {return *this;}

    void SetMCConfig();

    ClassDef(ERRunSim ,1)

};

#endif
