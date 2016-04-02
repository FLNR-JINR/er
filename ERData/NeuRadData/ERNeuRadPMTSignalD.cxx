// -------------------------------------------------------------------------
// -----                      ERNeuRadPMTSignalD source file           -----
// -------------------------------------------------------------------------
#include "ERNeuRadPMTSignalD.h"

#include "TMath.h"

#include <iostream>

using namespace std;

const Double_t ERNeuRadPMTSignalD::csdT = 0.1; //ns
const Int_t ERNeuRadPMTSignalD::csdTCount = 30; //count

ERNeuRadPMTSignalD::ERNeuRadPMTSignalD(){

}

ERNeuRadPMTSignalD::ERNeuRadPMTSignalD(Int_t iBundle, Int_t iFiber, Int_t fpoints_count):
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

ERNeuRadPMTSignalD::~ERNeuRadPMTSignalD(){

}

void ERNeuRadPMTSignalD::AddFiberPoint(ERNeuRadFiberPoint* fpoint){
	/*if (fFiberPoints.size() == 0){
		fFiberPoints.push_back(fpoint);
	}
	else{
		ERNeuRadFiberPoint* firstFPoint = (ERNeuRadFiberPoint*)fFiberPoints.front();
		if (firstFPoint->AnodeTime() < fpoint->AnodeTime()){
			fFiberPoints.push_back(fpoint);
		}
		else{
			fFiberPoints.push_front(fpoint);
		}
	}*/
	fAmplitudes[fCurFPoint]	= fpoint->Amplitude();
	fAnodeTimes[fCurFPoint++] = fpoint->AnodeTime();
	if (fpoint->AnodeTime() < fStartTime)
		fStartTime = fpoint->AnodeTime();
	if (fpoint->AnodeTime()+csdTCount*csdT > fFinishTime)
		fFinishTime = fpoint->AnodeTime()+csdTCount*csdT;
}


void ERNeuRadPMTSignalD::Generate(){
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
	for (int i =0; i < gdTCount; i++)
		cerr << fResFunction[i] << " ";
	cerr << endl;
	fResFunctionRoot.Adopt(gdTCount,fResFunction);
}

Double_t ERNeuRadPMTSignalD::Function(Double_t time){
	return 40.*time*TMath::Exp(-time/0.35);
}
/*
Double_t ERNeuRadPMTSignalD::GetStartTime(){
	if (this->Exist()){
		return fFiberPoints.front()->AnodeTime();
	}
	else{
		return -1.;
	}
}
*/

ClassImp(ERNeuRadPMTSignalD)