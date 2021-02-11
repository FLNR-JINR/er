/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERNDDigi_H
#define ERNDDigi_H

#include "TVector3.h"

#include "ERDigi.h"

class ERNDDigi : public ERDigi {
 public:
  ERNDDigi() = default;
  ERNDDigi(ERChannel stilbenNb, float edep, float lightYield, 
           float time, float neutronProb, float tac = -1.);
  /** Accessors **/
  float LightYield() const {return fLightYield;}
  float NeutronProb() const {return fNeutronProb;}
  float TAC() const {return fTAC;}
  /** Modifiers **/
  void SetTAC(const Double_t TAC) {fTAC = TAC;}
 protected:
  float fLightYield = -1.;
  float fNeutronProb = -1.;
  float fTAC = -1.;
  ClassDef(ERNDDigi, 1)
};

#endif

