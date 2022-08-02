/********************************************************************************
 *              Copyright (C) Joint Institute for Nuclear Research              *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/

#include "ERND.h"

#include <unordered_map>

#include "TClonesArray.h"
#include "TParticle.h"
#include "TVirtualMC.h"
#include "TString.h"

#include "FairRootManager.h"
#include "FairLogger.h"
#include "FairRunSim.h"

#include "ERMCTrack.h"
#include "ERStack.h"
#include "ERDecayMCEventHeader.h"

//--------------------------------------------------------------------------------------------------
ERND::ERND()
  : FairDetector("ERND", kTRUE)
{
  LOG(INFO) << "  ERND::ERND()" << FairLogger::endl;
  fNDPoints = new TClonesArray("ERNDPoint");
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
  fVerboseLevel = 1;
}
//--------------------------------------------------------------------------------------------------
ERND::ERND(const char* name, Bool_t active, Int_t verbose) 
  : FairDetector(name, active,verbose)
{
  fNDPoints = new TClonesArray("ERNDPoint");
  flGeoPar = new TList();
  flGeoPar->SetName( GetName());
}
//--------------------------------------------------------------------------------------------------
ERND::~ERND() {
  if (fNDPoints) {
    fNDPoints->Delete();
    delete fNDPoints;
  }
}
//--------------------------------------------------------------------------------------------------
void ERND::Initialize()
{
  FairDetector::Initialize();
}
//--------------------------------------------------------------------------------------------------
Bool_t ERND::ProcessHits(FairVolume* vol) {
  // Set constants for Birk's Law implentation (Geant 4parametrization)
  /*
  static const Double_t dP = 1.032 ;
  static const Double_t BirkC1 =  0.013/dP;
  static const Double_t BirkC2 =  9.6e-6/(dP * dP);
  */
  //Birks constants from Craun, R. L.; Smith, D. L. NIM 80,2, p. 239, 1970
  /*
  static const Double_t dP = 0.97;
  static const Double_t BirkC1 =  0.00856/dP;
  static const Double_t BirkC2 =  4.99e-6/(dP * dP);
  */
  // Bircks constant from experiment. S. Belogurov, E. Gazeeva
  static const Double_t dP = 1.16;
  static const Double_t BirkC1 =  0.027/dP;
  static const Double_t BirkC2 =  0.0/(dP * dP);

  static Int_t          eventID;           //!  event index
  static Int_t          trackID;           //!  track index
  static Int_t          mot0TrackID;       //!  0th mother track index
  static Int_t       pdg;              //!  pdg
  static TLorentzVector posIn, posOut;    //!  position
  static TLorentzVector momIn, momOut;    //!  momentum
  static Double32_t     time;              //!  time
  static Double32_t     length;            //!  length
  static Double32_t     eLoss;             //!  energy loss
  static Int_t          stilbenNr;
  static Double_t       lightYield;

  static Int_t          parentTrackId = -1;
  static Int_t          parentPdg = -1;

  gMC->SetMaxStep(fStep);

  if ( gMC->IsTrackEntering() ) { // Return true if this is the first step of the track in the current volume
    eLoss  = 0.;
    lightYield = 0.;
    eventID = gMC->CurrentEvent();
    gMC->TrackPosition(posIn);
    gMC->TrackMomentum(momIn);
    trackID  = gMC->GetStack()->GetCurrentTrackNumber();
    time   = gMC->TrackTime() * 1.0e09;  // Return the current time of flight of the track being transported
    length = gMC->TrackLength(); // Return the length of the current track from its origin (in cm)
    mot0TrackID  = gMC->GetStack()->GetCurrentTrack()->GetMother(0);
    pdg = gMC->TrackPid();
    gMC->CurrentVolOffID(3,stilbenNr); 
  }
  const Double_t stepEloss = gMC->Edep() * 1000; // MeV //Return the energy lost in the current step
  eLoss += stepEloss;
  Double_t curLightYield = 0.;
  // Apply Birk's law ( Adapted from G3BIRK/Geant3)
  // Correction for all charge states
  if (gMC->TrackCharge()!=0) { // Return the charge of the track currently transported
    Double_t BirkC1Mod = 0;
    // Apply correction for higher charge states
      if (TMath::Abs(gMC->TrackCharge())>=2)
        BirkC1Mod=BirkC1*7.2/12.6;
      else
        BirkC1Mod=BirkC1;

    if (gMC->TrackStep()>0)
    {
      Double_t dedxcm = stepEloss/gMC->TrackStep(); //[MeV/cm]
      curLightYield = stepEloss /(1.+BirkC1Mod*dedxcm+BirkC2*dedxcm*dedxcm); //[MeV]
      lightYield+=curLightYield;
    }
  }

	if (gMC->IsTrackExiting()    || //Return true if this is the last step of the track in the current volume 
	    gMC->IsTrackStop()       || //Return true if the track energy has fallen below the threshold
	    gMC->IsTrackDisappeared()) 
	{ 
    gMC->TrackPosition(posOut);
    gMC->TrackMomentum(momOut);

	  if (eLoss > 0. && gMC->TrackCharge()!=0)
    {
      FindParentParticle(trackID, parentTrackId, parentPdg);
      AddPoint( eventID, trackID, mot0TrackID, pdg,
                TVector3(posIn.X(),   posIn.Y(),   posIn.Z()),
                TVector3(posOut.X(),  posOut.Y(),  posOut.Z()),
                TVector3(momIn.Px(),  momIn.Py(),  momIn.Pz()),
                TVector3(momOut.Px(), momOut.Py(), momOut.Pz()),
                time, length, eLoss, stilbenNr, lightYield,
                parentTrackId, parentPdg);
    }
  }
  return kTRUE;
}
//--------------------------------------------------------------------------------------------------
void ERND::EndOfEvent() {
  LOG(DEBUG) << "ND Points Count: " << fNDPoints->GetEntriesFast() << FairLogger::endl;
  Print();
  Reset();
}
//--------------------------------------------------------------------------------------------------
void ERND::Register() {
  FairRootManager* ioman = FairRootManager::Instance();
  if (!ioman)
	Fatal("Init", "IO manager is not set");
  fMCTracks = (TClonesArray*) ioman->GetObject("MCTrack");
  ioman->Register("NDPoint","ND", fNDPoints, kTRUE);
}
//--------------------------------------------------------------------------------------------------
TClonesArray* ERND::GetCollection(Int_t iColl) const {
  if (iColl == 0) 
    return fNDPoints;
  else 
    return NULL;
}
//--------------------------------------------------------------------------------------------------
void ERND::Print(Option_t *option) const {
  for (Int_t i_point = 0; i_point < fNDPoints->GetEntriesFast(); i_point++){
    ERNDPoint* point = (ERNDPoint*)fNDPoints->At(i_point);
    point->Print();
  }
}
//--------------------------------------------------------------------------------------------------
void ERND::Reset() {
  fNDPoints->Clear();
}
//--------------------------------------------------------------------------------------------------
void ERND::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset) {
  LOG(DEBUG) << "   ERND::CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset)" 
            << FairLogger::endl;
  Int_t nEntries = cl1->GetEntriesFast();
  LOG(DEBUG) << "decector: " << nEntries << " entries to add" << FairLogger::endl;
  TClonesArray& clref = *cl2;
  ERNDPoint* oldpoint = NULL;
  for (Int_t i=0; i<nEntries; i++) {
  oldpoint = (ERNDPoint*) cl1->At(i);
   Int_t index = oldpoint->GetTrackID() + offset;
   oldpoint->SetTrackID(index);
   new (clref[cl2->GetEntriesFast()]) ERNDPoint(*oldpoint);
  }
  LOG(DEBUG) << "decector: " << cl2->GetEntriesFast() << " merged entries" << FairLogger::endl;
}
//--------------------------------------------------------------------------------------------------
ERNDPoint* ERND::AddPoint(
    Int_t eventID, Int_t trackID, Int_t mot0trackID, Int_t pdg,
    TVector3 posIn, TVector3 posOut, TVector3 momIn,
    TVector3 momOut, Double_t time, Double_t length,
    Double_t eLoss, Int_t stilbenNr, Float_t lightYield,
    Int_t parentTrackId, Int_t parentPdg) 
{
  TClonesArray& clref = *fNDPoints;
  Int_t size = clref.GetEntriesFast();
  return new(clref[size]) ERNDPoint(eventID, trackID, mot0trackID, pdg,
					  posIn, posOut, momIn, momOut, time, length, eLoss,stilbenNr, lightYield,
            parentTrackId, parentPdg);
}
//--------------------------------------------------------------------------------------------------
void ERND::FindParentParticle(const int track_id, int& parentTrackId, int& parentPdg)
{
  dynamic_cast<ERStack*>(gMC->GetStack())->FillTrackArray();
  std::unordered_map<int, ERMCTrack*> id_to_track;
  for (int i(0); i < fMCTracks->GetEntriesFast(); i++) {
    auto* track = dynamic_cast<ERMCTrack*>(fMCTracks->At(i));
    id_to_track[track->Id()] = track;
  }
  
  auto track_it = id_to_track.find(track_id);
  if (track_it == id_to_track.end())
    LOG(FATAL) << "ND: Track not found in stack!" << FairLogger::endl;

  auto* track = track_it->second;
  auto* parent_track = track;

  FairRunSim* run = FairRunSim::Instance();
  if (auto* header = dynamic_cast<ERDecayMCEventHeader*>(run->GetMCEventHeader()))
  {
    TArrayI reaction_track_ids = header->GetOutputTrackID();
    const auto found_in_reaction = [&reaction_track_ids](const int id)
    {
      for (int i = 0; i < reaction_track_ids.GetSize(); ++i)
      {
        if (reaction_track_ids[i] == id)
          return true;
      }
      return false;
    };

    while (!found_in_reaction(parent_track->Id()) && parent_track->GetMotherId() != -1)
    {
      parent_track = id_to_track[parent_track->GetMotherId()];
    } 
  }
  else
  {
    while ((fCandidatesForParentPdgs.find(parent_track->GetPdgCode()) != fCandidatesForParentPdgs.end())
        && parent_track->GetMotherId() != -1)
    {
      parent_track = id_to_track[parent_track->GetMotherId()];
    }
  }

  parentTrackId = parent_track->Id();
  if (parent_track->GetMotherId() != -1)
  {
    parentPdg = parent_track->GetPdgCode();
  }
}
//--------------------------------------------------------------------------------------------------
void ERND::ConstructGeometry() {
  TString fileName = GetGeometryFileName();
  if(fileName.EndsWith(".root")) {
    LOG(DEBUG) << "Constructing ERND geometry from ROOT file " << fileName.Data() << FairLogger::endl;
    ConstructRootGeometry();
  } else {
    LOG(DEBUG) << "Geometry file name is not set" << FairLogger::endl;
  }
  
}
//--------------------------------------------------------------------------------------------------
Bool_t ERND::CheckIfSensitive(std::string name)
{
  TString volName = name;
  if(volName.Contains("crystal")) {
    return kTRUE;
  }

  return kFALSE;
}
//--------------------------------------------------------------------------------------------------
ClassImp(ERND)
