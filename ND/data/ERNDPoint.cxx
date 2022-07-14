// -------------------------------------------------------------------------
// -----                      ERNDPoint source file                -----
// -------------------------------------------------------------------------

#include "ERNDPoint.h"

#include "FairLogger.h"

// -----   Default constructor   -------------------------------------------
ERNDPoint::ERNDPoint()
  : FairMCPoint(),
    fX_out(0.), fY_out(0.), fZ_out(0.),
    fPx_out(0.), fPy_out(0.), fPz_out(0.),
    fStilbenNr(-1)
{
}
// -------------------------------------------------------------------------



// -----   Standard constructor   ------------------------------------------
ERNDPoint::ERNDPoint(Int_t eventID, Int_t trackID,
		  Int_t mot0trackID,
		  Int_t pdg,
		  TVector3 posIn,
		  TVector3 posOut, TVector3 momIn, TVector3 momOut,
		  Double_t tof, Double_t length, Double_t eLoss,
      Int_t stilbenNr,Float_t lightYield,
      Int_t parentTrackId, Int_t parentPdgId)
  : FairMCPoint(trackID, -1., posIn, momIn, tof, length, eLoss),
    fMot0TrackID(mot0trackID),
    fEventID(eventID),
    fX_out(posOut.X()), fY_out(posOut.Y()), fZ_out(posOut.Z()),
    fPx_out(momOut.X()), fPy_out(momOut.Y()), fPz_out(momOut.Z()),
    fStilbenNr(stilbenNr),fLightYield(lightYield), fPdg(pdg),
    fParentTrackId(parentTrackId), fParentPdgId(parentPdgId)
{
}
// -------------------------------------------------------------------------



// -------------------------------------------------------------------------
ERNDPoint::ERNDPoint(const ERNDPoint& right)
  : FairMCPoint(right),
    fX_out(right.fX_out), fY_out(right.fY_out), fZ_out(right.fZ_out),
    fPx_out(right.fPx_out), fPy_out(right.fPy_out), fPz_out(right.fPz_out),
    fStilbenNr(right.fStilbenNr),fLightYield(right.fLightYield), fPdg(right.fPdg)
{
}
// -------------------------------------------------------------------------



// -----   Destructor   ----------------------------------------------------
ERNDPoint::~ERNDPoint()
{
}
// -------------------------------------------------------------------------



// -----   Public method Print   -------------------------------------------
void ERNDPoint::Print(const Option_t* opt /* = 0*/) const
{
  LOG(DEBUG2) << "    ERNDPoint: track " << fTrackID << " mother track = " << fMot0TrackID << FairLogger::endl;
  LOG(DEBUG2) << "    Position (" << fX << ", " << fY << ", " << fZ << ") cm" << FairLogger::end;
  LOG(DEBUG2) << "    Momentum (" << fPx << ", " << fPy << ", " << fPz << ") MeV" << FairLogger::end;
  LOG(DEBUG2) << "    Time " << fTime << " ns,  Length " << fLength << " cm" << FairLogger::end;
  LOG(DEBUG2) << "    Energy loss " << fELoss << " MeV "<< FairLogger::end;
}
// -------------------------------------------------------------------------



// -----   Point x coordinate from linear extrapolation   ------------------
Double_t ERNDPoint::GetX(Double_t z) const
{
  if ( (fZ_out-z)*(fZ-z) >= 0. ) return (fX_out+fX)/2.;
  Double_t dz = fZ_out - fZ;
  return ( fX + (z-fZ) / dz * (fX_out-fX) );
}
// -------------------------------------------------------------------------



// -----   Point y coordinate from linear extrapolation   ------------------
Double_t ERNDPoint::GetY(Double_t z) const
{
  if ( (fZ_out-z)*(fZ-z) >= 0. ) return (fY_out+fY)/2.;
  Double_t dz = fZ_out - fZ;
  //  if ( TMath::Abs(dz) < 1.e-3 ) return (fY_out+fY)/2.;
  return ( fY + (z-fZ) / dz * (fY_out-fY) );
}
// -------------------------------------------------------------------------



// -----   Public method IsUsable   ----------------------------------------
Bool_t ERNDPoint::IsUsable() const
{
  Double_t dz = fZ_out - fZ;
  if ( TMath::Abs(dz) < 1.e-4 ) return kFALSE;
  return kTRUE;
}
// -------------------------------------------------------------------------



ClassImp(ERNDPoint)
