/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "ERBeamDetTOFDigi.h"

#include "FairLogger.h"

//---------------------------------------------------------------------------------
ERBeamDetTOFDigi::ERBeamDetTOFDigi(Double_t edep, Double_t time, Int_t tofNb)
  :ERDigi(edep, time, 0), fToFNb(tofNb)
{
}
//---------------------------------------------------------------------------------
void ERBeamDetTOFDigi::Print(const Option_t* opt /* = 0 */) const {
  LOG(INFO) << "-I- ERBeamDetTOFDigi:  edep = " << fEdep << ", time = " << fTime
            << ", tof = " << fToFNb << FairLogger::endl;
}
//---------------------------------------------------------------------------------
ClassImp(ERBeamDetTOFDigi)
