#include "ERmuSiVertexFinder.h"

#include <vector>
#include <iostream>
using namespace std;

#include "TVector3.h"
#include "TMath.h"
#include "TMatrix.h"
#include "TF3.h"
#include "TError.h"
#include "Fit/BinData.h"
#include "Fit/Fitter.h"
#include "Math/WrappedMultiTF1.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "FairLogger.h"

#include "ERDetectorList.h"
#include "ERmuSiTrack.h"
#include "ERmuSiPoint.h"

// ----------------------------------------------------------------------------
ERmuSiVertexFinder::ERmuSiVertexFinder()
  : FairTask("ER muSi vertex finding scheme")
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERmuSiVertexFinder::ERmuSiVertexFinder(Int_t verbose)
  : FairTask("ER muSi vertex finding scheme ", verbose)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERmuSiVertexFinder::~ERmuSiVertexFinder()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERmuSiVertexFinder::SetParContainers()
{
  // Get run and runtime database
  FairRunAna* run = FairRunAna::Instance();
  if ( ! run ) Fatal("SetParContainers", "No analysis run");

  FairRuntimeDb* rtdb = run->GetRuntimeDb();
  if ( ! rtdb ) Fatal("SetParContainers", "No runtime database");
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
InitStatus ERmuSiVertexFinder::Init()
{
  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  
  fmuSiTracks = (TClonesArray*) ioman->GetObject("muSiTrack");
  //todo check

  // Register output array fmuSiHits
  fmuSiVertices = new TClonesArray("ERmuSiVertex",1000);

  ioman->Register("muSiVertex", "muSi", fmuSiVertices, kTRUE);
   
  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERmuSiVertexFinder::Exec(Option_t* opt)
{ 
  Reset();
  LOG(INFO) << FairLogger::endl;
  LOG(INFO) <<  "ERmuSiVertexFinder started:" << FairLogger::endl;
  //Попарно ищем общий перепендикуляр к трекам
  for(Int_t iTrack = 0; iTrack < fmuSiTracks->GetEntriesFast(); iTrack++){
    ERmuSiTrack* track1 = (ERmuSiTrack*)fmuSiTracks->At(iTrack);
    
    //Фитируем трек
    ROOT::Fit::BinData data(3,2);
    double xx[2];
    xx[0] = track1->Hit(0)->GetX();
    xx[1] = track1->Hit(0)->GetY();
    data.Add(xx,track1->Hit(0)->GetZ(), 0.001);
    xx[0] = track1->Hit(1)->GetX();
    xx[1] = track1->Hit(1)->GetY();
    data.Add(xx,track1->Hit(1)->GetZ(), 0.001);
    xx[0] = track1->Hit(2)->GetX();
    xx[1] = track1->Hit(2)->GetY();
    data.Add(xx,track1->Hit(2)->GetZ(), 0.001);

    TF2 * f = new TF2("f2","[0]*x + [1]*y + [2]*1");
    f->SetParameters(2,2,2);
    ROOT::Fit::Fitter fitter;
    ROOT::Math::WrappedMultiTF1 wf(*f,2);
    fitter.SetFunction(wf);
    bool ret = fitter.Fit(data);
    const ROOT::Fit::FitResult & res = fitter.Result();
    //res.Print(std::cout);
    f->SetFitResult(res);
    //Параметры фита трека - направляющий вектор
    TVector3 directTrack1(track1->Hit(0)->GetX()-track1->Hit(1)->GetX(),
                          track1->Hit(0)->GetY()-track1->Hit(1)->GetY(),
                          f->Eval(track1->Hit(0)->GetX(),track1->Hit(0)->GetY()) 
                          - f->Eval(track1->Hit(1)->GetX(),track1->Hit(1)->GetY()));
    for(Int_t jTrack = iTrack+1; jTrack < fmuSiTracks->GetEntriesFast(); jTrack++){
      if (iTrack == jTrack)
        continue;
      ERmuSiTrack* track2 = (ERmuSiTrack*)fmuSiTracks->At(jTrack);
       //Фитируем трек
      ROOT::Fit::BinData data2(3,2);
      xx[0] = track2->Hit(0)->GetX();
      xx[1] = track2->Hit(0)->GetY();
      data2.Add(xx,track2->Hit(0)->GetZ(), 0.001);
      xx[0] = track2->Hit(1)->GetX();
      xx[1] = track2->Hit(1)->GetY();
      data2.Add(xx,track2->Hit(1)->GetZ(), 0.001);
      xx[0] = track2->Hit(2)->GetX();
      xx[1] = track2->Hit(2)->GetY();
      data2.Add(xx,track2->Hit(2)->GetZ(), 0.001);

      ROOT::Fit::Fitter fitter2;
      ROOT::Math::WrappedMultiTF1 wf2(*f,3);
      fitter2.SetFunction(wf2);
      bool ret2 = fitter2.Fit(data2);
      const ROOT::Fit::FitResult & res2 = fitter2.Result();
      //res2.Print(std::cout);
      f->SetFitResult(res2);
      //Параметры фита трека - направляющий вектор
      TVector3 directTrack2(track2->Hit(0)->GetX()-track2->Hit(1)->GetX(),
                          track2->Hit(0)->GetY()-track2->Hit(1)->GetY(),
                          f->Eval(track2->Hit(0)->GetX(),track2->Hit(0)->GetY()) 
                          - f->Eval(track2->Hit(1)->GetX(),track2->Hit(1)->GetY()));

      //Строим вектор перпендикулярный к обоим прямым
      TVector3 p = directTrack1.Cross(directTrack2);

      //Для нахождения общего перпендикуляра записываем уравнения прямых в параметрическом виде.
      //Разность между точками на прямых прямо пропорциональна вектору p
      TMatrixT<double> mat(3,3);
      mat(0,0) = directTrack1.X();
      mat(0,1)  = -directTrack2.X();
      mat(0,2)  = -p.X();
      mat(1,0)  = directTrack1.Y();
      mat(1,1)  = -directTrack2.Y();
      mat(1,2)  = -p.Y();
      mat(2,0)  = directTrack1.Z();
      mat(2,1)  = -directTrack2.Z();
      mat(2,2)  = -p.Z();

      TMatrixT<double> right(3,1);

      right(0,0) = track2->Hit(0)->GetX()-track1->Hit(0)->GetX();
      right(1,0) = track2->Hit(0)->GetY()-track1->Hit(0)->GetY();
      right(2,0) = track2->Hit(0)->GetZ()-track1->Hit(0)->GetZ();

      TMatrixT<double> matInv(3,3);
      matInv = mat.Invert();
      TMatrixT<double> lamda(3,1);
      lamda = matInv*right;
      TVector3 orig(track1->Hit(0)->GetX(), track1->Hit(0)->GetY(), track1->Hit(0)->GetZ());
      TVector3 orig2(track2->Hit(0)->GetX(), track2->Hit(0)->GetY(), track2->Hit(0)->GetZ());
      TVector3 vertex = orig + lamda(0,0)*directTrack1;
      TVector3 vertex2 = orig2 + lamda(1,0)*directTrack2;
      if (lamda(2,0) < fDistanceCut){
        ERmuSiVertex* vert = AddVertex((vertex.X() + vertex2.X())/2., (vertex.Y() + vertex2.Y())/2.,(vertex.Z() + vertex2.Z())/2.);
        vert->AddTrack(iTrack);
        vert->AddTrack(jTrack);
      }
    }
  }
  Int_t rems = 0;
  //merge vertices
  for (Int_t iVert = 0; iVert < fmuSiVertices->GetEntriesFast(); iVert++){
    ERmuSiVertex* vert1 = (ERmuSiVertex*)fmuSiVertices->At(iVert);
    for (Int_t jVert = iVert+1; jVert < fmuSiVertices->GetEntriesFast(); jVert++){
      ERmuSiVertex* vert2 = (ERmuSiVertex*)fmuSiVertices->At(jVert);
      Float_t dist = TMath::Sqrt((vert2->X()-vert1->X())*(vert2->X()-vert1->X()) +  
                                  (vert2->Y()-vert1->Y())*(vert2->Y()-vert1->Y()) + 
                                  (vert2->Z()-vert1->Z())*(vert2->Z()-vert1->Z()));
      if(dist < 0.5 ){
        LOG(INFO) << "Vertecies merging!" << FairLogger::endl;
        vert1->SetX((vert1->X()+vert2->X())/2.);
        vert1->SetY((vert1->Y()+vert2->Y())/2.);
        vert1->SetZ((vert1->Z()+vert2->Z())/2.);
        for (Int_t iTrack = 0; iTrack < vert2->TrackNb(); iTrack++){
          vert1->AddTrack(vert2->Track(iTrack));
        }
        rems++;
        fmuSiVertices->RemoveAt(jVert);
      }
      fmuSiVertices->Compress();
    }
  }
  LOG(INFO) << "Vertecies count: " << fmuSiVertices->GetEntriesFast() << FairLogger::endl;
  for(Int_t iVert=0; iVert < fmuSiVertices->GetEntriesFast(); iVert++){
    ERmuSiVertex* vert = (ERmuSiVertex*)fmuSiVertices->At(iVert);
    LOG(INFO) << "Vertex "<< iVert << ": (" <<  vert->X() << "," << vert->Y() << "," << vert->Z() << ")" << FairLogger::endl;
  }
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERmuSiVertexFinder::Reset()
{
  if (fmuSiVertices) {
    fmuSiVertices->Delete();
  }
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERmuSiVertexFinder::Finish()
{   
  
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
ERmuSiVertex* ERmuSiVertexFinder::AddVertex(Float_t x, Float_t y, Float_t z){
  ERmuSiVertex *vertex = new((*fmuSiVertices)[fmuSiVertices->GetEntriesFast()])
                              ERmuSiVertex(x,y,z);
  return vertex;
}
//----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
ClassImp(ERmuSiVertexFinder)