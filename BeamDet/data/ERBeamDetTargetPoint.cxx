// -------------------------------------------------------------------------
// -----                      ERBeamDetTargetPoint source file                   -----
// -------------------------------------------------------------------------

#include "ERBeamDetTargetPoint.h"
#include <iostream>
using namespace std;
// -----   Default constructor   -------------------------------------------
ERBeamDetTargetPoint::ERBeamDetTargetPoint()
  : FairMCPoint(),
    fX_out(0.), fY_out(0.), fZ_out(0.),
    fPx_out(0.), fPy_out(0.), fPz_out(0.),
    fLightYield(0.)
{
}
// -------------------------------------------------------------------------
ERBeamDetTargetPoint::ERBeamDetTargetPoint(Int_t eventID, Int_t trackID,
                  Int_t mot0trackID,
                  Int_t pid,
                  TVector3 posIn,
                  TVector3 posOut, TVector3 momIn, TVector3 momOut,
                  Double_t tof, Double_t length, Double_t eLoss, Double_t lightYield)
  : FairMCPoint(trackID, -1., posIn, momIn, tof, length, eLoss),
    fEventID(eventID),
    fPid(pid),
    fX_in(posIn.X()), fY_in(posIn.Y()), fZ_in(posIn.Z()),
    fPx_in(momIn.X()), fPy_in(momIn.Y()), fPz_in(momIn.Z()),
    fX_out(posOut.X()), fY_out(posOut.Y()), fZ_out(posOut.Z()),
    fPx_out(momOut.X()), fPy_out(momOut.Y()), fPz_out(momOut.Z()),
    fLightYield(lightYield)
{
}
// -------------------------------------------------------------------------
ERBeamDetTargetPoint::~ERBeamDetTargetPoint()
{
}
// -------------------------------------------------------------------------
void ERBeamDetTargetPoint::Print(const Option_t* opt /* = 0*/) const
{
  cout << "-I- ERBeamDetTargetPoint: track " << fTrackID << " mother track = " << fMot0TrackID << endl;
  cout << "    particle ID " << fPid << endl;
  cout << "    Position (" << fX << ", " << fY << ", " << fZ << ") cm" << endl;
  cout << "    Momentum (" << fPx << ", " << fPy << ", " << fPz << ") GeV" << endl;
  cout << "    Time " << fTime << " ns,  Length " << fLength << " cm" << endl;
  cout << "    Energy loss " << fELoss << " keV "<< endl;
}
// -------------------------------------------------------------------------
// -----   Point x coordinate from linear extrapolation   ------------------
Double_t ERBeamDetTargetPoint::GetX(Double_t z) const
{
  if ( (fZ_out-z)*(fZ-z) >= 0. ) return (fX_out+fX)/2.;
  Double_t dz = fZ_out - fZ;
  return ( fX + (z-fZ) / dz * (fX_out-fX) );
}
// -------------------------------------------------------------------------
// -----   Point y coordinate from linear extrapolation   ------------------
Double_t ERBeamDetTargetPoint::GetY(Double_t z) const
{
  if ( (fZ_out-z)*(fZ-z) >= 0. ) return (fY_out+fY)/2.;
  Double_t dz = fZ_out - fZ;
  //  if ( TMath::Abs(dz) < 1.e-3 ) return (fY_out+fY)/2.;
  return ( fY + (z-fZ) / dz * (fY_out-fY) );
}
// -------------------------------------------------------------------------
// -----   Public method IsUsable   ----------------------------------------
Bool_t ERBeamDetTargetPoint::IsUsable() const
{
  Double_t dz = fZ_out - fZ;
  if ( TMath::Abs(dz) < 1.e-4 ) return kFALSE;
  return kTRUE;
}
// -------------------------------------------------------------------------
ClassImp(ERBeamDetTargetPoint)