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
ERQTelescopeCsIDigi::ERQTelescopeCsIDigi(Double_t edep, Double_t time, Int_t channel)
  : ERDigi(edep, time, channel)
{
}
//-------------------------------------------------------------------------------------------------
void ERQTelescopeCsIDigi::Print(const Option_t* opt /* = 0 */) const {
  LOG(INFO) << "-I- ERQTelescopeCsIDigi: edep = " << fEdep << ", time = " << fTime
            << ", channel = " << fChannel << FairLogger::endl;
}
//-------------------------------------------------------------------------------------------------
ClassImp(ERQTelescopeCsIDigi)
