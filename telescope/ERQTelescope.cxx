// -------------------------------------------------------------------------
// -----                        ERQTelescope header file               -----
// -----                  Created data  by developer name              -----
// -------------------------------------------------------------------------
#include "ERQTelescope.h"

#include <iostream>
using namespace std;

#include "FairRootManager.h"
#include "TClonesArray.h"
#include "TParticle.h"
#include "TVirtualMC.h"
#include "TString.h"
// -----   Default constructor   -------------------------------------------
ERQTelescope::ERQTelescope() :
  FairDetector("ERQTelescope", kTRUE),
  fSiPoint(NULL),
  fCsIPoint(NULL)
{
  ResetParameters();
  fSiPoint =  new TClonesArray("ERQTelescopeSiPoint");
  fCsIPoint = new TClonesArray("ERQTelescopeCsIPoint");
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fVerboseLevel = 1;
  fVersion = 1;
}
// -------------------------------------------------------------------------
// -----   Standard constructor   ------------------------------------------
ERQTelescope::ERQTelescope(const char* name, Bool_t active, Int_t verbose)
  : FairDetector(name, active,1),
  fSiPoint(NULL),
  fCsIPoint(NULL)
  {
  ResetParameters();
  fSiPoint =  new TClonesArray("ERQTelescopeSiPoint");
  fCsIPoint = new TClonesArray("ERQTelescopeCsIPoint");
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fVersion = 1;
  fVerboseLevel = verbose;
}

ERQTelescope::~ERQTelescope() {
  if (fSiPoint) {
    fSiPoint->Delete();
    delete fSiPoint;
  }
  if (fCsIPoint) {
    fCsIPoint->Delete();
    delete fCsIPoint;
  }
}

void ERQTelescope::Initialize()
{
  FairDetector::Initialize();
}

//=3=3=3=3=3=3=3==3=3=3=3=
// -----   Private method AddPoint   --------------------------------------------
ERQTelescopeSiPoint* ERQTelescope::Add_SiPoint() {
  TClonesArray& clref = *fSiPoint;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) ERQTelescopeSiPoint(eventID, trackID, mot0TrackID, mass,
    TVector3(posIn.X(),   posIn.Y(),   posIn.Z()),
    TVector3(posOut.X(),  posOut.Y(),  posOut.Z()),
    TVector3(momIn.Px(),  momIn.Py(),  momIn.Pz()),
    TVector3(momOut.Px(), momOut.Py(), momOut.Pz()),
    time, length, eLoss, fN_Station, fX_Strip, fY_Strip);

  }

  ERQTelescopeCsIPoint* ERQTelescope::Add_CsIPoint() {
    TClonesArray& clref = *fCsIPoint;
    Int_t size = clref.GetEntriesFast();
    return new(clref[size]) ERQTelescopeCsIPoint(eventID, trackID, mot0TrackID, mass,
      TVector3(posIn.X(),   posIn.Y(),   posIn.Z()),
      TVector3(posOut.X(),  posOut.Y(),  posOut.Z()),
      TVector3(momIn.Px(),  momIn.Py(),  momIn.Pz()),
      TVector3(momOut.Px(), momOut.Py(), momOut.Pz()),
      time, length, eLoss, fN_Wall, fN_Block);

    }

  // ----------------------------------------------------------------------------
//=3=3=3=3=3=3=3==3=3=3=3=

Bool_t ERQTelescope::ProcessHits(FairVolume* vol) {
  if ( gMC->IsTrackEntering() ) { // Return true if this is the first step of the track in the current volume
    eLoss  = 0.;
    eventID = gMC->CurrentEvent();
    gMC->TrackPosition(posIn);
    gMC->TrackMomentum(momIn);
    trackID  = gMC->GetStack()->GetCurrentTrackNumber();
    time   = gMC->TrackTime() * 1.0e09;  // Return the current time of flight of the track being transported
    length = gMC->TrackLength(); // Return the length of the current track from its origin (in cm)
    mot0TrackID  = gMC->GetStack()->GetCurrentTrack()->GetMother(0);
    mass = gMC->ParticleMass(gMC->TrackPid()); // GeV/c2
    // gMC->CurrentVolID(sensor);
    // gMC->CurrentVolOffID(1, sector);
  }

  eLoss += gMC->Edep(); // GeV //Return the energy lost in the current step

	if (gMC->IsTrackExiting()    || //Return true if this is the last step of the track in the current volume
	    gMC->IsTrackStop()       || //Return true if the track energy has fallen below the threshold
	    gMC->IsTrackDisappeared())
	{
    gMC->TrackPosition(posOut);
    gMC->TrackMomentum(momOut);
    TString volName = gMC->CurrentVolName();
	  if (eLoss > 0.){
      if(volName.Contains("Si"))
    {
      gMC->CurrentVolOffID(0, fX_Strip);
      gMC->CurrentVolOffID(1, fY_Strip);
      gMC->CurrentVolOffID(2, fN_Station);
      Add_SiPoint();
    }
    if(volName.Contains("CsI"))
    {
      gMC->CurrentVolID(fN_Block);
      gMC->CurrentVolOffID(1, fN_Wall);
      Add_CsIPoint();
    }
  }
  }
  return kTRUE;
}

