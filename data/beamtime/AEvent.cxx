/*
 * AEvent.cpp
 *
 *  Created on: Dec 28, 2016
 *      Author: daria
 */

#include "AEvent.h"

AEvent::AEvent() : fNPoints(1024) {	//fNPoints is number of points in one event, 1024 or 1000

	Init();
	Reset();

}

AEvent::AEvent(const Int_t npoints) : fNPoints(npoints) {

	Init();
	Reset();
}

AEvent::~AEvent() {
	// TODO Auto-generated destructor stub
	delete fGraphSignal;
	delete fGraphCFD;
	delete fInputEvent;
}


void AEvent::ProcessEvent() {

	if (fInputEvent == NULL) {
		Warning("AEvent::ProcessEvent", "Input event wasn't set. Function won't be processed.");
		return;
	}

	const Double_t *amp = fInputEvent->GetAmp();
	const Double_t *time = fInputEvent->GetTime();

	for(Int_t j = 0; j < fNPoints; j++) {
		fAmpPos[j] = amp[j]*(-1.);
		fTime[j] = time[j];
	}

	fZeroLevel = FindZeroLevel();
	for(Int_t j = 0; j < fNPoints; j++) {
		fAmpPos[j] = fAmpPos[j] - fZeroLevel;
	}

	SetMaxAmplitudes();

	SetGraphs();
	FindFrontProperties();
	SetLED();
//	SetGraphs();
	SetCFD();
	SetChargeCFD();
	SetChargeLED();

	return;

}

void AEvent::Reset() {

	for (Int_t i = 0; i < fNPoints; i++) {
		fAmpPos[i] = 0;
		fTime[i] = 0;
		fAmpCFD[i] = 0;
	}
	fTimeLED = -100.;
	fEdgeSlope=0.;
	fEdgeXi=0.;
	fEdgeSlope=-100.;
	fTime10=0.;
	fTime90=0.;
	fAmpMax = 0.;
	fTimeAmpMax = 0.;
	fTimeCFD = -100.;
	fZeroLevel = 0.;
	fChargeCFD = -10.;
	fChargeLED = -10.;
}

void AEvent::SetInputEvent(RawEvent** event) {

	if (event == 0) {
		Warning("AEvent::SetInputEvent", "Input event was set as 0.");
	}
	fInputEvent = *event;

}

void AEvent::Init() {

	fAmpPos.Set(fNPoints);
	fTime.Set(fNPoints);
	fAmpCFD.Set(fNPoints);

	fGraphSignal = new TGraph();
	fGraphCFD = new TGraph();
	fInputEvent = 0;

	fCFratio = 0.;
	fCFtimeDelay = 0.;

	fNoiseRangeMin = 0.;
	fNoiseRangeMax = 1.;

}

void AEvent::SetGraphs() {

	fGraphSignal->Set(fNPoints);

	for (Int_t i=0; i<fNPoints; i++) {
		fGraphSignal->SetPoint(i, fTime[i], fAmpPos[i]);
	}

	return;
}

void AEvent::SetCFD() {

	Double_t time = 0;
	Double_t mytime = fCFtimeDelay;
	fGraphCFD->Set(fNPoints);

	//working variables
	Double_t maxCFD = 0., minCFD = 0.;
	Double_t TmaxCFD = 0., TminCFD = 0.;	
	Double_t ampCFD;
	const Double_t timeStep = 0.1;
	Int_t i = 0; //for graph 
 
	//while goes by the graph with the step of timeStep
	while( mytime < (200. - fCFtimeDelay) ) {

		ampCFD = fGraphSignal->Eval(mytime)*fCFratio*(-1) + fGraphSignal->Eval(mytime - fCFtimeDelay);
		fGraphCFD->SetPoint(i, mytime, ampCFD);

		//point for max CFD amplitude
		if( ampCFD > maxCFD) {
			maxCFD = ampCFD;
			TmaxCFD = mytime;
		}

		//point for min CFD amplitude
		if( ampCFD < minCFD) {
			minCFD = ampCFD;
			TminCFD = mytime;
		}

		i++;
		mytime = mytime + timeStep;
	}

	//looking for the first point with the closest values to 0 and writing to fTimeCFD
	time = TminCFD;
	while( (fGraphCFD->Eval(time) <= 0) && (time <= TmaxCFD) /*&& (time >= TminCFD)*/ ) {
		fTimeCFD = time;
		time = time + timeStep;
	}

}

