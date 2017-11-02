// -------------------------------------------------------------------------
// -----                      ERRTelescopeCsIPoint source file                -----
// -------------------------------------------------------------------------

#include "ERRTelescopeCsIPoint.h"
#include <iostream>
using namespace std;
// -----   Default constructor   -------------------------------------------
ERRTelescopeCsIPoint::ERRTelescopeCsIPoint()
  : FairMCPoint(),
    fTelescopeNb(0.),
    fX_out(0.), fY_out(0.), fZ_out(0.),
    fPx_out(0.), fPy_out(0.), fPz_out(0.),
    fCrystallNb(-1),
    fPID(0)
{
}
// -------------------------------------------------------------------------



// -----   Standard constructor   ------------------------------------------
ERRTelescopeCsIPoint::ERRTelescopeCsIPoint(Int_t eventID, Int_t trackID, Int_t telescopeNb,
		  Int_t mot0trackID,
		  Double_t pid,
		  TVector3 posIn,
		  TVector3 posOut, TVector3 momIn, TVector3 momOut,
		  Double_t tof, Double_t length, Double_t eLoss,Int_t crystall)
  : FairMCPoint(trackID, -1., posIn, momIn, tof, length, eLoss),
    fEventID(eventID),
    fTelescopeNb(telescopeNb),
    fX_out(posOut.X()), fY_out(posOut.Y()), fZ_out(posOut.Z()),
    fPx_out(momOut.X()), fPy_out(momOut.Y()), fPz_out(momOut.Z()),
    fCrystallNb(crystall),
    fPID(pid)
{
}
// -------------------------------------------------------------------------



// -------------------------------------------------------------------------
ERRTelescopeCsIPoint::ERRTelescopeCsIPoint(const ERRTelescopeCsIPoint& right)
  : FairMCPoint(right),
    fTelescopeNb(right.fTelescopeNb),
    fX_out(right.fX_out), fY_out(right.fY_out), fZ_out(right.fZ_out),
    fPx_out(right.fPx_out), fPy_out(right.fPy_out), fPz_out(right.fPz_out),
    fCrystallNb(right.fCrystallNb),
    fPID(right.fPID)
{
}
// -------------------------------------------------------------------------



// -----   Destructor   ----------------------------------------------------
ERRTelescopeCsIPoint::~ERRTelescopeCsIPoint()
{
}
// -------------------------------------------------------------------------



// -----   Public method Print   -------------------------------------------
void ERRTelescopeCsIPoint::Print(const Option_t* opt /* = 0*/) const
{
  cout << "-I- ERRTelescopeCsIPoint: track " << fTrackID << " mother track = " << fMot0TrackID << endl;
  cout << "    Position (" << fX << ", " << fY << ", " << fZ << ") cm" << endl;
  cout << "    Momentum (" << fPx << ", " << fPy << ", " << fPz << ") GeV" << endl;
  cout << "    Time " << fTime << " ns,  Length " << fLength << " cm" << endl;
  cout << "    Energy loss " << fELoss << " keV "<< endl;
}
// -------------------------------------------------------------------------



// -----   Point x coordinate from linear extrapolation   ------------------
Double_t ERRTelescopeCsIPoint::GetX(Double_t z) const
{
  if ( (fZ_out-z)*(fZ-z) >= 0. ) return (fX_out+fX)/2.;
  Double_t dz = fZ_out - fZ;
  return ( fX + (z-fZ) / dz * (fX_out-fX) );
}
// -------------------------------------------------------------------------



// -----   Point y coordinate from linear extrapolation   ------------------
Double_t ERRTelescopeCsIPoint::GetY(Double_t z) const
{
  if ( (fZ_out-z)*(fZ-z) >= 0. ) return (fY_out+fY)/2.;
  Double_t dz = fZ_out - fZ;
  //  if ( TMath::Abs(dz) < 1.e-3 ) return (fY_out+fY)/2.;
  return ( fY + (z-fZ) / dz * (fY_out-fY) );
}
// -------------------------------------------------------------------------



// -----   Public method IsUsable   ----------------------------------------
Bool_t ERRTelescopeCsIPoint::IsUsable() const
{
  Double_t dz = fZ_out - fZ;
  if ( TMath::Abs(dz) < 1.e-4 ) return kFALSE;
  return kTRUE;
}
// -------------------------------------------------------------------------



ClassImp(ERRTelescopeCsIPoint)