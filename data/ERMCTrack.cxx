// -------------------------------------------------------------------------
// -----                      ERMCTrack header file                    -----
// -----                  Created 11/05/15  by V.Schetinin            -----
// -------------------------------------------------------------------------


#include "ERMCTrack.h"

#include<iostream>

#include "TParticle.h"
#ifndef ROOT_TParticlePDG
 #include "TParticlePDG.h"
#endif
#ifndef ROOT_TDatabasePDG
 #include "TDatabasePDG.h"
#endif

// -----   Default constructor   -------------------------------------------
ERMCTrack::ERMCTrack()
  : TObject(),
    fPdgCode(0),
    fMotherId(-1),
    fPx(0.),
    fPy(0.),
    fPz(0.),
    fStartX(0.),
    fStartY(0.),
    fStartZ(0.),
    fStartT(0.),
    fNPoints(0)
{
}
// -------------------------------------------------------------------------

// -----   Standard constructor   ------------------------------------------
ERMCTrack::ERMCTrack(Int_t pdgCode, Int_t motherId, Double_t px,
		       Double_t py, Double_t pz, Double_t x, Double_t y,
		       Double_t z, Double_t t, Int_t nPoints = 0)
  : TObject(),
    fPdgCode(pdgCode),
    fMotherId(motherId),
    fPx(px),
    fPy(py),
    fPz(pz),
    fStartX(x),
    fStartY(y),
    fStartZ(z),
    fStartT(t),
    fNPoints(0)
{
  if (nPoints >= 0) fNPoints = nPoints;
  //  else              fNPoints = 0;
  fMass = GetMass();
  fEnergy = GetEnergy();
}
// -------------------------------------------------------------------------



// -----   Copy constructor   ----------------------------------------------
ERMCTrack::ERMCTrack(const ERMCTrack& track)
  : TObject(track),
    fPdgCode(track.fPdgCode),
    fMotherId(track.fMotherId),
    fPx(track.fPx),
    fPy(track.fPy),
    fPz(track.fPz),
    fStartX(track.fStartX),
    fStartY(track.fStartY),
    fStartZ(track.fStartZ),
    fStartT(track.fStartT),
    fNPoints(track.fNPoints),
    fMass(track.fMass),
    fEnergy(track.fEnergy)
{
  //  *this = track;
}
// -------------------------------------------------------------------------



// -----   Constructor from TParticle   ------------------------------------
ERMCTrack::ERMCTrack(TParticle* part)
  : TObject(),
    fPdgCode(part->GetPdgCode()),
    fMotherId(part->GetMother(0)),
    fPx(part->Px()),
    fPy(part->Py()),
    fPz(part->Pz()),
    fStartX(part->Vx()),
    fStartY(part->Vy()),
    fStartZ(part->Vz()),
    fStartT(part->T()*1e09),
    fNPoints(0)
{
    fMass = GetMass();
    fEnergy = GetEnergy();
}
// -------------------------------------------------------------------------



// -----   Destructor   ----------------------------------------------------
ERMCTrack::~ERMCTrack() { }
// -------------------------------------------------------------------------



// -----   Public method Print   -------------------------------------------
void ERMCTrack::Print(Int_t trackId) const {
  /*std::cout << "Track " << trackId << ", mother : " << fMotherId
	     << ", Type " << fPdgCode << ", momentum (" << fPx << ", "
	     << fPy << ", " << fPz << ") GeV" << std::endl;*/
  //std::cout << "       Ref " << GetNPoints(kREF) << std::endl;
}
// -------------------------------------------------------------------------



// -----   Public method GetMass   -----------------------------------------
Double_t ERMCTrack::GetMass() const {
  if ( TDatabasePDG::Instance() ) {
    TParticlePDG* particle = TDatabasePDG::Instance()->GetParticle(fPdgCode);
    if ( particle ) return particle->Mass();
    else return 0.;
  }
  return 0.;
}
// -------------------------------------------------------------------------

// -----   Public method GetCharge   ---------------------------------------
Double_t ERMCTrack::GetCharge() const {
  if ( TDatabasePDG::Instance() ) {
    TParticlePDG* particle = TDatabasePDG::Instance()->GetParticle(fPdgCode);
    if ( particle ) return particle->Charge();
    else return 0.;
  }
  return 0.;
}
// -------------------------------------------------------------------------