// -----   Public method EndOfEvent   -----------------------------------------
void ERQTelescope::BeginEvent() {
}


void ERQTelescope::EndOfEvent() {
  if (fVerboseLevel > 1) {
    Print();
  }
  Reset();
}

// -----   Public method Register   -------------------------------------------
void ERQTelescope::Register() {
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman)
	Fatal("Init", "IO manager is not set");
  ioman->Register("ERQTelescopeSiPoint","QTelescope", fSiPoint, kTRUE);
  ioman->Register("ERQTelescopeCsIPoint","QTelescope", fCsIPoint, kTRUE);
}
// ----------------------------------------------------------------------------




// -----   Public method GetCollection   --------------------------------------
TClonesArray* ERQTelescope::GetCollection(Int_t iColl) const {
  if (iColl == 0)
    return fSiPoint;
  if (iColl == 0)
    return fCsIPoint;
    return NULL;
}
// ----------------------------------------------------------------------------


// -----   Public method Print   ----------------------------------------------
void ERQTelescope::Print(Option_t *option) const
{
  if(fSiPoint->GetEntriesFast() > 0)
  {
    std::cout << "======== Si Points ==================" << std::endl;
    for (Int_t iPoint = 0; iPoint < fSiPoint->GetEntriesFast(); iPoint++){
      ERQTelescopeSiPoint* point = (ERQTelescopeSiPoint*)fSiPoint->At(iPoint);
      point->Print();
    }
  }
  if(fCsIPoint->GetEntriesFast() > 0)
  {
    std::cout << "======== CsI Points ==================" << std::endl;
    for (Int_t iPoint = 0; iPoint < fCsIPoint->GetEntriesFast(); iPoint++){
      ERQTelescopeCsIPoint* point = (ERQTelescopeCsIPoint*)fCsIPoint->At(iPoint);
      point->Print();
    }
  }
}// ----------------------------------------------------------------------------

// -----   Public method Reset   ----------------------------------------------
void ERQTelescope::Reset() {
  LOG(INFO) << "  ERQTelescope::Reset()" << FairLogger::endl;
  fSiPoint->Clear();
  fCsIPoint->Clear();
  ResetParameters();
}
// ----------------------------------------------------------------------------

// -----   Public method CopyClones   -----------------------------------------
void ERQTelescope::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset) {
  LOG(INFO) << "   ERQTelescope::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset)"
            << FairLogger::endl;
  Int_t nEntries = cl1->GetEntriesFast();
  LOG(INFO) << "QTelescope: " << nEntries << " entries to add" << FairLogger::endl;
  TClonesArray& clref = *cl2;
  ERQTelescopeSiPoint* oldpoint = NULL;
  for (Int_t i=0; i<nEntries; i++) {
  oldpoint = (ERQTelescopeSiPoint*) cl1->At(i);
   Int_t index = oldpoint->GetTrackID() + offset;
   oldpoint->SetTrackID(index);
   new (clref[cl2->GetEntriesFast()]) ERQTelescopeSiPoint(*oldpoint);
  }
  LOG(INFO) << "decector: " << cl2->GetEntriesFast() << " merged entries" << FairLogger::endl;
}
// ----------------------------------------------------------------------------



// -----   Public method ConstructGeometry   ----------------------------------
void ERQTelescope::ConstructGeometry() {
  TString fileName = GetGeometryFileName();
  if(fileName.EndsWith(".root")) {
    cout << "Constructing ERQTelescope geometry from ROOT file " << fileName.Data() << FairLogger::endl;
    ConstructRootGeometry();
  } else {
    cerr << "Geometry file name is not set" << FairLogger::endl;
  }

}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
Bool_t ERQTelescope::CheckIfSensitive(std::string name)
{
  //cout << name << endl;
  TString volName = name;
  if(volName.Contains("box")) {
    return kTRUE;
  }
  return kFALSE;
}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
void ERQTelescope::ResetParameters() {
  LOG(INFO) << "   ERQTelescope::ResetParameters() " << FairLogger::endl;
};
// ----------------------------------------------------------------------------
ClassImp(ERQTelescope)

