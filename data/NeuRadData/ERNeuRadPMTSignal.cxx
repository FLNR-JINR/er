// -------------------------------------------------------------------------
// -----                      ERNeuRadPMTSignal source file           -----
// -------------------------------------------------------------------------
#include "ERNeuRadPMTSignal.h"
#include "TMath.h"
#include <iostream>

using namespace std;

const Double_t ERNeuRadPMTSignal::cdT = 0.1; //ns

ERNeuRadPMTSignal::ERNeuRadPMTSignal():
	fModuleIndex(-1),
	fFiberIndex(-1),
	fPECount(0),
	fPEAmplitudes(NULL),
	fPEAnodeTimes(NULL),
	fPETimes(NULL),
	fCurFPoint(0),
	fStartTime(99999999.),
	fFinishTime(-1.),
	fPEAmplitudesSum(0)
{
}

ERNeuRadPMTSignal::ERNeuRadPMTSignal(Int_t iModule, Int_t iFiber, Int_t pe_count, Int_t side):
	fModuleIndex(iModule),
	fFiberIndex(iFiber),
	fPECount(pe_count),
	fPEAmplitudes(new Double_t[pe_count]),
	fPEAnodeTimes(new Double_t[pe_count]),
	fPETimes(new Int_t[pe_count]),
	fCurFPoint(0),
	fStartTime(99999999.),
	fFinishTime(-1.),
	fSide(side),
	fPEAmplitudesSum(0)
{
}

ERNeuRadPMTSignal::~ERNeuRadPMTSignal(){
	delete [] fPEAmplitudes;
	delete [] fPEAnodeTimes;
	delete [] fPETimes;
}

void ERNeuRadPMTSignal::AddPhotoElectron(ERNeuRadPhotoElectron* pe){
	//Добавляет фотоэлектрон к сигналу на PMT
	fPEAmplitudes[fCurFPoint]	= pe->Amplitude();
	fPEAnodeTimes[fCurFPoint] = pe->AnodeTime();
	//Вычисление длины фотоэлектронного сигнала в количестве cdT
	fPETimes[fCurFPoint] = 40.;//OnePETime(pe->Amplitude());
	//Сдвиг начального и коненого времени общего сигнала
	if (pe->AnodeTime() < fStartTime)
		fStartTime = pe->AnodeTime();
	
	if (pe->AnodeTime()+fPETimes[fCurFPoint]*cdT > fFinishTime)
		fFinishTime = pe->AnodeTime()+fPETimes[fCurFPoint]*cdT;

	fPEAmplitudesSum+=pe->Amplitude();

	fCurFPoint++;
}


void ERNeuRadPMTSignal::Generate(){
	//добавление к общему сигналу
	//Количество узлов во внешнем сигнале
	Int_t gdTCount = (fFinishTime - fStartTime)/cdT;
	fResFunction = new Float_t[gdTCount];

	for (Int_t i = 0; i < gdTCount; i++)
		fResFunction[i] = 0.;

	for (Int_t iPE = 0; iPE < fPECount; iPE++){

		//вычисление функции сигнала в узлах сигнала
		Float_t *sig_func = new Float_t[fPETimes[iPE]];
		for (Int_t idT = 0; idT < fPETimes[iPE]; idT++){
			sig_func[idT] = OnePEFunction(idT*cdT, fPEAmplitudes[iPE]);
		}

		//Сдвиг относительно общего сигнала 
		//@bug Это выражение вносит небольшую погрешность в расчёт стартового времени
		Int_t shift = (Int_t) ((fPEAnodeTimes[iPE]-fStartTime)/cdT);
		//Указатель на узел общего сигнала, который соответствует началу сигнала одноэлектрона
		Float_t *cur_f = fResFunction + shift;
		//Добавление к общему сигналу 
		for (Int_t idT = 0; idT < fPETimes[iPE]; idT++){
			cur_f[idT] += sig_func[idT];
		}
		delete [] sig_func;
	}
	fResFunctionRoot.Adopt(gdTCount,fResFunction);
}

Double_t ERNeuRadPMTSignal::OnePEFunction(Double_t time, Double_t amplitude){
	//Аналитическая функция одноэлектронного сигнала
	return 8.*amplitude*time*TMath::Exp(-time/0.35);
}

Int_t ERNeuRadPMTSignal::OnePETime(Double_t amplitude){
	//Вычисляем время сигнала одноэлектрона в количестве cdT.
	Int_t counts = 0;
	//Насчитываем отрезк времени пока сигнал имеет существенное для нас значение.
	Bool_t find = kFALSE;
	Double_t threshold = 0.001;
	while(1){
		Double_t val = OnePEFunction((counts++)*cdT, amplitude);
		if (!find && val>threshold){ //Сигнал перевалил порог в первый раз
			find = kTRUE;
		}
		if (find && val<threshold){
			break;
		}
		if (counts > 100)
			break;
	}
	return counts-1;
}

std::vector<Double_t> ERNeuRadPMTSignal::Intersections(Double_t discriminatorThreshold)
{
	//Возвращает точки пересечения с сигналом порога discriminatorThreshold
	std::vector<Double_t> intersections;
	
    Bool_t startIntersect = kFALSE;
	for (Int_t i = 0; i < fResFunctionRoot.GetSize(); i++){
		if (!startIntersect && fResFunctionRoot[i] > discriminatorThreshold){
			intersections.push_back(fStartTime + cdT*i);
			startIntersect = kTRUE;
		}
		if (startIntersect && fResFunctionRoot[i] < discriminatorThreshold){
			intersections.push_back(fStartTime + cdT*i);
			startIntersect = kFALSE;
		}
	}

	return intersections;
} 

 Double_t ERNeuRadPMTSignal::Integ(const Double_t start,const Double_t finish){
  //Вычисления интеграла сигнала методом трапеций
  if (finish<fStartTime)
    return 0;
  
  Double_t res = 0;
  //Начальная и конечная точки
  Int_t st = (Int_t)((start-fStartTime)/cdT) + 1;
  Int_t fn = (Int_t)((finish - fStartTime)/cdT);

  //Корректируем конечное время на последний узел сигнала
  if ((fStartTime+fn*cdT) > fFinishTime)
  	fn = (Int_t)(fFinishTime- fStartTime)/cdT;

  //Суммируем трапеции внутри промежутка
  for(Int_t i = st; i < fResFunctionRoot.GetSize()-1; i++){
    res += 0.5*(fResFunctionRoot[i] + fResFunctionRoot[i+1])*cdT;
  }
  
  return res;
 }

 Double_t ERNeuRadPMTSignal::FirstInteg(const Double_t window){
 	//Возвращает интеграл сигнала в окне window, начиная с начального узла сигнала
 	if ((fStartTime + window) > fFinishTime )
 	  return Integ(fStartTime, fFinishTime);
 	else
 		return Integ(fStartTime, fStartTime+window);
 }

 Float_t ERNeuRadPMTSignal::ThresholdTime(Float_t peThreshold){
 	//Возвращает время пересечения сигнала порога peThreshold [количество фотоэлектронов]
 	Int_t i = 0;
 	while((fResFunctionRoot[i] < peThreshold*OnePEIntegral()) && i < (fResFunctionRoot.GetSize()-1)){
 		i++;
 	}
 	Int_t prevI = i-1;
 	Int_t lastI = i;
 	Float_t prevT = fStartTime + prevI*cdT;
 	Float_t lastT = fStartTime + lastI*cdT;
 	if (i==fResFunctionRoot.GetSize()-1)
 		return -1;
 	return (peThreshold*OnePEIntegral()-fResFunctionRoot[prevI])/(fResFunctionRoot[lastI]-fResFunctionRoot[prevI])*(lastT-prevT)+prevT;
 }

Double_t ERNeuRadPMTSignal::OnePEIntegral(){
	Double_t res = 0.;
	for (Int_t i=0; i<39; i++){
		res += 0.5*(OnePEFunction(i*cdT,8.) + OnePEFunction((i+1)*cdT,8.))*cdT;
	}
	return res;
}

ClassImp(ERNeuRadPMTSignal)