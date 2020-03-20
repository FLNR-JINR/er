/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERDigi.h"

#include "FairLogger.h"
//--------------------------------------------------------------------------------
ERDigi::ERDigi(Double_t edep, Double_t time, Int_t channel)
  : fEdep(edep), fTime(time), fChannel(channel)
{
}
//--------------------------------------------------------------------------------
void ERDigi::Print(const Option_t* opt /* = 0 */) const
{
  LOG(INFO) << "-I- ERDigi: edep =  " << fEdep << ", time = " << fTime 
            << ", channel = " << fChannel << FairLogger::endl;
}
//--------------------------------------------------------------------------------
ClassImp(ERDigi)