// -----   Public method GetRapidity   -------------------------------------
Double_t ERMCTrack::GetRapidity() const {
  Double_t e = GetEnergy();
  Double_t y = 0.5 * TMath::Log( (e+fPz) / (e-fPz) );
  return y;
}
// -------------------------------------------------------------------------




// -----   Public method GetNPoints   --------------------------------------
Int_t ERMCTrack::GetNPoints(DetectorId detId) const {
 /* if      ( detId == kREF  ) return (  fNPoints &   1);
  else if ( detId == kMVD  ) return ( (fNPoints & ( 7 <<  1) ) >>  1);
  else if ( detId == kSTS  ) return ( (fNPoints & (31 <<  4) ) >>  4);
  else if ( detId == kRICH ) return ( (fNPoints & ( 1 <<  9) ) >>  9);
  else if ( detId == kMUCH ) return ( (fNPoints & (31 << 10) ) >> 10);
  else if ( detId == kTRD  ) return ( (fNPoints & (31 << 15) ) >> 15);
  else if ( detId == kTOF  ) return ( (fNPoints & (15 << 20) ) >> 20);
  else if ( detId == kECAL ) return ( (fNPoints & ( 1 << 24) ) >> 24);
  else if ( detId == kPSD  ) return ( (fNPoints & ( 1 << 25) ) >> 25);
  else {
    std::cerr << "GetNPoints: Unknown detector ID "
	       << detId << std::endl;
  */
    return 0;
  //}
}
// -------------------------------------------------------------------------



// -----   Public method SetNPoints   --------------------------------------
void ERMCTrack::SetNPoints(Int_t iDet, Int_t nPoints) {
/*
  if ( iDet == kREF ) {
    if      ( nPoints < 0 ) nPoints = 0;
    else if ( nPoints > 1 ) nPoints = 1;
    fNPoints = ( fNPoints & ( ~ 1 ) )  |  nPoints;
  }

  else if ( iDet == kMVD ) {
    if      ( nPoints < 0 ) nPoints = 0;
    else if ( nPoints > 7 ) nPoints = 7;
    fNPoints = ( fNPoints & ( ~ (  7 <<  1 ) ) )  |  ( nPoints <<  1 );
  }

  else if ( iDet == kSTS ) {
    if      ( nPoints <  0 ) nPoints =  0;
    else if ( nPoints > 31 ) nPoints = 31;
    fNPoints = ( fNPoints & ( ~ ( 31 <<  4 ) ) )  |  ( nPoints <<  4 );
  }

  else if ( iDet == kRICH ) {
    if      ( nPoints < 0 ) nPoints = 0;
    else if ( nPoints > 1 ) nPoints = 1;
    fNPoints = ( fNPoints & ( ~ (  1 <<  9 ) ) )  |  ( nPoints <<  9 );
  }

  else if ( iDet == kMUCH ) {
    if      ( nPoints <  0 ) nPoints =  0;
    else if ( nPoints > 31 ) nPoints = 31;
    fNPoints = ( fNPoints & ( ~ ( 31 << 10 ) ) )  |  ( nPoints << 10 );
  }

  else if ( iDet == kTRD ) {
    if      ( nPoints <  0 ) nPoints =  0;
    else if ( nPoints > 31 ) nPoints = 31;
    fNPoints = ( fNPoints & ( ~ ( 31 << 15 ) ) )  |  ( nPoints << 15 );
  }

  else if ( iDet == kTOF ) {
    if      ( nPoints <  0 ) nPoints =  0;
    else if ( nPoints > 15 ) nPoints = 15;
    fNPoints = ( fNPoints & ( ~ ( 15 << 20 ) ) )  |  ( nPoints << 20 );
  }

  else if ( iDet == kECAL ) {
    if      ( nPoints < 0 ) nPoints = 0;
    else if ( nPoints > 1 ) nPoints = 1;
    fNPoints = ( fNPoints & ( ~ (  1 << 24 ) ) )  |  ( nPoints << 24 );
  }

  else if ( iDet == kPSD ) {
    if      ( nPoints < 0 ) nPoints = 0;
    else if ( nPoints > 1 ) nPoints = 1;
    fNPoints = ( fNPoints & ( ~ (  1 << 25 ) ) )  |  ( nPoints << 25 );
  }

  else std::cerr << "Unknown detector ID "
		  << iDet << std::endl;
*/
}
// -------------------------------------------------------------------------


ClassImp(ERMCTrack)
