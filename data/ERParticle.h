/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERParticle_H
#define ERParticle_H

#include "TNamed.h"
#include "TTree.h"
#include "TString.h"
#include "TLorentzVector.h"

#include "FairMultiLinkedData.h"

class ERParticle: public FairMultiLinkedData {
  public:
	ERParticle() = default;
	ERParticle(const TLorentzVector& detectorState, const TLorentzVector& targetState);
  protected:
    TLorentzVector fDetectorState;
	TLorentzVector fTargetState;
	ClassDef(ERParticle,1);
};

#endif
