// -------------------------------------------------------------------------
// -----                     ERNeuRadPixelSignal source file           -----
// -------------------------------------------------------------------------

#include "ERNeuRadPixelSignal.h"

#include "TMath.h"

#include <iostream>

const Double_t ERNeuRadPixelSignal::cdT = 0.1; // ns

ERNeuRadPixelSignal::ERNeuRadPixelSignal():
    fModuleNb(-1),
    fPixelNb(-1),
    fPECount(0),
    fCurFPoint(0),
    fStartTime(99999999.),
    fFinishTime(-1.),
    fPEAmplitudesSum(0)
{
}

ERNeuRadPixelSignal::ERNeuRadPixelSignal(Int_t iModule, Int_t iPixel, Int_t pe_count, Int_t side):
    fModuleNb(iModule),
    fPixelNb(iPixel),
    fPECount(pe_count),
    fCurFPoint(0),
    fStartTime(99999999.),
    fFinishTime(-1.),
    fSide(side),
    fPEAmplitudesSum(0)
{
    fPEAmplitudes.Set(pe_count);
    fPEAnodeTimes.Set(pe_count);
    fPETimes.Set(pe_count);
}

ERNeuRadPixelSignal::~ERNeuRadPixelSignal()
{
}

void ERNeuRadPixelSignal::AddPhotoElectron(ERNeuRadPhotoElectron* pe)
{
    // Добавляет фотоэлектрон к сигналу на PMT
    fPEAmplitudes[fCurFPoint] = pe->Amplitude();
    fPEAnodeTimes[fCurFPoint] = pe->AnodeTime();
    // Вычисление длины фотоэлектронного сигнала в количестве cdT
    fPETimes[fCurFPoint] = 40.;//OnePETime(pe->Amplitude());
    // Сдвиг начального и коненого времени общего сигнала
    if (pe->AnodeTime() < fStartTime) {
        fStartTime = pe->AnodeTime();
    }

    if (pe->AnodeTime()+fPETimes[fCurFPoint]*cdT > fFinishTime) {
        fFinishTime = pe->AnodeTime()+fPETimes[fCurFPoint]*cdT;
    }

    fPEAmplitudesSum+=pe->Amplitude();

    fCurFPoint++;
}

void ERNeuRadPixelSignal::Generate()
{
    // Добавление к общему сигналу
    // Количество узлов во внешнем сигнале
    Int_t gdTCount = (fFinishTime - fStartTime)/cdT+1;
    fResFunction = new Float_t[gdTCount];

    for (Int_t i = 0; i < gdTCount; i++) {
        fResFunction[i] = 0.;
    }

    for (Int_t iPE = 0; iPE < fPECount; iPE++) {
        // Вычисление функции сигнала в узлах сигнала
        Float_t *sig_func = new Float_t[fPETimes[iPE]];
        for (Int_t idT = 0; idT < fPETimes[iPE]; idT++) {
            sig_func[idT] = OnePEFunction(idT*cdT, fPEAmplitudes[iPE]);
        }

        // Сдвиг относительно общего сигнала 
        //@bug Это выражение вносит небольшую погрешность в расчёт стартового времени
        Int_t shift = (Int_t) ((fPEAnodeTimes[iPE]-fStartTime)/cdT);
        // Указатель на узел общего сигнала, который соответствует началу сигнала одноэлектрона
        Float_t *cur_f = fResFunction + shift;
        // Добавление к общему сигналу 
        for (Int_t idT = 0; idT < fPETimes[iPE]; idT++) {
            cur_f[idT] += sig_func[idT];
        }
        delete [] sig_func;
    }
    fResFunctionRoot.Adopt(gdTCount,fResFunction);
}

Double_t ERNeuRadPixelSignal::OnePEFunction(Double_t time, Double_t amplitude){
	//Аналитическая функция одноэлектронного сигнала
	//return 8.*amplitude*time*time*TMath::Exp(-time/0.45);
	if(time>0) return 8.*amplitude*pow((time+0.22),5)*TMath::Exp(-pow((time+0.22),1.5)/0.4);
	else return 0;
}

// Вычисляем время сигнала одноэлектрона в количестве cdT.
Int_t ERNeuRadPixelSignal::OnePETime(Double_t amplitude)
{
    Int_t counts = 0;
    // Насчитываем отрезк времени пока сигнал имеет существенное для нас значение.
    Bool_t find = kFALSE;
    Double_t threshold = 0.001;
    while(1) {
        Double_t val = OnePEFunction((counts++)*cdT, amplitude);
        if (!find && val>threshold) {
            // Сигнал перевалил порог в первый раз
            find = kTRUE;
        }
        if (find && val<threshold) {
            break;
        }
        if (counts > 100) {
            break;
        }
    }
    return counts-1;
}

// Возвращает точки пересечения с сигналом порога discriminatorThreshold
std::vector<Double_t> ERNeuRadPixelSignal::Intersections(Double_t discriminatorThreshold)
{
    std::vector<Double_t> intersections;

    Bool_t startIntersect = kFALSE;
    for (Int_t i = 0; i < fResFunctionRoot.GetSize(); i++) {
        if (!startIntersect && fResFunctionRoot[i] > discriminatorThreshold) {
            intersections.push_back(fStartTime + cdT*i);
            startIntersect = kTRUE;
        }
        if (startIntersect && fResFunctionRoot[i] < discriminatorThreshold) {
            intersections.push_back(fStartTime + cdT*i);
            startIntersect = kFALSE;
        }
    }

    return intersections;
} 

// Вычисления интеграла сигнала методом трапеций
Double_t ERNeuRadPixelSignal::Integ(const Double_t start,const Double_t finish)
{
    if (finish<fStartTime) {
        return 0;
    }

    Double_t res = 0;
    // Начальная и конечная точки
    Int_t st = (Int_t)((start-fStartTime)/cdT) + 1;
    Int_t fn = (Int_t)((finish - fStartTime)/cdT);

    // Корректируем конечное время на последний узел сигнала
    if ((fStartTime+fn*cdT) > fFinishTime) {
        fn = (Int_t)(fFinishTime- fStartTime)/cdT;
    }

    // Суммируем трапеции внутри промежутка
    for(Int_t i = st; i < fResFunctionRoot.GetSize()-1; i++) {
        res += 0.5*(fResFunctionRoot[i] + fResFunctionRoot[i+1])*cdT;
    }

    return res;
}

// Возвращает интеграл сигнала в окне window, начиная с начального узла сигнала
Double_t ERNeuRadPixelSignal::FirstInteg(const Double_t window)
{
    if ((fStartTime + window) > fFinishTime ) {
        return Integ(fStartTime, fFinishTime);
    } else {
        return Integ(fStartTime, fStartTime+window);
    }
}

// Возвращает время пересечения сигнала порога peThreshold [количество фотоэлектронов]
Float_t ERNeuRadPixelSignal::ThresholdTime(Float_t peThreshold)
{
    Int_t i = 0;
    while((fResFunctionRoot[i] < peThreshold*OnePEIntegral()) && i < (fResFunctionRoot.GetSize()-1)) {
        i++;
    }

    Int_t prevI = i-1;
    Int_t lastI = i;
    Float_t prevT = fStartTime + prevI*cdT;
    Float_t lastT = fStartTime + lastI*cdT;
    if (i==fResFunctionRoot.GetSize()-1) {
        return -1;
    }
    return (peThreshold*OnePEIntegral()-fResFunctionRoot[prevI])/(fResFunctionRoot[lastI]-fResFunctionRoot[prevI])*(lastT-prevT)+prevT;
}

Double_t ERNeuRadPixelSignal::OnePEIntegral()
{
    Double_t res = 0.;
    for (Int_t i=0; i<39; i++) {
        res += 0.5*(OnePEFunction(i*cdT,8.) + OnePEFunction((i+1)*cdT,8.))*cdT;
    }
    return res;
}

ClassImp(ERNeuRadPixelSignal)
