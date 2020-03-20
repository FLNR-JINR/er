/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#include "ERTrack.h"

#include "FairLogger.h"

ERTrack::ERTrack(const TVector3& detectorVertex, const TVector3& targetVertex, Double_t edep)
    : fTargetVertex(targetVertex), fDetectorVertex(detectorVertex), fEdep(edep) 
{
}

void ERTrack::Print(const Option_t* opt /*= 0*/) const {
    LOG(INFO) << "-I- ERDigi: detector position  = (" << fDetectorVertex.X() << ", " 
              << fDetectorVertex.Y() << ", " << fDetectorVertex.Z() 
              << "), target position = (" << fTargetVertex.X() << ", " << fTargetVertex.Y()
              << ", " << fTargetVertex.Z() << ")" << FairLogger::endl;
}

ClassImp(ERTrack)