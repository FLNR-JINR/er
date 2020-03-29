/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERBeamDetMWPCDigi.h"

#include "FairLogger.h"

//---------------------------------------------------------------------------------
ERBeamDetMWPCDigi::ERBeamDetMWPCDigi(Double_t edep, Double_t time, Int_t mwpcNb, 
                                     Int_t planeNb, Int_t wireNb)
  : ERDigi(edep, time, wireNb), fMWPCNb(mwpcNb), fPlaneNb(planeNb)  
{
}
//---------------------------------------------------------------------------------
void ERBeamDetMWPCDigi::Print(const Option_t* opt /* = 0 */) const {
  LOG(INFO) << "-I- ERBeamDetMWPCDigi:  edep = " << fEdep << ", time = " << fTime
            << ", channel = " << fChannel << ", plane = " << fPlaneNb 
            << ", mwpc = " << fMWPCNb << FairLogger::endl;
}
//---------------------------------------------------------------------------------

ClassImp(ERBeamDetMWPCDigi)
