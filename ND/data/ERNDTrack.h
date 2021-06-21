/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef ERNDTrack_H
#define ERNDTrack_H

#include "TVector3.h"

class ERNDTrack : public TObject {
  public:
    ERNDTrack() = default;
    ERNDTrack(const TVector3& detectorVertex, const TVector3& targetVertex,
              float edep, float time, float tac);
    /** Accessors **/
    TVector3 DetectorVertex() const {return fDetectorVertex;}
    TVector3 TargetVertex() const {return fTargetVertex;}
    TVector3 Direction() const;
    float Edep() const {return fEdep;}
    float Time() const {return fTime;}
    float TAC() const {return fTAC;}
  protected:
    TVector3 fTargetVertex;
    TVector3 fDetectorVertex;
    float fEdep = -1.;
    float fTime = -1.;
    float fTAC = -1.;
    ClassDef(ERNDTrack,1)
};

#endif