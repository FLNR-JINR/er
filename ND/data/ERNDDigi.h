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
  ERNDDigi(ERChannel stilbenNb, float edep, float edep_n1, float edep_n2, float edep_mis, int parentTrackID, float lightYield, 
           float time, float neutronProb, float x_in, float y_in, float z_in, float tac = -1.);	//sonya:new variables x_in,y_in,z_in - enter point of neutron (proton in fact) in stilben
           											//sonya:new variables edep_n1 - energy deposit of n1,
           											// edep_n2 - energy deposit of n2, 
           											// edep_mis - energy deposit of miscellaneous particles
  /** Accessors **/
  float LightYield() const {return fLightYield;}
  float NeutronProb() const {return fNeutronProb;}
  float TAC() const {return fTAC;}
  float EdepN1() const {return fEdep_n1;}
  float EdepN2() const {return fEdep_n2;}
  float EdepMis() const {return fEdep_mis;}
  float ParentTrackID() const {return fParentTrackID;}
  /** Modifiers **/
  void SetTAC(const Double_t TAC) {fTAC = TAC;}
 protected:
  float fLightYield = -1.;
  float fNeutronProb = -1.;
  float fTAC = -1.;
  float fX_in = -1.;		//sonya:new variables x_in,y_in,z_in - enter point of neutron
  float fY_in = -1.;
  float fZ_in = -1.;
  float fEdep_n1 = -1;
  float fEdep_n2 = -1;
  float fEdep_mis = -1;
  int fParentTrackID = -1;
  ClassDef(ERNDDigi, 1)
};

#endif

