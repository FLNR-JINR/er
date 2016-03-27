#include "ERmuSiHitProducer.h"

#include <vector>

#include "TVector3.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"

#include "ERDetectorList.h"
#include "ERmuSiPoint.h"
// ----------------------------------------------------------------------------
ERmuSiHitProducer::ERmuSiHitProducer()
  : FairTask("ER muSi hit producing scheme")
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERmuSiHitProducer::ERmuSiHitProducer(Int_t verbose)
  : FairTask("ER muSi hit producing scheme ", verbose)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERmuSiHitProducer::~ERmuSiHitProducer()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERmuSiHitProducer::SetParContainers()
{
  // Get run and runtime database
  FairRunAna* run = FairRunAna::Instance();
  if ( ! run ) Fatal("SetParContainers", "No analysis run");

  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  if ( ! rtdb ) Fatal("SetParContainers", "No runtime database");
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
InitStatus ERmuSiHitProducer::Init()
{
  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  
  fmuSiPoints = (TClonesArray*) ioman->GetObject("muSiPoint");
  //todo check

  // Register output array fmuSiHits
  fmuSiHits = new TClonesArray("ERmuSiHit",1000);

  ioman->Register("muSiHit", "muSi hits", fmuSiHits, kTRUE);
   
  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERmuSiHitProducer::Exec(Option_t* opt)
{
  //Расскидываем поинты по станциям
  std::vector<ERmuSiPoint*> PointsByStation[3];
  for (Int_t iPoint = 0; iPoint < fmuSiPoints->GetEntriesFast(); iPoint++){
    ERmuSiPoint* point  = (ERmuSiPoint*) fmuSiPoints->At(iPoint);
    PointsByStation[point->Station()].push_back(point);
  }
  //Комбинаторно на каждой станции генерируем хиты
  TVector3 dpos = TVector3(0.01, 0.01, 0.01); //ошибка пока фиксирована
  for (Int_t iStation =0; iStation < 3; iStation++){
    for (std::vector<ERmuSiPoint*>::iterator it1 = PointsByStation[iStation].begin(); it1 != PointsByStation[iStation].end(); ++ it1){
      ERmuSiPoint* point1 = (*it1);

      TVector3 *pos = new TVector3((point1->GetXIn()+point1->GetXOut())/2.,
                            (point1->GetYIn()+point1->GetYOut())/2.,
                            (point1->GetZIn()+point1->GetZOut())/2.);
      ERmuSiHit* hit = AddHit(kMUSI, *pos, dpos,point1->Index(), point1->Station());
      delete pos;

      for (std::vector<ERmuSiPoint*>::iterator it2 = PointsByStation[iStation].begin(); it2 != PointsByStation[iStation].end(); ++ it2){
        if (it1 == it2){
          continue;
        }
        ERmuSiPoint* point2 = (*it2);

        pos = new TVector3((point1->GetXIn()+point1->GetXOut())/2.,
                            (point2->GetYIn()+point2->GetYOut())/2.,
                            (point1->GetZIn()+point1->GetZOut())/2.);
        hit = AddHit(kMUSI, *pos, dpos,-1,point1->Station());
        delete pos;
      }
    }
  }

  for (Int_t iPoint = 0; iPoint < fmuSiPoints->GetEntriesFast(); iPoint++){
    ERmuSiPoint* point = (ERmuSiPoint*)fmuSiPoints->At(iPoint);
    
    TVector3 pos = TVector3((point->GetXIn()+point->GetXOut())/2.,
                            (point->GetYIn()+point->GetYOut())/2.,
                            (point->GetZIn()+point->GetZOut())/2.);
    

    

  }
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERmuSiHitProducer::Reset()
{
  if (fmuSiHits) {
    fmuSiHits->Delete();
  }
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERmuSiHitProducer::Finish()
{   

}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERmuSiHit* ERmuSiHitProducer::AddHit(Int_t detID, TVector3& pos, TVector3& dpos, Int_t index, Int_t station)
{
  ERmuSiHit *hit = new((*fmuSiHits)[fmuSiHits->GetEntriesFast()])
              ERmuSiHit(fmuSiHits->GetEntriesFast(),detID, pos, dpos, index, station);
  return hit;
}
// ----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
ClassImp(ERmuSiHitProducer)
