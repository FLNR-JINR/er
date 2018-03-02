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
ERQTelescopeTrack::ERQTelescopeTrack(Double_t targetX,   Double_t targetY,   Double_t targetZ, 
                    				 Double_t telescopeX, Double_t telescopeY, Double_t telescopeZ)
: fTargetX(targetX),
  fTargetY(targetY),
  fTargetZ(targetZ),
  fTelescopeX(telescopeX),
  fTelescopeY(telescopeY),
  fTelescopeZ(telescopeZ)
{
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERQTelescopeTrack)