/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

// -------------------------------------------------------------------------
// -----                  ERNeuRadPixelSignal header file              -----
// -----                  Created 02/03/15  by v. Schetinin            -----
// -------------------------------------------------------------------------

#ifndef ERNeuRadPixelSignal_H
#define ERNeuRadPixelSignal_H

#include "FairMultiLinkedData.h" // mother class

#include "TArrayF.h"
#include "TArrayI.h"

#include <vector>

class ERNeuRadPhotoElectron;

class ERNeuRadPixelSignal : public FairMultiLinkedData
{
protected:

    Int_t fModuleNb;
    Int_t fPixelNb;
    Int_t fSide;

    Double_t fStartTime;
    Double_t fFinishTime;

    // Массив амплитуд фотоэлектронов сигнала
    TArrayF fPEAmplitudes;
    // Суммарная амплитуда фотоэлектронов сигнала
    Double_t fPEAmplitudesSum;
    // Массив времен прихода на анод фотоэлектронов сигнала
    TArrayF fPEAnodeTimes;
    // Массив длин одноэлектронных сигналов
    TArrayI fPETimes; //!
    // Количество фотоэлектронов в сигнале
    Int_t fPECount;

    //constants
    // Гранулирование сигнала по времени
    static const Double_t cdT; // ns

    Int_t fCurFPoint;

    // Используется для работы с общем сигналом. Так как была необходима адресная арифметика
    Float_t* fResFunction; //!
    // Хранит результирующий сигнал, как значения функции в узлах
    TArrayF fResFunctionRoot;

    Double_t OnePEFunction(Double_t time, Double_t amplitude);
    Int_t OnePETime(Double_t amplitude);

public:

    Int_t GetModuleNb() const { return fModuleNb; }
    Int_t GetPixelNb()  const { return fPixelNb; }
    Int_t GetSide()     const { return fSide; }
    Double_t GetStartTime()     const { return fStartTime; }
    Double_t GetFinishTime()    const { return fFinishTime; }
    Float_t GetPEamp(Int_t i)   const { return fPEAmplitudes[i]; }
    Double_t GetAmplitudesSum() const { return fPEAmplitudesSum; }
    Float_t GetPEtime(Int_t i)  const { return fPEAnodeTimes[i]; }
    // fPETimes getter missing
    Int_t GetPECount()  const { return fPECount; }
    Double_t GetdT()    const { return cdT; }

    /** Default constructor **/
    ERNeuRadPixelSignal();

    /** Constructor with arguments **/
    ERNeuRadPixelSignal(Int_t iPMT, Int_t iChannel, Int_t peCount, Int_t side);

    virtual ~ERNeuRadPixelSignal();

    virtual void AddPhotoElectron(ERNeuRadPhotoElectron* fpoint);

    virtual void Generate();

    virtual bool Exist() const {return fCurFPoint > 0;}

    TArrayF* ResultSignal() { return &fResFunctionRoot; }

    void Print() const;

    //пока заглушки
    virtual std::vector<Double_t> Intersections(Double_t discriminatorThreshold);

    virtual Double_t MaxInteg(const Double_t window, const Double_t dt) { return -1.; }
    virtual Double_t Integ(const Double_t start, const Double_t finish);
    virtual Double_t FirstInteg(const Double_t window);
    virtual Double_t FullInteg() { return Integ(fStartTime, fFinishTime); }
    virtual Double_t Mean(const Double_t time) { return -1.; } //TODO wat!?

    virtual Float_t ThresholdTime(Float_t peThreshold);

    virtual Double_t OnePEIntegral();

    ClassDef(ERNeuRadPixelSignal, 1);
};

#endif // ERNeuRadPixelSignal_H
