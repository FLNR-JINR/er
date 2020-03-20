/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERNDPID_H
#define ERNDPID_H

#include "TClonesArray.h"
#include "TLorentzVector.h"

#include "ERTask.h"
#include "ERParticle.h"
#include "ERNDSetup.h"

class ERNDPID : public ERTask {
  public:
    ERNDPID();
    virtual InitStatus Init(); 
    virtual void Exec(Option_t* opt);
    virtual void Reset();
  protected:
    //Input arrays
    TClonesArray* fNDTracks = nullptr;
    //Output arrays
    TClonesArray *fNDParticles = nullptr;
    ERNDSetup* fSetup = nullptr;
    ERParticle* AddParticle(const TLorentzVector& detectorState, const TLorentzVector& targetState);
    ClassDef(ERNDPID,1)
};

#endif