void AEvent::FindFrontProperties() {

	//in percents
	const Double_t minHeight = 0.1;
	const Double_t maxHeight = 0.9;

	const Double_t timeStep = 0.05;	//in ns
	Double_t time = 0;			//in ns

	if (!fGraphSignal) {
		Warning("AEvent::FindFrontProperties", "Graph was not set");
		return;
	}

	//TODO search of minimum should be done from the lower edge on the time axis

	while (fGraphSignal->Eval(time) <= maxHeight*fAmpMax && time<200.) {
		fTime90 = time;
		time = time + timeStep;
	};

	time = fTime90;
	while( fGraphSignal->Eval(time) >= minHeight*fAmpMax && time>0) {
		fTime10 = time;
		time = time - timeStep;
	}

	TF1 *fit1 = new TF1("fit1","[1]*x+[0]");	//function for one parameter fitting in the range of pmin-pmax
	fit1->SetRange(fTime10,fTime90);

	fGraphSignal->Fit(fit1,"RQN","goff");
	fEdgeSlope = fit1->GetParameter(1);
	fEdgeXi = fit1->GetChisquare();

	delete fit1;
}

Double_t AEvent::FindZeroLevel() {

	SetGraphs();
	Double_t correction = 0;
	TF1 *fit1 = new TF1("fit1","[0]");	//function for one parameter fitting in the range of pmin-pmax
	fit1->SetRange(fNoiseRangeMin,fNoiseRangeMax);

	if (!fGraphSignal) {
		Warning("AEvent::FindZeroLevel", "Graph was not set");
		return 0;
	}

	fGraphSignal->Fit(fit1,"RQN","goff");
	correction = fit1->GetParameter(0);

	delete fit1;
	return correction;
}

void AEvent::SetChargeCFD(Int_t tmin, Int_t tmax) { // tmin = -3, tmax = 17
	
	Double_t integral = 0.;					//voltage
	Double_t time_sig = 0;					//approximate signal duration in seconds
	const Double_t res = 50.; 				//resistance 50 Om
	time_sig = (double)(-tmin + tmax)*(1e-9);

	/*for(Int_t i = 0; i < fNPoints; i++) {
		if( fTime[i] > (fTimeCFD + tmin) && fTime[i] < (fTimeCFD + tmax) ) {
			integral = integral + fAmpPos[i];
		}
	}*/

	Int_t imin = 0, imax = 0;

	Int_t i = 0;
	while ( fTime[i] < (fTimeCFD + tmin) && (i < (fGraphSignal->GetN()-1)) ) { imin = i; i++; }
	while ( fTime[i] < (fTimeCFD + tmax) && (i < (fGraphSignal->GetN()-1)) ) { imax = i; i++; }

	integral = fGraphSignal->Integral(imin, imax);

	fChargeCFD = integral*time_sig/res;

	return;

}

void AEvent::SetChargeLED(Int_t tmin, Int_t tmax) { // tmin = -3, tmax = 17
	
	Double_t integral = 0.;					//voltage
	Double_t time_sig = 0;					//approximate signal duration in seconds
	const Double_t res = 50.; 				//resistance 50 Om
	time_sig = (double)(-tmin + tmax)*(1e-9);

/*	for(Int_t i = 0; i < fNPoints; i++) {
		if( fTime[i] > (fTimeLED + tmin) && fTime[i] < (fTimeLED + tmax) ) {
			integral = integral + fAmpPos[i];
		}
	}*/

	Int_t imin = 0, imax = 0;

	Int_t i = 0;
	while ( fTime[i] < (fTimeLED + tmin) && (i < (fGraphSignal->GetN()-1)) ) { imin = i; i++; }
//	i = 0;
	while ( fTime[i] < (fTimeLED + tmax) && (i < (fGraphSignal->GetN()-1)) ) { imax = i; i++; }

	integral = fGraphSignal->Integral(imin, imax);

	fChargeLED = integral*time_sig/res;

	return;

}

Double_t AEvent::GetfCFD() {
		return fTimeCFD;
}

Double_t AEvent::GetOnefTime(Int_t i) {
		return fTime[i];
}

Double_t AEvent::GetOnefAmpPos(Int_t i) {
		return fAmpPos[i];
}

Double_t AEvent::GetT_10() {
		return fTime10;
}

Double_t AEvent::GetT_90() {
		return fTime90;
}

Double_t AEvent::GetEdgeSlope() {
		return fEdgeSlope;
}

Double_t AEvent::GetEdgeXi() {
		return fEdgeXi;
}

void AEvent::SetMaxAmplitudes() {
	Double_t maxAmp = 0.;
	Double_t maxAmpT = 0.;

	maxAmp = fAmpPos[0];
	for(Int_t j=0; j < fNPoints; j++) {
		if(fAmpPos[j] > maxAmp) {
			maxAmp = fAmpPos[j];
			maxAmpT = fTime[j];
		}
	}
	fAmpMax = maxAmp;
	fTimeAmpMax = maxAmpT;

}

void AEvent::SetLED(Double_t threshold) {
	
	Double_t time = 0;
	const Double_t timeStep = 0.05;
	while( time < fTimeAmpMax && fGraphSignal->Eval(time) <= threshold ) {
		fTimeLED = time;
		time = time + timeStep;
	}
	//fTimeLED = time; найти номера точек которые лежат на 3 нс раньше и на 20 нс позже чем точка с временем timeled (искать точки в пределах условных)
	// сделать через функцию getpoint и цикл по точкам от 

}
