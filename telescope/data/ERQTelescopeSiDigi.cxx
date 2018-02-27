/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERQTelescopeSiDigi.h"

#include "FairLogger.h"

using namespace std;

//-------------------------------------------------------------------------------------------------
ERQTelescopeSiDigi::ERQTelescopeSiDigi()
  :fID(-1),
  fEdep(0.)
{
}
//-------------------------------------------------------------------------------------------------
ERQTelescopeSiDigi::ERQTelescopeSiDigi(Int_t id, Double_t Edep, Double_t time, Int_t stationNb, Int_t stripNb)
  :fID(id),
  fEdep(Edep),
  fTime(time),
  fStationNb(stationNb),
  fStripNb(stripNb)
{
}
//-------------------------------------------------------------------------------------------------
ERQTelescopeSiDigi::ERQTelescopeSiDigi(const ERQTelescopeSiDigi& right)
  :fID(right.fID),
  fEdep(right.fEdep),
  fTime(right.fTime),
  fStationNb(right.fStationNb),
  fStripNb(right.fStripNb)
{
}
//-------------------------------------------------------------------------------------------------
ERQTelescopeSiDigi::~ERQTelescopeSiDigi(){
}
//-------------------------------------------------------------------------------------------------
void ERQTelescopeSiDigi::Print(const Option_t* opt /* = 0 */) const {
  LOG(DEBUG) << "-I- ERQTelescopeSiDigi:  " << FairLogger::endl;
  LOG(DEBUG) << "    Edep : " << fEdep << FairLogger::endl;
}
//-------------------------------------------------------------------------------------------------
ClassImp(ERQTelescopeSiDigi)
