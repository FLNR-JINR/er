/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "ERQTelescopeTrack.h"
//--------------------------------------------------------------------------------------------------
ERQTelescopeTrack::ERQTelescopeTrack(const TVector3& targetVertex, const TVector3& xStationVertex, 
                                     const TVector3& yStationVertex, const TVector3& xStationLocalVertex,
                                     const TVector3& yStationLocalVertex, const Int_t xChannel, const Int_t yChannel,
                                     const Double_t xEdep, const Double_t yEdep)
: fTargetVertex(targetVertex), fXStationVertex(xStationVertex), fYStationVertex(yStationVertex),
  fXStationLocalVertex(xStationLocalVertex), fYStationLocalVertex(yStationLocalVertex),
  fXChannel(xChannel), fYChannel(yChannel), fXEdep(xEdep), fYEdep(yEdep)
{
}
//--------------------------------------------------------------------------------------------------
TVector3 ERQTelescopeTrack::GetDirection() const {
    auto direction = fXStationVertex - fTargetVertex;
    direction.SetMag(1.);
    return direction;
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERQTelescopeTrack)