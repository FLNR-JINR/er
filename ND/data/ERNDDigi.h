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
	ERNDDigi(TVector3& pos, TVector3& dpos, Int_t stilbenNb, Double_t edep, Double_t lightYield, 
	         Double_t time, Double_t neutronProb, Double_t tac = -1.);
	/** Accessors **/
	Double_t LightYield() const {return fLightYield;}
	Double_t NeutronProb() const {return fNeutronProb;}
	Double_t TAC() const {return fTAC;}
	TVector3 Position() const {return fPos;}
	TVector3 DPosition() const {return fDPos;}
	/** Modifiers **/
	void Position(TVector3& pos) {fPos = pos;}
	void DPosition(TVector3& dpos) {fDPos = dpos;}
	void SetTAC(const Double_t TAC) {fTAC = TAC;}
 protected:
	Double_t fLightYield = -1.;
	Double_t fNeutronProb = -1.;
	Double_t fTAC = -1.;
	TVector3 fPos;
	TVector3 fDPos;

	ClassDef(ERNDDigi, 1)
};

#endif

