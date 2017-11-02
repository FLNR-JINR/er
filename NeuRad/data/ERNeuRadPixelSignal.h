// -------------------------------------------------------------------------
// -----                  ERNeuRadPixelSignal header file              -----
// -----                  Created 02/03/15  by v. Schetinin            -----
// -------------------------------------------------------------------------

#ifndef ERNeuRadPixelSignal_H
#define ERNeuRadPixelSignal_H

#include "TArrayF.h"
#include "TArrayI.h"

#include "FairMultiLinkedData.h"

class ERNeuRadPhotoElectron;

class ERNeuRadPixelSignal : public FairMultiLinkedData {

protected:

    Int_t fPixelNb;
    Int_t fModuleNb;
    Int_t fSide;
    //Массив амплитуд фотоэлектронов сигнала
    TArrayF fPEAmplitudes;     
    //Суммарная амплитуда фотоэлектронов сигнала
    Double_t fPEAmplitudesSum;
    //Массив времен прихоа на анод фотоэлектронов сигнала
    TArrayF fPEAnodeTimes;
    //Массив длин одноэлектронных сигналов
    TArrayI fPETimes;     //!
    //Количество фотоэлектронов в сигнале
    Int_t fPECount;

    Int_t fCurFPoint;

    Double_t fStartTime;
    Double_t fFinishTime;

    //Используется для работы с общем сигналом. Так как была необходима адресная арифметика
    Float_t* fResFunction;    //!
    //Хранит результирующий сигнал, как значения функции в узлах
    TArrayF fResFunctionRoot; 

    Double_t OnePEFunction(Double_t time, Double_t amplitude);
    Int_t OnePETime(Double_t amplitude);

public:

    /** Default constructor **/
    ERNeuRadPixelSignal();

    /** Constructor with arguments **/
    ERNeuRadPixelSignal(Int_t iModule, Int_t iPixel, Int_t fpoints_count, Int_t side);

    /** Destructor **/
    virtual ~ERNeuRadPixelSignal();

    virtual void AddPhotoElectron(ERNeuRadPhotoElectron* fpoint);

    virtual void Generate();

    virtual bool Exist() {return fCurFPoint > 0;}

    TArrayF* ResultSignal() {return &fResFunctionRoot;}

    //пока заглушки
    virtual std::vector<Double_t> Intersections(Double_t discriminatorThreshold);

    virtual Double_t MaxInteg(const Double_t window, const Double_t dt) {return -1.;}
    virtual Double_t Integ(const Double_t start,const Double_t finish);
    virtual Double_t FirstInteg(const Double_t window);
    virtual Double_t FullInteg() {return Integ(fStartTime,fFinishTime);}
    virtual Double_t Mean(const Double_t time) {return -1.;}

    virtual Float_t ThresholdTime(Float_t peThreshold);

    virtual Double_t OnePEIntegral();

    /** Getters **/

    //TODO merge these two blocks?!
    Double_t GetStartTime() const {return fStartTime;}
    Double_t GetFinishTime() const {return fFinishTime;}
    Int_t GetPECount() const {return fPECount;}

    virtual Double_t StartTime() const {return fStartTime;} 
    virtual Double_t FinishTime() const {return fFinishTime;}
    virtual Int_t PECount() const {return fPECount;}


    Double_t AmplitudesSum() const {return fPEAmplitudesSum;}
    Float_t GetPEamp(Int_t i) const {return fPEAmplitudes[i];}
    Float_t GetPEtime(Int_t i) const {return fPEAnodeTimes[i];}
    Double_t dT() const {return cdT;}
    Int_t ModuleNb() const {return fModuleNb;}
    Int_t PixelNb() const {return fPixelNb;}
    Int_t Side() const {return fSide;}

/** Data members **/

    //constants
    // Гранулирование сигнала по времени
    static const Double_t cdT; // ns

    ClassDef(ERNeuRadPixelSignal,1)
};

#endif // ERNeuRadPixelSignal_H
