/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "ERNDTrack.h"

#include "FairLogger.h"

ERNDTrack::ERNDTrack(const TVector3& detectorVertex, const TVector3& targetVertex,
                     float edep, float time, float tac)
    : fTargetVertex(targetVertex)
    , fDetectorVertex(detectorVertex)
    , fEdep(edep)
    , fTime(time)
    , fTAC(tac)
{
}

TVector3 ERNDTrack::Direction() const {
	TVector3 direction = fDetectorVertex - fTargetVertex;
	direction.SetMag(1.);
	return direction;
}

ClassImp(ERNDTrack)