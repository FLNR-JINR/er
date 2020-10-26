/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERNDDigi.h"

ERNDDigi::ERNDDigi(ERChannel stilbenNb, 
				   float edep, float lightYoeld, float time, 
				   float neutronProb, float tac/*-1*/)
	: ERDigi(edep, time, stilbenNb), fLightYield(lightYoeld),
	  fNeutronProb(neutronProb), fTAC(tac) {
}

ClassImp(ERNDDigi)