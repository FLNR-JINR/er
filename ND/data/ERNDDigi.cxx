/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
//sonya:new variables x_in,y_in,z_in - enter point of neutron

#include "ERNDDigi.h"

ERNDDigi::ERNDDigi(ERChannel stilbenNb, float edep, float edep_n1, float edep_n2, float edep_mis, int parentTrackID, float lightYoeld, float time, 
				   float neutronProb, float x_in, float y_in, float z_in, float tac/*-1*/)
	: ERDigi(edep, time, stilbenNb),fEdep_n1(edep_n1),fEdep_n2(edep_n2),fEdep_mis(edep_mis), fParentTrackID(parentTrackID), fLightYield(lightYoeld),
		fNeutronProb(neutronProb), fX_in(x_in), fY_in(y_in), fZ_in(z_in) , fTAC(tac)
{}

ClassImp(ERNDDigi)
