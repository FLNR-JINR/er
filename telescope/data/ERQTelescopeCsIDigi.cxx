/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERQTelescopeCsIDigi.h"

#include "FairLogger.h"

using namespace std;

//-------------------------------------------------------------------------------------------------
ERQTelescopeCsIDigi::ERQTelescopeCsIDigi()
  :fID(-1),
  fEdep(0.)
{
}
//-------------------------------------------------------------------------------------------------
ERQTelescopeCsIDigi::ERQTelescopeCsIDigi(Int_t id, Float_t Edep, Double_t time, Int_t wallNb, Int_t blockNb)
  :fID(id),
  fEdep(Edep),
  fTime(time),
  fWallNb(wallNb),
  fBlockNb(blockNb)
{
}
//-------------------------------------------------------------------------------------------------
ERQTelescopeCsIDigi::ERQTelescopeCsIDigi(const ERQTelescopeCsIDigi& right)
  :fID(right.fID),
  fEdep(right.fEdep),
  fTime(right.fTime),
  fWallNb(right.fWallNb),
  fBlockNb(right.fBlockNb)
{
}
//-------------------------------------------------------------------------------------------------
ERQTelescopeCsIDigi::~ERQTelescopeCsIDigi(){
}
//-------------------------------------------------------------------------------------------------
void ERQTelescopeCsIDigi::Print(const Option_t* opt /* = 0 */) const {
  LOG(DEBUG) << "-I- ERQTelescopeCsIDigi:  " << FairLogger::endl;
  LOG(DEBUG) << "    Edep : " << fEdep << FairLogger::endl;
}
//-------------------------------------------------------------------------------------------------
ClassImp(ERQTelescopeCsIDigi)
