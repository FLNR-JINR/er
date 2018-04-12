/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "ERQTelescopeTrack.h"

ERQTelescopeTrack::ERQTelescopeTrack()
{
}
//--------------------------------------------------------------------------------------------------
ERQTelescopeTrack::ERQTelescopeTrack(Double_t targetX, Double_t targetY, Double_t targetZ, 
                    				         Double_t globalX, Double_t globalY, Double_t globalZ,
                                     Double_t  localX, Double_t  localY, Double_t  localZ,
                                     Double_t sumEdep)
: fTargetX(targetX),
  fTargetY(targetY),
  fTargetZ(targetZ),
  fTelescopeGlobalX(globalX),
  fTelescopeGlobalY(globalY),
  fTelescopeGlobalZ(globalZ),
  fTelescopeLocalX(localX),
  fTelescopeLocalY(localY),
  fTelescopeLocalZ(localZ),
  fSumEdep(sumEdep)
{
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERQTelescopeTrack)