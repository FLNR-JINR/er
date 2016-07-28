// -------------------------------------------------------------------------
// -----                      ERNeuRadPMTSignal source file           -----
// -------------------------------------------------------------------------
#include "ERNeuRadPMTSignal.h"
#include "TMath.h"
#include <iostream>

using namespace std;

const Double_t ERNeuRadPMTSignal::csdT = 0.1; //ns
const Int_t ERNeuRadPMTSignal::csdTCount = 30; //count

ERNeuRadPMTSignal::ERNeuRadPMTSignal():
	fModuleIndex(-1),
	fFiberIndex(-1),
	fPECount(0),
	fAmplitudes(NULL),
	fAnodeTimes(NULL),
	fTimeShifts(NULL),
	fCurFPoint(0),
	fStartTime(99999999.),
	fFinishTime(-1.)
{
}

ERNeuRadPMTSignal::ERNeuRadPMTSignal(Int_t iModule, Int_t iFiber, Int_t pe_count):
	fModuleIndex(iModule),
	fFiberIndex(iFiber),
	fPECount(pe_count),
	fAmplitudes(new Double_t[pe_count]),
	fAnodeTimes(new Double_t[pe_count]),
	fTimeShifts(new Int_t[pe_count]),
	fCurFPoint(0),
	fStartTime(99999999.),
	fFinishTime(-1.)
{
}

ERNeuRadPMTSignal::~ERNeuRadPMTSignal(){
	//delete [] fResFunction;
	delete [] fAmplitudes;
	delete [] fAnodeTimes;
	delete [] fTimeShifts;
}

void ERNeuRadPMTSignal::AddFiberPoint(ERNeuRadFiberPoint* fpoint){
	fAmplitudes[fCurFPoint]	= fpoint->Amplitude();
	fAnodeTimes[fCurFPoint++] = fpoint->AnodeTime();
	if (fpoint->AnodeTime() < fStartTime)
		fStartTime = fpoint->AnodeTime();
	if (fpoint->AnodeTime()+csdTCount*csdT > fFinishTime)
		fFinishTime = fpoint->AnodeTime()+csdTCount*csdT;
}


void ERNeuRadPMTSignal::Generate(){
	//Поиск сдвигов относительно начала сигнала
	for(Int_t ifpoint = 0; ifpoint < fPECount; ifpoint++){
		fTimeShifts[ifpoint] = (Int_t) ((fAnodeTimes[ifpoint]-fStartTime)/csdT);
	}
	//добавление к общему сигналу
	Int_t gdTCount = (fFinishTime - fStartTime)/csdT;
	fResFunction = new Float_t[gdTCount];
	for (Int_t i = 0; i < gdTCount; i++)
		fResFunction[i] = 0.;
	for (Int_t ifpoint = 0; ifpoint < fPECount; ifpoint++){
		//вычисление функции сигнала
		Float_t *sig_func = new Float_t[csdTCount];
		for (Int_t idT = 0; idT < csdTCount; idT++){
			sig_func[idT] = Function(idT*csdT, fAmplitudes[ifpoint]);
		}
		Float_t *cur_f = fResFunction + fTimeShifts[ifpoint]; 
		for (Int_t idT = 0; idT < csdTCount; idT++){
			cur_f[idT] += sig_func[idT];
		}
		delete [] sig_func;
	}
	fResFunctionRoot.Adopt(gdTCount,fResFunction);
}

Double_t ERNeuRadPMTSignal::Function(Double_t time, Double_t amplitude){
	return 8.*amplitude*time*TMath::Exp(-time/0.35);
}

std::vector<Double_t> ERNeuRadPMTSignal::GetIntersections(Double_t discriminatorThreshold)
{
	std::vector<Double_t> intersections;
	
    Bool_t startIntersect = kFALSE;
	for (Int_t i = 0; i < fResFunctionRoot.GetSize(); i++){
		if (!startIntersect && fResFunctionRoot[i] > discriminatorThreshold){
			intersections.push_back(fStartTime + csdT*i);
			startIntersect = kTRUE;
		}
		if (startIntersect && fResFunctionRoot[i] < discriminatorThreshold){
			intersections.push_back(fStartTime + csdT*i);
			startIntersect = kFALSE;
		}
	}

	return intersections;
} 

 Double_t ERNeuRadPMTSignal::GetInteg(const Double_t start,const Double_t finish){
  if (finish<fStartTime)
    return 0;

  Double_t res = 0;
  //Начальная и конечная точкиж
  Int_t st = (Int_t)((start-fStartTime)/csdT) + 1;
  Int_t fn = (Int_t)((finish - fStartTime)/csdT);
  
  if ((fStartTime+fn*csdT) > fFinishTime)
  	fn = (Int_t)(fFinishTime- fStartTime)/csdT;
  //Суммируем трапеции внутри промежутка
  for(Int_t i = st; i < fn-1; i++){
    res += 0.5*(fResFunctionRoot[i] + fResFunctionRoot[i+1])*(finish-start);
  }
  
  return res;
 }

 Double_t ERNeuRadPMTSignal::GetFirstInteg(const Double_t window){
 	if ((fStartTime + window) > fFinishTime )
 	  return GetInteg(fStartTime, fFinishTime);
 	else
 		return GetInteg(fStartTime, fStartTime+window);
 }

 Float_t ERNeuRadPMTSignal::GetThresholdTime(Float_t peThreshold){
 	Int_t i = 0;
 	while((fResFunctionRoot[i] < peThreshold*OnePEIntegral()) && i < (fResFunctionRoot.GetSize()-1)){
 		i++;
 	}
 	Int_t prevI = i-1;
 	Int_t lastI = i;
 	Float_t prevT = fStartTime + prevI*csdT;
 	Float_t lastT = fStartTime + lastI*csdT;
 	if (i==fResFunctionRoot.GetSize()-1)
 		return -1;
 	return (peThreshold*OnePEIntegral()-fResFunctionRoot[prevI])/(fResFunctionRoot[lastI]-fResFunctionRoot[prevI])*(lastT-prevT)+prevT;
 }

ClassImp(ERNeuRadPMTSignal)