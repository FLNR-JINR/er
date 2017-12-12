/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

// -------------------------------------------------------------------------
// -----                  ERNeuRadPhotoElectron header file            -----
// -----                  Created 02/03/15  by v. Schetinin            -----
// -------------------------------------------------------------------------

#ifndef ERNeuRadPhotoElectron_H
#define ERNeuRadPhotoElectron_H

#include "FairMultiLinkedData.h" // mother class

class ERNeuRadPhotoElectron : public FairMultiLinkedData {

private:

    Int_t fIndex;
    Int_t fSide; // 0 - front, 1 - back
    Double_t fLYTime;
    Double_t fCathodeTime;
    Double_t fAnodeTime;
    Int_t fPhotonCount;
    Double_t fAmplitude;

public:

    /** Default constructor **/
    ERNeuRadPhotoElectron();

    /** Standard constructor **/
    ERNeuRadPhotoElectron(Int_t index,
                          Int_t side,
                          Double_t lyTime,
                          Double_t cathode_time,
                          Double_t anode_time,
                          Int_t photon_count, 
                          Double_t amplitude);

    /** Destructor **/
    ~ERNeuRadPhotoElectron();

    /** Accessors **/
    Int_t     Index()       const { return fIndex; }
    Int_t     Side()        const { return fSide; }
    Double_t  LYTime()      const { return fLYTime; }
    Double_t  CathodeTime() const { return fCathodeTime; }
    Double_t  AnodeTime()   const { return fAnodeTime; }
    Int_t     PhotonCount() const { return fPhotonCount; }
    Double_t  Amplitude()   const { return fAmplitude; }

    void Print() const;

    ClassDef(ERNeuRadPhotoElectron, 1);
};

#endif // ERNeuRadPhotoElectron_H
