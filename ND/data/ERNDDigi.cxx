/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERNDDigi.h"

ERNDDigi::ERNDDigi(TVector3& pos, TVector3& dpos, Int_t stilbenNb, 
				   Double_t edep, Double_t lightYoeld, Double_t time, 
				   Double_t neutronProb, Double_t tac/*-1*/)
	: ERDigi(edep, time, stilbenNb), fPos(pos), fDPos(dpos), fLightYield(lightYoeld),
	  fNeutronProb(neutronProb), fTAC(tac)
{
}

ClassImp(ERNDDigi)