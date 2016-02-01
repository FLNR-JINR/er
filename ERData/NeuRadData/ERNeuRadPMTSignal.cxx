// -------------------------------------------------------------------------
// -----                      ERNeuRadPMTSignal source file            -----
// -------------------------------------------------------------------------

#include "ERNeuRadPMTSignal.h"

#include "FairLogger.h"
#include <iostream>
#include <algorithm>
using namespace std;

// -----   Default constructor   -------------------------------------------
ERNeuRadPMTSignal::ERNeuRadPMTSignal(){

}
// -------------------------------------------------------------------------
// -----   Constructor with arguments   ------------------------------------
ERNeuRadPMTSignal::ERNeuRadPMTSignal(Int_t iFiber, Int_t iBundle)
  :fFiberIndex(iFiber),
  fBundleIndex(iBundle)
{  
}
//-------------------------------------------------------------------------

//-----   Destructor   ----------------------------------------------------
ERNeuRadPMTSignal::~ERNeuRadPMTSignal(){

}
//-------------------------------------------------------------------------

// -------------------------------------------------------------------------
void ERNeuRadPMTSignal::AddFiberPoint(ERNeuRadFiberPoint* fpoint){
  //Определяем какие точки преломления добавляет поинт во временную ось
  Double_t t1 = fpoint->AnodeTime();
  Double_t A1 = 0.;
  Double_t t2 = t1 + 0.4;
  Double_t A2 = fpoint->Amplitude();
  Double_t t3 = t2 + 0.1;
  Double_t A3 = fpoint->Amplitude();
  Double_t t4 = t3 + 0.8;
  Double_t A4 = 0.;
  SignalPointsMap separateSignal;
  separateSignal.insert(SignalPoint(t1,A1));
  separateSignal.insert(SignalPoint(t2,A2));
  separateSignal.insert(SignalPoint(t3,A3));
  separateSignal.insert(SignalPoint(t4,A4));
  fSeparateSignals.push_back(separateSignal);
  //if first fiber point in signal
  if (fSignalPoints.size() == 0){
    //LOG(INFO) << "First fiber point" << FairLogger::endl;
    //LOG(INFO) << t1 << " " << A1 << FairLogger::endl;
    //LOG(INFO) << t2 << " " << A2 << FairLogger::endl;
    //LOG(INFO) << t3 << " " << A3 << FairLogger::endl;
    //LOG(INFO) << t4 << " " << A4 << FairLogger::endl;
    fSignalPoints.insert(SignalPoint(t1,A1));
    fSignalPoints.insert(SignalPoint(t2,A2));
    fSignalPoints.insert(SignalPoint(t3,A3));
    fSignalPoints.insert(SignalPoint(t4,A4));
    return;
  }
  //LOG(INFO) << "Next fiber point" << FairLogger::endl;
  //LOG(INFO) << t1 << " " << A1 << FairLogger::endl;
  //LOG(INFO) << t2 << " " << A2 << FairLogger::endl;
  //LOG(INFO) << t3 << " " << A3 << FairLogger::endl;
  //LOG(INFO) << t4 << " " << A4 << FairLogger::endl; 

  /*
  Необходимо вычислить сумму текущего сигнала и сигнала от нового FiberPoint
  Необходимо не только добавить новые точки излома на ось времени и посчитать в них результирующую амлитуду
  но также, если это необходимо пересчитать амплитуды в тех точках, что уже были на оси.
  Для этого сначала введем map изменений, а после всех вычислений применим его
  */
  
  struct Change{SignalPointsMap::iterator it; Double_t value;};
  vector<Change> changes;
  vector<Double_t> fpTimes;
  
  SignalPointsMap::iterator itFirstSPoint,itSecSPoint, itSPrevPoint,itSNextPoint;
  
  pair<SignalPointsMap::iterator,bool> ret;
  ret = fSignalPoints.insert(SignalPoint(t1,A1));
  itFirstSPoint= ret.first;
  fpTimes.push_back(t1);
  itSNextPoint = itFirstSPoint;
  itSNextPoint++;
  //Изменяем амлитуду первой точки, если она попадает на участок с существующим сигналом
  if (!(itFirstSPoint == fSignalPoints.begin() || itSNextPoint == fSignalPoints.end())){
    //Находим предыдущую и следующую точки текущего сигнала, составляем уравнение прямой
    itSPrevPoint = itFirstSPoint;
    itSPrevPoint--;
    Double_t newValue = itFirstSPoint->second +(itSNextPoint->second - itSPrevPoint->second)/(itSNextPoint->first - itSPrevPoint->first)
            *(itFirstSPoint->first-itSPrevPoint->first) + itSPrevPoint->second; 
    Change change = Change{itFirstSPoint,newValue};
    changes.push_back(change);
  }

  ret = fSignalPoints.insert(SignalPoint(t2,A2));
  fpTimes.push_back(t2);
  itSecSPoint = ret.first;
  itSNextPoint = itSecSPoint;
  itSNextPoint++;
  //Изменяем амлитуду второй точки, если она попадает на участок с существующим сигналом
  if (!(itSNextPoint == fSignalPoints.end())){
    itSPrevPoint = itSecSPoint;
    itSPrevPoint--;
    while (find(fpTimes.begin(), fpTimes.end(), itSPrevPoint->first) != fpTimes.end())
      itSPrevPoint--;
    Double_t newValue = itSecSPoint->second + (itSNextPoint->second - itSPrevPoint->second)/(itSNextPoint->first - itSPrevPoint->first)
            *(itSecSPoint->first-itSPrevPoint->first) + itSPrevPoint->second;
    Change change = Change{itSecSPoint,newValue};
    changes.push_back(change);
  }
  
  //Изменяем амлитуды точек между добавленными
  itSNextPoint = itFirstSPoint;
  itSNextPoint++;
  for (SignalPointsMap::iterator it = itSNextPoint; it != itSecSPoint; ++it){
    Double_t newValue = it->second + (itSecSPoint->second - itFirstSPoint->second)/(itSecSPoint->first - itFirstSPoint->first)
            *(it->first-itFirstSPoint->first) + itFirstSPoint->second;
    Change change = Change{it,newValue};
    changes.push_back(change);
  }

  ret = fSignalPoints.insert(SignalPoint(t3,A3));
  fpTimes.push_back(t3);
  //Изменяем амлитуду третьей точки, если она попадает на участок с существующим сигналом
  itFirstSPoint = itSecSPoint;
  itSecSPoint = ret.first;
  itSNextPoint = itSecSPoint;
  itSNextPoint++;
  if (!(itSNextPoint == fSignalPoints.end())){
    itSPrevPoint = itSecSPoint;
    itSPrevPoint--;
    while (find(fpTimes.begin(), fpTimes.end(), itSPrevPoint->first) != fpTimes.end())
      itSPrevPoint--;
    Double_t newValue = itSecSPoint->second + (itSNextPoint->second - itSPrevPoint->second)/(itSNextPoint->first - itSPrevPoint->first)
            *(itSecSPoint->first-itSPrevPoint->first) + itSPrevPoint->second;
    Change change = Change{itSecSPoint,newValue};
    changes.push_back(change);
  }

  //Изменяем амлитуды точек между добавленными
  itSNextPoint = itFirstSPoint;
  itSNextPoint++;
  for (SignalPointsMap::iterator it = itSNextPoint; it != itSecSPoint; ++it){
    Double_t newValue = it->second + (itSecSPoint->second - itFirstSPoint->second)/(itSecSPoint->first - itFirstSPoint->first)
            *(it->first-itFirstSPoint->first) + itFirstSPoint->second;
    Change change = Change{it,newValue};
    changes.push_back(change);
  }

  ret = fSignalPoints.insert(SignalPoint(t4,A4));
  fpTimes.push_back(t4);
  //Изменяем амлитуду четвертой точки, если она попадает на участок с существующим сигналом
  itFirstSPoint = itSecSPoint;
  itSecSPoint = ret.first;
  itSNextPoint = itSecSPoint;
  itSNextPoint++;
  if (!(itSNextPoint == fSignalPoints.end())){
    itSPrevPoint = itSecSPoint;
    itSPrevPoint--;
    while (find(fpTimes.begin(), fpTimes.end(), itSPrevPoint->first) != fpTimes.end())
      itSPrevPoint--;
    Double_t newValue = itSecSPoint->second + (itSNextPoint->second - itSPrevPoint->second)/(itSNextPoint->first - itSPrevPoint->first)
            *(itSecSPoint->first-itSPrevPoint->first) + itSPrevPoint->second;
    Change change = Change{itSecSPoint,newValue};
    changes.push_back(change);
  }

  //Изменяем амлитуды точек между добавленными
  itSNextPoint = itFirstSPoint;
  itSNextPoint++;
  for (SignalPointsMap::iterator it = itSNextPoint; it != itSecSPoint; ++it){
    Double_t newValue = it->second + (itSecSPoint->second - itFirstSPoint->second)/(itSecSPoint->first - itFirstSPoint->first)
            *(it->first-itFirstSPoint->first) + itFirstSPoint->second;
    Change change = Change{it,newValue};
    changes.push_back(change);
  }
  
  //Применяем изменения к сигналу
  for (vector<Change>::iterator it = changes.begin(); it != changes.end(); ++it){
    (*it).it->second = (*it).value;
  }
  
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
vector<Double_t> ERNeuRadPMTSignal::GetIntersections(Double_t discriminatorThreshold){
  vector<Double_t> intersections;

  SignalPointsMap::iterator itSp= fSignalPoints.begin();
  itSp++;
  SignalPointsMap::iterator itPrevSp;
  bool inSignal = false;
  for (; itSp != fSignalPoints.end(); itSp++){
    itPrevSp= itSp;
    itPrevSp--;
    if (!inSignal){
      if(itSp->second < discriminatorThreshold){
        continue;
      }else{
        inSignal = true;
        Double_t intersect = (discriminatorThreshold - itPrevSp->second)/(itSp->second - itPrevSp->second)
                      *(itSp->first - itPrevSp->first) + itPrevSp->first;
                      
        intersections.push_back(intersect);
      }
    }
    else{
      if(itSp->second > discriminatorThreshold){
        continue;
      }else{
        inSignal = false;
        Double_t intersect = (discriminatorThreshold - itPrevSp->second)/(itSp->second - itPrevSp->second)
                      *(itSp->first - itPrevSp->first) + itPrevSp->first;
                      
        intersections.push_back(intersect);
      }  
    }
  }
  return intersections;
}
// -------------------------------------------------------------------------

//-------------------------------------------------------------------------
Double_t ERNeuRadPMTSignal::GetMaxInteg(const Double_t window, const Double_t dt){
  Double_t start = GetStartTime();
  Double_t finish = start+window;
  Double_t max = -9999999.;
  do{
    Double_t curInteg = GetInteg(start, finish);
    if (curInteg > max)
      max = curInteg;
    start += dt;
    finish += start + window;
  }
  while(finish<GetFinishTime());
  
  return max;
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
Double_t ERNeuRadPMTSignal::GetInteg(const Double_t start,const Double_t finish){
  if (finish<GetStartTime())
    return 0;
  
  Double_t res = 0;
  SignalPointsMap::iterator itStartSp = fSignalPoints.upper_bound (start);
  SignalPointsMap::iterator itFinishSp = fSignalPoints.lower_bound(finish);
  
  //Первая трапеция 
  res += 0.5*(GetMean(start)+itStartSp->second)*(itStartSp->first-start);
  
  //Суммируем трапеции внутри промежутка
  for(SignalPointsMap::iterator prev = itStartSp;prev!=itFinishSp;++prev){
    SignalPointsMap::iterator next = prev;
    next++;
    
    res += 0.5*(prev->second + next->second)*(next->first - prev->first);
  }
  
  //Последняя трапеция
  res += 0.5*(itFinishSp->second + GetMean(finish))*(finish-itFinishSp->first);
  
  return res;
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
Double_t ERNeuRadPMTSignal::GetMean(const Double_t time){
  SignalPointsMap::iterator itSp= fSignalPoints.begin();
  if (GetStartTime() > time)
    return 0;
  if (GetFinishTime() < time)
    return 0;
  while(itSp->first < time)
    itSp++;
  SignalPointsMap::iterator itPrevSp = itSp;
  itPrevSp--;
  
  return (time-itPrevSp->first)/(itSp->first-itPrevSp->first)*(itSp->second-itPrevSp->second)+itPrevSp->second;
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
Double_t ERNeuRadPMTSignal::GetFirstInteg(const Double_t window){
  return GetInteg(GetStartTime(),GetStartTime()+window );
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
Double_t ERNeuRadPMTSignal::GetStartTime(){
  return fSignalPoints.begin()->first;
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
Double_t ERNeuRadPMTSignal::GetFinishTime(){
  SignalPointsMap::iterator itSp= fSignalPoints.end();
  return (--itSp)->first;
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------

ClassImp(ERNeuRadPMTSignal);