// -------------------------------------------------------------------------
// -----                      ERNeuRadPMTSignal source file            -----
// -------------------------------------------------------------------------

#include "ERNeuRadPMTSignal.h"

using namespace std;

// -----   Default constructor   -------------------------------------------
ERNeuRadPMTSignal::ERNeuRadPMTSignal(){

}
// -------------------------------------------------------------------------

// -----   Destructor   ----------------------------------------------------
ERNeuRadPMTSignal::~ERNeuRadPMTSignal(){

}
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
void ERNeuRadPMTSignal::AddFiberPoint(ERNeuRadFiberPoint* fpoint){
  //Определяем какие точки преломления добавляет поинт во временную ось
  Double_t t1 = fpoint->AnodeTime();
  Double_t A1 = 0.;
  Double_t t2 = 12.8/7. * fpoint->AnodeTime();
  Double_t A2 = fpoint->Amplitude();
  Double_t t3 = 15.2/7. * fpoint->AnodeTime();
  Double_t A3 = fpoint->Amplitude();
  Double_t t4 = 23./7. * fpoint->AnodeTime();
  Double_t A4 = 0.;

  //if first fiber point in signal
  if (fSignalPoints.size() == 0){
    fSignalPoints.insert(SignalPoint(t1,A1));
    fSignalPoints.insert(SignalPoint(t2,A2));
    fSignalPoints.insert(SignalPoint(t3,A3));
    fSignalPoints.insert(SignalPoint(t4,A4));
    return;
  }
/*
  SignalPointsMap::iterator itFirstSPoint,itSecSPoint, itSPrevPoint, itSNextPoint;
  
  pair<SignalPointsMap::iterator,bool> ret;
  ret = fSignalPoints.insert(SignalPoint(t1,A1));
  
  itFirstSPoint= ret.first;
  //Изменяем амлитуду первой точки, если она попадает на участок с существующим сигналом
  if (!(itFirstSPoint == fSignalPoints.begin() || itFirstSPoint == fSignalPoints.end())){
    itSPrevPoint = itFirstSPoint-1;
    itSNextPoint = itFirstSPoint+1;
    itFirstSPoint.second += (itSNextPoint.second - itSPrevPoint.second)/(itSNextPoint.first - itSPrevPoint.first)
            *(itFirstSPoint.first-itSPrevPoint.first) + itSPrevPoint.second;
  }

  ret = fSignalPoints.insert(SignalPoint(t2,A2));
  itSecSPoint = ret.first;
  //Изменяем амлитуду второй точки, если она попадает на участок с существующим сигналом
  if (!(itSecSPoint == fSignalPoints.end())){
    itSPrevPoint = itSecSPoint-1;
    if (itSPrevPoint == itFirstSPoint && itFirstSPoint != fSignalPoints.begin())
      itSPrevPoint = itSPrevPoint-1;
    itSNextPoint = itSecSPoint+1;
    itSecSPoint.second += (itSNextPoint.second - itSPrevPoint.second)/(itSNextPoint.first - itSPrevPoint.first)
            *(itSecSPoint.first-itSPrevPoint.first) + itSPrevPoint.second;
  }

  //Изменяем амлитуды точек между добавленными
  for (SignalPointsMap::iterator it = itFirstSPoint+1; it != itSecSPoint; ++it){
    
  }
*/
}
// -------------------------------------------------------------------------

ClassImp(ERNeuRadPMTSignal);