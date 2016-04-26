// -------------------------------------------------------------------------
// -----                      ERNeuRadPMTSignalF source file           -----
// -------------------------------------------------------------------------
#include "ERNeuRadPMTSignalF.h"

#include "TMath.h"

#include <iostream>

using namespace std;

const Double_t ERNeuRadPMTSignalF::csdT = 0.1; //ns
const Int_t ERNeuRadPMTSignalF::csdTCount = 30; //count

ERNeuRadPMTSignalF::ERNeuRadPMTSignalF(){

}

ERNeuRadPMTSignalF::ERNeuRadPMTSignalF(Int_t iBundle, Int_t iFiber, Int_t fpoints_count):
	ERNeuRadPMTSignal(iBundle, iFiber)
{
	fAmplitudes = new Double_t[fpoints_count];
    fAnodeTimes = new Double_t[fpoints_count];
    fTimeShifts = new Int_t[fpoints_count];
    fCurFPoint = 0;
    fStartTime = 99999999.;
    fFinishTime = -1.;
    fFPointsCount = fpoints_count;
}

ERNeuRadPMTSignalF::~ERNeuRadPMTSignalF(){

}

void ERNeuRadPMTSignalF::AddFiberPoint(ERNeuRadFiberPoint* fpoint){
	fAmplitudes[fCurFPoint]	= fpoint->Amplitude();
	fAnodeTimes[fCurFPoint++] = fpoint->AnodeTime();
	if (fpoint->AnodeTime() < fStartTime)
		fStartTime = fpoint->AnodeTime();
	if (fpoint->AnodeTime()+csdTCount*csdT > fFinishTime)
		fFinishTime = fpoint->AnodeTime()+csdTCount*csdT;
}


void ERNeuRadPMTSignalF::Generate(){
	//Поиск сдвигов относительно начала сигнала
	for(Int_t ifpoint = 0; ifpoint < fFPointsCount; ifpoint++){
		fTimeShifts[ifpoint] = (Int_t) ((fAnodeTimes[ifpoint]-fStartTime)/csdT);
	}
	//вычисление функции сигнала
	Float_t *sig_func = new Float_t[csdTCount];
	for (Int_t idT = 0; idT < csdTCount; idT++){
		sig_func[idT] = Function(idT*csdT);
	}
	//добавление к общему сигналу
	Int_t gdTCount = (fFinishTime - fStartTime)/csdT;
	fResFunction = new Float_t[gdTCount];
	for (Int_t i = 0; i < gdTCount; i++)
		fResFunction[i] = 0.;
	for (Int_t ifpoint = 0; ifpoint < fFPointsCount; ifpoint++){
		Float_t *cur_f = fResFunction + fTimeShifts[ifpoint]; 
		for (Int_t idT = 0; idT < csdTCount; idT++){
			cur_f[idT] += sig_func[idT];
		}
	}
	fResFunctionRoot.Adopt(gdTCount,fResFunction);
}

Double_t ERNeuRadPMTSignalF::Function(Double_t time){
	return 40.*time*TMath::Exp(-time/0.35);
}

ClassImp(ERNeuRadPMTSignalF)