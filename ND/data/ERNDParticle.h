/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERNDParticle_H
#define ERNDParticle_H

#include "TLorentzVector.h"

class ERNDParticle: public TObject {
  public:
	ERNDParticle() = default;
	ERNDParticle(const TLorentzVector& lv, float tof);
    TLorentzVector LV() const { return fLV; }
    float ToF() const { return fToF; }
  protected:
    TLorentzVector fLV;
    float fToF = -1.;
	ClassDef(ERNDParticle,1);
};

#endif
