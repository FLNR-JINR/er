#include "ERmuSiMatcher.h"

#include <iostream>
#include <map>
using namespace std;

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"

#include "ERDetectorList.h"

#include "ERMCTrack.h"
#include "ERmuSiPoint.h"
#include "ERmuSiHit.h"
#include "ERmuSiTrack.h"
#include "ERmuSiVertex.h"
// ----------------------------------------------------------------------------
ERmuSiMatcher::ERmuSiMatcher()
  : FairTask("ER muSi matching scheme"),
  fTrueTracksNb(0), 
  fWrongTracksNb(0),
  fMCTracksNb(0),
  fShortMCTracksNb(0),
  fNotFoundedVerteciesNb(0)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERmuSiMatcher::ERmuSiMatcher(Int_t verbose)
  : FairTask("ER muSi matching scheme ", verbose),
  fTrueTracksNb(0), 
  fWrongTracksNb(0),
  fMCTracksNb(0),
  fShortMCTracksNb(0),
  fNotFoundedVerteciesNb(0)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERmuSiMatcher::~ERmuSiMatcher()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERmuSiMatcher::SetParContainers()
{
  // Get run and runtime database
  FairRunAna* run = FairRunAna::Instance();
  if ( ! run ) Fatal("SetParContainers", "No analysis run");

  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  if ( ! rtdb ) Fatal("SetParContainers", "No runtime database");
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
InitStatus ERmuSiMatcher::Init()
{
  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  
  fMCTracks = (TClonesArray*) ioman->GetObject("MCTrack");
  fmuSiPoints = (TClonesArray*) ioman->GetObject("muSiPoint");
  fmuSiHits = (TClonesArray*) ioman->GetObject("muSiHit");
  fmuSiTracks = (TClonesArray*) ioman->GetObject("muSiTrack");
  fmuSiVertecies = (TClonesArray*) ioman->GetObject("muSiVertex");

  fHVertexDz = new TH1F("Vertex z quality", "Vertex z quality", 5000, 0., .5);
  fHVertexDxy = new TH1F("Vertex xy quality", "Vertex xy quality", 5000, 0., .5);
  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERmuSiMatcher::Exec(Option_t* opt)
{
 LOG(INFO) << FairLogger::endl;
 LOG(INFO) << "ERmuSiMatcher:" << FairLogger::endl;
 Int_t trueTracks = 0;
 Int_t wrongTracks = 0;
 for(Int_t iTrack = 0; iTrack < fmuSiTracks->GetEntriesFast(); iTrack++){
 	ERmuSiTrack* track = (ERmuSiTrack*)fmuSiTracks->At(iTrack);
    ERmuSiHit* hit0 = track->Hit(0);
    ERmuSiHit* hit1 = track->Hit(1);
    ERmuSiHit* hit2 = track->Hit(2);

    if (hit0->GetRefIndex() == -1 || hit1->GetRefIndex() == -1 || hit2->GetRefIndex() == -1){ //fakes
    	wrongTracks++;
    	continue;
    }
 	ERmuSiPoint* point0 = (ERmuSiPoint*)fmuSiPoints->At(hit0->GetRefIndex());
 	ERmuSiPoint* point1 = (ERmuSiPoint*)fmuSiPoints->At(hit1->GetRefIndex());
 	ERmuSiPoint* point2 = (ERmuSiPoint*)fmuSiPoints->At(hit2->GetRefIndex());
 	if ((point0->GetTrackID() == point1->GetTrackID()) && (point1->GetTrackID() == point2->GetTrackID()))
 		trueTracks++;
 	else
 		wrongTracks++;  
 }
 
 map<Int_t,Int_t> pointsOnTracks;
 for (Int_t iPoint = 0; iPoint < fmuSiPoints->GetEntriesFast(); iPoint++){
    ERmuSiPoint* point = (ERmuSiPoint*)fmuSiPoints->At(iPoint);
    pointsOnTracks[point->GetTrackID()]++;
 }
 Int_t shortMCTracks = 0;
 for (map<Int_t,Int_t>::iterator it = pointsOnTracks.begin(); it != pointsOnTracks.end(); ++it){
  if(it->second < 3)
    shortMCTracks++;
 }

 LOG(INFO) << "True tracks:" << trueTracks << FairLogger::endl;
 LOG(INFO) << "Wrong tracks:" << wrongTracks << FairLogger::endl;
 fTrueTracksNb += trueTracks;
 fWrongTracksNb += wrongTracks;
 fMCTracksNb += pointsOnTracks.size();
 fShortMCTracksNb+= shortMCTracks;

 Int_t notFoundedVerteciesNb = 0;
 //Выделяем mc вершины по массиву треков
 vector<MCVertex> MCVertecies;
 for (Int_t iTrack = 0; iTrack < fMCTracks->GetEntriesFast(); iTrack++){
    ERMCTrack* mcTrack = (ERMCTrack*)fMCTracks->At(iTrack);
    Bool_t addedToMC = kFALSE;
    if (mcTrack->GetMotherId() == -1){
      for(vector<MCVertex>::iterator it = MCVertecies.begin(); it!=MCVertecies.end(); it++){
        if ( ((*it).x - mcTrack->GetStartX() < 0.01) &&
             ((*it).y - mcTrack->GetStartY() < 0.01) &&
             ((*it).y - mcTrack->GetStartY() < 0.01))
            addedToMC = kTRUE;
      }
      if (!addedToMC)
        MCVertecies.push_back(MCVertex(mcTrack->GetStartX(), mcTrack->GetStartY(), mcTrack->GetStartZ()));
    }
  }
  //Смотрим какие вершины нашли
  for(vector<MCVertex>::iterator it = MCVertecies.begin(); it!=MCVertecies.end(); it++){
      MCVertex mcVertex = (*it);
      Bool_t founded = kFALSE;
      Float_t distFounded = 99999999.;
      ERmuSiVertex* vertFounded;
      for(Int_t iVert =0; iVert < fmuSiVertecies->GetEntriesFast(); iVert++ ){
        ERmuSiVertex* vert = (ERmuSiVertex*)fmuSiVertecies->At(iVert);
        Float_t dist = TMath::Sqrt((vert->X()-mcVertex.x)*(vert->X()-mcVertex.x) +
                                   (vert->Y()-mcVertex.y)*(vert->Y()-mcVertex.y) +
                                   (vert->Z()-mcVertex.z)*(vert->Z()-mcVertex.z));
        if (dist < 1.){
          founded = kTRUE;
          if (dist < distFounded){ //выделяем наилучшую найденную вершину
            distFounded = dist;
            vertFounded = vert;
          }
        }
      }
      if (!founded){
        notFoundedVerteciesNb++;
      }else{
        fHVertexDz->Fill(TMath::Abs(vertFounded->Z() - mcVertex.z));
        fHVertexDxy->Fill(TMath::Sqrt((vertFounded->Y() - mcVertex.y)*(vertFounded->Y() - mcVertex.y)
                                      +(vertFounded->X() - mcVertex.x)*(vertFounded->Z() - mcVertex.z)));
      }
   }

   LOG(INFO) << "Not founded primary vertecies:" << notFoundedVerteciesNb << FairLogger::endl;
   fNotFoundedVerteciesNb += notFoundedVerteciesNb;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERmuSiMatcher::Reset()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERmuSiMatcher::Finish()
{  
  LOG(INFO) << FairLogger::endl;
  LOG(INFO) << "========= ERmuSiMatcher : ================" << FairLogger::endl;

  LOG(INFO) << "MC tracks: " << fMCTracksNb << FairLogger::endl;
  LOG(INFO) << "Short MC tracks: " << fShortMCTracksNb << FairLogger::endl;
  LOG(INFO) << "True tracks: " << fTrueTracksNb << FairLogger::endl;
  LOG(INFO) << "Wrong tracks: " << fWrongTracksNb << FairLogger::endl;
  LOG(INFO) << "Eff. all: " << (Float_t)fTrueTracksNb/(Float_t)fMCTracksNb << FairLogger::endl;
  LOG(INFO) << "Eff. long: " << (Float_t)fTrueTracksNb/(Float_t)(fMCTracksNb-fShortMCTracksNb)<< FairLogger::endl;
  LOG(INFO) << "Not founded primary vertecies: " << fNotFoundedVerteciesNb << FairLogger::endl;

  fHVertexDz->Write();
  fHVertexDxy->Write();

}
// ----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
ClassImp(ERmuSiMatcher)
