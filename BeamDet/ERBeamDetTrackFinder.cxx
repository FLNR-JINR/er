#include "ERBeamDetTrackFinder.h"

#include <vector>
#include <iostream>
using namespace std;

#include "TVector3.h"
#include "TMath.h"

#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include <iostream>

#include "ERDetectorList.h"
#include "ERBeamDetTrack.h"
#include "ERBeamDetPoint.h"

// ----------------------------------------------------------------------------
ERBeamDetTrackFinder::ERBeamDetTrackFinder()
  : FairTask("ER BeamDet track finding scheme")
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERBeamDetTrackFinder::ERBeamDetTrackFinder(Int_t verbose)
  : FairTask("ER BeamDet track finding scheme ", verbose)
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
ERBeamDetTrackFinder::~ERBeamDetTrackFinder()
{
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERNeuRadDigitizer::SetParContainers()
{
  fBeamDetSetup = ERBeamDetSetup::Instance();
  fBeamDetSetup->SetParContainers();
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
InitStatus ERBeamDetTrackFinder::Init()
{
  // Get input array
  FairRootManager* ioman = FairRootManager::Instance();
  if ( ! ioman ) Fatal("Init", "No FairRootManager");
  
  fBeamDetDigi = (TClonesArray*) ioman->GetObject("BeamDetDigi");

  // Register output array fBeamDetHits
  fBeamDetTrack = new TObject("ERBeamDetTrack");

  ioman->Register("BeamDetTrack.", "BeamDet track", fBeamDetTrack, kTRUE);
   
  return kSUCCESS;
}
// -------------------------------------------------------------------------

// -----   Public method Exec   --------------------------------------------
void ERBeamDetTrackFinder::Exec(Option_t* opt)
{ 
  Reset();
  std::cout << std::endl;
  std::cout << "ERBeamDetTrackFinder:" << std::endl;
/*
  1) Он просто по номеру проволочки востанавливает четыре отдельные координаты: xmw1, ymw1, xmw2, ymw2.
2) Дальше собирает из них две точки в пространстве:
VmwFa(xmw1,ymw1,0)
VmwCl(xmw2,ymw2,растояние между MWPC)
3) Формирует вектор пучка
Vbeam = VmwCl - VmwFa
4) Есть координата Zdist
Zdist = MWclosDist - header->UpMat.MWXYdist/2.;
Если я правильно понял это середина отрезка между X и Y плоскостями ближайшей к мишени MWPC
5) Координаты на мишени:
xbt = xmw2 - Zdist*tan(Vbeam.Theta())*cos(Vbeam.Phi());
ybt = ymw2 - Zdist*tan(Vbeam.Theta())*sin(Vbeam.Phi());
То есть он вообще никак не учитывает то, что у нас еть смещение пучка между станциями X и Y. Ок. Это вроде также как обсуждали в последний раз.
6) Отбор событий, попавших в мишень. Я думаю нам его тоже стоит сделать и не писать события не попавшие.

Как не писать события в выходной файл можно посмотреть в BeamDetCalibratorNew. FairRun->MarkFill()
*/
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void ERBeamDetTrackFinder::Reset()
{
  if (fBeamDetTrack) {
    fBeamDetTrack->Delete();
  }
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERBeamDetTrackFinder::Finish()
{   
  
}
// ----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
ClassImp(ERBeamDetTrackFinder)