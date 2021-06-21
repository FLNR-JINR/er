/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#ifndef ERNDTrackFinder_H
#define ERNDTrackFinder_H

#include "TClonesArray.h"
#include "TVector3.h"

#include "ERTask.h"
#include "ERNDTrack.h"
#include "ERNDSetup.h"

class ERNDTrackFinder : public ERTask {
  public:
    ERNDTrackFinder();
    virtual InitStatus Init(); 
    virtual void Exec(Option_t* opt);
    virtual void Reset();
    void SetTargetVertex(const TVector3& vertex);
  protected:
    //Input arrays
    TClonesArray* fNDDigis = nullptr;
    TClonesArray* fBeamDetTrack = nullptr; 
    //Output arrays
    TClonesArray *fNDTracks = nullptr;
    ERNDSetup* fSetup = nullptr;
    TVector3 fTargetVertex;
    bool fUserTargetVertexIsSet = false;
    ERNDTrack* AddTrack(const TVector3& detectorVertex, const TVector3& targetVertex,
                        float edep, float time, float tac);
    ClassDef(ERNDTrackFinder,1)
};

#endif