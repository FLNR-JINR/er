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
ERDigi::ERDigi(float edep, float time, unsigned short channel)
  : fEdep(edep), fTime(time), fChannel(channel)
{}
//--------------------------------------------------------------------------------
void ERDigi::Print() const {
  LOG(INFO) << "ERDigi: edep =  " << fEdep << ", time = " << fTime 
            << ", channel = " << fChannel << FairLogger::endl;
}
//--------------------------------------------------------------------------------
ClassImp(ERDigi)