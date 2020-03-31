/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef ERTrack_H
#define ERTrack_H

#include "TVector3.h"

#include "FairMultiLinkedData.h"

class ERTrack : public FairMultiLinkedData {
  public:
    ERTrack() = default;
    ERTrack(const TVector3& detectorVertex, const TVector3& targetVertex, Double_t edep);
    virtual void Print(const Option_t* opt = 0) const;
    /** Accessors **/
    TVector3 DetectorVertex() const {return fDetectorVertex;}
    TVector3 TargetVertex() const {return fTargetVertex;}
    Double_t Edep() const {return fEdep;}
  protected:
    TVector3 fTargetVertex;
    TVector3 fDetectorVertex;
    Double_t fEdep = -1.;

    ClassDef(ERTrack,1)
};

#endif