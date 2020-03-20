/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERQTelescopeSiDigi.h"

#include "FairLogger.h"

//-------------------------------------------------------------------------------------------------
ERQTelescopeSiDigi::ERQTelescopeSiDigi(Double_t edep, Double_t time, Int_t stationNb, Int_t channel)
  : ERDigi(edep, time, channel), fStationNb(stationNb)
{
}
//-------------------------------------------------------------------------------------------------
void ERQTelescopeSiDigi::Print(const Option_t* opt /* = 0 */) const {
  LOG(INFO) << "-I- ERQTelescopeSiDigi:  edep = " << fEdep << ", time = " << fTime
            << ", station number = " << fStationNb << ", channel = " << fChannel << FairLogger::endl;
}
//-------------------------------------------------------------------------------------------------
ClassImp(ERQTelescopeSiDigi)
